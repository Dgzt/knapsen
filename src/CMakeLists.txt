
set(knapsen_HDRS
	enums.h
	table/scoretable.h
	table/mytextitem.h
	table/mysvgitem.h
	engine/card.h
	engine/deck.h
	dialogs/waitingforopponent.h
	dialogs/endgamedialog.h
	dialogs/endrounddialog.h
	engine/bot.h
	engine/client.h
	engine/player.h
	engine/gamesequence.h
	engine/server.h
	engine/centralcards.h
	
	engine/trumpcard.h
	
	dialogs/waitforserverdialog.h
	dialogs/waitforclientdialog.h
	dialogs/newgamedialog.h
	table/centralwidget.h
	mainwidget.h
)

set(knapsen_SRCS
	table/scoretable.cpp
	table/mytextitem.cpp
	table/mysvgitem.cpp
	engine/card.cpp
	engine/deck.cpp
	dialogs/waitingforopponent.cpp
	dialogs/endgamedialog.cpp
	dialogs/endrounddialog.cpp
	engine/bot.cpp
	engine/client.cpp
	engine/player.cpp
	engine/gamesequence.cpp
	engine/server.cpp
	engine/centralcards.cpp
	
	engine/trumpcard.cpp
	
	dialogs/waitforserverdialog.cpp
	dialogs/waitforclientdialog.cpp
	dialogs/newgamedialog.cpp
	table/centralwidget.cpp
	mainwidget.cpp
	main.cpp
)

set(knapsen_UIS
	#dialogs/newgamewidget.ui
	dialogs/newgamewidget_local.ui
	dialogs/newgamewidget_client.ui
	dialogs/newgamewidget_server.ui
	dialogs/endroundwidget.ui
	dialogs/endgamewidget.ui
	playersettingswidget.ui
	gamesettingswidget.ui
	dialogs/waitingwidget.ui
	dialogs/waitforclientwidget.ui
)

kde4_add_kcfg_files(knapsen_SRCS settings.kcfgc )

kde4_add_ui_files(knapsen_SRCS ${knapsen_UIS} )

kde4_add_executable(knapsen ${knapsen_SRCS} )

#target_link_libraries(knapsen ${QT_QTSVG_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${KDE4_KDEUI_LIBS} kdegames )
target_link_libraries(knapsen ${QT_QTSVG_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${KDE4_KDEUI_LIBS} ${KDEGAMES_LIBRARY} )

########### install files ###############

install(TARGETS   knapsen          ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( PROGRAMS knapsen.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES    knapsen.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES    knapsenui.rc  DESTINATION  ${DATA_INSTALL_DIR}/knapsen )
