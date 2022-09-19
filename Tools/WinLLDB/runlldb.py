import sys
import os
import subprocess

path = ""

for i in sys.path:
	if i.find("\\lib") != -1:
		path = i
		break

end = path.index( "\\lib", 0 )
finalPath = path[0:end]

args = ""

for i in sys.argv:
	if i.find(".py") == -1:
		args += " " + i

dir_path = os.path.dirname(os.path.realpath(__file__))
dir_path_tools = os.path.dirname(dir_path)
dir_path_project = os.path.dirname(dir_path_tools)
dir_path_project_name = os.path.basename(os.path.normpath(dir_path_project))

with open('current-dir.txt', 'w') as f:
	f.write(dir_path_project_name)

setHomeCmd = "set PYTHONHOME=" + finalPath
setExeCmd = "set GEOEXE=" + dir_path_project + "\\Geometria\\" + dir_path_project_name + ".exe"
with open('set-vars.bat', 'w') as f:
    f.write("set PYTHONHOME=" + finalPath + "\n")
    f.write("set PYTHONPATH=" + finalPath + "\\lib" + "\n")
    f.write(setExeCmd)