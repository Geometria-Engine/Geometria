#pragma once
#include <vector>
#include "ImGUIElement.h"

class RendererCore;
struct Matrix;
struct ScriptBehaviour;
struct Vertex;
class Shader;
class Model;

class DrawCall : public ScriptBehaviour
{
public:

	DrawCall()
	{
		Hierarchy::allScripts.pop_back();
		ClassType = ScriptBehaviour::Class::Object;
	}

	void OnSave()
	{
		SaveDrawCall();
	}

	void SaveDrawCall();

	void Close();

	DrawCall& Target();

	enum Type
	{
		Dynamic,
		Hybrid,
		Object,
		UI
	};

	enum Sorting
	{
		Update,
		AtStartup,
		Static
	};

	enum Mode
	{
		To3D,
		To2D
	};

	int id = 0;
	int sceneId = 0;

	bool refresh = false;

	void Refresh();
	void OnDestroy()
	{
		DeleteFromRAM();
	}
	void DeleteFromRAM();

	int uiWidth = 800, uiHeight = 600;

	std::vector<Model*> allModels;
	std::vector<ImGUIElement*> allImGUI;
	std::vector<Vertex> allVerts;
	std::vector<uint32_t> allIndices;

	Type type = Type::Dynamic;
	Sorting sort = Sorting::Update;
	Mode mode = Mode::To3D;

	int _lastVertCount = 0;
	int _lastIndexCount = 0;
	int _modifyVertexTimer = 0;
	int _modifyVertexTimeToDestroy = 0;

	std::vector<int> modifyVectors;
	std::vector<std::vector<int>> modifyVerticesArray;

	int changeCounter = 0;
	int beginSort = 0, endSort = 1000;
	int start = 0;
	bool elementsChanged = false;
	bool deleteProcess = false, changeProcess = false;
	int highestModelId = 0;
	int updateCounter = 0;

	bool updateHybrid = false, isMain = false, updateSorting = false;

	int mvIndexBegin, mvIndexEnd;

	Vertex* bufferPointer = nullptr;

	Shader* mainShader = nullptr;
	uint32_t VAO, VBO, EBO;

	Matrix worldMatrix;
};

class Scene
{
public:
	int id;
	std::vector<DrawCall*> _drawCalls;

	DrawCall* MainDrawCall()
	{
		return _drawCalls[0];
	}

	DrawCall* CreateDrawCall()
	{
		return CreateDrawCall(false);
	}

	DrawCall* CreateDrawCall(bool startEmpty);
};

class SceneManager
{
public:
	static std::vector<Scene> _allScenes;

	static Scene& MainScene()
	{
		return _allScenes[0];
	}

	static Scene& CreateScene()
	{
		return CreateScene(false);
	}

	static Scene& CreateScene(bool createDrawCall)
	{
		Scene s;
		_allScenes.push_back(s);

		if (createDrawCall)
			_allScenes[_allScenes.size() - 1].CreateDrawCall();

		_allScenes[_allScenes.size() - 1]._drawCalls.clear();
		_allScenes[_allScenes.size() - 1].id = _allScenes.size() - 1;
		return _allScenes[_allScenes.size() - 1];
	}
};
