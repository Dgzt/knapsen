
SET( TEST_LIBRARIES
  Knapsen_LIBRARY
  ${QT_QTTEST_LIBRARY} 
  ${QT_LIBRARY}
  ${KDE4_KDEUI_LIBS}
)

SET( CardTests_SRCS cardtest.cpp )
#ADD_EXECUTABLE( CardTests ${CardTests_SRCS} )
kde4_add_executable( CardTests ${CardTests_SRCS} )
TARGET_LINK_LIBRARIES( CardTests ${TEST_LIBRARIES} )
ADD_TEST( NAME CardTests COMMAND CardTests )
