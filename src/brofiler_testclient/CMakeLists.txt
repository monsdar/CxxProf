

#The minimum CMake version which is needed
cmake_minimum_required(VERSION 2.8)

######################
# TODO: Please check if the ../../ are correct for SELFCONTAINED_CMAKE
######################
#This includes the environment
set( SELFCONTAINED_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/)
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SELFCONTAINED_CMAKE} )
include( setEnv )
include( createTest )

#This enables testing for this project
enable_testing()

#Find the dependencies
find_package(brofiler)
find_package(brofiler_dyntest)

#Build the actual component
FILE(GLOB Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/*.cpp)
FILE(GLOB Headers ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/*.h)

set( INCLUDEDIRS ${Boost_INCLUDE_DIRS}
                 ${brofiler_INCLUDE_DIRS}
                 ${brofiler_dyntest_INCLUDE_DIRS}
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/)
set( LIBRARYDIRS ${brofiler_LIBRARY_DIRS}
                 ${brofiler_dyntest_LIBRARY_DIRS})
set( LIBRARIES   ${brofiler_Release_LIBRARIES}
                 ${brofiler_dyntest_Release_LIBRARIES}
                 ${Boost_LIBRARIES})
set( ADDITIONALSOURCES ${Sources} ${Headers})

#create the actual tests
createTest(BrofilerSimple)
