#include <experimental/filesystem>

class DrawCall;
class ImGUIElement;
class Editor
{
public:
	static DrawCall* editorDrawCall;

	static ImGUIElement* Inspector, *FileBrowser, *HierarchyWindow, *CurrentFileModeObject, *MainMenuBar,
		*InspBar, *HierBar, *FileBrBar, *EditorGuizmo, *EditorRightClick, *AddScriptWindow;

	static ImGUIElement* EditorViewButton, * GameViewButton;

	static float fBThumbnailSize;
	static std::experimental::filesystem::path EndPath, FileBrowserPath;
	static int actualHierarchySize;
	static bool fBFileMode;
	static std::string fBFileModeObj, fBFileModeVar;

	static void Begin();
	static void SwitchEditorView();
	static void SwitchGameView();
	static void MainMenuBarBegin();
	static void HierarchyBegin();
	static void InspectorBegin();
	static void FileBrowserBegin();

	static void FileBrowser_ChooseFileMode(std::string var, std::string obj, ImGUIElement* fileObject);
};
