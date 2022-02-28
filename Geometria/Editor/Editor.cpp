#include "Editor.h"
#include "geometria.h"

DrawCall* Editor::editorDrawCall;

std::experimental::filesystem::path Editor::FileBrowserPath = std::experimental::filesystem::current_path(), 
	Editor::EndPath = Editor::FileBrowserPath;

float Editor::fBThumbnailSize = 256;

int Editor::actualHierarchySize = 0;

bool Editor::fBFileMode = false;

std::string Editor::fBFileModeVar, Editor::fBFileModeObj;

ImGUIElement* Editor::Inspector, * Editor::FileBrowser, * Editor::HierarchyWindow, * Editor::CurrentFileModeObject, * Editor::MainMenuBar,
* Editor::InspBar, * Editor::HierBar, * Editor::FileBrBar, * Editor::EditorGuizmo, * Editor::EditorRightClick, * Editor::AddScriptWindow, 

* Editor::GameViewButton, * Editor::EditorViewButton;

ScriptBehaviour* selectedObject;
ImGUIElement* selectedItem;

void FB_GoBack()
{
	Editor::FileBrowser->Delete();
	Editor::FileBrowserPath = Editor::FileBrowserPath.parent_path();
	Editor::FileBrowserBegin();
}

void FB_Refresh()
{
	if(!Editor::fBFileMode)
		Editor::FileBrowser->text = "File Browser";
	else
		Editor::FileBrowser->text = "File Browser (Choosing " + Editor::fBFileModeVar + " from " + Editor::fBFileModeObj + ")";

	Editor::FileBrowser->Delete();
	Editor::FileBrowserBegin();
}

void Inspector_Refresh()
{
	Editor::Inspector->Delete();
	Editor::InspectorBegin();
}

void FB_ChangeWindowToNormal()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.12f, 0.51f, 0.8f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.14f, 0.12f, 0.51f, 0.8f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.32f, 0.61f, 1);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.42f, 0.71f, 1);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.12f, 0.51f, 0.6f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.12f, 0.51f, 1);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.32f, 0.61f, 1);
}

void FB_ChangeWindowToGreen()
{
	if (Editor::fBFileMode)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.52f, 0, 1);

		style.Colors[ImGuiCol_Button] = ImVec4(0.14f, 0.41f, 0, 0.8f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.14f, 0.51f, 0, 1);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.14f, 0.61f, 0, 1);

		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.41f, 0, 0.6f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.51f, 0, 1);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.14f, 0.61f, 0, 1);
	}
}

void ChangeName(std::string* text, int* id)
{
	int copies = 0;
	selectedItem->text = *text;
	/*for (int i = 0; i < Editor::HierarchyWindow->allElements.size(); i++)
	{
		if (Editor::HierarchyWindow->allElements[i]->text == *text)
		{
			copies++;
		}

		if (Editor::HierarchyWindow->allElements[i]->iRef == id)
		{
			if(copies == 0)
				Editor::HierarchyWindow->allElements[i]->text = *text;
			else
				Editor::HierarchyWindow->allElements[i]->text = *text + " " + std::to_string(copies + 1);
		}
	}*/
}

void Hierarchy_Refresh()
{
	Editor::HierarchyWindow->Delete();
	Editor::HierarchyBegin();
}

void Hierarchy_Clicked(ImGUIElement* i, bool t)
{
	i->clicked = t;
	for (int f = 0; f < i->allElements.size(); f++)
	{
		Hierarchy_Clicked(i->allElements[f], t);
	}
}

void Hierarchy_Select(ScriptBehaviour* obj, ImGUIElement* item)
{
	selectedObject = obj;

	if (selectedItem != nullptr)
		selectedItem->clicked = false;

	selectedItem = item;
	selectedItem->clicked = true;
	for (int i = 0; i < Editor::HierarchyWindow->allElements.size(); i++)
	{
		if (Editor::HierarchyWindow->allElements[i]->text != obj->objectClassName)
			Editor::HierarchyWindow->allElements[i]->clicked = false;
	}

	if (Editor::EditorGuizmo != nullptr)
	{
		Editor::EditorGuizmo->scriptRef = selectedObject;
	}
	Inspector_Refresh();
}

