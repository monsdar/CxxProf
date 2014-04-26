
#######################################
#
# createTest.cmake helps creating Tests
#  
# Usage:
#   - Integrate into your CMakeLists.txt
#   - Fill the LibraryDir, IncludeDir and Library lists
#   - Add your test via the createTest command
#
#######################################


macro(createTest NAME)
    include_directories(${INCLUDEDIRS})
    add_executable( ${PROJECT_NAME}_test_${NAME} ${ADDITIONALSOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/${NAME}TestSuite.cpp)
    target_link_libraries( ${PROJECT_NAME}_test_${NAME}
                           ${LIBRARIES})

    #NOTE: What about clang-based systems?
    if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME}_test_${NAME}
                              ${CMAKE_DL_LIBS}
                              ${CMAKE_THREAD_LIBS_INIT}
                              rt)
    endif()

    add_test(${NAME} ${PROJECT_NAME}_test_${NAME} )
    
    install(TARGETS ${PROJECT_NAME}_test_${NAME}
        RUNTIME DESTINATION ${PROJECT_NAME}/test)
endmacro(createTest NAME)
