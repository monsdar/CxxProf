
############################
# Disclaimer:
#   This script is NOT a dynamic CMake Find-script as you may know it.
#   It just works in the Selfcontained environment it is meant for.
#
# The following Variables will be filled:
#   golib_FOUND             - Set to TRUE if the lib has been found
#   golib_INCLUDE_DIRS      - Set to the directories where the includes are found
#   golib_LIBRARY_DIRS      - Set to the directories where the libraries are found
#   golib_Release_LIBRARIES - Release libraries which need to be linked
#   golib_Debug_LIBRARIES   - Debug libraries which need to be linked
############################

#get the current script directory
GET_FILENAME_COMPONENT(SCRIPT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set( golib_FOUND TRUE )
set( golib_INCLUDE_DIRS ${SCRIPT_DIR}/../install/golib/include )
set( golib_LIBRARY_DIRS ${SCRIPT_DIR}/../install/golib/lib )
set( golib_Release_LIBRARIES golib.lib )
set( golib_Debug_LIBRARIES golib_debug.lib )
