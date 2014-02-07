
set(knapsen_HDRS
    enums.h
    table/animation.h
    table/scoretable.h
    table/text.h
    table/svgcard.h
    table/centralwidget.h
    table/cardsgroup.h
    engine/card.h
    dialogs/waitingforopponent.h
    dialogs/endgamedialog.h
    dialogs/endrounddialog.h
    engine/bot.h
    engine/client.h
    engine/player.h
    engine/server.h
    engine/trump.h
    dialogs/waitforserverdialog.h
    dialogs/waitforclientdialog.h
    dialogs/newgamedialog.h
    mainwindow.h
)

set(knapsen_SRCS
    table/animation.cpp
    table/scoretable.cpp
    table/text.cpp
    table/svgcard.cpp
    table/centralwidget.cpp
    table/cardsgroup.cpp
    engine/card.cpp
    dialogs/waitingforopponent.cpp
    dialogs/endgamedialog.cpp
    dialogs/endrounddialog.cpp
    engine/bot.cpp
    engine/client.cpp
    engine/player.cpp
    engine/server.cpp
    engine/trump.cpp
    dialogs/waitforserverdialog.cpp
    dialogs/waitforclientdialog.cpp
    dialogs/newgamedialog.cpp
    mainwindow.cpp
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

ADD_LIBRARY( Knapsen_LIBRARY STATIC ${knapsen_SRCS} )

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

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
