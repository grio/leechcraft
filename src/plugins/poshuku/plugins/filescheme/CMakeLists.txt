INCLUDE_DIRECTORIES (${POSHUKU_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})
SET (FILESCHEME_SRCS
	filescheme.cpp
	schemereply.cpp
	)
SET (FILESCHEME_HEADERS
	filescheme.h
	schemereply.h
	)
SET (FILESCHEME_RESOURCES
	poshukufileschemeresources.qrc
	)
SET (FILESCHEME_TRANSLATIONS
	leechcraft_poshuku_filescheme_ru_RU.ts
	leechcraft_poshuku_filescheme_uk_UA.ts
	leechcraft_poshuku_filescheme_ar.ts
	leechcraft_poshuku_filescheme_de.ts
	leechcraft_poshuku_filescheme_en.ts
	leechcraft_poshuku_filescheme_eo.ts
	leechcraft_poshuku_filescheme_es.ts
	leechcraft_poshuku_filescheme_fr.ts
	leechcraft_poshuku_filescheme_pl.ts
	)
SET (FILESCHEME_COMPILED_TRANSLATIONS
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_ru_RU.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_uk_UA.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_ar.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_de.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_en.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_eo.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_es.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_fr.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_filescheme_pl.qm
	)
QT4_WRAP_CPP (FILESCHEME_MOC_SRCS ${FILESCHEME_HEADERS})
QT4_ADD_RESOURCES (FILESCHEME_RCCS ${FILESCHEME_RESOURCES})
ADD_CUSTOM_COMMAND (OUTPUT
	${FILESCHEME_COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${FILESCHEME_TRANSLATIONS}
	DEPENDS ${FILESCHEME_TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_LIBRARY (leechcraft_poshuku_filescheme SHARED
	${FILESCHEME_COMPILED_TRANSLATIONS}
	${FILESCHEME_SRCS}
	${FILESCHEME_MOC_SRCS}
	${FILESCHEME_RCCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_poshuku_filescheme
	${QT_LIBRARIES}
	plugininterface
	xmlsettingsdialog
	)
INSTALL (TARGETS leechcraft_poshuku_filescheme DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${FILESCHEME_COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
