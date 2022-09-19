#include "DebugTools.h"

std::vector<std::string> _Geometria_CoreLog::nullptrValues;
std::string _Geometria_CoreLog::currentText;

void DebugTools::SignalHandler(int signal)
{
	std::cout << "Error: Signal " << signal << std::endl;
	Files::Write("currentLine.log", _Geometria_CoreLog::currentText);
	throw "Error!";
}