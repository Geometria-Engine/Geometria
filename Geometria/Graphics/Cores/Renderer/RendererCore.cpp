#include "RendererCore.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <future>
#include <mutex>
#include <thread>
#include "../MainAPI/Graphics.h"
#include "../../../Files/Files.h"
#include "../Texture/Texture.h"
#include "../../Externals/SceneAndDrawCall.h"
#include "../../../Application/Application.h"
#include "Multithreading/Multithreading.h"


//uint32_t RendererCore::VAO, RendererCore::VBO, RendererCore::EBO, RendererCore::reveal_texture, RendererCore::rbo_depth, RendererCore::accum_texture, RendererCore::oit_fbo, RendererCore::colorNT_texture;
//std::vector<Model*> allModels;
//std::vector<int> recentlyAdded;
//std::vector<Vertex> RendererCore::allVerts;
//std::vector<int> RendererCore::allIndices, RendererCore::modifyVectors;
//int RendererCore::_lastIndexCount, RendererCore::_lastVertCount, RendererCore::mvIndexBegin, RendererCore::mvIndexEnd;
//Matrix RendererCore::worldMatrix;
//Shader* RendererCore::mainShader, *RendererCore::fullScreenShader;

std::vector<Scene> SceneManager::_allScenes;

float vertices[] = {
		-1.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		-1.5f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		1.5f, -0.5f, 0.0f,
		1.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 2,   
	2, 3, 0,
	4, 5, 6,
	6, 7, 4
};

void RendererCore::AddModel(Model& m)
{
	AddModel(m, *SceneManager::MainScene().MainDrawCall());
}

void RendererCore::AddModel(Model& m, DrawCall& d)
{
	d.allModels.push_back(&m);
	d.objectsAndPointers.push_back(&m);
	m.hasOwner = true;

	// ADD TO HIERARCHY FOR EDITING
	d.AddChild(m);

	d.allModels[d.allModels.size() - 1]->SceneBelongsTo = d.sceneId;
	d.allModels[d.allModels.size() - 1]->DWBelongsTo = d.id;
	d.allModels[d.allModels.size() - 1]->modelId = m.scriptId;

	int texCount = 0;
	for (int i = 0; i < m.vertices.size(); i++)
	{
		m.vertices[i].color = glm::vec4(m.color.r, m.color.g, m.color.b, m.color.a);

		if (m.texture != nullptr)
		{
			m.vertices[i].textureGroupId = m.texture->texGroupId;

			float x = (float)m.texture->finalRect.x,
				y = (float)m.texture->finalRect.y,
				width = (float)m.texture->finalRect.x + (float)m.texture->finalRect.width,
				height = (float)m.texture->finalRect.y + (float)m.texture->finalRect.height;

			if (m.flipXTexture)
				std::swap(x, width);

			if (m.flipYTexture)
				std::swap(y, height);

			switch (texCount)
			{
			case 0:
				m.vertices[i].texCoords =
					glm::vec2(x / (float)TextureManager::textureGroups[m.texture->texGroupId].width,
						y / (float)TextureManager::textureGroups[m.texture->texGroupId].height);

				texCount++;
				break;
			case 1:
				m.vertices[i].texCoords =
					glm::vec2(x / (float)TextureManager::textureGroups[m.texture->texGroupId].width,
						height / (float)TextureManager::textureGroups[m.texture->texGroupId].height);

				texCount++;
				break;
			case 2:

				m.vertices[i].texCoords =
					glm::vec2(width / (float)TextureManager::textureGroups[m.texture->texGroupId].width,
						height / (float)TextureManager::textureGroups[m.texture->texGroupId].height);

				texCount++;
				break;
			case 3:
				m.vertices[i].texCoords =
					glm::vec2(width / (float)TextureManager::textureGroups[m.texture->texGroupId].width,
						y / (float)TextureManager::textureGroups[m.texture->texGroupId].height);

				texCount = 0;
				break;
			}
		}

		d.allVerts.push_back(m.vertices[i]);
		m.indexVertices.push_back(d.allVerts.size() - 1);

		d.allVerts[d.allVerts.size() - 1].modelId = d.allModels.size() - 1;
		d.allVerts[d.allVerts.size() - 1].modelVertexId = m.indexVertices.size() - 1;
	}

	//allModels[allModels.size() - 1]->OnChange();

	//SortVertices(m.indexVertices, m.modelIndex);
	//m.PrintData();
}

