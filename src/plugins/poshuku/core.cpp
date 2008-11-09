#include "core.h"
#include <algorithm>
#include <memory>
#include <QString>
#include <QUrl>
#include <QWidget>
#include <QIcon>
#include <QFile>
#include <QNetworkCookieJar>
#include <QDir>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QtDebug>
#include <plugininterface/proxy.h>
#include "browserwidget.h"
#include "customwebview.h"
#include "addtofavoritesdialog.h"
#include "xmlsettingsmanager.h"
#include "customcookiejar.h"
#include "authenticationdialog.h"
#include "sslerrorsdialog.h"
#include "restoresessiondialog.h"

Core::Core ()
: CookieSaveTimer_ (new QTimer ())
, SaveSessionScheduled_ (false)
{
	bool cleanShutdown = XmlSettingsManager::Instance ()->
		Property ("CleanShutdown", true).toBool ();
	XmlSettingsManager::Instance ()->setProperty ("CleanShutdown", false);

	NetworkAccessManager_.reset (new QNetworkAccessManager (this));
	QFile file (QDir::homePath () +
			"/.leechcraft/poshuku/cookies.txt");
	if (file.open (QIODevice::ReadOnly))
	{
		CustomCookieJar *jar = new CustomCookieJar (this);
		jar->Load (file.readAll ());
		NetworkAccessManager_->setCookieJar (jar);
	}

	connect (NetworkAccessManager_.get (),
			SIGNAL (authenticationRequired (QNetworkReply*,
					QAuthenticator*)),
			this,
			SLOT (handleAuthentication (QNetworkReply*,
					QAuthenticator*)));
	connect (NetworkAccessManager_.get (),
			SIGNAL (proxyAuthenticationRequired (const QNetworkProxy&,
					QAuthenticator*)),
			this,
			SLOT (handleProxyAuthentication (const QNetworkProxy&,
					QAuthenticator*)));
	connect (NetworkAccessManager_.get (),
			SIGNAL (sslErrors (QNetworkReply*,
					const QList<QSslError>&)),
			this,
			SLOT (handleSslErrors (QNetworkReply*,
					const QList<QSslError>&)));

	connect (CookieSaveTimer_.get (),
			SIGNAL (timeout ()),
			this,
			SLOT (saveCookies ()));
	CookieSaveTimer_->start (10000);

	FavoritesModel_.reset (new FavoritesModel (this));

	FavoriteTagsCompletionModel_.reset (new TagsCompletionModel (this));
	FavoriteTagsCompletionModel_->
		UpdateTags (XmlSettingsManager::Instance ()->
				Property ("FavoriteTags", tr ("untagged")).toStringList ());
	connect (FavoriteTagsCompletionModel_.get (),
			SIGNAL (tagsUpdated (const QStringList&)),
			this,
			SLOT (favoriteTagsUpdated (const QStringList&)));

	if (!cleanShutdown)
		RestoreSession (true);
	else if (XmlSettingsManager::Instance ()->
			property ("RestorePreviousSession").toBool ())
		RestoreSession (false);
}

Core& Core::Instance ()
{
	static Core core;
	return core;
}

void Core::Release ()
{
	CookieSaveTimer_.reset ();

	saveCookies ();

	NetworkAccessManager_.reset ();
	FavoritesModel_.reset ();
	FavoriteTagsCompletionModel_.reset ();

	XmlSettingsManager::Instance ()->setProperty ("CleanShutdown", true);
	XmlSettingsManager::Instance ()->Release ();
}

bool Core::IsValidURL (const QString& url) const
{
	return QUrl (url).isValid ();
}

