

#The minimum CMake version which is needed
cmake_minimum_required(VERSION 2.8)

#This includes the environment
set( SELFCONTAINED_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/)
set( SELFCONTAINED_THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/)
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SELFCONTAINED_CMAKE} )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SELFCONTAINED_THIRDPARTY} )
include( setEnv )
include( createTest )

#This enables testing for this project
enable_testing()

#Use CxxProf (else it wouldn't be used and linked)
add_definitions(-DUSECXXPROF)

#Find the dependencies
find_package(cxxprof_static)
find_package(cxxprof_dyntest)

#Build the actual component
FILE(GLOB Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/*.cpp)
FILE(GLOB Headers ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/*.h)

set( INCLUDEDIRS ${Boost_INCLUDE_DIRS}
                 ${cxxprof_static_INCLUDE_DIRS}
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/)
set( LIBRARIES   ${cxxprof_static_LIBRARIES}
                 ${Boost_LIBRARIES})
set( ADDITIONALSOURCES ${Sources} ${Headers})

#create the actual tests
createTest(CxxProfSimple)

#this installs the dyntest-plugin to the build-directory for testing it
if(WIN32)
install(FILES ${CMAKE_INSTALL_PREFIX}/cxxprof_dyn_test/bin/cxxprof_dyn_test.dll
        DESTINATION ${CMAKE_BINARY_DIR})
else()
install(FILES ${CMAKE_INSTALL_PREFIX}/cxxprof_dyn_test/lib/libcxxprof_dyn_test.so
        DESTINATION ${CMAKE_BINARY_DIR})
endif()
