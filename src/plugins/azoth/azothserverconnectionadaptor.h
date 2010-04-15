/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2010  Georg Rudoy
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

#ifndef PLUGINS_AZOTH_AZOTHSERVERCONNECTIONADAPTOR_H
#define PLUGINS_AZOTH_AZOTHSERVERCONNECTIONADAPTOR_H
#include <QDBusAbstractAdaptor>

namespace LeechCraft
{
	namespace Plugins
	{
		namespace Azoth
		{
			class AzothServerConnection;

			class AzothServerConnectionAdaptor : public QDBusAbstractAdaptor
			{
				Q_OBJECT
				Q_CLASSINFO ("D-Bus Interface", "org.LeechCraft.Azoth.Client")

				AzothServerConnection *AZC_;
			public:
				AzothServerConnectionAdaptor (AzothServerConnection*);
			public slots:
				/** Called by the server to notify core that it has loaded.
				 */
				Q_NOREPLY void ServerReady ();

				/** Called by the server to push all the protocol plugins
				 * to the server again.
				 */
				Q_NOREPLY void ReaddProtocolPlugins ();
			};
		};
	};
};

#endif
