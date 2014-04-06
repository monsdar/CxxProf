
@echo off

REM Copy to install/bin directory and execute

DEL brofiler_dyn_debug.dll
DEL brofiler_dyn.dll
copy ..\..\brofiler_dyn\bin\brofiler_dyn.dll .
REM copy %~dp0\..\..\brofiler_dyn\bin\brofiler_dyn_debug.dll .

set PATH=%PATH%;../../../thirdparty/boost/bin/
set PATH=%PATH%;../../../thirdparty/pluma/bin/
set PATH=%PATH%;../../../thirdparty/zmq/bin/
set PATH=%PATH%;../../brofiler/bin/

brofiler_howto.exe
REM brofiler_howto_debug.exe