BrowserWidget* Core::NewURL (const QString& url)
{
	BrowserWidget *widget = new BrowserWidget;
	widget->GetView ()->page ()->
		setNetworkAccessManager (GetNetworkAccessManager ());
	widget->SetURL (QUrl (url));

	connect (widget,
			SIGNAL (titleChanged (const QString&)),
			this,
			SLOT (handleTitleChanged (const QString&)));
	connect (widget,
			SIGNAL (iconChanged (const QIcon&)),
			this,
			SLOT (handleIconChanged (const QIcon&)));
	connect (widget,
			SIGNAL (needToClose ()),
			this,
			SLOT (handleNeedToClose ()));
	connect (widget,
			SIGNAL (addToFavorites (const QString&, const QString&)),
			this,
			SLOT (handleAddToFavorites (const QString&, const QString&)));
	connect (widget,
			SIGNAL (urlChanged (const QString&)),
			this,
			SLOT (handleURLChanged (const QString&)));

	Widgets_.push_back (widget);

	emit addNewTab (tr ("Loading..."), widget);
	return widget;
}

CustomWebView* Core::MakeWebView ()
{
	return NewURL ("")->GetView ();
}

FavoritesModel* Core::GetFavoritesModel () const
{
	return FavoritesModel_.get ();
}

TagsCompletionModel* Core::GetFavoritesTagsCompletionModel () const
{
	return FavoriteTagsCompletionModel_.get ();
}

QNetworkAccessManager* Core::GetNetworkAccessManager () const
{
	return NetworkAccessManager_.get ();
}

void Core::DoCommonAuth (const QString& msg, QAuthenticator *authen)
{
	std::auto_ptr<AuthenticationDialog> dia (
			new AuthenticationDialog (msg,
				authen->user (),
				authen->password (),
				qApp->activeWindow ())
			);
	if (dia->exec () == QDialog::Rejected)
		return;

	authen->setUser (dia->GetLogin ());
	authen->setPassword (dia->GetPassword ());
}

void Core::RestoreSession (bool ask)
{
	QSettings settings (Proxy::Instance ()->GetOrganizationName (),
			Proxy::Instance ()->GetApplicationName () + "_Poshuku");
	int size = settings.beginReadArray ("Saved session");
	if (!size) ;
	else if (ask)
	{
		std::auto_ptr<RestoreSessionDialog> dia (new RestoreSessionDialog ());
		for (int i = 0; i < size; ++i)
		{
			settings.setArrayIndex (i);
			QString title = settings.value ("Title").toString ();
			QString url = settings.value ("URL").toString ();
			dia->AddPair (title, url);
		}

		if (dia->exec () == QDialog::Accepted)
		{
			RestoredURLs_ = dia->GetSelectedURLs ();
			QTimer::singleShot (5000, this, SLOT (restorePages ()));
		}
	}
	else
	{
		for (int i = 0; i < size; ++i)
		{
			settings.setArrayIndex (i);
			RestoredURLs_ << settings.value ("URL").toString ();
		}
		QTimer::singleShot (5000, this, SLOT (restorePages ()));
	}
	settings.endArray ();
}

void Core::ScheduleSaveSession ()
{
	if (SaveSessionScheduled_)
		return;

	QTimer::singleShot (1000, this, SLOT (saveSession ()));

	SaveSessionScheduled_ = true;
}

void Core::saveCookies () const
{
	QDir dir = QDir::home ();
	dir.cd (".leechcraft");
	if (!dir.exists ("poshuku") &&
			!dir.mkdir ("poshuku"))
	{
		emit error (tr ("Could not create Poshuku directory."));
		return;
	}

	QFile file (QDir::homePath () +
			"/.leechcraft/poshuku/cookies.txt");
	if (!file.open (QIODevice::WriteOnly | QIODevice::Truncate))
		emit error (tr ("Could not save cookies, error opening cookie file."));
	else
		file.write (static_cast<CustomCookieJar*> (NetworkAccessManager_->cookieJar ())->Save ());
}

void Core::handleTitleChanged (const QString& newTitle)
{
	emit changeTabName (dynamic_cast<QWidget*> (sender ()), newTitle);

	ScheduleSaveSession ();
}

void Core::handleURLChanged (const QString& newURL)
{
	emit changeTabName (dynamic_cast<QWidget*> (sender ()),
			tr ("Loading %1").arg (QUrl (newURL).host ()));

	ScheduleSaveSession ();
}

void Core::handleIconChanged (const QIcon& newIcon)
{
	emit changeTabIcon (dynamic_cast<QWidget*> (sender ()), newIcon);
}