void FB_OpenFileOrDirectory(std::string folder)
{
	std::ostringstream newRoute;
	newRoute << Editor::FileBrowserPath.u8string() << "/" + folder;

	std::cout << newRoute.str() << std::endl;

	std::experimental::filesystem::path NewPath = std::experimental::filesystem::path(newRoute.str());

	if (std::experimental::filesystem::is_regular_file(NewPath))
	{
		if (Editor::fBFileMode)
		{
			std::cout << "Executing File Change, Size of events: " << Editor::CurrentFileModeObject->onFileChangeEvents.size() << std::endl;
			Editor::CurrentFileModeObject->OnFileChange(NewPath.u8string());
			Editor::fBFileMode = false;
			Editor::FileBrowser->Delete();
			Editor::FileBrowserBegin();
		}
	}
	else if (std::experimental::filesystem::is_directory(NewPath))
	{
		Editor::FileBrowser->Delete();
		Editor::FileBrowserPath = NewPath;
		Editor::FileBrowserBegin();
	}
}

void RC_Open()
{
	Editor::EditorRightClick->Move(Vector2(Graphics::MousePosition().x, Graphics::MousePosition().y));
}

//TODO: MAKE SURE THIS IS TRULLY A TEMPORARY SOLUTION!!!
void RC_AddModelFromButton()
{
	Hierarchy::EditorMode(false);
	Model* newModel = new Model(Model::Square(), Vector3::zero(), Vector3::zero(), Vector3::one());
	newModel->objectClassName = "Model" + std::to_string(rand() % 100000);
	RendererCore::AddModel(*newModel);

	DrawCall* owner = RendererCore::FindDrawCall(newModel->SceneBelongsTo, newModel->DWBelongsTo);

	for (int i = 0; i < Editor::HierarchyWindow->allElements.size(); i++)
	{
		if (Editor::HierarchyWindow->allElements[i]->text == owner->objectClassName &&
			Editor::HierarchyWindow->allElements[i]->UITag == std::to_string(owner->scriptId))
		{
			ImGUIElement* newModelButton = new ImGUIElement(ImGUIElement::GUIType::ListButton, *Editor::HierarchyWindow->allElements[i], newModel->objectClassName);
			newModelButton->iRef = &newModel->scriptId;
			newModelButton->OnClick(std::bind(Hierarchy_Select, newModel, newModelButton));
			Editor::EditorRightClick->isOpen = false;
			break;
		}
	}
}

void DeleteObjectFromInspector()
{
	if (Input::GetKeyDown(GLFW_KEY_DELETE))
	{
		Editor::Inspector->Delete();

		for (int i = 0; i < Editor::HierarchyWindow->allElements.size(); i++)
		{
			if (Editor::HierarchyWindow->allElements[i]->text == selectedObject->objectClassName)
			{
				Editor::HierarchyWindow->allElements[i]->Delete();
				Editor::HierarchyWindow->allElements.erase(Editor::HierarchyWindow->allElements.begin() + i);
				break;
			}
		}

		selectedObject->OnDelete();
		selectedObject = nullptr;
	}
}

void Editor::Begin()
{
	Editor::actualHierarchySize = Hierarchy::allScripts.size();

	editorDrawCall = SceneManager::MainScene().CreateDrawCall();
	editorDrawCall->sort = DrawCall::Sorting::AtStartup;
	editorDrawCall->type = DrawCall::Type::UI;
	editorDrawCall->objectClassName = "Editor Draw Call";
	editorDrawCall->Close();

	//editorDrawCall = &editorDC;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Editor/EditorResources/Fonts/Raleway-Regular.ttf", 20);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.12f, 0.51f, 0.8f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.14f, 0.12f, 0.51f, 0.8f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.32f, 0.61f, 1);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.42f, 0.71f, 1);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.12f, 0.51f, 0.6f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.12f, 0.51f, 1);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.32f, 0.61f, 1);


	/*ImGUIElement* style = new ImGUIElement(ImGUIElement::GUIType::Style);

	style->StyleOptions.font = "Editor/EditorResources/Fonts/Raleway-Regular.ttf";
	style->StyleOptions.fontSize = 20;

	RendererCore::AddImGUIElement(*style, editorDrawCall->Target());*/

	Hierarchy::EditorMode(true);
	Editor::EditorRightClick = new ImGUIElement(ImGUIElement::GUIType::Window, "Right Click Options");
	Editor::EditorRightClick->isOpen = false;
	Editor::EditorRightClick->enableOpenAndClose = true;
	Editor::EditorRightClick->OpenWithMouseButton(1);
	Editor::EditorRightClick->OnOpen([&] { RC_Open(); });
	{
		ImGUIElement* text = new ImGUIElement(ImGUIElement::GUIType::Text, *Editor::EditorRightClick, "Create");
		new ImGUIElement(ImGUIElement::GUIType::Text, *Editor::EditorRightClick);
		ImGUIElement* button = new ImGUIElement(ImGUIElement::GUIType::ListButton, *Editor::EditorRightClick, "Model");
		button->OnClick([&] {RC_AddModelFromButton(); });
	}

	RendererCore::AddImGUIElement(*Editor::EditorRightClick, editorDrawCall->Target());
	Hierarchy::EditorMode(false);

	MainMenuBarBegin();

	HierarchyBegin();

	InspectorBegin();

	FileBrowserBegin();
}

