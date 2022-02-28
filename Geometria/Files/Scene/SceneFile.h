#pragma once
#include <string>
#include "yaml-cpp/yaml.h"

struct ScriptBehaviour;

class SceneSaveAndLoad
{
public:
	static std::ostringstream sceneCppSave;
	static std::ostringstream* objectName, *allPointers;
	static std::string scriptName;
	static std::string allIncludes;

	static std::vector<std::pair<std::string, std::ostringstream>> cppStructure, cppStructureEnd;

	static YAML::Emitter* sceneTITSSave;
	static YAML::Node loadTITS;
	static bool isSavePointer;
	static std::vector<YAML::Node> dataArrayVector;
	static std::vector<std::string> loadArray;

	static std::vector<std::pair<std::string, ScriptBehaviour*>> BaseClasses;
	static void StartClassInit();

	//LOAD FUNCTIONS
	static void StartSceneLoad(std::string filepath);

	static std::string ModifyCode(std::string string);

	static std::string CreateOrAddIntoStructure(std::string name, std::string objectType, std::string content);
	static std::string CreateOrAddFunctionIntoStructureOnce(std::string name, std::string content);

	static bool StartLoadArray(std::string name);

	static int LoadValueInt(std::string name);
	static std::string LoadValueString(std::string name);
	static float LoadValueFloat(std::string name);

	static int LoadResourceInt(std::string name);
	static std::string LoadResourceString(std::string name);
	static float LoadResourceFloat(std::string name);

	static void EndLoadArray();

	//SAVE FUNCTIONS
	static void StartSceneSave(void* sc);

	static void StartSaveArray(std::string name);

	static std::string SaveValueToTITS(std::string name, int value);
	static std::string SaveValueToTITS(std::string name, std::string value);
	static std::string SaveValueToTITS(std::string name, float value);

	static void EndSaveArray();
};
