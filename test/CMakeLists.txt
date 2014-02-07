
#add_subdirectory(engine)

#MACRO( add_test testname_testsrc )
#
#  SET( test_${testname}_SRCS ${testsrc} )
#  
#  qt4_automoc( ${test_${testname}_SRCS} )
#  
#  ADD_EXECUTABLE( test_${testname} ${test_${testname}_SRCS} ${test_additional} )
#  
#  TARGET_LINK_LIBRARIES( test_${testname} 
#    ${QT_QTCORE_LIBRARY} 
#    ${QT_QTTEST_LIBRARY} 
#    ${QT_QTGUI_LIBRARY}
#    ${GSOAP_LIBRARIES} 
#    ${QT_QTLOCATION_LIBRARY}
#  )
#  
#  ADD_TEST( test_${testname} test_${testname} )
#
##ENDMACRO( add_test )

SET( TEST_LIBRARIES
  Knapsen_LIBRARY
  ${QT_QTTEST_LIBRARY} 
  ${QT_LIBRARY}
  ${KDE4_KDEUI_LIBS}
)

SET( CardTests_SRCS engine/cardtest.cpp )
#ADD_EXECUTABLE( CardTests ${CardTests_SRCS} )
kde4_add_executable( CardTests ${CardTests_SRCS} )
TARGET_LINK_LIBRARIES( CardTests ${TEST_LIBRARIES} )
ADD_TEST( NAME CardTests COMMAND CardTests )