void Editor::SwitchEditorView()
{
	if (editorDrawCall != nullptr)
	{
		Editor::EditorViewButton->colorRef = new Color(1, 0.9, 0);
		Editor::GameViewButton->colorRef = new Color(Color::white());
	}

	Graphics::MainCamera()->editorModeCamera = true;
}

void Editor::SwitchGameView()
{
	if (editorDrawCall != nullptr)
	{
		Editor::EditorViewButton->colorRef = new Color(Color::white());
		Editor::GameViewButton->colorRef = new Color(1, 0.9, 0);
	}

	Graphics::MainCamera()->editorModeCamera = false;
}

void Editor::MainMenuBarBegin()
{
	Hierarchy::EditorMode(true);
	Editor::MainMenuBar = new ImGUIElement(ImGUIElement::GUIType::MainMenuBar);
	
	ImGUIElement* basics = new ImGUIElement(ImGUIElement::GUIType::Menu, *Editor::MainMenuBar, "Get Started Here!", new Color(1, 0.9, 0));
	basics->isColored = true;
	{
		new ImGUIElement(ImGUIElement::GUIType::Text, *basics, "Each one of these 3 buttons open different tools.");
		new ImGUIElement(ImGUIElement::GUIType::Text, *basics, "These tools are the ones that you're going to need the most");
		new ImGUIElement(ImGUIElement::GUIType::Text, *basics, "while making your game or application in this editor.");
		new ImGUIElement(ImGUIElement::GUIType::Text, *basics, "You can select from one to all of them and close them");
		new ImGUIElement(ImGUIElement::GUIType::Text, *basics, "at anytime.");
		new ImGUIElement(ImGUIElement::GUIType::Text, *basics, "If you want information about what these windows do,");
		new ImGUIElement(ImGUIElement::GUIType::Text, *basics, "you can hover your mouse to their \"(?)\" icons.");
		new ImGUIElement(ImGUIElement::GUIType::Text, *basics, "");

		Editor::HierBar = new ImGUIElement(ImGUIElement::GUIType::MenuItem, *basics, "Hierarchy", new Color(1, 0.9, 0));
		Editor::HierBar->isColored = true;
		ImGUIElement::ContinueInSameLine(*basics);
		new ImGUIElement(ImGUIElement::GUIType::HelpMarker, *basics, "The Hierarchy is the list of all objects that are present in the Scene.");

		Editor::InspBar = new ImGUIElement(ImGUIElement::GUIType::MenuItem, *basics, "Inspector", new Color(1, 0.9, 0));
		Editor::InspBar->isColored = true;
		ImGUIElement::ContinueInSameLine(*basics);
		new ImGUIElement(ImGUIElement::GUIType::HelpMarker, *basics, "The Inspector is the window that shows all of the properties of one or more objects, and allows you to edit them. In order to see and edit the properties of an object you have to select it from the Hierarchy first.");

		Editor::FileBrBar = new ImGUIElement(ImGUIElement::GUIType::MenuItem, *basics, "File Browser", new Color(1, 0.9, 0));
		Editor::FileBrBar->isColored = true;
		ImGUIElement::ContinueInSameLine(*basics);
		new ImGUIElement(ImGUIElement::GUIType::HelpMarker, *basics, "The File Browser is used for search files and see what assets and resources are in your project.");
	}

	EditorViewButton = new ImGUIElement(ImGUIElement::GUIType::Button, *Editor::MainMenuBar, "Editor", new Color(Color::white()));
	EditorViewButton->isColored = true;
	EditorViewButton->OnClick([&] {SwitchEditorView(); });

	GameViewButton = new ImGUIElement(ImGUIElement::GUIType::Button, *Editor::MainMenuBar, "Game", new Color(Color::white()));
	GameViewButton->isColored = true;
	GameViewButton->OnClick([&] {SwitchGameView(); });

	SwitchEditorView();

	Editor::EditorGuizmo = new ImGUIElement(ImGUIElement::GUIType::Guizmo, *Editor::MainMenuBar);
	RendererCore::AddImGUIElement(*Editor::MainMenuBar, editorDrawCall->Target());
	Hierarchy::EditorMode(false);
}

