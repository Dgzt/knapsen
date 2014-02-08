
add_subdirectory(engine)

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

