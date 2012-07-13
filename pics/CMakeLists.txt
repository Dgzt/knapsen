set(knapsen_PICS
	background.png
	localmode.png
	clientmode.png
	servermode.png
	playersettings.png
	gamesettings.png
)

kde4_install_icons( ${ICON_INSTALL_DIR}   )

install( FILES ${knapsen_PICS} DESTINATION ${DATA_INSTALL_DIR}/knapsen/pics )
