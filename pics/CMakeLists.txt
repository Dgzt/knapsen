set(knapsen_PICS
	background.png
)

kde4_install_icons( ${ICON_INSTALL_DIR}   )

install( FILES ${knapsen_PICS} DESTINATION ${DATA_INSTALL_DIR}/knapsen/pics )