void RendererCore::UpdateSorting(DrawCall& d)
{
	d.updateSorting = true;
}

void RendererCore::AddImGUIElement(ImGUIElement& i)
{
	AddImGUIElement(i, *SceneManager::MainScene().MainDrawCall());
}

void RendererCore::AddImGUIElement(ImGUIElement& i, DrawCall& d)
{
	std::cout << "Draw Call ID: " << d.scriptId << std::endl;

	if (d.allImGUI.size() != 0)
	{
		int index = -1;
		for (int s = 0; s < d.allImGUI.size(); s++)
		{
			bool checkUITag = d.allImGUI[s]->UITag == i.UITag && d.allImGUI[s]->UITag != "";
			bool checkText = !checkUITag && d.allImGUI[s]->text == i.text;
			if (checkUITag || checkText)
			{
				index = s;
				break;
			}
		}

		i.ownerSceneId = d.sceneId;
		i.ownerDrawId = d.id;

		if (index != -1)
		{
			d.allImGUI[index] = &i;
		}
		else
		{
			d.allImGUI.push_back(&i);
		}
	}
	else
	{
		d.allImGUI.push_back(&i);
	}
}

void RendererCore::SetUp()
{
	/*for (auto i : SceneManager::_allScenes)
	{
		for (auto d : i._drawCalls)
		{
			d->DeleteFromRAM();
			d = nullptr;
		}

		i._drawCalls.clear();
	}*/

	SceneManager::_allScenes.clear();

	SceneManager::CreateScene();
	SceneManager::MainScene()._drawCalls.clear();
	SceneManager::MainScene().CreateDrawCall(true);
	SceneManager::MainScene().MainDrawCall()->isMain = true;
	SceneManager::MainScene().MainDrawCall()->objectClassName = "Main Draw Call";
	//SceneManager::MainScene().MainDrawCall().AddMyselfToHierarchy();
}

//Threading
std::vector<std::future<void>> threads;
bool endRenderThreads = false;
std::mutex NUT;

void SortVerticesMultithreading()
{
	auto wait_duration = std::chrono::milliseconds(250);

	while (!Graphics::CanClose() && !endRenderThreads)
	{
		//std::cout << "Running!" << std::endl;
		std::lock_guard<std::mutex> mutex(NUT);
		RendererCore::SortVertices();
		NUT.unlock();
		std::this_thread::sleep_for(wait_duration);
	}

	std::cout << "I'm done Sorting!" << std::endl;
}

void RendererCore::Start()
{
	OpenGL_Start();

	SetUpWorldMatrix();
}

//glGenVertexArrays(1, &VAO);
//glGenBuffers(1, &VBO);
//
//// Bind VAO, then bind VBO
//glBindVertexArray(VAO);
//glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//// Copy the vertex data into the buffer's memory
//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//// Set attributes that describe how OpenGL should interpret the vertex data
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//glEnableVertexAttribArray(0);

float fullscreen_quad_verts[] = { -1.0, -1.0, 0.0,
									-1.0,  3.0, 0.0,
									 3.0, -1.0, 0.0 };

int32_t binding_idx = 0;
GLuint fullscreen_quad_vao, fullscreen_quad_vbo;

void RendererCore::OpenGL_Start()
{
	//==[ OPENGL ]==//
	{
		for (int scene = 0; scene < SceneManager::_allScenes.size(); scene++)
		{
			for (int draw = 0; draw < SceneManager::_allScenes[scene]._drawCalls.size(); draw++)
			{
				DrawCall& d = *SceneManager::_allScenes[scene]._drawCalls[draw];
				OpenGL_Start_DrawCall(d);
			}
		}
	}
}

