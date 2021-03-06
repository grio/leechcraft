/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2006-2010  Georg Rudoy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#ifndef PLUGINS_AZOTH_PLUGINS_XOOX_GLOOXACCOUNT_H
#define PLUGINS_AZOTH_PLUGINS_XOOX_GLOOXACCOUNT_H
#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QMap>
#include <gloox/messagehandler.h>
#include <gloox/jid.h>
#include <interfaces/iaccount.h>
#include <interfaces/imessage.h>

namespace gloox
{
	class Client;
	class RosterItem;
}

namespace LeechCraft
{
	namespace Plugins
	{
		namespace Azoth
		{
			namespace Plugins
			{
				class IProtocol;

				namespace Xoox
				{
					class ClientConnection;

					struct GlooxAccountState
					{
						IAccount::State State_;
						QString Status_;
						int Priority_;
					};

					class GlooxAccount;
					typedef boost::shared_ptr<GlooxAccount> GlooxAccount_ptr;

					class GlooxAccount : public QObject
									   , public IAccount
									   , public gloox::MessageHandler
					{
						Q_OBJECT
						Q_INTERFACES (LeechCraft::Plugins::Azoth::Plugins::IAccount);

						QString Name_;
						IProtocol *ParentProtocol_;

						QString JID_;
						QString Nick_;
						QString Resource_;
						qint16 Priority_;

						boost::shared_ptr<ClientConnection> ClientConnection_;
						// Bare JID → resource → session.
						QMap<gloox::JID, QMap<QString, gloox::MessageSession*> > Sessions_;
					public:
						GlooxAccount (const QString&, QObject*);

						QObject* GetObject ();
						IProtocol* GetParentProtocol () const;
						AccountFeatures GetAccountFeatures () const;
						QList<ICLEntry*> GetCLEntries ();
						QString GetAccountName () const;
						void RenameAccount (const QString&);
						QByteArray GetAccountID () const;
						void OpenConfigurationDialog ();
						void ChangeState (State, const QString& = QString ());
						void Synchronize ();

						QByteArray Serialize () const;
						static GlooxAccount* Deserialize (const QByteArray&, QObject*);

						IMessage* CreateMessage (IMessage::MessageType,
								const QString&, const QString&,
								gloox::RosterItem*);

						// MessageHandler
						void handleMessage (const gloox::Message&, gloox::MessageSession*);
					private slots:
						void handleGotRosterItems (const QList<QObject*>&);
					private:
						void InitializeSession (gloox::MessageSession*);
					signals:
						void gotCLItems (const QList<QObject*>);
					};
				}
			}
		}
	}
}

#endif
