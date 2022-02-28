#include "Behaviour.h"

int Hierarchy::highestScriptId = 0;
std::vector<ScriptBehaviour*> Hierarchy::deleteList;
std::vector<ScriptBehaviour*> Hierarchy::allScripts, Hierarchy::allStaticScripts;
std::vector<ScriptBehaviour*> Hierarchy::allUpdateScripts, Hierarchy::allUpdateEditorScripts;
std::vector<std::pair<std::string, ScriptBehaviour*>> Hierarchy::scriptsWithVisualAccess;
std::vector<std::string> Hierarchy::listOfStaticScripts;
bool Hierarchy::_setEditor = false;

Matrix Transform::GetTransform()
{
	Matrix m;
	m = Matrix(1.0f);
	m = Matrix::Translate(m, position);
	m = Matrix::Rotate(m, rotation);
	m = Matrix::Scale(m, scale);
	return m;
}

void Transform::LoadTransform()
{
	if (SceneSaveAndLoad::StartLoadArray("Transform"))
	{
		/*SceneSaveAndLoad::LoadValue(position.x, "Position X");
		SceneSaveAndLoad::LoadValue(position.y, "Position Y");
		SceneSaveAndLoad::LoadValue(position.z, "Position Z");
		SceneSaveAndLoad::LoadValue(position.w, "Position W");

		SceneSaveAndLoad::LoadValue(rotation.x, "Rotation X");
		SceneSaveAndLoad::LoadValue(rotation.y, "Rotation Y");
		SceneSaveAndLoad::LoadValue(rotation.z, "Rotation Z");
		SceneSaveAndLoad::LoadValue(rotation.w, "Rotation W");

		SceneSaveAndLoad::LoadValue(scale.x, "Scale X");
		SceneSaveAndLoad::LoadValue(scale.y, "Scale Y");
		SceneSaveAndLoad::LoadValue(scale.z, "Scale Z");
		SceneSaveAndLoad::LoadValue(scale.w, "Scale W");*/

		SceneSaveAndLoad::EndLoadArray();
	}
}

void Transform::SaveTransform()
{
	SceneSaveAndLoad::StartSaveArray("Transform");

	/*SceneSaveAndLoad::SaveValue("Position X", position.x);
	SceneSaveAndLoad::SaveValue("Position Y", position.y);
	SceneSaveAndLoad::SaveValue("Position Z", position.z);
	SceneSaveAndLoad::SaveValue("Position W", position.w);

	SceneSaveAndLoad::SaveValue("Rotation X", rotation.x);
	SceneSaveAndLoad::SaveValue("Rotation Y", rotation.y);
	SceneSaveAndLoad::SaveValue("Rotation Z", rotation.z);
	SceneSaveAndLoad::SaveValue("Rotation W", rotation.w);

	SceneSaveAndLoad::SaveValue("Scale X", scale.x);
	SceneSaveAndLoad::SaveValue("Scale Y", scale.y);
	SceneSaveAndLoad::SaveValue("Scale Z", scale.z);
	SceneSaveAndLoad::SaveValue("Scale W", scale.w);*/

	SceneSaveAndLoad::EndSaveArray();
}

void Hierarchy::EditorMode(bool e)
{
	_setEditor = e;
}

void Hierarchy::AddScript(ScriptBehaviour* s)
{
	Hierarchy::allScripts.push_back(s);
	Hierarchy::allScripts[Hierarchy::allScripts.size() - 1]->StartScript();
}

void ScriptBehaviour::StartScript()
{
	OnStartup();
	isEditor = Hierarchy::_setEditor;

	OnInternal();

	if (ClassType == Class::Object || ClassType == Class::Static || hasOwner)
	{
		if (Application::_engineState == Application::State::Game)
		{
			if (!_gameStart)
			{
				OnStart();
				_gameStart = true;
				_editorStart = false;
			}
		}
		else if (isEditor)
		{
			if (!_editorStart)
			{
				OnEditorStart();
				_editorStart = true;
				_gameStart = false;
			}
		}

		if (ClassType == Class::Script || ClassType == Class::Static)
		{
			if (isEditor)
				Hierarchy::allUpdateEditorScripts.push_back(this);
			else
				Hierarchy::allUpdateScripts.push_back(this);
		}

		for (int i = 0; i < scripts.size(); i++)
		{
			scripts[i]->hasOwner = true;
			scripts[i]->StartScript();
		}
	}
}

void ScriptBehaviour::AddMyselfToHierarchy()
{
	Hierarchy::AddScript(this);
	scriptId = Hierarchy::highestScriptId;
	Hierarchy::highestScriptId++;
}

void ScriptBehaviour::AddChild(ScriptBehaviour& child)
{
	scripts.push_back(&child);
	child.hasOwner = true;
}

void Hierarchy::StartScripts()
{
	Hierarchy::allUpdateScripts.clear();

	for (int i = 0; i < Hierarchy::allScripts.size(); i++)
	{
		Hierarchy::allScripts[i]->StartScript();
	}
}

void Hierarchy::UpdateScripts()
{
	if (Application::_engineState == Application::State::Game)
	{
		for (int i = 0; i < Hierarchy::allUpdateScripts.size(); i++)
		{
			Hierarchy::allUpdateScripts[i]->OnUpdate();
		}
	}
	else
	{
		for (int i = 0; i < Hierarchy::allUpdateEditorScripts.size(); i++)
		{
			Hierarchy::allUpdateEditorScripts[i]->OnEditorUpdate();
		}
	}
}

void Hierarchy::AddObjectToDeleteList(ScriptBehaviour* id)
{
	/*deleteList.push_back(id);*/
}
