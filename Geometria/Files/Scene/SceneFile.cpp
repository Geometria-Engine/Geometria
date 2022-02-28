#include "SceneFile.h"
#include "../../Graphics/Cores/Renderer/RendererCore.h"
#include "../../Graphics/Externals/Model.h"
#include "../../Graphics/Externals/SceneAndDrawCall.h"
#include "../../Behaviours/Behaviour.h"
#include <fstream>
#include "../Files.h"
#include <experimental/filesystem>

std::vector<std::pair<std::string, ScriptBehaviour*>> SceneSaveAndLoad::BaseClasses;
std::vector<YAML::Node> SceneSaveAndLoad::dataArrayVector;
std::ostringstream SceneSaveAndLoad::sceneCppSave;
std::ostringstream* SceneSaveAndLoad::objectName, * SceneSaveAndLoad::allPointers;
std::string SceneSaveAndLoad::scriptName, SceneSaveAndLoad::allIncludes;
YAML::Emitter* SceneSaveAndLoad::sceneTITSSave;
YAML::Node SceneSaveAndLoad::loadTITS;
std::vector<std::string> SceneSaveAndLoad::loadArray;
std::vector<std::pair<std::string, std::ostringstream>> SceneSaveAndLoad::cppStructure, SceneSaveAndLoad::cppStructureEnd;
bool SceneSaveAndLoad::isSavePointer;

void SceneSaveAndLoad::StartClassInit()
{
	//for (int i = 0; i < BaseClasses.size(); i++)
	//{
	//	ScriptBehaviour* s = (ScriptBehaviour*)BaseClasses[i].second;
	//	s->OnLoad();
	//}
}

void SceneSaveAndLoad::StartSceneLoad(std::string filepath)
{
	loadTITS = YAML::Load(Files::Read(filepath.c_str()));
}

std::string SceneSaveAndLoad::ModifyCode(std::string string)
{
	std::string str = string;
	std::ostringstream name;

	if (SceneSaveAndLoad::scriptName != "")
	{
		name << SceneSaveAndLoad::scriptName << "->";

		while (str.find("CurrentObject::") != std::string::npos)
			str.replace(str.find("CurrentObject::"), std::string("CurrentObject::").length(), name.str());

		return str;
	}
	else if (SceneSaveAndLoad::objectName != nullptr)
	{
		name << SceneSaveAndLoad::objectName->str() << "->";

		while (str.find("CurrentObject::") != std::string::npos)
			str.replace(str.find("CurrentObject::"), std::string("CurrentObject::").length(), name.str());

		return str;
	}
	
	return string;
}

std::string SceneSaveAndLoad::CreateOrAddIntoStructure(std::string name, std::string objectType, std::string content)
{
	bool exists = false;
	int index = -1;
	for (int i = 0; i < cppStructure.size(); i++)
	{
		if (cppStructure[i].first == name)
		{
			exists = true;
			index = i;
		}
	}

	if (!exists)
	{
		index = cppStructure.size();
		cppStructure.push_back(std::make_pair(name, std::ostringstream()));
	}

	std::ostringstream objectName;
	objectName << objectType << std::to_string(rand() % 10000000);

	std::string finalContent = SceneSaveAndLoad::ModifyCode(content);

	cppStructure[index].second << objectType << "* " << objectName.str() << " = new " << finalContent << ";" << std::endl;

	std::ostringstream result;
	result << objectName.str();
	return result.str();
}

std::string SceneSaveAndLoad::CreateOrAddFunctionIntoStructureOnce(std::string name, std::string content)
{
	bool exists = false;
	int index = -1;
	for (int i = 0; i < cppStructureEnd.size(); i++)
	{
		if (cppStructureEnd[i].first == name)
		{
			exists = true;
			index = i;
		}
	}

	if (!exists)
	{
		index = cppStructureEnd.size();
		cppStructureEnd.push_back(std::make_pair(name, std::ostringstream()));
	}

	if(cppStructureEnd[index].second.str().find(content) == std::string::npos)
		cppStructureEnd[index].second << content << ";" << std::endl;

	return "";
}

