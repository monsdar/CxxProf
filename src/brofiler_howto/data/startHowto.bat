
@echo off

REM Copy to install/bin directory and execute

DEL brofiler_dyn_network.dll
copy ..\install\brofiler_dyn_network\bin\brofiler_dyn_network.dll .
copy ..\install\brofiler_howto\bin\brofiler_howto.exe .

set PATH=%PATH%;../thirdparty/boost/bin/
set PATH=%PATH%;../thirdparty/pluma/bin/
set PATH=%PATH%;../thirdparty/zmq/bin/
set PATH=%PATH%;../install/brofiler_static/bin/

brofiler_howto.exe
