@ECHO OFF
TITLE Geometria Project Generator
cls

color 0F
echo Welcome to Geometria's Project Generator! (Powered by Premake)
echo You have these options that allows you to choose what type of project
echo you want to generate.
echo.
echo.
echo 1.- Visual Studio 2019
echo 2.- Visual Studio 2017
echo 3.- Visual Studio 2015
echo 4.- Visual Studio 2013
echo 5.- Visual Studio 2012
echo 6.- Visual Studio 2010
echo 7.- Visual Studio 2008
echo 8.- Visual Studio 2005
echo 9.- Makefile v2
echo 10.- Makefile (deprecated, according to Premake's documentation)
echo 11.- XCode
echo 12.- CodeLite
echo.
echo.
echo Please type the number of the option you want to choose
echo and press 'Enter' to confirm
set /p answer=:
if /i "%answer:~,1%" EQU "1" premake5-windows.exe vs2019
if /i "%answer:~,1%" EQU "2" premake5-windows.exe vs2017
if /i "%answer:~,1%" EQU "3" premake5-windows.exe vs2015
if /i "%answer:~,1%" EQU "4" premake5-windows.exe vs2013
if /i "%answer:~,1%" EQU "5" premake5-windows.exe vs2012
if /i "%answer:~,1%" EQU "6" premake5-windows.exe vs2010
if /i "%answer:~,1%" EQU "7" premake5-windows.exe vs2008
if /i "%answer:~,1%" EQU "8" premake5-windows.exe vs2005
if /i "%answer:~,1%" EQU "9" premake5-windows.exe gmake2
if /i "%answer:~,1%" EQU "10" premake5-windows.exe gmake
if /i "%answer:~,1%" EQU "11" premake5-windows.exe xcode4
if /i "%answer:~,1%" EQU "12" premake5-windows.exe codelite
PAUSE