bool SceneSaveAndLoad::StartLoadArray(std::string name)
{
	loadArray.push_back(name);
	return true;
}

int SceneSaveAndLoad::LoadValueInt(std::string name)
{
	std::string finalName = name;
	while (finalName.find("::") != std::string::npos)
		finalName.replace(finalName.find("::"), std::string("::").length(), ".");

	while (finalName.find("().") != std::string::npos)
		finalName.replace(finalName.find("()."), std::string("().").length(), ".");

	std::cout << finalName << std::endl;

	return loadTITS[finalName].as<int>();
}

std::string SceneSaveAndLoad::LoadValueString(std::string name)
{
	std::string finalName = name;
	while (finalName.find("::") != std::string::npos)
		finalName.replace(finalName.find("::"), std::string("::").length(), ".");

	while (finalName.find("().") != std::string::npos)
		finalName.replace(finalName.find("()."), std::string("().").length(), ".");

	return loadTITS[finalName].as<std::string>();
}

float SceneSaveAndLoad::LoadValueFloat(std::string name)
{
	std::string finalName = name;
	while (finalName.find("::") != std::string::npos)
		finalName.replace(finalName.find("::"), std::string("::").length(), ".");

	while (finalName.find("().") != std::string::npos)
		finalName.replace(finalName.find("()."), std::string("().").length(), ".");

	return loadTITS[finalName].as<float>();
}

int SceneSaveAndLoad::LoadResourceInt(std::string name)
{
	return SceneSaveAndLoad::LoadValueInt(name);
}

std::string SceneSaveAndLoad::LoadResourceString(std::string name)
{
	return SceneSaveAndLoad::LoadValueString(name);
}

float SceneSaveAndLoad::LoadResourceFloat(std::string name)
{
	return SceneSaveAndLoad::LoadValueFloat(name);
}

void SceneSaveAndLoad::EndLoadArray()
{
	loadArray.pop_back();
}

void SceneSaveAndLoad::StartSceneSave(void* sc)
{
	std::cout << "Saving..." << std::endl;
	sceneCppSave.clear();
	Scene* scene = static_cast<Scene*>(sc);

	if (sceneTITSSave != nullptr)
	{
		delete sceneTITSSave;
		sceneTITSSave = nullptr;
	}

	sceneTITSSave = new YAML::Emitter();
	*sceneTITSSave << YAML::BeginMap;

	if (SceneSaveAndLoad::objectName != nullptr)
	{
		delete objectName;
		objectName = nullptr;
	}

	allIncludes = "";

	if (allPointers != nullptr)
	{
		delete allPointers;
		allPointers = nullptr;
	}

	allPointers = new std::ostringstream();
	allPointers->clear();

	for (int i = 0; i < Hierarchy::allScripts.size(); i++)
	{
		std::cout << "There's currently " << Hierarchy::allScripts.size() << " objects" << std::endl;
		if (Hierarchy::allScripts[i]->ClassType == ScriptBehaviour::Class::Object && !Hierarchy::allScripts[i]->hasOwner)
		{
			Hierarchy::allScripts[i]->OnSave();
			std::cout << "Saving " << i << std::endl;
		}
	}

	*sceneTITSSave << YAML::EndMap;

	std::ofstream fout("Game/SampleScene.tits.cpp"), fheaderOut("Game/SampleScene.tits.h"), ftitsOut("Game/SampleScene.tits");

	ftitsOut << sceneTITSSave->c_str();

	fheaderOut << "struct SampleScene" << std::endl;
	fheaderOut << "{" << std::endl;
	fheaderOut << "	static void Init();" << std::endl;
	fheaderOut << "};";

	std::ostringstream finalFile;

	finalFile << "#include \"SampleScene.tits.h\"" << std::endl << "#include \"geometria.h\"" << std::endl << std::endl;
	finalFile << SceneSaveAndLoad::allIncludes << std::endl;

	finalFile << allPointers->str() << std::endl;
	finalFile << "void SampleScene::Init()" << std::endl << "{" << std::endl;
	finalFile << "SceneSaveAndLoad::StartSceneLoad(\"Game/SampleScene.tits\");" << std::endl;
	
	for (int i = 0; i < SceneSaveAndLoad::cppStructure.size(); i++)
	{
		finalFile << SceneSaveAndLoad::cppStructure[i].second.str() << std::endl;
	}

	for (int i = 0; i < SceneSaveAndLoad::cppStructureEnd.size(); i++)
	{
		finalFile << SceneSaveAndLoad::cppStructureEnd[i].second.str() << std::endl;
	}

	finalFile << sceneCppSave.str();
	finalFile << "}";
	std::cout << sceneCppSave.str() << std::endl;
	fout << finalFile.str();

	fout.close();
	ftitsOut.close();
	fheaderOut.close();
}

