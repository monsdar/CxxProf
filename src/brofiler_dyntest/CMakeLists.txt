

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


#This is needed for the dllexport mechanism
add_definitions(-DBROFILER_DYNTEST)

#Build the actual component
file(GLOB Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB Headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../
                    ${brofiler_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})
link_directories(${brofiler_LIBRARY_DIRS})

add_library(${PROJECT_NAME} SHARED ${Sources} ${Headers})
target_link_libraries(${PROJECT_NAME} optimized ${brofiler_Release_LIBRARIES} ${Boost_LIBRARIES})  
target_link_libraries(${PROJECT_NAME} debug ${brofiler_Debug_LIBRARIES} ${Boost_LIBRARIES})  

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${PROJECT_NAME}/lib
        LIBRARY DESTINATION ${PROJECT_NAME}/lib
        RUNTIME DESTINATION ${PROJECT_NAME}/bin)
install(FILES ${Headers} DESTINATION ${PROJECT_NAME}/include/${PROJECT_NAME})

#this configures and installs the FindPackage scripts
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Find.cmake.in
                ${CMAKE_CURRENT_BINARY_DIR}/Find${PROJECT_NAME}.cmake 
                @ONLY)
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/Find${PROJECT_NAME}.cmake
         DESTINATION ${SELFCONTAINED_CMAKE})