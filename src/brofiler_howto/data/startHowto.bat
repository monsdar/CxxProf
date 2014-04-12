
@echo off

DEL brofiler_dyn_debug.dll
DEL brofiler_dyn.dll
copy ..\install\brofiler_dyn\bin\brofiler_dyn.dll .
copy ..\install\brofiler_howto\bin\brofiler_howto.exe .

set PATH=%PATH%;../thirdparty/boost/bin/
set PATH=%PATH%;../thirdparty/pluma/bin/
set PATH=%PATH%;../thirdparty/zmq/bin/
set PATH=%PATH%;../install/brofiler/bin/

brofiler_howto.exe
