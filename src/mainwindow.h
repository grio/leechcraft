#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDialog>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QList>
#include <QModelIndex>
#include "ui_leechcraft.h"

class QLabel;
class QSplashScreen;
class QDockWidget;
class QTabWidget;
class QModelIndex;

namespace LeechCraft
{
	namespace Util
	{
		class GraphWidget;
		class XmlSettingsDialog;
	};

	class Core;
	class PluginInfo;
	class PluginManagerDialog;
	class FancyPopupManager;
	class SettingsSink;
	class LogToolBox;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

		Ui::LeechCraft Ui_;

		QSystemTrayIcon *TrayIcon_;
		QLabel *DownloadSpeed_;
		QLabel *UploadSpeed_;
		QLabel *Clock_;
		Util::GraphWidget *DSpeedGraph_;
		Util::GraphWidget *USpeedGraph_;
		LeechCraft::Util::XmlSettingsDialog *XmlSettingsDialog_;
		SettingsSink *SettingsSink_;
		PluginManagerDialog *PluginManagerDialog_;
		FancyPopupManager *FancyPopupManager_;
		LeechCraft::LogToolBox *LogToolBox_;
		QSplashScreen *SplashScreen_;
		bool IsShown_;
		bool WasMaximized_;
	public:
		MainWindow (QWidget *parent = 0, Qt::WFlags flags = 0);
		virtual ~MainWindow ();
		QModelIndexList GetSelectedRows () const;
		QTabWidget* GetTabWidget () const;
	public slots:
		void catchError (QString);
	protected:
		virtual void closeEvent (QCloseEvent*);
	private:
		void InitializeInterface ();
		void SetStatusBar ();
		void SetTrayIcon ();
		void ReadSettings ();
		void WriteSettings ();
	private slots:
		void on_ActionAddTask__triggered ();
		void on_ActionSettings__triggered ();
		void on_ActionQuit__triggered ();
		void on_ActionFullscreenMode__triggered (bool);
		void on_ActionLogger__triggered ();
		void updatePanes (const QItemSelection&, const QItemSelection&);
		void updateSpeedIndicators ();
		void updateClock ();
		void showHideMain ();
		void handleTrayIconActivated (QSystemTrayIcon::ActivationReason);
		void handleDownloadFinished (const QString&);
		void filterParametersChanged ();
		void historyFilterParametersChanged ();
		void updateIconSet ();
		void on_ActionPluginManager__triggered ();
		void historyActivated (const QModelIndex&);
		void handleLoadProgress (const QString&);
	};
};

#endif
