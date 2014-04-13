
#######################################
#
# SetEnv.cmake prepares the environment. It provides Variables pointing to Thirdparty.
#
# Thirdparty Variables
#
#   Boost
#     Boost_INCLUDE_DIRS
#     Boost_LIBRARIES
#
#   Pluma
#     Pluma_INCLUDE_DIRS
#     Pluma_LIBRARIES
#
#   ZeroMQ
#     Zeromq_INCLUDE_DIRS
#     Zeromq_LIBRARIES
#
#   Sqlite3
#     Sqlite3_INCLUDE_DIRS
#     Sqlite3_LIBRARIES
#
#   Sqlite3cc
#     Sqlite3cc_INCLUDE_DIRS
#     Sqlite3cc_LIBRARIES
#
#######################################

#set the project name according to where the CMakeLists.txt is located
get_filename_component(ProjectId ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

#set some global options
set( BUILD_SHARED_LIBS ON)          #Always create and link shared libs. Turn this OFF to build static libs 
set( CMAKE_DEBUG_POSTFIX "_debug")  #This is the Postfix used for Debug libs

#set the pathes where to search for Thirdparty
GET_FILENAME_COMPONENT(SCRIPT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
set( THIRDPARTY_DIR ${SCRIPT_DIR}/../thirdparty/)
set( SOURCE_DIR ${SCRIPT_DIR}/../src/)

#Boost
IF(NOT Boost_FOUND)
    set(BOOST_ROOT ${THIRDPARTY_DIR}/Boost)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_USE_MULTITHREADED ON)
    add_definitions( -DBOOST_ALL_NO_LIB )   #needed for VisualStudio
    add_definitions( -DBOOST_ALL_DYN_LINK ) #needed for VisualStudio
    find_package(Boost 1.55.0 REQUIRED COMPONENTS filesystem serialization system unit_test_framework thread date_time chrono)
ENDIF(NOT Boost_FOUND)

#Pluma
set( Pluma_INCLUDE_DIRS ${THIRDPARTY_DIR}/pluma/include)
set( Pluma_LIBRARIES    optimized ${THIRDPARTY_DIR}/pluma/lib/pluma.lib
                        debug ${THIRDPARTY_DIR}/pluma/lib/pluma-d.lib)

#ZeroMQ
set( Zeromq_INCLUDE_DIRS    ${THIRDPARTY_DIR}/zmq/include)
set( Zeromq_LIBRARIES       optimized ${THIRDPARTY_DIR}/zmq/lib/libzmq-v120-mt-4_0_4.lib
                            debug ${THIRDPARTY_DIR}/zmq/lib/libzmq-v120-mt-gd-4_0_4.lib)

#Sqlite3
set( Sqlite3_INCLUDE_DIRS   ${THIRDPARTY_DIR}/sqlite3/include)
set( Sqlite3_LIBRARIES      optimized ${THIRDPARTY_DIR}/sqlite3/lib/sqlite3_vc120.lib
                            debug ${THIRDPARTY_DIR}/sqlite3/lib/sqlite3_vc120.lib)

#Sqlite3cc
set( Sqlite3cc_INCLUDE_DIRS ${THIRDPARTY_DIR}/sqlite3cc/include)
set( Sqlite3cc_LIBRARIES    optimized ${THIRDPARTY_DIR}/sqlite3cc/lib/sqlite3cc-vc120-0_1_1.lib
                            debug ${THIRDPARTY_DIR}/sqlite3cc/lib/sqlite3cc-vc120-0_1_1.lib)