void RendererCore::OpenGL_Start_DrawCall(DrawCall& d)
{
	if (d.allIndices.size() == 0)
	{
		d.mainShader = new Shader(Files::Read("EngineResources/basic.vert"), Files::Read("EngineResources/basic.frag"));

		uint32_t indSize = 600000;
		d.allIndices.resize(indSize);

		int offset = 0;
		for (int i = 0; i < indSize; i += 6)
		{
			d.allIndices[i + 0] = 0 + offset;
			d.allIndices[i + 1] = 1 + offset;
			d.allIndices[i + 2] = 2 + offset;

			d.allIndices[i + 3] = 2 + offset;
			d.allIndices[i + 4] = 3 + offset;
			d.allIndices[i + 5] = 0 + offset;

			offset += 4;
		}

		glGenVertexArrays(1, &d.VAO);
		glGenBuffers(1, &d.VBO);
		glGenBuffers(1, &d.EBO);
		glBindVertexArray(d.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), NULL, GL_STREAM_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, d.allIndices.size() * sizeof(uint32_t), &d.allIndices[0], GL_DYNAMIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

		if (Application::IsPlatform(Application::Platform::Windows))
		{
			if (Graphics::IsIntelGPU())
				SetVAttr_IntelWindows(d);
			else if (Graphics::IsAMDGPU())
				SetVAttr_AMDWindows(d);
			else
				SetVAttr_Universal(d);
		}
		else
		{
			SetVAttr_Universal(d);
		}
	}

	if (d.type == DrawCall::Type::UI)
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(Graphics::GetMainWindow().openGLWindow, true);
		ImGui_ImplOpenGL3_Init("#version 130");
		ImGui::StyleColorsDark();
	}
}