void Core::handleNeedToClose ()
{
	BrowserWidget *w = dynamic_cast<BrowserWidget*> (sender ());
	emit removeTab (w);

	std::remove (Widgets_.begin (), Widgets_.end (), w);
	w->deleteLater ();
}

void Core::handleAddToFavorites (const QString& title, const QString& url)
{
	std::auto_ptr<AddToFavoritesDialog> dia (new AddToFavoritesDialog (title,
				url,
				GetFavoritesTagsCompletionModel (),
				qApp->activeWindow ()));
	if (dia->exec () == QDialog::Rejected)
		return;

	FavoritesModel_->AddItem (dia->GetTitle (), url, dia->GetTags ());
	FavoriteTagsCompletionModel_->UpdateTags (dia->GetTags ());
}

void Core::handleAuthentication (QNetworkReply *reply, QAuthenticator *authen)
{
	QString msg = tr ("The URL<br /><code>%1</code><br />with "
			"realm<br /><em>%2</em><br />requires authentication.")
		.arg (reply->url ().toString ())
		.arg (authen->realm ());
	msg = msg.left (200);

	DoCommonAuth (msg, authen);
}

void Core::handleProxyAuthentication (const QNetworkProxy& proxy, QAuthenticator *authen)
{
	QString msg = tr ("The proxy <br /><code>%1</code><br />with "
			"realm<br /><em>%2</em><br />requires authentication.")
		.arg (proxy.hostName () + ":" + QString::number (proxy.port ()))
		.arg (authen->realm ());
	msg = msg.left (200);

	DoCommonAuth (msg, authen);
}

void Core::handleSslErrors (QNetworkReply *reply, const QList<QSslError>& errors)
{
	QSettings settings (Proxy::Instance ()->GetOrganizationName (),
			Proxy::Instance ()->GetApplicationName () + "_Poshuku");
	settings.beginGroup ("SSL exceptions");
	QStringList keys = settings.allKeys ();
	if (keys.contains (reply->url ().toString ())) 
	{
		if (settings.value (reply->url ().toString ()).toBool ())
			reply->ignoreSslErrors ();
	}
	else if (keys.contains (reply->url ().host ()))
	{
		if (settings.value (reply->url ().host ()).toBool ())
			reply->ignoreSslErrors ();
	}
	else
	{
		QString msg = tr ("The URL<br /><code>%1</code><br />has SSL errors."
				" What do you want to do?")
			.arg (reply->url ().toString ());
		std::auto_ptr<SslErrorsDialog> dia (
				new SslErrorsDialog (msg,
					errors,
					qApp->activeWindow ())
				);

		bool ignore = (dia->exec () == QDialog::Accepted);
		if (ignore)
			reply->ignoreSslErrors ();

		SslErrorsDialog::RememberChoice choice;

		if (choice != SslErrorsDialog::RCNot)
		{
			if (choice == SslErrorsDialog::RCFile)
				settings.setValue (reply->url ().toString (),
						ignore);
			else
				settings.setValue (reply->url ().host (),
						ignore);
		}
	}
	settings.endGroup ();
}

void Core::favoriteTagsUpdated (const QStringList& tags)
{
	XmlSettingsManager::Instance ()->setProperty ("FavoriteTags", tags);
}

void Core::saveSession ()
{
	int pos = 0;
	QSettings settings (Proxy::Instance ()->GetOrganizationName (),
			Proxy::Instance ()->GetApplicationName () + "_Poshuku");
	settings.beginWriteArray ("Saved session");
	settings.remove ("");
	for (Widgets_t::const_iterator i = Widgets_.begin (),
			end = Widgets_.end (); i != end; ++i)
	{
		settings.setArrayIndex (pos++);
		settings.setValue ("Title", (*i)->GetView ()->title ());
		settings.setValue ("URL", (*i)->GetView ()->url ().toString ());
	}
	settings.endArray ();

	SaveSessionScheduled_ = false;
}

void Core::restorePages ()
{
	for (QStringList::const_iterator i = RestoredURLs_.begin (),
			end = RestoredURLs_.end (); i != end; ++i)
		NewURL (*i);
}

