@ECHO OFF
CALL python "../Tools/WinLLDB/runlldb.py"
CALL set-vars.bat
CALL "../Tools/WinLLDB/lldb.exe" -o run "%GEOEXE%" -o quit
if EXIST set-vars.bat (
	DEL set-vars.bat
)