void Hierarchy_CreateItem(ScriptBehaviour& script, ImGUIElement* owner)
{
	script;

	if (owner == nullptr)
	{
		ImGUIElement* button = new ImGUIElement(ImGUIElement::GUIType::ListButton, *Editor::HierarchyWindow, script.objectClassName);
		button->UITag = std::to_string(script.scriptId);
		button->iRef = &script.scriptId;
		button->OnClick(std::bind(Hierarchy_Select, &script, button));
		owner = button;
	}
	else
	{
		ImGUIElement* button = new ImGUIElement(ImGUIElement::GUIType::ListButton, *owner, script.objectClassName);
		button->UITag = std::to_string(script.scriptId);
		button->iRef = &script.scriptId;
		button->OnClick(std::bind(Hierarchy_Select, &script, button));
	}

	for (int i = 0; i < script.scripts.size(); i++)
	{
		if(script.scripts[i]->ClassType != ScriptBehaviour::Class::Script)
			Hierarchy_CreateItem(*script.scripts[i], owner);
	}

}

void Editor::HierarchyBegin()
{
	Hierarchy::EditorMode(true);

	bool lastHierarchyState = false;
	if (Editor::HierarchyWindow != nullptr)
	{
		lastHierarchyState = Editor::HierarchyWindow->isOpen;
	}

	Editor::HierarchyWindow = new ImGUIElement(ImGUIElement::GUIType::Window, *HierBar, "Hierarchy");
	Editor::HierarchyWindow->enableOpenAndClose = true;
	Editor::HierarchyWindow->isOpen = lastHierarchyState;

	Hierarchy_CreateItem(*SceneManager::MainScene().MainDrawCall(), nullptr);

	for (int i = 0; i < Editor::actualHierarchySize; i++)
	{
		if (Hierarchy::allScripts[i]->ClassType == ScriptBehaviour::Class::Object)
		{
			Hierarchy_CreateItem(*Hierarchy::allScripts[i], nullptr);
		}
	}
	RendererCore::AddImGUIElement(*Editor::HierarchyWindow, editorDrawCall->Target());

	//ImGUIElement* demo = new ImGUIElement(ImGUIElement::GUIType::ImGUIDemoWindow);
	//RendererCore::AddImGUIElement(*demo, editorDrawCall->Target());
	Hierarchy::EditorMode(false);

}

void Inspector_AttachNewScriptToObject(ScriptBehaviour* thing)
{
	int size = selectedObject->scripts.size();
	selectedObject->scripts.push_back(thing->Clone());
	selectedObject->scripts[size]->owner = selectedObject;
	Inspector_Refresh();
	std::cout << "Added " << typeid(*selectedObject->scripts[size]).name() << "! Current Size: " << selectedObject->scripts.size() << std::endl;
	Editor::AddScriptWindow->Delete();
	Editor::AddScriptWindow->isOpen = false;
}

void Inspector_AddScript()
{
	Hierarchy::EditorMode(true);
	bool add = Editor::AddScriptWindow == nullptr;
	Editor::AddScriptWindow = new ImGUIElement(ImGUIElement::GUIType::Window, "Add New Script");
	Editor::AddScriptWindow->isOpen = true;
	Editor::AddScriptWindow->Move(Vector2(Graphics::MousePosition().x, Graphics::MousePosition().y));
	
	for (int i = 0; i < Hierarchy::scriptsWithVisualAccess.size(); i++)
	{
		ImGUIElement* button = new ImGUIElement(ImGUIElement::GUIType::ListButton, *Editor::AddScriptWindow, Hierarchy::scriptsWithVisualAccess[i].first);
		button->OnClick(std::bind(Inspector_AttachNewScriptToObject, Hierarchy::scriptsWithVisualAccess[i].second));
	}

	RendererCore::AddImGUIElement(*Editor::AddScriptWindow, Editor::editorDrawCall->Target());
	Hierarchy::EditorMode(false);
}

