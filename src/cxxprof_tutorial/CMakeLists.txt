

#The minimum CMake version which is needed
cmake_minimum_required(VERSION 2.8)

######################
# TODO: Please check if the ../../ are correct for SELFCONTAINED_CMAKE
######################
#This includes the environment
set( SELFCONTAINED_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/)
set( SELFCONTAINED_THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/)
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SELFCONTAINED_CMAKE} )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SELFCONTAINED_THIRDPARTY} )
include( setEnv )

#Use CxxProf (else it wouldn't be used and linked)
add_definitions(-DUSECXXPROF)

#Find the dependencies
find_package(cxxprof_static)

#Build the actual component
FILE(GLOB Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
FILE(GLOB Headers ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

include_directories(${Boost_INCLUDE_DIRS}
                    ${cxxprof_static_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_link_libraries(${PROJECT_NAME}   ${cxxprof_static_LIBRARIES}
                                        ${Boost_LIBRARIES})

#NOTE: What about clang-based systems?
if(UNIX)
target_link_libraries(${PROJECT_NAME}   ${CMAKE_DL_LIBS})
endif()

#-- Set the Debug and Release names for the libraries
SET_TARGET_PROPERTIES( ${PROJECT_NAME}
    PROPERTIES
    DEBUG_OUTPUT_NAME ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}
    RELEASE_OUTPUT_NAME ${PROJECT_NAME}
)
                                            
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_NAME}/bin)

#this installs the start.bat
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/startTutorial.bat
        DESTINATION ${PROJECT_NAME}/bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/startTutorial.sh
        DESTINATION ${PROJECT_NAME}/bin)

        
