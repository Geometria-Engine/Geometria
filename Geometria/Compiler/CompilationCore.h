#include "Compiler.h"
class CompilationCore
{
public:
	static Compiler VisualStudioDEVENV()
	{
		Compiler devenvComp;
		devenvComp.command = devenvComp.ModifyCommandLine("devenv $SolutionPath /build \"Release|x64\"");

		devenvComp.AddImportantFilePath(devenvComp.resourcesPath.u8string() + "/Geometria.vcxproj");
		devenvComp.AddImportantFilePath(devenvComp.resourcesPath.u8string() + "/Geometria.vcxproj.filters");
		devenvComp.AddImportantFilePath(devenvComp.resourcesPath.u8string() + "/Geometria.vcxproj.user");

		devenvComp.AddCustomValue("$SolutionDir", "$(SolutionDir)");
		devenvComp.AddCustomValue("$Platform", "$(Platform)");
		devenvComp.AddCustomValue("$Config", "$(Configuration)");
	}

	static Compiler VisualStudioMSBuild()
	{
		return Compiler("Compiler/MSBuild.comp");
	}

	static Compiler CurrentCompiler;

	static void Compile();
};