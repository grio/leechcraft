IF (NOT QT_USE_FILE)
	CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
	IF (COMMAND cmake_policy)
		cmake_policy (SET CMP0003 NEW)
	ENDIF (COMMAND cmake_policy)

	PROJECT (leechcraft_auscrie)

	IF (NOT CMAKE_MODULE_PATH)
		SET (CMAKE_MODULE_PATH "/usr/local/share/leechcraft/cmake;/usr/share/leechcraft/cmake")
	ENDIF (NOT CMAKE_MODULE_PATH)

	FIND_PACKAGE (Boost REQUIRED)
	FIND_PACKAGE (Qt4 REQUIRED)
	FIND_PACKAGE (LeechCraft REQUIRED)
ENDIF (NOT QT_USE_FILE)

SET (QT_USE_QTNETWORK TRUE)
INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIR}
	${LEECHCRAFT_INCLUDE_DIR}
	)
SET (SRCS
	auscrie.cpp
	shooterdialog.cpp
	poster.cpp
	requestbuilder.cpp
	)
SET (HEADERS
	auscrie.h
	shooterdialog.h
	poster.h
	requestbuilder.h
	)
SET (FORMS
	shooterdialog.ui
	)
SET (TRANSLATIONS
	leechcraft_auscrie_ru_RU.ts
	leechcraft_auscrie_uk_UA.ts
	leechcraft_auscrie_de.ts
	leechcraft_auscrie_es.ts
	leechcraft_auscrie_oc.ts
	leechcraft_auscrie_en.ts
	)
SET (RESOURCES auscrieresources.qrc)
SET (COMPILED_TRANSLATIONS
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_auscrie_ru_RU.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_auscrie_uk_UA.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_auscrie_de.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_auscrie_es.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_auscrie_oc.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_auscrie_en.qm
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
ADD_LIBRARY (leechcraft_auscrie SHARED
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${MOC_SRCS}
	${UIS_H}
	${RCCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_auscrie
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_auscrie DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