void SceneSaveAndLoad::StartSaveArray(std::string name)
{
	/**mainEmitter << YAML::Key << name << YAML::Value << YAML::BeginSeq;*/
}

std::string Type()
{
	if (SceneSaveAndLoad::isSavePointer)
	{
		return "->";
	}

	return ".";
}

std::string SceneSaveAndLoad::SaveValueToTITS(std::string name, int value)
{
	std::ostringstream finalName;

	if (SceneSaveAndLoad::scriptName != "")
	{
		finalName << SceneSaveAndLoad::scriptName << Type() << name;
	}
	else if (SceneSaveAndLoad::objectName != nullptr)
	{
		finalName << SceneSaveAndLoad::objectName->str() << Type() << name;
	}
	else
	{
		finalName << name;
	}

	std::string replaceFinal = finalName.str();
	while (replaceFinal.find("::") != std::string::npos)
		replaceFinal.replace(replaceFinal.find("::"), std::string("::").length(), ".");

	while (replaceFinal.find("().") != std::string::npos)
		replaceFinal.replace(replaceFinal.find("()."), std::string("().").length(), ".");

	//Save Into .tits file

	*sceneTITSSave << YAML::Key << replaceFinal << YAML::Value << value;

	std::ostringstream result;
	result << "SaveRV(" << finalName.str() << ")";
	return result.str();
}

std::string SceneSaveAndLoad::SaveValueToTITS(std::string name, std::string value)
{
	std::ostringstream finalName;
	if (SceneSaveAndLoad::scriptName != "")
	{
		finalName << SceneSaveAndLoad::scriptName << Type() << name;
	}
	else if (SceneSaveAndLoad::objectName != nullptr)
	{
		finalName << SceneSaveAndLoad::objectName->str() << Type() << name;
	}

	//Save Into .tits file
	std::string changeV = value;

	while (changeV.find(std::experimental::filesystem::current_path().u8string()) != std::string::npos)
	{
		std::cout << "Found Path!" << std::endl;
		std::string path = std::experimental::filesystem::current_path().u8string();
		changeV.replace(changeV.find(path), path.size() + 1, "");
	}

	*sceneTITSSave << YAML::Key << finalName.str() << YAML::Value << changeV;

	std::ostringstream result;
	result << "SaveRV(" << finalName.str() << ")";
	return result.str();
}

std::string SceneSaveAndLoad::SaveValueToTITS(std::string name, float value)
{
	std::ostringstream finalName;
	if (SceneSaveAndLoad::scriptName != "")
	{
		finalName << SceneSaveAndLoad::scriptName << Type() << name;
	}
	else if (SceneSaveAndLoad::objectName != nullptr)
	{
		finalName << SceneSaveAndLoad::objectName->str() << Type() << name;
	}

	//Save Into .tits file

	*sceneTITSSave << YAML::Key << finalName.str() << YAML::Value << value;

	std::ostringstream result;
	result << "SaveRV(" << finalName.str() << ")";
	return result.str();
}

void SceneSaveAndLoad::EndSaveArray()
{
	/**mainEmitter << YAML::EndSeq;*/
}
