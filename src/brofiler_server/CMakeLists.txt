

#The minimum CMake version which is needed
cmake_minimum_required(VERSION 2.8)

######################
# TODO: Please check if the ../../ are correct for SELFCONTAINED_CMAKE
######################
#This includes the environment
set( SELFCONTAINED_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/)
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SELFCONTAINED_CMAKE} )
include( setEnv )

#Find the dependencies
find_package(brofiler_dyn_network) #We just need the Headers...

#Build the actual component
FILE(GLOB Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
FILE(GLOB Headers ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

include_directories(${Boost_INCLUDE_DIRS}
                    ${Zeromq_INCLUDE_DIRS}
                    ${brofiler_dyn_network_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_link_libraries(${PROJECT_NAME}   ${Boost_LIBRARIES}
                                        ${Zeromq_LIBRARIES}
                                        ${brofiler_dyn_network_LIBRARIES})

#-- Set the Debug and Release names for the libraries
SET_TARGET_PROPERTIES( ${PROJECT_NAME}
    PROPERTIES
    DEBUG_OUTPUT_NAME ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}
    RELEASE_OUTPUT_NAME ${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_NAME}/bin)

#this installs the start.bat
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/startServer.bat
        DESTINATION ${PROJECT_NAME}/bin)
