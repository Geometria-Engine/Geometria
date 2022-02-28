#pragma once
#ifndef IMGUIELEMENT_H
#define IMGUIELEMENT_H
#include "../../Behaviours/Behaviour.h"

#include "../../ImGUI/imgui.h"
#include "../../ImGUI/imgui_impl_glfw.h"
#include "../../ImGUI/imgui_impl_opengl3.h"
#include "../../ImGUI/imgui_internal.h"
#include "../../ImGUI/ImGuizmo.h"

#include "Graphics/Cores/Texture/Texture.h"

#include "Animation/Animation.h"

#include <functional>

struct ImGUIEFont
{
	std::string url, name;
	int size;
	ImFont* font = nullptr;

	ImGUIEFont(const char* name, const char* url, int size)
	{
		ImGuiIO& io = ImGui::GetIO();
		font = io.Fonts->AddFontFromFileTTF(url, size);
		this->name = name;
		this->url = url;
		this->size = size;
	}
};

struct ImGUIEBoxShadow
{
	Color col;
	Vector2 offset;
	float blur = 0;
};

class ImGUIElement : public ScriptBehaviour
{
public:
	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
			std::string* str = user_data->Str;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		else if (user_data->ChainCallback)
		{
			// Forward to user callback, if any
			data->UserData = user_data->ChainCallbackUserData;
			return user_data->ChainCallback(data);
		}
		return 0;
	}

	bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;

		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}

	void OnLoad()
	{
		SceneSaveAndLoad::StartLoadArray("ImGUIElement");
		transform.LoadTransform();

		LoadElements();

		SceneSaveAndLoad::EndLoadArray();
	}

	void LoadElements();

	void OnStartup()
	{
		ClassType = Class::Pointer;
	}

	void OnSave()
	{
		SceneSaveAndLoad::StartSaveArray("ImGUIElement");
		transform.SaveTransform();

		SaveExternalScripts(allElements);

		SceneSaveAndLoad::EndSaveArray();
	}

	ImGUIElement* elementTrigger = nullptr;
	ImGUIElement* owner = nullptr;

	std::vector<ImGUIElement*> allElements {};

	static std::vector<ImGUIEFont*> allFonts;

	static ImGUIEFont* AddFont(const char* name, const char* url, int size);
	static ImGUIEFont* GetFont(const char* nameOrUrl, int size);

	ImGUIEFont* font = nullptr;

	enum GUIType
	{
		None,
		Window,
		Button,
		ListButton,
		DragVector3,
		Text,
		InputField,
		ColorEditorButtonRGBA,
		ColorEditorButtonRGB,
		Columns,
		EndColumns,
		NextColumn,
		ImGUIDemoWindow,
		SameLine,
		SliderFloat,
		Style,
		MainMenuBar,
		Menu,
		MenuItem,
		HelpMarker,
		FileSearch,
		Guizmo,
		DragInt,
		AppWindow,
		Image
	};

	GUIType guiType = GUIType::None;

	int ownerSceneId = 0;
	int ownerDrawId = 0;

	int keyInput = -1;
	int mouseInput = -1;


	bool isOpen = true;
	bool isEditor = true;
	bool _lastOpenState = false;
	bool enableOpenAndClose = false;
	bool isColored = false;
	Vector2 moveToPosition = Vector2(-1, -1) , scaleTo;
	
	void Move(Vector2 position);
	void Scale(Vector2 scale);
	void ScaleWithScreenResolution(Vector2 percentageScale);

	void SetCurrentSizeToScreenSize(bool setX, bool setY);
	void SetCurrentPosToAlignPivot();

	bool _requestForceMove = false, _requestForceScale = false;

	std::string text, secondText, textFinal, storedPath;
	bool deleteMyself = false;

	static bool IsMouseOnAnyWindow();
	static bool _isMouseOnAnyWindow;

	Vector2 size, screenSize = Vector2(-1, -1);
	Vector2 lastPosition = Vector2(-1, -1), lastSize = Vector2(-1, -1);

	ImDrawList* imgui_drawList = nullptr;

	struct
	{
		bool CanResizeManually = true;
		bool DynamicResizing = false;
		float* size;

	} ColumnProperties;

	enum AlignTo
	{
		Nothing,
		Right,
		Center,
		Left,
		Top,
		Bottom,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Custom
	};

	AlignTo Alignment = AlignTo::Nothing;
	AlignTo ScreenPivot = AlignTo::Nothing;

	Vector2 alignPivot = Vector2(-1, -1);
	Vector2 localScreenPosition = Vector2(0, 0);
	void SetAlignPivot(Vector2 a);

	void ForceAlignToCustom();

	void OpenWithMouseButton(int input);

	Vector4* v4ref = 0;
	std::string* target = 0;
	Color* colorRef = 0;
	float* fRef = 0, minVal = 0, maxVal = 100;
	int* iRef = 0;
	ScriptBehaviour* scriptRef = 0;
	int val = 0;
	bool clicked = false;
	std::string UITag;

	Color fontColor = Color(-1, -1, -1, -1);
	int fontSize = 20;

	bool IsColorRefUsedForBackground()
	{
		return guiType == Window || guiType == Button || guiType == InputField;
	}

	bool wrapped = false;

	enum ItemPosition
	{
		Relative,
		Absolute
	};

	ItemPosition itemPos = Relative;

	enum AutoResize
	{
		Disabled,
		OnlyWidth,
		OnlyHeight
	};

	AutoResize autoResize = AutoResize::Disabled;

	void SetColor(Color col);
	void SetFontColor(Color col);

	float borderSize = 1.0f;
	float borderRadius = 0;

	Vector2 minScale = Vector2(-1, -1), maxScale = Vector2(-1, -1);

	void SetMinScale(Vector2 min);
	void SetMaxScale(Vector2 max);

	std::string strData;

	Vector2 padding = Vector2(10, 10);

	Texture* backgroundImage = nullptr;

	bool EnableTitle = true;
	bool EnableScrolling = true;
	bool EnableResize = true;
	bool CanBeMoved = true;
	bool SaveInFile = true;

	bool containsWindowsInside = false;

	std::vector<std::pair<Color, float>> backgroundGradients;

	void AddBackgroundGradient(std::vector<std::pair<Color, float>> bg);
	void RemoveBackgroundGradient();

	void RenderGradients(ImDrawList* drawList, std::vector<std::pair<Color, float>>& b, ImVec2 topL, ImVec2 botR);
	void RenderBoxShadow();

	ImGUIEBoxShadow* boxShadow = nullptr;

	ImGUIEBoxShadow* AddBoxShadow(Color shadowCol, Vector2 offset);
	ImGUIEBoxShadow* AddBoxShadow(Color shadowCol, Vector2 offset, float blur);
	ImGUIEBoxShadow* RemoveBoxShadow();

	Animation::Timeline* onHoverAnim = nullptr,
		*onHoverOutAnim = nullptr;

	std::vector<std::pair<Animation::Timeline*, std::string>> broadcastAnimations;

	void AddBroadcastAnimation(Animation::Timeline* t, std::string s)
	{
		broadcastAnimations.push_back(std::make_pair(t, s));
	}

	void RunBroadcastAnimations();

	int bcastOnClickIndex = -1;
	void BroadcastOnClick(std::string message);

	bool isHovering = false;

	std::vector<std::function<void()>> onClickEvents {};

	std::vector<std::function<void()>> onChangeEvents {};

	std::vector<std::function<void()>> onFileChangeEvents {};

	std::vector<std::function<void()>> onOpenEvents {};

	std::vector<std::function<void()>> onUIUpdateBeginEvents {};

	std::vector<std::function<void()>> onUIUpdateEndEvents {};

	void OnOpen(std::function<void()> theEvent)
	{
		onOpenEvents.push_back(theEvent);
	}

	void OnClick(std::function<void()> theEvent)
	{
		onClickEvents.push_back(theEvent);
	}

	void OnChange(std::function<void()> theEvent)
	{
		onChangeEvents.push_back(theEvent);
	}

	void OnUIUpdateBegin(std::function<void()> theEvent)
	{
		onUIUpdateBeginEvents.push_back(theEvent);
	}

	void OnUIUpdateEnd(std::function<void()> theEvent)
	{
		onUIUpdateEndEvents.push_back(theEvent);
	}

	void OnFileChange(std::function<void()> theEvent)
	{
		onFileChangeEvents.push_back(theEvent);
	}

	void OnFileChange(std::string path)
	{
		storedPath = path;
		for (int i = 0; i < onFileChangeEvents.size(); i++)
		{
			onFileChangeEvents[i]();
		}
	}

	void ForceFocus()
	{
		if (guiType == GUIType::Window)
		{
			ImGui::SetWindowFocus(textFinal.c_str());
		}
	}

	void Delete();

	void OnBegin();

	bool isSelected;

	ImGUIElement();
	ImGUIElement(GUIType gT);
	ImGUIElement(GUIType gT, ImGUIElement& owner);
	ImGUIElement(GUIType gT, std::string t);
	ImGUIElement(GUIType gT, int value);
	ImGUIElement(GUIType gT, ImGUIElement& owner, int value);
	ImGUIElement(GUIType gT, ImGUIElement& owner, float* floatR);
	ImGUIElement(GUIType gT, ImGUIElement& owner, float* floatR, float min, float max);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, int* iR);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, std::string t2);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, Vector4* v4r);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, std::string* targ);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, Color* colRef);
	ImGUIElement(GUIType gT, ImGUIElement& owner, ScriptBehaviour* sr);
	void OnUpdate();

	static ImGUIElement* AddColumns(int v, ImGUIElement& owner)
	{
		return AddColumns(v, owner, true, false);
	}

	static ImGUIElement* AddColumns(int v, ImGUIElement& owner, bool resize, bool dynamicResizing, float* getSize)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::Columns, owner, v);
		i->ColumnProperties.CanResizeManually = resize;
		i->ColumnProperties.DynamicResizing = dynamicResizing;
		i->ColumnProperties.size = getSize;
		return i;
	}

	static ImGUIElement* AddColumns(int v, ImGUIElement& owner, bool resize, bool dynamicResizing)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::Columns, owner, v);
		i->ColumnProperties.CanResizeManually = resize;
		i->ColumnProperties.DynamicResizing = dynamicResizing;
		return i;
	}

	static ImGUIElement* StopColumns(ImGUIElement& owner)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::EndColumns, owner);
		return i;
	}

	static ImGUIElement* JumpToNextColumn(ImGUIElement& owner)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::NextColumn, owner);
		return i;
	}

	static ImGUIElement* ContinueInSameLine(ImGUIElement& owner)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::SameLine, owner);
		return i;
	}
};
#endif
