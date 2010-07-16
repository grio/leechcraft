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

#ifndef PLUGINS_AZOTH_INTERFACES_ICLENTRY_H
#define PLUGINS_AZOTH_INTERFACES_ICLENTRY_H
#include <QFlags>

namespace LeechCraft
{
	namespace Plugins
	{
		namespace Azoth
		{
			namespace Plugins
			{
				class IAccount;

				class ICLEntry
				{
				public:
					virtual ~ICLEntry () {}

					enum Feature
					{
						FPermanentEntry = 0x0001,
						FSessionEntry = 0x0002,
						FIsChat = 0x0004,
						FIsMUC = 0x0040,
						FIsPrivateChat = 0x0400,
						FSupportsRenames = 0x0100
					};

					Q_DECLARE_FLAGS (Features, Feature);

					virtual QObject* GetObject () = 0;
					virtual IAccount* GetParentAccount () const = 0;
					virtual Features GetEntryFeatures () const = 0;
					virtual QString GetEntryName () const = 0;
					virtual void SetEntryName (const QString&) = 0;
					virtual QByteArray GetEntryID () const = 0;
					virtual QStringList Groups () const = 0;
				};

				Q_DECLARE_OPERATORS_FOR_FLAGS (ICLEntry::Features);
			}
		}
	}
}

Q_DECLARE_INTERFACE (LeechCraft::Plugins::Azoth::Plugins::ICLEntry,
		"org.Deviant.LeechCraft.Plugins.Azoth.Plugins.ICLEntry/1.0");

#endif