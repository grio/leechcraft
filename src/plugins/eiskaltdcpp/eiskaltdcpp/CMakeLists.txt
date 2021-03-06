project (leechcraft_eiskaltdcpp)
cmake_minimum_required (VERSION 2.6)

set (QT_USE_QTNETWORK TRUE)
set (QT_USE_QTXML TRUE)
set (QT_USE_QTDBUS TRUE)
include (${QT_USE_FILE})
include_directories (${LEECHCRAFT_INCLUDE_DIR}
	${UPNP_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR})

set (HDRS   ArenaWidget.h
            EmoticonObject.h
            Func.h
            HistoryInterface.h
            PMWindow.h
            PoolAlloc.h
            Version.h)
set (SRCS   Antispam.cpp
            AntiSpamFrame.cpp
            ArenaWidget.cpp
            DownloadQueue.cpp
            DownloadQueueModel.cpp
            EmoticonDialog.cpp
            EmoticonFactory.cpp
            FavoriteHubModel.cpp
            FavoriteHubs.cpp
            FavoriteUsers.cpp
            FavoriteUsersModel.cpp
            FileBrowserModel.cpp
            FinishedTransfers.cpp
            FinishedTransfersModel.cpp
            HashProgress.cpp
            HubFrame.cpp
            HubManager.cpp
            IPFilter.cpp
            IPFilterFrame.cpp
            IPFilterModel.cpp
            Magnet.cpp
            MainLayoutWrapper.cpp
            Notification.cpp
            PMWindow.cpp
            PublicHubs.cpp
            PublicHubModel.cpp
            QuickConnect.cpp
            SearchFrame.cpp
            SearchModel.cpp
            Settings.cpp
            SettingsConnection.cpp
            SettingsDownloads.cpp
            SettingsGUI.cpp
            SettingsLog.cpp
            SettingsNotification.cpp
            SettingsPersonal.cpp
            SettingsSharing.cpp
            ShareBrowser.cpp
            ShellCommandRunner.cpp
            SpyFrame.cpp
            SpyModel.cpp
            ToolBar.cpp
            TransferView.cpp
            TransferViewModel.cpp
            UPnP.cpp
            UPnPMapper.cpp
            UserListModel.cpp
            WulforSettings.cpp
            WulforUtil.cpp)
set (MOC_HDRS Antispam.h
            AntiSpamFrame.h
            DownloadQueue.h
            DownloadQueueModel.h
            EmoticonDialog.h
            EmoticonFactory.h
            FavoriteHubModel.h
            FavoriteHubs.h
            FavoriteUsers.h
            FavoriteUsersModel.h
            FileBrowserModel.h
            FinishedTransfers.h
            FinishedTransfersModel.h
            HashProgress.h
            HubFrame.h
            HubManager.h
            IPFilter.h
            IPFilterFrame.h
            IPFilterModel.h
            Magnet.h
            MainLayoutWrapper.h
            Notification.h
            PMWindow.h
            PublicHubs.h
            PublicHubModel.h
            Settings.h
            SettingsConnection.h
            SettingsDownloads.h
            SettingsGUI.h
            SettingsLog.h
            SettingsNotification.h
            SettingsPersonal.h
            SettingsSharing.h
            QuickConnect.h
            SearchFrame.h
            SearchModel.h
            ShareBrowser.h
            ShellCommandRunner.h
            SpyFrame.h
            SpyModel.h
            ToolBar.h
            TransferView.h
            TransferViewModel.h
            UPnP.h
            UPnPMapper.h
            UserListModel.h
            WulforSettings.h
            WulforUtil.h )
set (UI_FORMS ui/UIAbout.ui
            ui/UIAntiSpam.ui
            ui/HubFrame.ui
            ui/PrivateMessage.ui
            ui/UIDownloadQueue.ui
            ui/UIFavoriteHubs.ui
            ui/UIFavoriteHubEditor.ui
            ui/UIFavoriteUsers.ui
            ui/UIFinishedTransfers.ui
            ui/UIHashProgressDialog.ui
            ui/UIIPFilter.ui
            ui/UIMagnet.ui
            ui/UIPublicHubs.ui
            ui/UIQuickConnect.ui
            ui/UISearchFrame.ui
            ui/UISettings.ui
            ui/UISettingsConnection.ui
            ui/UISettingsDownloads.ui
            ui/UISettingsGUI.ui
            ui/UISettingsLog.ui
            ui/UISettingsNotification.ui
            ui/UISettingsPersonal.ui
            ui/UISettingsSharing.ui
            ui/UIShareBrowser.ui
            ui/UISpy.ui
            ui/UITransferView.ui)
set (LANGUAGES en
               ru
               be
               hu)

if (USE_ASPELL)
  set (SRCS ${SRCS} SpellCheck.cpp)
  set (MOC_HDRS ${MOC_HDRS} SpellCheck.h)
endif (USE_ASPELL)

set (COMPILED_TRANSLATIONS ${CMAKE_CURRENT_SOURCE_DIR}/translations/eiskaltdcpp.en.qm
                           ${CMAKE_CURRENT_SOURCE_DIR}/translations/eiskaltdcpp.ru.qm
                           ${CMAKE_CURRENT_SOURCE_DIR}/translations/eiskaltdcpp.be.qm
                           ${CMAKE_CURRENT_SOURCE_DIR}/translations/eiskaltdcpp.hu.qm)

#set (PO_FILES po/ru.po)
#set (PO_FILES po/be.po)

#set (MO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/po/en.mo
#              ${CMAKE_CURRENT_SOURCE_DIR}/po/be.mo
#              ${CMAKE_CURRENT_SOURCE_DIR}/po/ru.mo)

