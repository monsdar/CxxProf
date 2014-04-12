
@echo off

REM Copy to install/bin directory and execute

DEL brofiler_dyn.dll
copy ..\install\brofiler_dyn\bin\brofiler_dyn.dll .
copy ..\install\brofiler_howto\bin\brofiler_howto.exe .

set PATH=%PATH%;../thirdparty/boost/bin/
set PATH=%PATH%;../thirdparty/pluma/bin/
set PATH=%PATH%;../thirdparty/zmq/bin/
set PATH=%PATH%;../install/brofiler/bin/

brofiler_howto.exe
