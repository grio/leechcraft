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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H
#include <boost/shared_ptr.hpp>
#include <QAbstractItemModel>
#include <QMap>
#include <QMultiMap>
#include <QStringList>
#include <QPluginLoader>
#include <QIcon>
#include "interfaces/iinfo.h"

namespace LeechCraft
{
	class MainWindow;
	class PluginManager : public QAbstractItemModel
						, public IPluginsManager
	{
		Q_OBJECT
		Q_INTERFACES (IPluginsManager);

		struct DepTreeItem;
		typedef boost::shared_ptr<DepTreeItem> DepTreeItem_ptr;
		struct DepTreeItem
		{
			QObject *Plugin_;
			bool Initialized_;
			// This plugin depends upon.
			QMultiMap<QString, DepTreeItem_ptr> Needed_;
			QMultiMap<QString, DepTreeItem_ptr> Used_;
			// What depends on this plugin.
			QList<DepTreeItem_ptr> Belongs_;

			DepTreeItem ();
			void Print (int = 0);
		};

		struct Finder
		{
			QObject *Object_;

			Finder (QObject*);
			bool operator() (DepTreeItem_ptr) const;
		};

		// No plugins depend on these, thus, these's Belongs_ field is
		// empty.
		QList<DepTreeItem_ptr> Roots_;

		typedef boost::shared_ptr<QPluginLoader> QPluginLoader_ptr;
		typedef QList<QPluginLoader_ptr> PluginsContainer_t;

		// Only currently loaded plugins
		mutable PluginsContainer_t PluginContainers_;
		typedef QList<QObject*> Plugins_t;
		mutable Plugins_t Plugins_;

		// All plugins ever seen
		PluginsContainer_t AvailablePlugins_;
		QMap<QString, PluginsContainer_t::const_iterator> FeatureProviders_;

		typedef QList<PluginsContainer_t::iterator> UnloadQueue_t;
		UnloadQueue_t UnloadQueue_;

		QStringList Headers_;

		QIcon DefaultPluginIcon_;

		QStringList PluginLoadErrors_;
	public:
		typedef PluginsContainer_t::size_type Size_t;
		PluginManager (QObject *parent = 0);
		virtual ~PluginManager ();

		virtual int columnCount (const QModelIndex& = QModelIndex ()) const;
		virtual QVariant data (const QModelIndex&, int = Qt::DisplayRole) const;
		virtual Qt::ItemFlags flags (const QModelIndex&) const;
		virtual QVariant headerData (int, Qt::Orientation, int = Qt::DisplayRole) const;
		virtual QModelIndex index (int, int, const QModelIndex& = QModelIndex()) const;
		virtual QModelIndex parent (const QModelIndex&) const;
		virtual int rowCount (const QModelIndex& = QModelIndex ()) const;
		virtual bool setData (const QModelIndex&, const QVariant&, int);

		Size_t GetSize () const;
		void Init ();
		void Release ();
		QString Name (const Size_t& pos) const;
		QString Info (const Size_t& pos) const;

		QObjectList GetAllPlugins () const;
		QString GetPluginLibraryPath (const QObject*) const;

		void InjectPlugin (QObject *object);
		void ReleasePlugin (QObject *object);

		QObject* GetObject ();

		QObject* GetProvider (const QString&) const;
		void Unload (QObject*);

		const QStringList& GetPluginLoadErrors () const;
	private:
		void FindPlugins ();
		void ScanDir (const QString&);
		/** Tries to load all the plugins and filters out those who fail
		 * various sanity checks.
		 */
		void CheckPlugins ();

		QList<Plugins_t::iterator> FindProviders (const QString&);
		QList<Plugins_t::iterator> FindProviders (const QSet<QByteArray>&);

		/** Returns dependency item that matches the given object.
		 */
		DepTreeItem_ptr GetDependency (QObject *object);

		/** Calculates the deps.
		 */
		void CalculateDependencies ();
		DepTreeItem_ptr CalculateSingle (Plugins_t::iterator);
		DepTreeItem_ptr CalculateSingle (QObject*, Plugins_t::iterator);

		/** Preinitializes the plugins, pushes second-level plugins to
		 * first-level ones and calls IInfo::Init() on each one.
		 */
		void InitializePlugins ();
		bool InitializeSingle (DepTreeItem_ptr);
		void Release (DepTreeItem_ptr);
		void DumpTree ();
		DepTreeItem_ptr FindTreeItem (QObject*);
		PluginsContainer_t::iterator Find (DepTreeItem_ptr);
		PluginsContainer_t::iterator Find (QObject*);
		void Unload (PluginsContainer_t::iterator);
	private slots:
		void processUnloadQueue ();
	signals:
		void pluginInjected (QObject*);
	};
};

#endif