void RendererCore::SetVAttr_Universal(DrawCall& d)
{
	int pointerPosition = 0;

	// Vertex Position
	glVertexAttribPointer(pointerPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Color
	glVertexAttribPointer(pointerPosition, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Texture UV Coordinates
	glVertexAttribPointer(pointerPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Texture Group ID
	glVertexAttribPointer(pointerPosition, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureGroupId));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	glBindVertexArray(0);
}

void RendererCore::SetVAttr_AMDWindows(DrawCall& d)
{
	int pointerPosition = 0;

	// Differences with Universal:
	// - To be honest, i don't fucking know what the fuck the AMD Windows Drivers even want at this point

	// Texture Group ID
	glVertexAttribPointer(pointerPosition, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureGroupId));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Color
	glVertexAttribPointer(pointerPosition, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Vertex Position
	glVertexAttribPointer(pointerPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Texture UV Coordinates
	glVertexAttribPointer(pointerPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	glBindVertexArray(0);
}

void RendererCore::SetVAttr_IntelWindows(DrawCall& d)
{
	int pointerPosition = 0;

	// Diferences with Universal:
	// - Intel on Windows wants to send everything but the position first.

	// Color
	glVertexAttribPointer(pointerPosition, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Texture UV Coordinates
	glVertexAttribPointer(pointerPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Texture Group ID
	glVertexAttribPointer(pointerPosition, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureGroupId));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	// Vertex Position
	glVertexAttribPointer(pointerPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(pointerPosition);
	pointerPosition++;

	glBindVertexArray(0);
}

int RendererCore::WindowWidth()
{
	return Graphics::GetMainWindow().width;
}

int RendererCore::WindowHeight()
{
	return Graphics::GetMainWindow().height;
}

DrawCall* RendererCore::FindDrawCall(int sceneId, int id)
{
	for (int scene = 0; scene < SceneManager::_allScenes.size(); scene++)
	{
		Scene& s = SceneManager::_allScenes[scene];

		if (s.id == sceneId)
		{
			for (int draw = 0; draw < SceneManager::_allScenes[scene]._drawCalls.size(); draw++)
			{
				DrawCall& d = *s._drawCalls[draw];

				if (d.id == id)
				{
					return &d;
					break;
				}
			}
			break;
		}
	}

	return nullptr;
}

void RendererCore::SetUpWorldMatrix()
{
	for (int scene = 0; scene < SceneManager::_allScenes.size(); scene++)
	{
		for (int draw = 0; draw < SceneManager::_allScenes[scene]._drawCalls.size(); draw++)
		{
			DrawCall& d = *SceneManager::_allScenes[scene]._drawCalls[draw];
			d.worldMatrix = Matrix(1.0f);
			d.worldMatrix = Matrix::Translate(d.worldMatrix, Vector3(0));
			d.worldMatrix = Matrix::Rotate(d.worldMatrix, Vector3(0));
			d.worldMatrix = Matrix::Scale(d.worldMatrix, Vector3(1));
		}
	}
}

bool preRender = true;
void RendererCore::Render()
{
	if (preRender)
	{
		endRenderThreads = false;
		//threads.push_back(std::async(std::launch::async, SortVerticesMultithreading));
		Multithreading::RunUpdateThread([]() { RendererCore::SortVertices(); }, 250);
		preRender = false;
	}

	//==[ OPENGL ]==//
	{
		OpenGL_Render();
	}
}

void RendererCore::EndThreads()
{
	endRenderThreads = true;
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i].wait();
	}
	preRender = true;
	threads.clear();
	std::vector<std::future<void>>().swap(threads);
}

struct {
	bool operator()(Vertex a, Vertex b) const { return a.position.z < b.position.z; }
} vertexOrder;

void Swap(int& first, int& second)
{
	int temp = first;
	first = second;
	second = temp;
}

struct MVTimer
{
	int modelId = 0;
	int timer = 0;
	int sceneId = 0;
	int drawId = 0;
};

std::vector<MVTimer> vTimer;

void RendererCore::SortVertices()
{
	/*for (int i = 0; i < vTimer.size(); i++)
	{
		vTimer[i].timer++;
		SceneManager::_allScenes[vTimer[i].sceneId]._drawCalls[vTimer[i].drawId].allModels[vTimer[i].modelId]->OnChange();

		if (vTimer[i].timer > 3)
		{
			vTimer.erase(vTimer.begin() + i);
		}
	}*/

	for (int scene = 0; scene < SceneManager::_allScenes.size(); scene++)
	{
		for (int draw = 0; draw < SceneManager::_allScenes[scene]._drawCalls.size(); draw++)
		{
			DrawCall& d = *SceneManager::_allScenes[scene]._drawCalls[draw];

			if (d.sort != DrawCall::Sorting::Static)
			{
				//std::cout << vTimer.size() << std::endl;

				int begin = 0, end = 0, beginBackwards = 0, endBackwards = 0;
				int splitIn = d.allVerts.size() / 20;

				if (splitIn < 500)
				{
					splitIn = d.allVerts.size();
				}

				if (d.start < 2)
				{
					splitIn = d.allVerts.size();
					std::cout << "Loading and Sorting... (Phase " << d.start + 1 << ") Split in: " << splitIn << std::endl;
				}
				else if (d.start == 2)
				{
					std::cout << "Finished!" << std::endl;
					d.start++;
				}

				/*if (d.start > 2)
				{
					begin = d.allModels[d.allVerts[begin].modelId]->indexVertices[0];
				}*/

				end = begin + splitIn;

				if (d.start > 2 && end - 1 < d.allVerts.size())
				{
					int size = d.allModels[d.allVerts[end - 1].modelId]->indexVertices.size() - 1;
					end = d.allModels[d.allVerts[end - 1].modelId]->indexVertices[size];
				}

				//begin -= splitIn;

				int amount = end - begin;

				/*bool doSort = false;
				if (d.sort == DrawCall::Sorting::AtStartup && d.start < 2)
					doSort = true;
				else if (d.sort == DrawCall::Sorting::Update)
					doSort = true;
				else
					doSort = false;*/

				bool startupSort = DrawCall::Sorting::AtStartup && d.start < 2;
				if (d.sort == DrawCall::Sorting::Update || startupSort || d.updateSorting)
				{
					std::vector<int> change;

					/*std::cout << "====" << std::endl;
					for (int i = 0; i < d.allVerts.size(); i++)
					{
						std::cout << d.allVerts[i].modelId << std::endl;
					}*/

					std::sort(d.allModels.begin(), d.allModels.end(), [&](Model* i, Model* j)
						{
							if (d.mode == DrawCall::Mode::To2D)
							{
								if (i->sortingLayer == j->sortingLayer)
									return i->sortingLayer == j->sortingLayer && i->sortingOrder > j->sortingOrder;

								return i->sortingLayer > j->sortingLayer;
							}

							return Vector3::Distance(i->transform.position + i->transform.scale, Graphics::MainCamera()->GetCurrentPosition()) > Vector3::Distance(j->transform.position + j->transform.scale, Graphics::MainCamera()->GetCurrentPosition());
						});

					int vertexCount = 0;
					for (int i = 0; i < d.allModels.size(); i++)
					{
						int changes = 0;
						for (int j = 0; j < d.allModels[i]->indexVertices.size(); j++)
						{
							if (!d.allModels[i]->isDeleted)
							{
								int lastOrder = d.allVerts[d.allModels[i]->indexVertices[j]].order;
								if (lastOrder != vertexCount)
								{
									d.allVerts[d.allModels[i]->indexVertices[j]].modelId = d.allModels[i]->scriptId;
									d.allVerts[d.allModels[i]->indexVertices[j]].modelVertexId = j;
									d.allVerts[d.allModels[i]->indexVertices[j]].order = vertexCount;
									d.allModels[i]->indexVertices[j] = vertexCount;

									changes++;
								}
								vertexCount++;
							}
						}

						if (changes != 0)
						{
							change.push_back(i);
						}
					}

					std::sort(d.allVerts.begin(), d.allVerts.end(), [&](Vertex i, Vertex j)
						{
							return i.order < j.order;
						});

					if (d.deleteProcess)
					{
						for (int i = 0; i < d.allVerts.size();)
						{
							if (d.allVerts[i].isDeleted)
							{
								d.allVerts.erase(d.allVerts.begin() + i);
							}
							else
							{
								i++;
							}
						}

						for (int i = 0; i < d.allModels.size();)
						{
							if (d.allModels[i]->isDeleted)
							{
								d.allModels.erase(d.allModels.begin() + i);
							}
							else
							{
								i++;
							}
						}
					}

					if (d.start < 2)
					{
						d.start++;
					}
					else
					{
						for (int i : change)
						{
							d.allModels[i]->OnChange(true, false);
						}
					}
				}
				else
				{
					//std::cout << draw << ": Not Updating!" << std::endl;
				}

				if (d.updateSorting)
					d.updateSorting = false;
			}
			else
			{
				//std::cout << draw << ": Is Static!" << std::endl;
				d.start = 3;
			}
		}
	}
}

//std::vector<std::vector<int>> modifyVerticesArray;

void RendererCore::ModifyVerticesOnBuffer(std::vector<int> index, bool rC, DrawCall& d)
{
	for (int i = 0; i < index.size(); i++)
	{
		int repeat = 0;
		for (int m : d.modifyVectors)
		{
			if (m == index[i])
				repeat++;
		}

		if (repeat == 0)
		{
			d.modifyVectors.push_back(index[i]);
		}
	}

	//if (rC)
	//{
	//	MVTimer t;
	//	t.modelId = d.allVerts[index[0]].modelId;
	//	t.sceneId = d.sceneId;
	//	t.drawId = d.id;
	//	vTimer.push_back(t);
	//}
}

int vertexChangeArrayRange = 100;
void RendererCore::GetBeginAndEndVectors()
{
	for (int scene = 0; scene < SceneManager::_allScenes.size(); scene++)
	{
		for (int draw = 0; draw < SceneManager::_allScenes[scene]._drawCalls.size(); draw++)
		{
			DrawCall& d = *SceneManager::_allScenes[scene]._drawCalls[draw];

			if (d.modifyVectors.size() != 0)
			{
				std::sort(d.modifyVectors.begin(), d.modifyVectors.end());

				int lastVertex = 0;
				int halfvertexChangeArrayRange = vertexChangeArrayRange / 2;
				for (int i = 0; i < d.modifyVectors.size(); i++)
				{
					if (d.modifyVectors[i] - lastVertex > 100 || i == 0)
					{
						std::vector<int> e;
						e.resize(2);
						d.modifyVerticesArray.push_back(e);

						if (d.modifyVectors[i] - halfvertexChangeArrayRange < 0)
							d.modifyVerticesArray[d.modifyVerticesArray.size() - 1][0] = 0;
						else
							d.modifyVerticesArray[d.modifyVerticesArray.size() - 1][0] = d.modifyVectors[i] - halfvertexChangeArrayRange;
					}
					else if (i != 0)
					{
						if (d.modifyVectors[i] + halfvertexChangeArrayRange > d.allVerts.size())
							d.modifyVerticesArray[d.modifyVerticesArray.size() - 1][1] = d.allVerts.size();
						else
							d.modifyVerticesArray[d.modifyVerticesArray.size() - 1][1] = d.modifyVectors[i] + halfvertexChangeArrayRange;
					}

					lastVertex = d.modifyVectors[i];
				}

				for (int i = 0; i < d.modifyVerticesArray.size();)
				{
					if (d.modifyVerticesArray[i][1] < d.modifyVerticesArray[i][0])
					{
						d.modifyVerticesArray.erase(d.modifyVerticesArray.begin() + i);
					}
					else
					{
						i++;
					}
				}

				/*mvIndexBegin = modifyVectors[0];
				mvIndexEnd = modifyVectors[modifyVectors.size() - 1];*/
			}
		}
	}
}

//RendererCore::mainShader->Enable();
//glBindVertexArray(VAO);
//glDrawArrays(GL_TRIANGLES, 0, 3);

 /*(indices.size() / 6) * 4*/

int depthFunct;
static std::vector<Vertex> copy;
bool onStart = true;
static int oit_visualization_mode = 2;

void SingleSort(DrawCall& d)
{
	std::vector<int> change;

	std::stable_sort(d.allModels.begin(), d.allModels.end(), [&](Model* i, Model* j)
		{
			return Vector3::Distance(i->transform.position + i->transform.scale, Graphics::MainCamera()->GetCurrentPosition()) > Vector3::Distance(j->transform.position + i->transform.scale, Graphics::MainCamera()->GetCurrentPosition());
		});

	int vertexCount = 0;
	for (int i = 0; i < d.allModels.size(); i++)
	{
		int changes = 0;
		for (int j = 0; j < d.allModels[i]->indexVertices.size(); j++)
		{
			int lastOrder = d.allVerts[d.allModels[i]->indexVertices[j]].order;
			if (lastOrder != vertexCount)
			{
				d.allVerts[d.allModels[i]->indexVertices[j]].modelId = i;
				d.allVerts[d.allModels[i]->indexVertices[j]].modelVertexId = j;
				d.allVerts[d.allModels[i]->indexVertices[j]].order = vertexCount;
				d.allModels[i]->indexVertices[j] = vertexCount;

				changes++;
			}
			vertexCount++;
		}

		if (changes != 0)
		{
			change.push_back(i);
		}
	}

	std::sort(d.allVerts.begin(), d.allVerts.end(), [&](Vertex i, Vertex j)
		{
			return i.order < j.order;
		});

	for (int i : change)
	{
		d.allModels[i]->OnChange(true, false);
	}
}

void RendererCore::OpenGL_Render()
{
	GetBeginAndEndVectors();

	//==[ OPENGL ]==//
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int scene = 0; scene < SceneManager::_allScenes.size(); scene++)
		{
			for (int draw = 0; draw < SceneManager::_allScenes[scene]._drawCalls.size(); draw++)
			{
				//std::cout << "Scene: " << scene + 1 << " Draw Call: " << draw + 1 << std::endl;
				DrawCall& d = *SceneManager::_allScenes[scene]._drawCalls[draw];

				if (d.deleteProcess)
				{
					RendererCore::SortVertices();
					d.deleteProcess = false;
				}

				if (d.type == DrawCall::Type::UI)
				{
					if (d.start > 2 && d.allImGUI.size() != 0)
					{
						//TODO: Make it so it is compatible with OpenGL 2 TOO!
						ImGui_ImplGlfw_NewFrame();
						ImGui_ImplOpenGL3_NewFrame();
						ImGui::NewFrame();

						for (int i = 0; i < d.allImGUI.size(); i++)
						{
							d.allImGUI[i]->OnUpdate();
						}

						ImGui::Render();
						ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
					}
				}

				/*if (sortTime > 2)
				{
					std::cout << "Sorting!" << std::endl;
					SingleSort(d);
					sortTime = 0;
				}
				else
				{
					std::cout << sortTime << std::endl;
				}*/

				if (d.start > 2 && d.allModels.size() != 0)
				{
					glBindBuffer(GL_ARRAY_BUFFER, d.VBO);

					if (d.allVerts.size() != d._lastVertCount || d.refresh)
					{
						if (d.type == DrawCall::Type::Dynamic || d.type == DrawCall::Type::UI)
						{
							glBufferData(GL_ARRAY_BUFFER, d.allVerts.size() * sizeof(Vertex), NULL, GL_STREAM_DRAW);
							d.bufferPointer = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
							d._lastVertCount = d.allVerts.size();

							memcpy(d.bufferPointer, &d.allVerts[0], d.allVerts.size() * sizeof(Vertex));
							glUnmapBuffer(GL_ARRAY_BUFFER);
						}
						else if (d.type == DrawCall::Type::Hybrid || d.type == DrawCall::Type::Object)
						{
							glBufferData(GL_ARRAY_BUFFER, d.allVerts.size() * sizeof(Vertex), d.allVerts.data(), GL_DYNAMIC_DRAW);
							d._lastVertCount = d.allVerts.size();
						}

						d.refresh = false;
					}
					else
					{
						if (d.type == DrawCall::Type::Dynamic || d.type == DrawCall::Type::UI)
						{
							int vertSize = d.modifyVerticesArray.size();
							if (vertSize != 0)
							{
								for (int m = 0; m < vertSize; m++)
								{
									for (int i = d.modifyVerticesArray[m][0]; i < d.modifyVerticesArray[m][1]; i++)
										d.bufferPointer[i] = d.allVerts[i];
								}
							}
						}
						else if (d.type == DrawCall::Type::Hybrid && d.updateHybrid == true)
						{
							glBufferSubData(GL_ARRAY_BUFFER, 0, d.allVerts.size() * sizeof(Vertex), d.allVerts.data());
							d.updateHybrid = false;
						}

					}

					//std::cout << mvIndexBegin << " - " << mvIndexEnd << " = " << mvIndexEnd - mvIndexBegin << std::endl;

					if (!d.elementsChanged)
					{
						d.modifyVectors.clear();
						//changeModelsInSinglethread.clear();
						d.updateCounter = 0;
					}
					else if (d.updateCounter < 10)
					{
						d.updateCounter++;
					}
					else
					{
						d.elementsChanged = false;
					}

					d.modifyVerticesArray.clear();

					d.mvIndexBegin = 0;
					d.mvIndexEnd = 0;

					d.mainShader->Enable();

					Matrix MVP;
					
					if (d.type != DrawCall::Type::UI)
						MVP = d.worldMatrix * Graphics::MainCamera()->GetViewProjection();
					else
					{
						MVP = d.worldMatrix * Graphics::MainCamera()->GetUIProjection(RendererCore::WindowWidth(), RendererCore::WindowHeight());
					}

					d.mainShader->GPU_SetInt("textures0", 0);
					//d.mainShader->GPU_SetInt("texture0", 1);
					d.mainShader->GPU_SetMatrix("MVP", MVP);

					TextureManager::EnableTextures();

					//glGetIntegerv(GL_DEPTH_FUNC, &depthFunct);
					glBindVertexArray(d.VAO);
					glDrawElements(GL_TRIANGLES, d.allVerts.size() * 2, GL_UNSIGNED_INT, nullptr);

					while (GLenum error = glGetError())
					{
						std::cout << "[ERROR] " << error << std::endl;
					}

					//glDrawElements(GL_TRIANGLES, allIndices.size(), GL_UNSIGNED_INT, nullptr);
				}
			}
		}
	}
}