#GETTEXT_CREATE_TRANSLATIONS ( po/ru.po ALL ${PO_FILES})
#GETTEXT_CREATE_TRANSLATIONS ( po/be.po ALL ${PO_FILES})

#Create moc files
QT4_WRAP_CPP (M_SRCS ${MOC_HDRS})
#Create .h files from ui files
QT4_WRAP_UI (U_SRCS ${UI_FORMS})
#Create translations
foreach (LANGUAGE ${LANGUAGES})
        set (TS ${CMAKE_CURRENT_SOURCE_DIR}/translations/eiskaltdcpp.${LANGUAGE}.ts)
        set (QM ${CMAKE_CURRENT_SOURCE_DIR}/translations/eiskaltdcpp.${LANGUAGE}.qm)
        set (TRANSLATIONS ${TRANSLATIONS} ${TS})
        set (TRANSLATIONS_BINARY ${TRANSLATIONS_BINARY} ${QM})
        add_custom_command (
                OUTPUT ${QM}
                COMMAND ${QT_LRELEASE_EXECUTABLE} ${TS}
                MAIN_DEPENDENCY ${TS})
endforeach()

add_custom_target (
        translations_qt
        COMMAND ${QT_LUPDATE_EXECUTABLE} ${HEADERS} ${SOURCES} -ts ${TRANSLATIONS})
add_custom_command (
        TARGET translations_qt
        COMMAND ${QT_LRELEASE_EXECUTABLE} ${TRANSLATIONS})

if (UNIX)
  FIND_PROGRAM (GZIP_CMD gzip)
  EXECUTE_PROCESS (
                OUTPUT_FILE eiskaltdcpp.1.gz
                COMMAND "${GZIP_CMD}" -9
                INPUT_FILE eiskaltdcpp.1
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/)
endif (UNIX)

set (LIBS ${LIBS} ${QT_LIBRARIES} ${UPNP_LIBRARY} ${Boost_LIBRARIES} ${ICONV_LIBRARIES})

if (FREE_SPACE_BAR)
  add_definitions (-DFREE_SPACE_BAR)
  set (LIBS ${LIBS} ${Boost_LIBRARIES})
endif (FREE_SPACE_BAR)

if (USE_ASPELL)
  add_definitions (-DUSE_ASPELL ${ASPELL_DEFINITIONS})
  include_directories (${ASPELL_INCLUDE_DIR})
  set (LIBS ${LIBS} ${ASPELL_LIBRARIES})
endif (USE_ASPELL)

if (APPLE)
  # set how it shows up in the Info.plist file
  SET(MACOSX_BUNDLE_ICON_FILE icon_128x128.icns)
  # set where in the bundle to put the icns file
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/icons/icon_128x128.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # include the icns file in the target
  SET(SRCS ${SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/icons/icon_128x128.icns)
endif (APPLE)

add_library (leechcraft_eiskaltdcpp SHARED
                ${M_SRCS}
                ${U_SRCS}
                ${SRCS}
                ${HDRS}
                ${MOC_HDRS}
                ${TRANSLATIONS}
                translations)
add_definitions (-I. -I.. -DCLIENT_DATA_DIR="${CLIENT_DATA_DIR}" -DCLIENT_SOUNDS_DIR="${CLIENT_SOUNDS_DEST}" -DCLIENT_TRANSLATIONS_DIR="${CLIENT_TRANSLATIONS_DEST}" -DCLIENT_ICONS_DIR="${CLIENT_ICONS_DEST}" )

if (APPLE)
target_link_libraries (leechcraft_eiskaltdcpp ${LIBS} dcpp iconv intl upnp ixml)
else (APPLE)
target_link_libraries (leechcraft_eiskaltdcpp ${LEECHCRAFT_LIBRARIES} ${LIBS} dcpp)
endif (APPLE)

  set_property (TARGET leechcraft_eiskaltdcpp PROPERTY OUTPUT_NAME ${PROJECT_NAME_GLOBAL})
  install (TARGETS leechcraft_eiskaltdcpp DESTINATION ${LC_PLUGINS_DEST})

if (NOT APPLE)

  install (DIRECTORY icons/appl icons/emot icons/user DESTINATION ${CLIENT_ICONS_DEST} PATTERN ".svn" EXCLUDE)
  install (FILES icons/icon_128x128.png DESTINATION share/icons/hicolor/128x128/apps RENAME eiskaltdcpp.png)
  install (FILES icons/icon_64x64.png DESTINATION share/icons/hicolor/64x64/apps RENAME eiskaltdcpp.png)
  install (FILES icons/icon_32x32.png DESTINATION share/icons/hicolor/32x32/apps RENAME eiskaltdcpp.png)
  install (FILES icons/icon_48x48.png DESTINATION share/icons/hicolor/48x48/apps RENAME eiskaltdcpp.png)
  install (FILES icons/icon_22x22.png DESTINATION share/icons/hicolor/22x22/apps RENAME eiskaltdcpp.png)
  install (FILES icons/icon_24x24.png DESTINATION share/icons/hicolor/24x24/apps RENAME eiskaltdcpp.png)
  install (FILES icons/icon_16x16.png DESTINATION share/icons/hicolor/16x16/apps RENAME eiskaltdcpp.png)
  install (FILES icons/icon_128x128.png DESTINATION share/pixmaps/ RENAME eiskaltdcpp.png)
  install (FILES ../eiskaltdcpp.desktop DESTINATION share/applications)
  install (FILES ${TRANSLATIONS_BINARY} DESTINATION ${CLIENT_TRANSLATIONS_DEST})
  INSTALL (FILES eiskaltdcpp.1.gz DESTINATION share/man/man1)

endif (NOT APPLE)
