PROJECT(knapsen)

CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

FIND_PACKAGE( Qt4 )
IF( NOT QT4_FOUND )
    MESSAGE( FATAL_ERROR 
"----------------------------------------------------------
    Qt4 development files not found!

    Ubuntu: apt-get install libqt4-dev
    Debian: apt-get install libqt4-dev
----------------------------------------------------------" )
ENDIF( NOT QT4_FOUND )

FIND_PACKAGE( KDE4 )
IF( NOT KDE4_FOUND )
    MESSAGE( FATAL_ERROR 
"----------------------------------------------------------
    KDE4 library development files not found!

    Ubuntu: apt-get install kdelibs5-dev
    Debian: apt-get install kdeLibs5-dev
----------------------------------------------------------" )
ENDIF( NOT KDE4_FOUND )

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#              KDEGames >= 4.9
#FIND_PACKAGE( KDEGames )
#IF( NOT KDEGAMES_FOUND )
#    MESSAGE( FATAL_ERROR 
#"----------------------------------------------------------
#    KDEGames library not found!
#
#    Ubuntu: sudo apt-get install libkdegames-dev
#----------------------------------------------------------" )
#ENDIF( NOT KDEGAMES_FOUND )
FIND_PACKAGE( KDEGames REQUIRED )
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#               KDEGames < 4.9
#FIND_PACKAGE( LibKDEGames REQUIRED )
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


INCLUDE( KDE4Defaults )

include_directories( ${QT_QTSVG_INCLUDE_DIR} )
include_directories( ${QT_QTNETWORK_INCLUDE_DIR} )
include_directories( ${KDE4_INCLUDES} )
include_directories( ${KDEGAMES_INCLUDE_DIRS} )

# Generate config-kmess.h
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config-knapsen.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/config-knapsen.h )

# Information message
MESSAGE(STATUS "${PROJECT_NAME} will be built to install into ${CMAKE_INSTALL_PREFIX}")

#add_subdirectory( doc )
add_subdirectory( pics )
add_subdirectory( src )
