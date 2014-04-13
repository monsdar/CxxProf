
@echo off

REM Copy to install/bin directory and execute

DEL %~dp0\brofiler_dyn_network.dll
copy %~dp0\..\..\brofiler_dyn_network\bin\brofiler_dyn_network.dll .

set PATH=%PATH%;%~dp0/../../../thirdparty/boost/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/pluma/bin/
set PATH=%PATH%;%~dp0/../../../thirdparty/zmq/bin/
set PATH=%PATH%;%~dp0/../../../install/brofiler_static/bin/

%~dp0\brofiler_howto.exe
