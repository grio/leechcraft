INCLUDE_DIRECTORIES (${POSHUKU_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})
SET (WYFV_SRCS
	wyfv.cpp
	core.cpp
	xmlsettingsmanager.cpp
	wyfvplugin.cpp
	player.cpp
	playerfactory.cpp
	abstractplayercreator.cpp
	youtubeplayer.cpp
	vkontakteruplayer.cpp
	relatedwidget.cpp
	relateditem.cpp
	)
SET (WYFV_HEADERS
	wyfv.h
	core.h
	xmlsettingsmanager.h
	wyfvplugin.h
	player.h
	playerfactory.h
	abstractplayercreator.h
	youtubeplayer.h
	vkontakteruplayer.h
	relatedwidget.h
	relateditem.h
	related.h
	)
SET (WYFV_FORMS
	player.ui
	relateditem.ui
	)
SET (WYFV_RESOURCES
	poshukuwyfvresources.qrc
	)
SET (WYFV_TRANSLATIONS
	leechcraft_poshuku_wyfv_ru_RU.ts
	leechcraft_poshuku_wyfv_uk_UA.ts
	leechcraft_poshuku_wyfv_ar.ts
	leechcraft_poshuku_wyfv_de.ts
	leechcraft_poshuku_wyfv_en.ts
	)
SET (WYFV_COMPILED_TRANSLATIONS
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_wyfv_ru_RU.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_wyfv_uk_UA.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_wyfv_ar.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_wyfv_de.qm
	${CMAKE_CURRENT_SOURCE_DIR}/leechcraft_poshuku_wyfv_en.qm
	)
QT4_WRAP_CPP (WYFV_MOC_SRCS ${WYFV_HEADERS})
QT4_WRAP_UI (WYFV_UIS_H ${WYFV_FORMS})
QT4_ADD_RESOURCES (WYFV_RCCS ${WYFV_RESOURCES})
ADD_CUSTOM_COMMAND (OUTPUT
	${WYFV_COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${WYFV_TRANSLATIONS}
	DEPENDS ${WYFV_TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_LIBRARY (leechcraft_poshuku_wyfv SHARED
	${WYFV_COMPILED_TRANSLATIONS}
	${WYFV_SRCS}
	${WYFV_MOC_SRCS}
	${WYFV_UIS_H}
	${WYFV_RCCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_poshuku_wyfv
	${QT_LIBRARIES}
	plugininterface
	xmlsettingsdialog
	)
INSTALL (TARGETS leechcraft_poshuku_wyfv DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${WYFV_COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
INSTALL (FILES poshukuwyfvsettings.xml DESTINATION ${LC_SETTINGS_DEST})
