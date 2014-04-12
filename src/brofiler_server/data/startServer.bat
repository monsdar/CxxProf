
@echo off

REM Copy to install/bin directory and execute

DEL brofiler_dyn.dll
copy ..\install\brofiler_dyn\bin\brofiler_dyn.dll .
copy ..\install\brofiler_server\bin\brofiler_server.exe .

set PATH=%PATH%;../thirdparty/boost/bin/
set PATH=%PATH%;../thirdparty/pluma/bin/
set PATH=%PATH%;../thirdparty/zmq/bin/

brofiler_server.exe
