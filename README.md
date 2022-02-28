# The RetroFunk Project

![Stars](https://img.shields.io/github/stars/TheNachoBIT/TheRetroFunkProject)
![Issues](https://img.shields.io/github/issues/TheNachoBIT/TheRetroFunkProject)
![License](https://img.shields.io/badge/license-MIT-blue)

## Installation

### Windows
* [Visual Studio](#visual-studio-windows)

### Linux
* [Make](#make-linux)

### Visual Studio (Windows)

Requirements:
* [Visual Studio (from 2005 to 2019, whatever version fits best for you) with the Deskop C++ Environment package](https://visualstudio.microsoft.com/vs/)

Once you have the requirements, go to the main folder and either open the ``"GenerateProjectWindows.bat"`` and follow the instructions,
or open the command prompt/terminal in the folder and type:

``
premake5-windows.exe (vs2019-2005)
``

Wait for the process to finish and now a .sln will be created in the main folder. Now you can open and run it!

### Make (Linux)

Requirements:
Run this command in the terminal to make sure you have everything installed:

``sudo apt install make build-essential gcc gcc-multilib g++-multilib libx11-dev libgl1-mesa-dev xorg-dev libglu1-mesa-dev openssl libssl-dev libcurl4-openssl-dev``

Once you have everything installed, go to the main folder and type in the terminal:

``./premake5-linux gmake2`` (Make sure the "premake5-linux" binary has all of the necessary read and write permissions)

And a Makefile project will be created once everything is finished. Now you can use:

``make``

and compile the entire project.
