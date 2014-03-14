

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
find_package(brofiler)

#Build the actual component
FILE(GLOB Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
FILE(GLOB Headers ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

include_directories(${Boost_INCLUDE_DIRS}
                    ${brofiler_INCLUDE_DIRS})
link_directories(${brofiler_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_link_libraries(${PROJECT_NAME} optimized ${brofiler_Release_LIBRARIES} ${Boost_LIBRARIES})  
target_link_libraries(${PROJECT_NAME} debug ${brofiler_Debug_LIBRARIES} ${Boost_LIBRARIES})  

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_NAME}/bin)

        