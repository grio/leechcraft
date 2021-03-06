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
#include "imessage.h"

namespace LeechCraft
{
	namespace Plugins
	{
		namespace Azoth
		{
			namespace Plugins
			{
				class IAccount;
				class IMessage;

				/** @brief Represents a single entry in contact list.
				 *
				 * Everything that should go to the contact list should
				 * implement this interface: plain contacts,
				 * metacontacts, transports to other accounts in
				 * protocols like XMPP, and such.
				 *
				 * By default, entries are considered to be normal chat
				 * entries, and Azoth core would manage the chat widget
				 * (and, consequently, chat tabs) for such entries
				 * itself. Nevertheless, some entries may want to have
				 * custom behavior for their widgets or even have no
				 * widgets at all. In this case, they should return
				 * FHasCustomChatWidget from GetEntryFeatures(). In this
				 * case Azoth core wouldn't care about their chat
				 * widgets at all. Instead, such entry would need to
				 * implement IMultiTabsWidget, for example, and the
				 * corresponding plugin would need to be a IMultiTabs.
				 */
				class ICLEntry
				{
				public:
					virtual ~ICLEntry () {}

					/** Represents the features that may be supproted by
					 * a contant list entry.
					 */
					enum Feature
					{
						FPermanentEntry			= 0x0001, //!< FPermanentEntry If this entry is permanent and would appear in the next session too.
						FSessionEntry			= 0x0002, //!< FSessionEntry If this entry is not permament and is for this session only.
						FIsChat					= 0x0040, //!< FIsChat This entry represents a standard chat.
						FIsMUC					= 0x0400, //!< FIsMUC This entry represents a multi-user chatroom.
						FIsPrivateChat			= 0x4000, //!< FIsPrivateChat This entry represents a private conversation in a multi-user chatroom.
						FSupportsRenames		= 0x0010, //!< FSupportsRenames This entry supports renaming, so calls to SetEntryName() are not senseless.
						FHasCustomChatWidget	= 0x0020  //!< FHasCustomChatWidget This entry has a custom chat widget.
					};

					Q_DECLARE_FLAGS (Features, Feature);

					/** Returns the entry as a QObject.
					 *
					 * @return Contact list entry as QObject.
					 */
					virtual QObject* GetObject () = 0;

					/** Returns the pointer to the parent account that
					 * this entry belongs to.
					 *
					 * @return The parent account of this entry.
					 */
					virtual IAccount* GetParentAccount () const = 0;

					/** Returns the OR-ed combination of Feature flags
					 * that describes the features supported by this
					 * contact list entry.
					 *
					 * @return The features supported by this entry.
					 */
					virtual Features GetEntryFeatures () const = 0;

					/** Returns the human-readable name of this entry.
					 *
					 * @return Human-readable name of this entry.
					 *
					 * @sa SetEntryName()
					 */
					virtual QString GetEntryName () const = 0;

					/** @brief Sets the human-readable name of this
					 * entry to a new value.
					 *
					 * The name is expected to be updated on the server
					 * immediately.
					 *
					 * @param[in] name The new human-readable name of
					 * this entry.
					 *
					 * @sa GetEntryName()
					 */
					virtual void SetEntryName (const QString& name) = 0;

					/** @brief Returns the ID of this entry.
					 *
					 * The ID must be unique among all entries of this
					 * protocol and should not depend on the value
					 * returned by GetEntryName() (the human-readable
					 * name).
					 *
					 * @return The unique and persistent ID of this
					 * entry.
					 */
					virtual QByteArray GetEntryID () const = 0;

					/** @brief Returns the list of human-readable names
					 * of the groups that this entry belongs to.
					 *
					 * @return The list of groups of this item.
					 */
					virtual QStringList Groups () const = 0;

					/** @brief Returns the list of destination variants
					 * for this entry.
					 *
					 * For example, for an entry representing a contact
					 * in XMPP protocol variants would be the list of
					 * resources for the contact.
					 *
					 * Strings in the list should not be null, though
					 * they may be empty.
					 *
					 * @return The list of variants of this entry.
					 */
					virtual QStringList Variants () const = 0;

					/** @brief Creates the message of the given type to
					 * the given variant.
					 *
					 * Variant is a string from the list returned by
					 * Variants(). If a different string is passed,
					 * particularly, a null one, the implementation must
					 * choose the best variant itself: for example, the
					 * resource with the highest priority in XMPP.
					 *
					 * @param[in] type The type of the message.
					 * @param[in] variant The variant to send to.
					 * @param[in] body Message boxy.
					 * @return The prepared message.
					 *
					 * @sa Variants()
					 */
					virtual IMessage* CreateMessage (IMessage::MessageType type,
							const QString& variant,
							const QString& body) = 0;
				};

				Q_DECLARE_OPERATORS_FOR_FLAGS (ICLEntry::Features);
			}
		}
	}
}

Q_DECLARE_INTERFACE (LeechCraft::Plugins::Azoth::Plugins::ICLEntry,
		"org.Deviant.LeechCraft.Plugins.Azoth.Plugins.ICLEntry/1.0");

#endif
