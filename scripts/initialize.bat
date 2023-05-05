@echo off
pushd %~dp0\..\
call third_party\premake5\premake5.exe vs2022
popd
PAUSE