#include "customwebview.h"
#include <QWebFrame>
#include <QMouseEvent>
#include <QtDebug>
#include "core.h"
#include "customwebpage.h"
#include "browserwidget.h"

CustomWebView::CustomWebView (QWidget *parent)
: QWebView (parent)
{
	connect (this,
			SIGNAL (urlChanged (const QUrl&)),
			this,
			SLOT (remakeURL (const QUrl&)));

	CustomWebPage *page = new CustomWebPage (this);
	connect (page,
			SIGNAL (gotEntity (const QByteArray&)),
			this,
			SIGNAL (gotEntity (const QByteArray&)));
	connect (page,
			SIGNAL (loadingURL (const QUrl&)),
			this,
			SLOT (remakeURL (const QUrl&)));
	setPage (page);
}

CustomWebView::~CustomWebView ()
{
}

void CustomWebView::Load (const QString& string, QString title)
{
	Load (Core::Instance ().MakeURL (string), title);
}

void CustomWebView::Load (const QUrl& url, QString title)
{
	if (url.scheme () == "javascript")
	{
		QVariant result = page ()->mainFrame ()->
			evaluateJavaScript (url.toString ().mid (11));
		if (result.canConvert (QVariant::String))
			setHtml (result.toString ());
		return;
	}
	if (title.isEmpty ())
		title = tr ("Loading...");
	emit titleChanged (title);
	load (url);
}

void CustomWebView::Load (const QNetworkRequest& req,
		QNetworkAccessManager::Operation op, const QByteArray& ba)
{
	emit titleChanged (tr ("Loading..."));
	QWebView::load (req, op, ba);
}

QWebView* CustomWebView::createWindow (QWebPage::WebWindowType type)
{
	if (type == QWebPage::WebModalDialog)
	{
		// We don't need to register it in the Core, so construct
		// directly.
		BrowserWidget *widget = new BrowserWidget (this);
		widget->setWindowFlags (Qt::Dialog);
		widget->setAttribute (Qt::WA_DeleteOnClose);
		connect (widget,
				SIGNAL (gotEntity (const QByteArray&)),
				&Core::Instance (),
				SIGNAL (gotEntity (const QByteArray&)));
		connect (widget,
				SIGNAL (titleChanged (const QString&)),
				widget,
				SLOT (setWindowTitle (const QString&)));
		return widget->GetView ();
	}
	else
		return Core::Instance ().MakeWebView ();
}

void CustomWebView::mousePressEvent (QMouseEvent *e)
{
	qobject_cast<CustomWebPage*> (page ())->SetButtons (e->buttons ());
	qobject_cast<CustomWebPage*> (page ())->SetModifiers (e->modifiers ());
	QWebView::mousePressEvent (e);
}

void CustomWebView::remakeURL (const QUrl& url)
{
	emit urlChanged (url.toString ());
}
