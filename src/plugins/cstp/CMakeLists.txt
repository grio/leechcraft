IF (NOT QT_USE_FILE)
	CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
	IF (COMMAND cmake_policy)
		cmake_policy (SET CMP0003 NEW)
	ENDIF (COMMAND cmake_policy)

	PROJECT (leechcraft_cstp)

	IF (NOT CMAKE_MODULE_PATH)
		SET (CMAKE_MODULE_PATH "/usr/local/share/leechcraft/cmake;/usr/share/leechcraft/cmake")
	ENDIF (NOT CMAKE_MODULE_PATH)

	FIND_PACKAGE (Boost REQUIRED)
	FIND_PACKAGE (Qt4 REQUIRED)
	FIND_PACKAGE (LeechCraft REQUIRED)
ENDIF (NOT QT_USE_FILE)

SET (QT_USE_QTNETWORK TRUE)
INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${LEECHCRAFT_INCLUDE_DIR}
	)

SET (SRCS
	cstp.cpp
	core.cpp
	task.cpp
	hook.cpp
	addtask.cpp
	xmlsettingsmanager.cpp
	mainviewdelegate.cpp
	morphfile.cpp
	)
SET (HEADERS
	cstp.h
	core.h
	task.h
	hook.h
	addtask.h
	xmlsettingsmanager.h
	mainviewdelegate.h
	morphfile.h
	)
SET (FORMS
	addtask.ui
	)
SET (RESOURCES
	cstpresources.qrc
	)
SET (TRANSLATIONS
	leechcraft_cstp_ru_RU.ts
	leechcraft_cstp_uk_UA.ts
	leechcraft_cstp_ar.ts
	leechcraft_cstp_de.ts
	leechcraft_cstp_eo.ts
	leechcraft_cstp_es.ts
	leechcraft_cstp_fr.ts
	leechcraft_cstp_oc.ts
	leechcraft_cstp_en.ts
	leechcraft_cstp_pl.ts
	)
SET (COMPILED_TRANSLATIONS
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_ru_RU.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_uk_UA.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_ar.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_de.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_eo.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_es.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_fr.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_oc.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_en.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_cstp_pl.qm
	)
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_GUI_LIB)
QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
QT4_WRAP_UI (UIS_H ${FORMS})
QT4_ADD_RESOURCES (RCCS ${RESOURCES})

ADD_CUSTOM_COMMAND (OUTPUT
	${COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${TRANSLATIONS}
	DEPENDS ${TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_LIBRARY (leechcraft_cstp SHARED
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${MOC_SRCS}
	${UIS_H}
	${RCCS})
LINK_DIRECTORIES (${Boost_LIBRARY_DIR})
TARGET_LINK_LIBRARIES (leechcraft_cstp
                        ${QT_LIBRARIES}
						${LEECHCRAFT_LIBRARIES}
						)
INSTALL (TARGETS leechcraft_cstp DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
INSTALL (FILES cstpsettings.xml DESTINATION ${LC_SETTINGS_DEST})
