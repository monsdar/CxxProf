
@echo off

REM Copy to install/bin directory and execute

set PATH=%PATH%;../../../thirdparty/boost/bin/
set PATH=%PATH%;../../../thirdparty/pluma/bin/
set PATH=%PATH%;../../../thirdparty/zmq/bin/

brofiler_server.exe
REM brofiler_server_debug.exe