void Editor::InspectorBegin()
{
	Hierarchy::EditorMode(true);
	bool lastInspectorState = false;
	if (Editor::Inspector != nullptr)
	{
		lastInspectorState = Editor::Inspector->isOpen;
	}
	Editor::Inspector = new ImGUIElement(ImGUIElement::GUIType::Window, *InspBar, "Inspector");
	Editor::Inspector->enableOpenAndClose = true;
	Editor::Inspector->isOpen = lastInspectorState;

	Editor::Inspector->OnUIUpdateBegin([&] {DeleteObjectFromInspector(); });

	if (selectedObject != nullptr)
	{
		ImGUIElement* name = new ImGUIElement(ImGUIElement::GUIType::InputField, *Inspector, "Name", &selectedObject->objectClassName);
		name->OnChange(std::bind(ChangeName, &selectedObject->objectClassName, &selectedObject->scriptId));

		ImGUIElement* position = new ImGUIElement(ImGUIElement::GUIType::DragVector3, *Inspector, "Position", &selectedObject->transform.position);
		ImGUIElement* rotation = new ImGUIElement(ImGUIElement::GUIType::DragVector3, *Inspector, "Rotation", &selectedObject->transform.rotation);
		ImGUIElement* scale = new ImGUIElement(ImGUIElement::GUIType::DragVector3, *Inspector, "Scale", &selectedObject->transform.scale);

		selectedObject->OnInspector();

		for (int i = 0; i < selectedObject->scripts.size(); i++)
		{
			selectedObject->scripts[i]->OnInspector();
		}

		ImGUIElement* newLine = new ImGUIElement(ImGUIElement::GUIType::Text, *Inspector, "");
		ImGUIElement* addScriptButton = new ImGUIElement(ImGUIElement::GUIType::Button, *Inspector, "Add Script");
		addScriptButton->Alignment = ImGUIElement::AlignTo::Center;
		addScriptButton->OnClick([&] { Inspector_AddScript(); });
	}

	RendererCore::AddImGUIElement(*Inspector, editorDrawCall->Target());
	Hierarchy::EditorMode(false);
}

void Editor::FileBrowserBegin()
{
	Hierarchy::EditorMode(true);
	bool lastFBState = false;
	if (FileBrowser != nullptr)
	{
		lastFBState = FileBrowser->isOpen;
	}

	FileBrowser = new ImGUIElement(ImGUIElement::GUIType::Window, *FileBrBar, "File Browser");
	FileBrowser->enableOpenAndClose = true;
	FileBrowser->isOpen = lastFBState;

	FileBrowser->UITag = "FileBrowser";
	FileBrowser->OnUIUpdateBegin([&] { FB_ChangeWindowToGreen(); });
	FileBrowser->OnUIUpdateEnd([&] { FB_ChangeWindowToNormal(); });

	ImGUIElement* FileModeText = new ImGUIElement(ImGUIElement::GUIType::Text, *FileBrowser, "Currently In File Mode.");
	FileModeText->Alignment = ImGUIElement::AlignTo::Center;
	FileModeText->isEnabled = Editor::fBFileMode;

	if (FileBrowserPath != EndPath)
	{
		ImGUIElement* backButton = new ImGUIElement(ImGUIElement::GUIType::Button, *FileBrowser, "<-");
		backButton->OnClick([&] { FB_GoBack(); });
		ImGUIElement::ContinueInSameLine(*FileBrowser);
	}

	ImGUIElement* thumbnailSlider = new ImGUIElement(ImGUIElement::GUIType::SliderFloat, *FileBrowser, &fBThumbnailSize, 16.f, 512.f);
	thumbnailSlider->OnChange([&] { FB_Refresh(); });

	ImGUIElement* add = ImGUIElement::AddColumns(5, *FileBrowser, false, true, &fBThumbnailSize);

	for (auto& files : std::experimental::filesystem::directory_iterator(FileBrowserPath))
	{
		ImGUIElement* button = new ImGUIElement(ImGUIElement::GUIType::Button, *FileBrowser, files.path().filename().u8string());
		button->size = Vector2(fBThumbnailSize, fBThumbnailSize);

		ImGUIElement* file = new ImGUIElement(ImGUIElement::GUIType::Text, *FileBrowser, files.path().filename().u8string());
		ImGUIElement* jump = ImGUIElement::JumpToNextColumn(*FileBrowser);

		button->OnClick(std::bind(FB_OpenFileOrDirectory, files.path().filename().u8string()));
	}

	ImGUIElement* stop = ImGUIElement::StopColumns(*FileBrowser);

	RendererCore::AddImGUIElement(*FileBrowser, editorDrawCall->Target());
	Hierarchy::EditorMode(false);
}

void Editor::FileBrowser_ChooseFileMode(std::string var, std::string obj, ImGUIElement* fileObject)
{
	Editor::fBFileModeVar = var;
	Editor::fBFileModeObj = obj;
	Editor::fBFileMode = true;
	Editor::FileBrowser->ForceFocus();
	Editor::CurrentFileModeObject = fileObject;
	FB_Refresh();
}
