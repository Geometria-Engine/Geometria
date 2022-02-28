#pragma once
#include <vector>
#include <iostream>
#include "../Tools/Tools.h"
#include "../Files/Scene/SceneFile.h"
#include "../Application/Application.h"

struct ScriptBehaviour;
class Hierarchy
{
public:
	static int highestScriptId;
	static std::vector<ScriptBehaviour*> deleteList;
	static std::vector<ScriptBehaviour*> allScripts, allStaticScripts;
	static std::vector<ScriptBehaviour*> allUpdateScripts, allUpdateEditorScripts;
	static std::vector<std::pair<std::string, ScriptBehaviour*>> scriptsWithVisualAccess;
	static std::vector<std::string> listOfStaticScripts;
	static bool _setEditor;
	static void EditorMode(bool e);
	static void AddScript(ScriptBehaviour* s);


	static void StartScripts();
	static void UpdateScripts();
	static void AddObjectToDeleteList(ScriptBehaviour* id);
};

struct Transform
{
	Vector4 position, rotation, scale;
	Transform()
	{
		position = Vector4(0);
		rotation = Vector4(0);
		scale = Vector4(1);
	}

	Matrix GetTransform();

	void LoadTransform();
	void SaveTransform();
};

#define VisualAccess(x) bool _SB_GetVisualAccessOf##x()\
{\
	Hierarchy::scriptsWithVisualAccess.push_back(std::make_pair(#x, new x()));\
	Hierarchy::allScripts.pop_back();\
	return true;\
}\
\
bool _SB_VisualAccessOf##x = _SB_GetVisualAccessOf##x();

#define AllowCloning(x) ScriptBehaviour* Clone()\
{\
	return new x(static_cast<x>(*this));\
}

#define RequireStaticScript(x)\
bool _SB_Found##x = false;\
for (int i = 0; i < Hierarchy::listOfStaticScripts.size(); i++)\
{\
	if (Hierarchy::listOfStaticScripts[i] == #x)\
	{\
		_SB_Found##x = true;\
		break;\
	}\
}\
\
if (!_SB_Found##x)\
{\
	Hierarchy::allScripts.push_back(new x());\
	Hierarchy::allScripts.pop_back();\
	Hierarchy::allScripts[Hierarchy::allScripts.size() - 1]->ClassType = ScriptBehaviour::Class::Static;\
	Hierarchy::allScripts[Hierarchy::allScripts.size() - 1]->StartScript();\
	Hierarchy::listOfStaticScripts.push_back(#x);\
}

#define VisualAccess_Title(x)\
ImGUIElement* NewLine = new ImGUIElement(ImGUIElement::GUIType::Text, *Editor::Inspector, "");\
ImGUIElement* title = new ImGUIElement(ImGUIElement::GUIType::Text, *Editor::Inspector, #x);\
title->Alignment = ImGUIElement::AlignTo::Center

#define VisualAccess_AddValue(type, name, value)\
ImGUIElement* _In##name = new ImGUIElement(ImGUIElement::GUIType::type, *Editor::Inspector, #name, value)

#define VisualAccess_AddEditUIValue(nameOfUI, type, name, value)\
ImGUIElement* nameOfUI = new ImGUIElement(ImGUIElement::GUIType::type, *Editor::Inspector, #name, value)


#define SaveExternalScripts(x) std::cout << "Saving " << #x << " with size of " << x.size() << std::endl; for(int i = 0; i < x.size(); i++) { x[i]->OnSave(); }

#define SaveNewPointer(x, ...) \
std::cout << "Saving New Pointer... " << std::endl;\
SceneSaveAndLoad::isSavePointer = true;\
SceneSaveAndLoad::objectName = new std::ostringstream();\
*SceneSaveAndLoad::objectName << #x << scriptId;\
*SceneSaveAndLoad::allPointers << #x << "* " << SceneSaveAndLoad::objectName->str() << " = nullptr;" << std::endl;\
SceneSaveAndLoad::sceneCppSave << std::endl << "SceneSaveAndLoad::StartLoadArray(\"" << SceneSaveAndLoad::objectName->str() << "\");" << std::endl << SceneSaveAndLoad::objectName->str() << " = new " << #x << "(" << SceneSaveAndLoad::ModifyCode(#__VA_ARGS__) << ");" << std::endl;\
if(this->objectClassName == "")\
{\
	SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "->objectClassName = \"" << SceneSaveAndLoad::objectName->str() << "\";" << std::endl;\
}\
else\
{\
	SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "->objectClassName = \"" << this->objectClassName << "\";" << std::endl;\
}

//Needs work
#define SaveResource(source, classType, item, group) SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "->" << #source << " = " << SceneSaveAndLoad::CreateOrAddIntoStructure(group, #classType, #item) << ";" << std::endl
#define SavePointerValue(source, value) SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "->" << #source << " = " << SceneSaveAndLoad::ModifyCode(#value) << ";" << std::endl
#define SaveObjectValue(source, value) SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "." << #source << " = " << SceneSaveAndLoad::ModifyCode(#value) << ";" << std::endl
#define SaveValue(source) SceneSaveAndLoad::SaveValueToTITS(#source, source)

#define SaveReadInt(name) SceneSaveAndLoad::LoadValueInt(#name)
#define SaveReadString(name) SceneSaveAndLoad::LoadValueString(#name)
#define SaveReadFloat(name) SceneSaveAndLoad::LoadValueFloat(#name)
#define SaveReadResourceString(name) SceneSaveAndLoad::LoadResourceString(#name)
#define SaveReadResourceInt(name) SceneSaveAndLoad::LoadResourceInt(#name)
#define SaveReadResourceFloat(name) SceneSaveAndLoad::LoadResourceFloat(#name)

#define SaveObjectOnlyInstruction(function, objectType) SceneSaveAndLoad::sceneCppSave << #function << "(" << #objectType << SceneSaveAndLoad::objectName->str() << ");" << std::endl
#define SaveObjectInstruction(function, objectType, ...) SceneSaveAndLoad::sceneCppSave << #function << "(" << #objectType << SceneSaveAndLoad::objectName->str() << ###__VA_ARGS__ << ");" << std::endl
#define SaveInstruction(function) SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::ModifyCode(#function) << std::endl
#define SaveResourceInstructionAtEnd(function, group) SceneSaveAndLoad::CreateOrAddFunctionIntoStructureOnce(group, #function)

#define SaveNewScript(x)\
std::string _SBSave_##x;\
std::string _SB_Temp_##x = #x;\
_SB_Temp_##x += std::to_string(scriptId);\
_SBSave_##x += _SB_Temp_##x;\
_SBSave_##x += " = ";\
_SBSave_##x += "CurrentObject::AddScript<";\
_SBSave_##x += #x;\
_SBSave_##x += ">();";\
SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::ModifyCode(_SBSave_##x) << std::endl;\
SceneSaveAndLoad::scriptName = _SB_Temp_##x;\
*SceneSaveAndLoad::allPointers << #x << "* " << SceneSaveAndLoad::scriptName << " = nullptr;" << std::endl\

#define SaveInclude(x, y)\
std::string includeFile_##x;\
includeFile_##x += "#include ";\
includeFile_##x += "\"";\
includeFile_##x += y;\
includeFile_##x += "\"";\
if(SceneSaveAndLoad::allIncludes.find(includeFile_##x) == std::string::npos)\
{\
	SceneSaveAndLoad::allIncludes += "#ifndef ";\
	SceneSaveAndLoad::allIncludes += #x;\
	SceneSaveAndLoad::allIncludes += "_SCENE\n";\
	SceneSaveAndLoad::allIncludes += "#define ";\
	SceneSaveAndLoad::allIncludes += #x;\
	SceneSaveAndLoad::allIncludes += "_SCENE\n";\
	SceneSaveAndLoad::allIncludes += includeFile_##x;\
	SceneSaveAndLoad::allIncludes += "\n";\
	SceneSaveAndLoad::allIncludes += "#endif\n";\
}

#define SaveEnd()if(ClassType == Class::Script)\
{\
	SceneSaveAndLoad::scriptName = "";\
}\
for(int i = 0; i < scripts.size(); i++) { scripts[i]->OnSave(); }\
SceneSaveAndLoad::isSavePointer = false;\
if(ClassType != Class::Script)\
{\
	SceneSaveAndLoad::sceneCppSave << "SceneSaveAndLoad::EndLoadArray();" << std::endl;\
}

struct ScriptBehaviour
{
	bool isEditor = false;

	bool _dontStore = false;
	bool hasOwner = false;
	ScriptBehaviour* owner = nullptr;
	std::vector<ScriptBehaviour*> scripts;
	std::vector<ScriptBehaviour*> objectsAndPointers;
	int scriptId = -1;
	Transform transform;
	std::string objectClassName;

	Transform& GetTransform()
	{
		if (ClassType == Class::Script)
		{
			if (owner != nullptr)
			{
				return owner->GetTransform();
			}
		}
		
		return transform;
	}

	std::string& Name()
	{
		return objectClassName;
	}

	bool _start = true;
	bool _gameStart = false;
	bool _editorStart = false;
	bool _isCollisionEnter = false;
	bool _isCollisionExit = false;
	bool isEnabled = true;
	bool isBeingDestroyed = false;

	enum class Class
	{
		Script,
		Object,
		Pointer,
		Static
	};

	Class ClassType = Class::Script;

	ScriptBehaviour() 
	{
		objectClassName.clear();
		this->AddMyselfToHierarchy();
	}

	ScriptBehaviour(bool add)
	{
		objectClassName.clear();
		if (add) { this->AddMyselfToHierarchy(); }
	}

	virtual void OnCreate() { return; }
	virtual void OnDelete() { return; }

	virtual void OnLoad() { return; }
	virtual void OnSave() { return; }

	virtual ScriptBehaviour* Clone() { return nullptr; };

	ScriptBehaviour* CreateNewObject(ScriptBehaviour s) { ScriptBehaviour* newScript = new ScriptBehaviour(s); newScript->OnLoad(); return newScript; }

	void StartScript();
	void DestroyScript()
	{
		isBeingDestroyed = true;
		OnDestroy();
		delete this;
	}

	virtual void OnDestroy() { return; }

	void AddMyselfToHierarchy();
	void AddChild(ScriptBehaviour& child);
	void RemoveMyselfFromHierarchy() { Hierarchy::allScripts.erase(Hierarchy::allScripts.begin() + scriptId); }

	virtual void OnStartup() { return; }
	virtual void OnInternal() { return; }
	virtual void OnStart() 
	{
		if (ClassType == Class::Script || ClassType == Class::Static)
		{
			Hierarchy::allUpdateScripts.push_back(this);
		}

		return; 
	}
	virtual void OnEditorStart() { return; }
	virtual void OnUpdate() { return; }
	virtual void OnEditorUpdate() { return; }

	virtual void OnInspector() { return; }

	virtual void OnCollisionEnter() 
	{
		if (owner != nullptr && !_isCollisionEnter)
		{
			owner->OnCollisionEnter();
		}
		else
		{
			for (auto i : scripts)
			{
				i->_isCollisionEnter = true;
				i->OnCollisionEnter();
			}
		}

		_isCollisionEnter = false;
		return; 
	}

	virtual void OnCollisionExit()
	{
		if (owner != nullptr && !_isCollisionExit)
		{
			owner->OnCollisionExit();
		}
		else
		{
			for (auto i : scripts)
			{
				i->_isCollisionExit = true;
				i->OnCollisionExit();
			}
		}

		_isCollisionExit = false;
		return;
	}

	template <typename T>
	T* AddScript()
	{
		T* script = new T();

		if (Application::_engineState == Application::State::Game)
			Hierarchy::_setEditor = false;

		int lastItem = scripts.size();
		scripts.push_back(script);
		scripts[lastItem]->owner = this;
		scripts[lastItem]->hasOwner = true;
		scripts[lastItem]->StartScript();
		return script;
	}

	template <typename T, typename ... Args>
	T* AddScript(Args ... a)
	{
		T* script = new T(a...);

		if (Application::_engineState == Application::State::Game)
			Hierarchy::_setEditor = false;

		int lastItem = scripts.size();
		scripts.push_back(script);
		scripts[lastItem]->owner = this;
		scripts[lastItem]->hasOwner = true;
		scripts[lastItem]->StartScript();
		return script;
	}


	template <typename T>
	T* GetScript()
	{
		if (owner != nullptr)
		{
			return owner->GetScript<T>();
		}
		else if (typeid(T) == typeid(*this))
		{
			if (dynamic_cast<T*>(this))
				return (T*)this;
		}
		else
		{
			for (int i = 0; i < scripts.size(); i++)
			{
				if (typeid(T) == typeid(*scripts[i]))
				{
					if (dynamic_cast<T*>(scripts[i]))
					{
						return (T*)scripts[i];
					}
				}
			}
		}

		return nullptr;
	}
};