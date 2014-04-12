
@echo off

copy ..\install\brofiler_server\bin\brofiler_server.exe .

set PATH=%PATH%;../thirdparty/boost/bin/
set PATH=%PATH%;../thirdparty/pluma/bin/
set PATH=%PATH%;../thirdparty/zmq/bin/

brofiler_server.exe
REM brofiler_server_debug.exe
