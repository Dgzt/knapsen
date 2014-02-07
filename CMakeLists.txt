project( knapsen )

cmake_minimum_required( VERSION 2.6 )

if( NOT CMAKE_CXX_COMPILER )
    message( FATAL_ERROR
"----------------------------------------------------------
    g++ is not found!

    Ubuntu: apt-get install g++
    Debian: apt-get install g++
----------------------------------------------------------" )
endif( NOT CMAKE_CXX_COMPILER )

find_package( Qt4 )
if( QT4_FOUND )
    if( ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} VERSION_LESS "4.6.0" )
        message( FATAL_ERROR 
"----------------------------------------------------------
    Qt is too old! Minimum version is 4.6.0!
----------------------------------------------------------" )
    endif( ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} VERSION_LESS "4.6.0" )
else( QT4_FOUND )
    message( FATAL_ERROR 
"----------------------------------------------------------
    Qt4 development files not found!

    Ubuntu: apt-get install libqt4-dev
    Debian: apt-get install libqt4-dev
----------------------------------------------------------" )
endif( QT4_FOUND )

find_package( KDE4 )
if( KDE4_FOUND )
    if( ${KDE_VERSION_MAJOR}.${KDE_VERSION_MINOR}.${KDE_VERSION_RELEASE} VERSION_LESS "4.9.0" )
        message( FATAL_ERROR 
"----------------------------------------------------------
    KDE4 is too old! Minimum version is 4.9.0!
----------------------------------------------------------" )
    endif( ${KDE_VERSION_MAJOR}.${KDE_VERSION_MINOR}.${KDE_VERSION_RELEASE} VERSION_LESS "4.9.0" )
else( KDE4_FOUND )
    message( FATAL_ERROR 
"----------------------------------------------------------
    KDE4 library development files not found!

    Ubuntu: apt-get install kdelibs5-dev
    Debian: apt-get install kdeLibs5-dev
----------------------------------------------------------" )
endif( KDE4_FOUND )

find_package( KDEGames )
if( NOT KDEGames_FOUND )
    message( FATAL_ERROR 
"----------------------------------------------------------
    KDEGames library development files not found!

    Ubuntu: sudo apt-get install libkdegames-dev
----------------------------------------------------------" )
endif( NOT KDEGames_FOUND )

INCLUDE( KDE4Defaults )

include_directories( ${QT_QTSVG_INCLUDE_DIR} )
include_directories( ${QT_QTNETWORK_INCLUDE_DIR} )
include_directories( ${KDE4_INCLUDES} )
include_directories( ${KDEGAMES_INCLUDE_DIRS} )

# Generate config-kmess.h
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config-knapsen.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/config-knapsen.h )

#Enable the unit testing
enable_testing( true )

# Information message
MESSAGE(STATUS "${PROJECT_NAME} will be built to install into ${CMAKE_INSTALL_PREFIX}")

#add_subdirectory( doc )
add_subdirectory( po )
add_subdirectory( pics )
add_subdirectory( test )
add_subdirectory( src )
