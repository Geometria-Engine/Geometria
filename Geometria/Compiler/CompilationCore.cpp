#include "CompilationCore.h"

Compiler CompilationCore::CurrentCompiler = CompilationCore::VisualStudioMSBuild();

void CompilationCore::Compile()
{
	CurrentCompiler.Compile();
}
