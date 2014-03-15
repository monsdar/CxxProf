
#######################################
#
# SetEnv.cmake prepares the environment. It provides Variables pointing to Thirdparty.
#
# Thirdparty Variables
#   Boost_INCLUDE_DIRS  - Include directory for Boost
#   Boost_LIBRARIES     - Libraries (shared) of Boost
#   Pluma_INCLUDE_DIRS  - Includes for Pluma
#   Pluma_LIBRARIES     - Libraries (shared) for Pluma
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

#this sets the Boost environment (just once)
IF(NOT Boost_FOUND)
    set(BOOST_ROOT ${THIRDPARTY_DIR}/Boost)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_USE_MULTITHREADED ON)
    add_definitions( -DBOOST_ALL_NO_LIB )   #needed for VisualStudio
    add_definitions( -DBOOST_ALL_DYN_LINK ) #needed for VisualStudio
    find_package(Boost 1.55.0 REQUIRED COMPONENTS system unit_test_framework thread date_time chrono)
ENDIF(NOT Boost_FOUND)

#sets the Pluma variables
set( Pluma_INCLUDE_DIRS ${THIRDPARTY_DIR}/pluma/include)
set( Pluma_LIBRARIES    ${THIRDPARTY_DIR}/pluma/lib/pluma.lib)

