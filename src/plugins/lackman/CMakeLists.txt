IF (NOT QT_USE_FILE)
	CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
	IF (COMMAND cmake_policy)
		cmake_policy (SET CMP0003 NEW)
	ENDIF (COMMAND cmake_policy)

	PROJECT (leechcraft_lackman)

	IF (NOT CMAKE_MODULE_PATH)
		SET (CMAKE_MODULE_PATH "/usr/local/share/leechcraft/cmake;/usr/share/leechcraft/cmake")
	ENDIF (NOT CMAKE_MODULE_PATH)

	FIND_PACKAGE (Boost REQUIRED)
	FIND_PACKAGE (Qt4 REQUIRED)
	FIND_PACKAGE (LeechCraft REQUIRED)
ENDIF (NOT QT_USE_FILE)

OPTION (TEST "Enable testing" ON)

SET (QT_USE_QTNETWORK TRUE)
SET (QT_USE_QTXML TRUE)
SET (QT_USE_QTXMLPATTERNS TRUE)
SET (QT_USE_QTSQL TRUE)
IF (TEST)
	SET (QT_USE_QTTEST TRUE)
ENDIF (TEST)
INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIR}
	${LEECHCRAFT_INCLUDE_DIR}
	)
SET (SRCS
	lackman.cpp
	core.cpp
	repoinfo.cpp
	repoinfofetcher.cpp
	storage.cpp
	deptreebuilder.cpp
	packagesmodel.cpp
	packagesdelegate.cpp
	xmlparsers.cpp
	externalresourcemanager.cpp
	pendingmanager.cpp
	packageprocessor.cpp
	versioncomparator.cpp
	)
SET (HEADERS
	lackman.h
	core.h
	repoinfo.h
	repoinfofetcher.h
	storage.h
	deptreebuilder.h
	packagesmodel.h
	packagesdelegate.h
	xmlparsers.h
	externalresourcemanager.h
	pendingmanager.h
	packageprocessor.h
	versioncomparator.h
	)
SET (TRANSLATIONS
	leechcraft_lackman_ru_RU.ts
	leechcraft_lackman_en.ts
	)
SET (COMPILED_TRANSLATIONS
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lackman_ru_RU.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_lackman_en.qm
	)
SET (RESOURCES
	lackmanresources.qrc
	)
SET (FORMS
	lackman.ui
	)

QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
QT4_WRAP_UI (UIS_H ${FORMS})
QT4_ADD_RESOURCES (RCCS ${RESOURCES})

ADD_CUSTOM_COMMAND (OUTPUT
	${COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${TRANSLATIONS}
	DEPENDS ${TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_LIBRARY (leechcraft_lackman SHARED
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${MOC_SRCS}
	${UIS_H}
	${RCCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_lackman
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)

IF (TEST)
	INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR}/tests)
	QT4_WRAP_CPP (VERSIONCOMPARATORTEST_MOC "tests/versioncomparatortest.cpp")
	ADD_EXECUTABLE (lc_lackman_versioncomparatortest WIN32
		tests/versioncomparatortest.cpp
		versioncomparator.cpp
	)
	TARGET_LINK_LIBRARIES (lc_lackman_versioncomparatortest
		${QT_LIBRARIES}
		${LEECHCRAFT_LIBRARIES}
	)
	
	ADD_TEST (VersionComparator lc_lackman_versioncomparatortest)
ENDIF (TEST)

INSTALL (TARGETS leechcraft_lackman DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})