#include <vector>

class Model;
struct Vertex;
struct Matrix;
class Shader;

class Scene;
class DrawCall;

class ImGUIElement;

#ifndef RENDERER_CORE_H
#define RENDERER_CORE_H
class RendererCore
{
public:
//	static std::vector<Vertex> allVerts;
//	static std::vector<int> allIndices;
//private:
//	static int _lastVertCount;
//	static int _lastIndexCount;
//	static int _modifyVertexTimer;
//	static int _modifyVertexTimeToDestroy;
//
//	static std::vector<int> modifyVectors;
//
//	static int mvIndexBegin, mvIndexEnd;
//public:
//	static Shader* mainShader, *fullScreenShader;
//	static uint32_t VAO, VBO, EBO, fullScreenVAO, fullScreenVBO, oit_fbo, accum_texture, reveal_texture, colorNT_texture, rbo_depth;

	//static Matrix worldMatrix;

	static void AddModel(Model& m);
	static void AddModel(Model& m, DrawCall& d);

	static void UpdateSorting(DrawCall& d);

	static void AddImGUIElement(ImGUIElement& i);
	static void AddImGUIElement(ImGUIElement& i, DrawCall& d);

	static void SetUp();

	static void Start();
	static void SetUpWorldMatrix();
	static void Render();

	static void SetVAttr_Universal(DrawCall& d);
	static void SetVAttr_AMDWindows(DrawCall& d);
	static void SetVAttr_IntelWindows(DrawCall& d);

	static void EndThreads();

	static void SortVertices();
	static void ModifyVerticesOnBuffer(std::vector<int> index, bool rC, DrawCall& d);

	static void GetBeginAndEndVectors();

	static void OpenGL_Start();

	static void OpenGL_Start_DrawCall(DrawCall& d);

	static int WindowWidth();
	static int WindowHeight();

	static DrawCall* FindDrawCall(int sceneId, int id);

	static void CheckForDeleteObjectsAndCleanup();

	static void OpenGL_Render();
};
#endif