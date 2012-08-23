project(knapsen)

cmake_minimum_required( VERSION 2.6 )

find_package( KDE4 "4.6" REQUIRED )
find_package( KDEGames REQUIRED )

include( KDE4Defaults )

include_directories( ${QT_QTSVG_INCLUDE_DIR} )
include_directories( ${QT_QTNETWORK_INCLUDE_DIR} )
include_directories( ${KDE4_INCLUDES} )



# Generate config-kmess.h
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config-knapsen.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/config-knapsen.h )

# Information message
MESSAGE(STATUS "${PROJECT_NAME} will be built to install into ${CMAKE_INSTALL_PREFIX}")

#add_subdirectory( doc )
add_subdirectory( pics )
add_subdirectory( src )
