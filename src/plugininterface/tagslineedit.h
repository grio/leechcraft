/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2006-2009  Georg Rudoy
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

#ifndef PLUGININTERFACE_TAGSLINEEDIT_H
#define PLUGININTERFACE_TAGSLINEEDIT_H
#include <memory>
#include <QLineEdit>
#include "piconfig.h"
#include "categoryselector.h"

namespace LeechCraft
{
	namespace Util
	{
		class TagsCompleter;

		/** @brief A line edit class suitable for use with TagsCompleter.
		 *
		 * One would need this extra class because of custom behavior of both
		 * tags completer and line edit semantics.
		 *
		 * @sa TagsCompleter
		 */
		class TagsLineEdit : public QLineEdit
		{
			Q_OBJECT

			friend class TagsCompleter;

			std::auto_ptr<CategorySelector> CategorySelector_;
			TagsCompleter *Completer_;
		public:
			/** @brief Constructs the line edit widget.
			 *
			 * Creates the line edit widget.
			 *
			 * @param[in] parent Parent widget.
			 */
			PLUGININTERFACE_API TagsLineEdit (QWidget *parent);
			/** @brief Adds the selector widget to the line edit.
			 *
			 * Because this function uses the completion model, it should be
			 * used after the association with a TagsCompleter.
			 *
			 * @sa TagsCompleter
			 */
			PLUGININTERFACE_API void AddSelector ();
		public slots:
			/** @brief Completes the string.
			 *
			 * Completes the current text in line edit with completion passed
			 * throught string parameter.
			 *
			 * @param[in] string String with completion.
			 */
			PLUGININTERFACE_API void insertTag (const QString& string);
		private slots:
			void handleTagsUpdated (const QStringList&);
			void handleSelectionChanged (const QStringList&);
		protected:
			virtual void keyPressEvent (QKeyEvent*);
			virtual void focusInEvent (QFocusEvent*);
			virtual void contextMenuEvent (QContextMenuEvent*);
			void SetCompleter (TagsCompleter*);
		private:
			QString textUnderCursor () const;
		signals:
			PLUGININTERFACE_API void tagsChosen ();
		};
	};
};

#endif

