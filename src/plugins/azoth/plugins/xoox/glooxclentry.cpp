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

#include "glooxclentry.h"
#include <QStringList>
#include <QtDebug>
#include <gloox/rosteritem.h>
#include <gloox/resource.h>
#include <interfaces/iaccount.h>
#include "glooxaccount.h"

namespace LeechCraft
{
	namespace Plugins
	{
		namespace Azoth
		{
			namespace Plugins
			{
				namespace Xoox
				{
					GlooxCLEntry::GlooxCLEntry (gloox::RosterItem *ri, GlooxAccount *parent)
					: QObject (parent)
					, ParentAccount_ (qobject_cast<IAccount*> (parent))
					, ParentAccountObject_ (parent)
					, RI_ (ri)
					{
					}

					QObject* GlooxCLEntry::GetObject ()
					{
						return this;
					}

					IAccount* GlooxCLEntry::GetParentAccount () const
					{
						return ParentAccount_;
					}

					ICLEntry::Features GlooxCLEntry::GetEntryFeatures () const
					{
						return FPermanentEntry | FSupportsRenames | FIsChat;
					}

					QString GlooxCLEntry::GetEntryName () const
					{
						std::string name = RI_->name ();
						if (!name.size ())
							name = RI_->jid ();
						return QString::fromUtf8 (name.c_str ());
					}

					void GlooxCLEntry::SetEntryName (const QString& name)
					{
						RI_->setName (name.toUtf8 ().constData ());
						ParentAccount_->Synchronize ();
					}

					QByteArray GlooxCLEntry::GetEntryID () const
					{
						return RI_->jid ().c_str ();
					}

					QStringList GlooxCLEntry::Groups () const
					{
						QStringList result;
						Q_FOREACH (const std::string& string, RI_->groups ())
							result << QString::fromUtf8 (string.c_str ());
						return result;
					}

					QStringList GlooxCLEntry::Variants () const
					{
						QStringList result;
						gloox::RosterItem::ResourceMap rmap = RI_->resources ();
						for (gloox::RosterItem::ResourceMap::const_iterator i = rmap.begin (),
								end = rmap.end (); i != end; ++i)
							result << QString::fromUtf8 (i->second->message ().c_str ());
						return result;
					}

					IMessage* GlooxCLEntry::CreateMessage (IMessage::MessageType type,
							const QString& variant, const QString& msg)
					{
						return ParentAccountObject_->CreateMessage (type, variant, msg, RI_);
					}

					void GlooxCLEntry::ReemitMessage (IMessage *msg)
					{
						emit gotMessage (msg);
					}
				}
			}
		}
	}
}
