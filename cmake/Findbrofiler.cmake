
############################
# Disclaimer:
#   This script is NOT a dynamic CMake Find-script as you may know it.
#   It just works in the Selfcontained environment it is meant for.
#
# The following Variables will be filled:
#   brofiler_FOUND             - Set to TRUE if the lib has been found
#   brofiler_INCLUDE_DIRS      - Set to the directories where the includes are found
#   brofiler_LIBRARY_DIRS      - Set to the directories where the libraries are found
#   brofiler_Release_LIBRARIES - Release libraries which need to be linked
#   brofiler_Debug_LIBRARIES   - Debug libraries which need to be linked
############################

#get the current script directory
GET_FILENAME_COMPONENT(SCRIPT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set( brofiler_FOUND TRUE )
set( brofiler_INCLUDE_DIRS ${SCRIPT_DIR}/../install/brofiler/include )
set( brofiler_LIBRARY_DIRS ${SCRIPT_DIR}/../install/brofiler/lib )
set( brofiler_Release_LIBRARIES brofiler.lib )
set( brofiler_Debug_LIBRARIES brofiler_debug.lib )
