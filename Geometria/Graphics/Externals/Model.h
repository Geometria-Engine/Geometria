#include "../../Behaviours/Behaviour.h"
#include "Vertex.h"
class Texture;

// Move this beautiful struct wherever you want
struct UIAnchors {
	Vector2 min, max;
	UIAnchors() {
		min = max = Vector2::zero();
	}

	UIAnchors(Vector2 val) {
		this->min = this->max = val;
	}

	UIAnchors(Vector2 min, Vector2 max) {
		this->min = min;
		this->max = max;
	}
};
struct UITransform {
	Vector2 position, size, pivot;
	float top, left, bottom, right; // UI Box sides

	UIAnchors anchors;

	enum UIPosition {
		TOP_LEFT, TOP_CENTER, TOP_RIGHT, 
		MIDDLE_LEFT, MIDDLE_CENTER, MIDDLE_RIGHT, 
		BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT
	};

	UITransform() {
		top = left = bottom = right = 0.f;
		position = Vector2::zero();
		size = 25.f;
		anchors = UIAnchors();
	}

	UITransform(Vector2 position, Vector2 size) {
		anchors = UIAnchors();
		this->position = position;
		this->size = size;
	}
	UITransform(UIPosition UIPos, Vector2 transformPos, Vector2 transformSize) {
		Vector2 _tmp;

		switch (UIPos) {
			case TOP_LEFT: _tmp = Vector2::up(); break;
			case TOP_CENTER: _tmp = Vector2(0.5f, 1); break;
			case TOP_RIGHT: _tmp = Vector2::one(); break;
			case MIDDLE_LEFT: _tmp = Vector2(0, 0.5f); break;
			case MIDDLE_CENTER: _tmp = Vector2(0.5f, 0.5f); break;
			case MIDDLE_RIGHT: _tmp = Vector2(1, 0.5f); break;
			case BOTTOM_LEFT: _tmp = Vector2::zero(); break;
			case BOTTOM_CENTER: _tmp = Vector2(0.5f, 0); break;
			case BOTTOM_RIGHT: _tmp = Vector2::right(); break;
		}
		
		anchors = UIAnchors(_tmp);
		position = transformPos;
		size = transformSize;
	}
	UITransform(Vector2 UIPos, Vector2 transformPos, Vector2 transformSize) {
		anchors = UIAnchors(Vector2::ClampRangeMagnitude(UIPos, 0.f, 1.f));
		position = transformPos;
		size = transformSize;
	}
	UITransform(Vector2 anchors, float top, float bottom, float left, float right) {
		this->anchors = anchors;
		position = size = Vector2();

		this->top = top;
		this->bottom = bottom;
		this->left = left;
		this->right = right;
	}
};

class Model : public ScriptBehaviour
{

public:
	void OnStartup();

	void OnLoad()
	{
		SceneSaveAndLoad::StartLoadArray("Model");
		transform.LoadTransform();
		SceneSaveAndLoad::EndLoadArray();
	}

	void OnSave();

	void OnInspector();

	enum class Primitives
	{
		SQUARE
	};

	enum UIPosition
	{
		TopLeft, Top, TopRight,
		Left, Center, Right,
		BotLeft, Bottom, BotRight
	};

	UIPosition uiPosition = Center;

	int SceneBelongsTo = 0;
	int DWBelongsTo = 0;

	int sortingLayer = 0;
	int sortingOrder = 0;

	Texture* texture = nullptr;
	bool initialized = false;

	UITransform uitransform;
	/*Vector2 originPoint;
	Vector2 strech;*/

	Color color = Color::white();

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	std::vector<float> modelVertexData;

	int modelId = 0;
	bool isDeleted = false;
	std::vector<int> indexVertices;
	std::vector<int> newIndexVertices;
	std::vector<Vertex*> bufferVertices;

	bool flipXTexture = false, flipYTexture = false;

	Matrix translationMatrix, rotationMatrix, finalMatrix;

	Model()
	{
		
	}

	Model(const std::vector<Vertex>& verts, const std::vector<int>& index) : vertices(verts), indices(index)
	{
		
	}

	Model(std::vector<float> vertices, Vector3 position, Vector3 rotation, Vector3 scale);

	Model(Primitives p)
	{
		*this = Model(p, Vector4(0), Vector4(0), Vector4(1));
	}

	Model(Primitives p, Vector3 position, Vector3 rotation, Vector3 scale)
	{
		if (p == Primitives::SQUARE)
			*this = Model::Square(*this, position, rotation, scale);
	}

	static std::vector<float> Square()
	{
		std::vector<float> s;
		s.resize(16);

		s[0] = -0.5f; s[1] = 0.5f; s[2] = 0; s[3] = 1;
		s[4] = -0.5f; s[5] = -0.5f; s[6] = 0; s[7] = 1;
		s[8] = 0.5f; s[9] = -0.5f; s[10] = 0; s[11] = 1;
		s[12] = 0.5f; s[13] = 0.5f; s[14] = 0; s[15] = 1;

		return s;
	}

	void OnDestroy()
	{
		OnDelete();
	}

	void OnDelete();
	void DeleteModel();

	Matrix SetTransformAndGetWorldMatrix(Vector3 position, Vector3 rotation, Vector3 scale);
	void SetVertices(Matrix matrix, std::vector<float> vertsToSet);

	static Model Square(Model& square, Vector3 position, Vector3 rotation, Vector3 scale);

	void OnChange();
	void OnChange(bool modifyTransform, bool reCheck);

	void ChangeModelIndex(int index, int result);

	void SetTexture(Texture& tex);
	void FlipTexture(bool x, bool y);

	static void SetTextureIntoModel(Model* model, std::string* path);

	void PrintData()
	{
		std::cout << "(" << vertices[0].position.x << " " << vertices[0].position.y << " " << vertices[0].position.z << ")" << std::endl;
		std::cout << "(" << vertices[1].position.x << " " << vertices[1].position.y << " " << vertices[1].position.z << ")" << std::endl;
		std::cout << "(" << vertices[2].position.x << " " << vertices[2].position.y << " " << vertices[2].position.z << ")" << std::endl;
		std::cout << "(" << vertices[3].position.x << " " << vertices[3].position.y << " " << vertices[3].position.z << ")" << std::endl;
	}
};
