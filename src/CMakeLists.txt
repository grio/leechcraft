CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
IF (COMMAND cmake_policy)
	cmake_policy (SET CMP0003 NEW)
ENDIF (COMMAND cmake_policy)
PROJECT (leechcraft)
SET (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET (CMAKE_BUILD_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET (CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
LIST (APPEND CMAKE_MODULE_PATH "cmake")
IF (MSVC)
	SET (CMAKE_CXX_FLAGS "/GX")
	SET (CMAKE_EXE_LINKER_FLAGS "/entry:mainCRTStartup")
ENDIF (MSVC)
IF (UNIX)
	SET (CMAKE_CXX_FLAGS "-fvisibility=hidden -Wall -Wextra")
ENDIF (UNIX)
IF (WIN32 AND NOT MSVC)
	SET (CMAKE_CXX_FLAGS "-Wall")
ENDIF (WIN32 AND NOT MSVC)
SET (LC_PLUGINS_DEST "lib/leechcraft/plugins")
# Check modules first in local dir
SET (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE (Boost REQUIRED COMPONENTS date_time filesystem system thread signals)
FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Threads REQUIRED)
#FIND_PACKAGE (Phonon)
FIND_PACKAGE (RBTorrent)
#FIND_PACKAGE (Wt)

IF (ENABLE_PACK)
	INCLUDE (InstallRequiredSystemLibraries)
	SET (CPACK_CMAKE_GENERATOR "Unix Makefiles")
	SET (CPACK_PACKAGE_NAME "leechcraft")
	SET (CPACK_GENERATOR "DEB;RPM")
	FIND_PROGRAM (DPKG_CMD dpkg)
		IF (NOT DPKG_CMD)
		MESSAGE (STATUS "Can not find dpkg in your path, default to i386.")
		SET (CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
	ENDIF (NOT DPKG_CMD)
	EXECUTE_PROCESS (COMMAND "${DPKG_CMD}" --print-architecture
		OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
		OUTPUT_STRIP_TRAILING_WHITESPACE
		)
	SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Extensible crossplatform download manager")
	SET (CPACK_PACKAGE_VENDOR "0xd34df00d")
	SET (CPACK_DEBIAN_PACKAGE_SECTION "net")
	SET (CPACK_DEBIAN_PACKAGE_DESCRIPTION "Extensible crossplatform download manager
 LeechCraft is a cross-platform extensibe download manager written
 in C++. Currently it offers full-featured BitTorrent client, feed
 reader, HTTP/FTP plugin, Remote access and much more. It also aims
 to be resource-efficient, working quite well on even old computers.")
	SET (CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-core (>= 4.4.3), libqt4-gui (>= 4.4.3), libqt4-network (>= 4.4.3), libqt4-webkit (>= 4.4.3), libqt4-xml (>= 4.4.3), libqt4-sql (>= 4.4.3), libqt4-script (>= 4.4.3), libphonon4, libtorrent-rasterbar-0.15, leechcraft-icons-oxygen (>= 0.3.0) | leechcraft-icons-tango (>= 0.3.0)")
	SET (CPACK_PACKAGE_DESCRIPTION_FILE	"${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
	SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/copyright.txt")
	SET (CPACK_PACKAGE_VERSION_MAJOR "0")
	SET (CPACK_PACKAGE_VERSION_MINOR "2")
	FIND_PACKAGE (Subversion)
	IF (Subversion_FOUND)
		Subversion_WC_INFO (${PROJECT_SOURCE_DIR} ProjectSVNInfo)
		SET (CPACK_PACKAGE_VERSION_PATCH ${ProjectSVNInfo_WC_LAST_CHANGED_REV})
	ELSE (Subversion_FOUND)
		SET (CPACK_PACKAGE_VERSION_PATCH "9999_svn")
	ENDIF (Subversion_FOUND)
	SET (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
	SET (CPACK_PACKAGE_CONTACT "0xd34df00d@gmail.com")
	SET (CPACK_DEBIAN_PACKAGE_MAINTAINER "Rudoy Georg <0xd34df00d@gmail.com>")
	SET (CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/scripts/postinst")

	SET (CPACK_SOURCE_GENERATOR "TGZ;ZIP;TBZ2")
	SET (CPACK_SOURCE_IGNORE_FILES "/.svn/;.log")
	SET (CPACK_PACKAGE_EXECUTABLES "leechcraft" "LeechCraft")
	SET (CPACK_STRIP_FILES TRUE)
	INCLUDE (CPack)
ENDIF (ENABLE_PACK)

ADD_SUBDIRECTORY (plugininterface)
ADD_SUBDIRECTORY (xmlsettingsdialog)

SET (LEECHCRAFT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

IF (NOT ENABLE_CONFIGURABLE)
	IF (RBTorrent_FOUND)
	   	ADD_SUBDIRECTORY (plugins/torrent)
	ELSE (RBTorrent_FOUND)
		MESSAGE (STATUS "Rasterbar libtorrent not found, not building BitTorrent")
   	ENDIF (RBTorrent_FOUND)
	ADD_SUBDIRECTORY (plugins/aggregator)
	ADD_SUBDIRECTORY (plugins/poshuku)
	ADD_SUBDIRECTORY (plugins/nufella)
	ADD_SUBDIRECTORY (plugins/chatter)
	ADD_SUBDIRECTORY (plugins/cstp)
#	IF (Wt_FOUND)
#		ADD_SUBDIRECTORY (plugins/remoter)
#	ELSE (Wt_FOUND)
#		MESSAGE (STATUS "Wt isn't found, not building Remoter")
#	ENDIF (Wt_FOUND)
	IF (QT_PHONON_FOUND)
		ADD_SUBDIRECTORY (plugins/lmp)
	ENDIF (QT_PHONON_FOUND)
	IF (QT_QTDBUS_FOUND)
		ADD_SUBDIRECTORY (plugins/dbusmanager)
	ENDIF (QT_QTDBUS_FOUND)
	ADD_SUBDIRECTORY (plugins/deadlyrics)
ELSE (NOT ENABLE_CONFIGURABLE)
	IF (ENABLE_HTTP)
		ADD_SUBDIRECTORY (plugins/cstp)
	ENDIF (ENABLE_HTTP)
	IF (ENABLE_TORRENT)
		ADD_SUBDIRECTORY (plugins/torrent)
	ENDIF (ENABLE_TORRENT)
#	IF (ENABLE_REMOTER)
#		ADD_SUBDIRECTORY (plugins/remoter)
#	ENDIF (ENABLE_REMOTER)
	IF (ENABLE_AGGREGATOR)
		ADD_SUBDIRECTORY (plugins/aggregator)
	ENDIF (ENABLE_AGGREGATOR)
	IF (ENABLE_POSHUKU)
		ADD_SUBDIRECTORY (plugins/poshuku)
	ENDIF (ENABLE_POSHUKU)
	IF (ENABLE_NUFELLA)
		ADD_SUBDIRECTORY (plugins/nufella)
	ENDIF (ENABLE_NUFELLA)
#	IF (ENABLE_CHATTER)
#		ADD_SUBDIRECTORY (plugins/chatter)
#	ENDIF (ENABLE_CHATTER)
	IF (ENABLE_DBUSMANAGER)
		ADD_SUBDIRECTORY (plugins/dbusmanager)
	ENDIF (ENABLE_DBUSMANAGER)
	IF (ENABLE_LMP)
		ADD_SUBDIRECTORY (plugins/lmp)
	ENDIF (ENABLE_LMP)
	IF (ENABLE_DEADLYRICS)
		ADD_SUBDIRECTORY (plugins/deadlyrics)
	ENDIF (ENABLE_DEADLYRICS)
ENDIF (NOT ENABLE_CONFIGURABLE)

SET (QT_USE_QTNETWORK TRUE)
SET (QT_USE_QTXML TRUE)
SET (QT_USE_QTSQL TRUE)
SET (QT_USE_QTSCRIPT TRUE)
INCLUDE (${QT_USE_FILE})

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIR}
	.
	plugininterface/)

SET (SRCS
	mainwindow.cpp
	commonjobadder.cpp
	core.cpp
	main.cpp
	pluginmanager.cpp
	xmlsettingsmanager.cpp
	filtermodel.cpp
	pluginmanagerdialog.cpp
	fancypopupmanager.cpp
	skinengine.cpp
	childactioneventfilter.cpp
	tabcontainer.cpp
	zombitechstyle.cpp
	logtoolbox.cpp
	historytreeview.cpp
	settingssink.cpp
	customcookiejar.cpp
	authenticationdialog.cpp
	sslerrorsdialog.cpp
	storagebackend.cpp
	sqlstoragebackend.cpp
	debugmessagehandler.cpp
	application.cpp
	iconchooser.cpp
	)
SET (HEADERS
	mainwindow.h
	commonjobadder.h
	core.h
	pluginmanager.h
	xmlsettingsmanager.h
	filtermodel.h
	pluginmanagerdialog.h
	fancypopupmanager.h
	skinengine.h
	childactioneventfilter.h
	tabcontainer.h
	zombitechstyle.h
	logtoolbox.h
	historytreeview.h
	settingssink.h
	customcookiejar.h
	authenticationdialog.h
	sslerrorsdialog.h
	storagebackend.h
	sqlstoragebackend.h
	debugmessagehandler.h
	application.h
	iconchooser.h
	interfaces/iinfo.h
	interfaces/iwindow.h
	interfaces/idownload.h
	interfaces/iwantnetworkaccessmanager.h
	interfaces/ihavesettings.h
	interfaces/icustomprovider.h
	interfaces/imultitabs.h
	interfaces/iembedtab.h
	interfaces/itaggablejobs.h
	interfaces/ijobholder.h
	interfaces/iimportexport.h
	interfaces/ifinder.h
	interfaces/iwebbrowser.h
	interfaces/structures.h
	)
SET (FORMS
	commonjobadder.ui
	leechcraft.ui
	pluginmanagerdialog.ui
	logtoolbox.ui
	settingssink.ui
	authenticationdialog.ui
	sslerrorsdialog.ui
	)
SET (RESOURCES mainresources.qrc)
SET (TRANSLATIONS
	leechcraft_ru_RU.ts
	leechcraft_uk_UA.ts
	)
SET (COMPILED_TRANSLATIONS
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_ru_RU.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_uk_UA.qm
	)

ADD_DEFINITIONS (-DQT_GUI_LIB)
ADD_DEFINITIONS (-D_REENTRANT)

QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
QT4_WRAP_UI (UIS_H ${FORMS})
QT4_ADD_RESOURCES (RCC ${RESOURCES})

ADD_CUSTOM_COMMAND (OUTPUT
	${COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${TRANSLATIONS}
	DEPENDS ${TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_EXECUTABLE (leechcraft WIN32
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${MOC_SRCS}
	${UIS_H}
	${RCC})
TARGET_LINK_LIBRARIES(leechcraft
						${QT_LIBRARIES}
						plugininterface
						xmlsettingsdialog)
INSTALL (TARGETS leechcraft DESTINATION bin)

INSTALL (DIRECTORY freedesktop/icons/ DESTINATION share/icons/hicolor
	PATTERN ".svn" EXCLUDE)
IF (UNIX)
	INSTALL (FILES freedesktop/leechcraft.desktop DESTINATION share/applications)
ENDIF (UNIX)