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

#ifndef PLUGINS_NETWORKMONITOR_NETWORKMONITOR_H
#define PLUGINS_NETWORKMONITOR_NETWORKMONITOR_H
#include <QDialog>
#include <QTranslator>
#include <interfaces/iinfo.h>
#include <interfaces/itoolbarembedder.h>
#include "ui_networkmonitor.h"

class QSortFilterProxyModel;

namespace LeechCraft
{
	namespace Plugins
	{
		namespace NetworkMonitor
		{
			class RequestModel;

			class Plugin : public QDialog
						 , public IInfo
						 , public IToolBarEmbedder
			{
				Q_OBJECT
				Q_INTERFACES (IInfo IToolBarEmbedder)

				Ui::NetworkMonitor Ui_;
				RequestModel *Model_;
				QSortFilterProxyModel *ProxyModel_;
				QNetworkAccessManager *NetworkAccessManager_;
				QList<QAction*> Actions_;
				std::auto_ptr<QTranslator> Translator_;
			public:
				void Init (ICoreProxy_ptr);
				void SecondInit ();
				void Release ();
				QByteArray GetUniqueID () const;
				QString GetName () const;
				QString GetInfo () const;
				QIcon GetIcon () const;
				QStringList Provides () const;
				QStringList Needs () const;
				QStringList Uses () const;
				void SetProvider (QObject*, const QString&);

				QList<QAction*> GetActions () const;
			public slots:
				void handleCurrentChanged (const QModelIndex&);
				void filterUpdated ();
			};
		};
	};
};

#endif

