#include "geometria.h"
#include "Editor/Editor.h"
#include "Game/SceneTest.h"
#include "Game/SampleScene.tits.h"
#include "Application/Application.h"
#include "Game/Scripts/TestScript.h"
#include "Physics/PhysicsManager.h"
#include "Physics/Rigidbody/Rigidbody.h"
#include "Physics/Colliders/BoxCollider.h"

//Original name: Chingatumadre Engine

static std::vector<Model*> models;
bool firstFrame = true, secondFrame = true;
int main(int argc, char** argv)
{
    Graphics::Init();
    Graphics::CreateWindow(640, 480, "Yooooooooooooooo");

    Graphics::Start();

    for (int i = 0; i < argc; i++)
    {
        std::string commandLine = argv[i];
        if (commandLine == "--bypass-intel")
        {
            Graphics::BypassIntel(true);
        }
    }

    if (Graphics::IsIntelGPUBypassed())
    {
        std::cout << "Intel GPU Bypassed!" << std::endl;
    }

    Application::Start();

   /* Graphics::MainCamera() = new Camera(Vector3(0, 0, 2), 70.0f, (float)Graphics::GetMainWindow().width / (float)Graphics::GetMainWindow().height, 0.1f, 1000.0f);
    Graphics::MainCamera()->objectClassName = "Main Camera";

    RendererCore::SetUp();*/

    //STARTUP COMPONENTS INSTANTIATE HERE!!!
    //======================================================================================

    //You can instantiate more draw calls with this
    /*for (int i = 0; i < 9; i++)
    {
        DrawCall& d = SceneManager::MainScene().CreateDrawCall();
        d.sort = SceneManager::MainScene().MainDrawCall().sort;
        d.type = SceneManager::MainScene().MainDrawCall().type;
    }*/

    //Put Textures
    /*Texture t("Resources/button.png", Texture::Type::Default);
    Texture t2("Resources/pixeltest2.png", Texture::Type::Default);
    Texture t3("Resources/pixeltest1.png", Texture::Type::Default);
    Texture t4("Resources/pixeltest4.png", Texture::Type::Default);
    Texture t5("Resources/pixeltest3.png", Texture::Type::Default);
    Texture t6("Resources/p5.png", Texture::Type::Default);
    Texture t7("Resources/p6.png", Texture::Type::Default);
    Texture t9("Resources/Icons.png", Texture::Type::Default);
    Texture t10("Resources/ayo.png", Texture::Type::Default);
    Texture t11("Resources/TOSHIanimations.0006.png", Texture::Type::Default);
    Texture t12("Resources/TOSHIanimations.0007.png", Texture::Type::Default);
    Texture t13("Resources/TOSHIanimations.0011.png", Texture::Type::Default);
    Texture t14("Resources/TOSHIanimations.0012.png", Texture::Type::Default);
    Texture t15("Resources/TOSHIanimations.0014.png", Texture::Type::Default);
    Texture t16("Resources/TOSHIanimations.0018.png", Texture::Type::Default);*/

    //Pack them and upload them to the RAM and GPU. Don't use this in Update yet!!
    //TextureManager::UploadToGPU();

    // MODEL SCENE EXAMPLES
    //===========================================================================

    //Spawn 100k quads in 1 draw call
    /*for (int i = 0; i < 10000; i++)
    {
        Model* model = new Model(Model::Square(), Vector3(i, 0, -i), Vector3(0), Vector3(1));
        models.push_back(model);
        RendererCore::AddModel(*model);
    }*/

    //Spawn 10k quads in "i" draw calls, with random colors
    /*for (int m = 0; m < 1000; m++)
    {
        int x = rand() % 200, y = rand() % 100, z = rand() % 100;
        float r = (rand() % 100) / 100.0f, g = (rand() % 100) / 100.0f, b = (rand() % 100) / 100.0f;
        Model* model = new Model(Model::Square(), Vector3(x - 50, y - 50, z - 50), Vector3(0), Vector3(1));
        model->color = Vector4(r, g, b, 0.2f);
        model->AddScript<Rigidbody>();
        model->AddScript<BoxCollider>();
        RendererCore::AddModel(*model);
    }

    Model* model = new Model(Model::Square(), Vector3(0, -100, 0), Vector3(0, 0, 0), Vector3(1000, 0.5f, 1000));
    model->GetTransform().rotation = Vector3(90, 0, 0);
    model->color = Color::white();
    model->AddScript<BoxCollider>();
    RendererCore::AddModel(*model);*/

    //SceneTest::Init();
    //SampleScene::Init();

    //Spawn 2 quads with textures
    /*Model* model = new Model(Model::Square(), Vector3(0, 0, 0), Vector3(0), Vector3(1, 1, 1));*/

    ////model->uitransform = UITransform(UITransform::UIPosition::BOTTOM_CENTER, Vector2(800, 600));
    //model->uitransform.position = Vector2(-100, 0);
    //model->uitransform.size = Vector2(300, 100);
    //model->uitransform.anchors.min = Vector2(1, 0.5);
    //model->uitransform.anchors.max = Vector2(1, 0.5);
    ///*model->uitransform.top = 0;
    //model->uitransform.left = 0;
    //model->uitransform.bottom = 0;
    //model->uitransform.right = 0;*/

    ////model->uitransform.position = Vector2(0);
    ////model->uitransform.size = Vector2(800, 600);
    /*model->texture = &t;
    models.push_back(model);
    RendererCore::AddModel(*model);

    Model* model2 = new Model(Model::Square(), Vector3(1, 0, 0), Vector3(0), Vector3(1));
    model2->texture = &t10;
    models.push_back(model2);
    RendererCore::AddModel(*model2);

    ImGUIElement* window = new ImGUIElement(ImGUIElement::GUIType::Window, "Editor");
    ImGUIElement* text = new ImGUIElement(ImGUIElement::GUIType::Text, *window, "Hey the Editor UI is working now");
    RendererCore::AddImGUIElement(*window, SceneManager::MainScene()._drawCalls[1]);*/

    //d.AddMyselfToHierarchy();

    //START AND UPDATE
    //===========================================================

    //RendererCore::Start();

    //Application::SetEditor();

    //SceneSaveAndLoad::StartSceneSave(&SceneManager::MainScene());

    //SceneTest::Init();

    //Editor::Begin();

    int speed = 4;

    //SceneSaveAndLoad::StartSceneSave(&SceneManager::MainScene());

    bool stopCamera = false;
    bool firstPerson = false;
    int sensitivity = 3;

    //This loop is the "Update" loop, which means the lines inside here will be called each frame.

    while (!Graphics::CanClose())
    {
        Input::UpdateKeyState();

        if (!ImGUIElement::IsMouseOnAnyWindow())
        {
            //Camera controls example
            if (Input::GetKey(GLFW_KEY_C) && speed == 4)
                speed = 30;
            else if (!Input::GetKey(GLFW_KEY_C) && speed == 30)
                speed = 4;

            if (!stopCamera)
            {
                if (Input::GetKey(GLFW_KEY_S))
                    Graphics::MainCamera()->editorPosition -= Graphics::MainCamera()->forward * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_W))
                    Graphics::MainCamera()->editorPosition += Graphics::MainCamera()->forward * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_A))
                    Graphics::MainCamera()->editorPosition -= Graphics::MainCamera()->right * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_D))
                    Graphics::MainCamera()->editorPosition += Graphics::MainCamera()->right * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_E))
                    Graphics::MainCamera()->editorPosition -= Graphics::MainCamera()->up * speed * Graphics::DeltaTime();
                if (Input::GetKey(GLFW_KEY_Q))
                    Graphics::MainCamera()->editorPosition += Graphics::MainCamera()->up * speed * Graphics::DeltaTime();
            }
        }

        if (firstPerson)
        {
            Graphics::MainCamera()->Mouse();
        }

        //Models movement example
        if (!firstFrame)
        {
            if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
            {
                if (Input::GetKey(GLFW_KEY_LEFT_ALT))
                {
                    if (Input::GetKeyDown(GLFW_KEY_TAB))
                        stopCamera = !stopCamera;

                    if (Input::GetKeyDown(GLFW_KEY_X))
                    {
                        firstPerson = !firstPerson;

                        if (firstPerson)
                            glfwSetInputMode(Graphics::GetMainWindow().openGLWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        else
                            glfwSetInputMode(Graphics::GetMainWindow().openGLWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }

                    if (Input::GetKeyDown(GLFW_KEY_P))
                    {
                        switch (Application::_engineState)
                        {
                        case Application::State::Game:
                            Application::SetEditor();
                            std::cout << "State: Editing..." << std::endl;
                            break;

                        case Application::State::Editor:
                            Application::SetGame();
                            std::cout << "State: Gaming..." << std::endl;
                            break;
                        }
                    }

                    if (Input::GetKeyDown(GLFW_KEY_Y))
                    {
                        if (!Graphics::MainCamera()->editorModeCamera)
                        {
                            Editor::SwitchEditorView();
                        }
                        else
                        {
                            Editor::SwitchGameView();
                        }
                    }

                    if (Input::GetKeyDown(GLFW_KEY_C))
                        CompilationCore::Compile();

                    if (Input::GetKeyDown(GLFW_KEY_S))
                    {
                        DrawCall* main = SceneManager::MainScene().MainDrawCall();
                        main->isMain = true;
                        main->sort = SceneManager::MainScene().MainDrawCall()->sort;
                        main->type = SceneManager::MainScene().MainDrawCall()->type;
                        Hierarchy::allScripts[SceneManager::MainScene().MainDrawCall()->id] = main;
                        std::cout << "Saved!" << std::endl;
                        SceneSaveAndLoad::StartSceneSave(&SceneManager::MainScene());
                    }

                    if (Input::GetKeyDown(GLFW_KEY_R))
                    {
                        std::cout << "Restarting Scene" << std::endl;
                        Application::Start();
                        std::cout << "Scene Restarted!" << std::endl;
                    }
                }
            }

            //models[0]->OnChange();
            if (Input::GetKey(GLFW_KEY_R))
            {
                //models[0]->rotation = Vector3(0);
                //models[0]->color -= Vector4(0, 0, 2.f * Graphics::DeltaTime(), 1.f * Graphics::DeltaTime());
                //models[0]->rotation += Vector3(0, 45.0f * Graphics::DeltaTime(), 45.0f * Graphics::DeltaTime());

                //Hierarchy::allScripts[1]->transform.position -= Vector3(0, 0, 10 * Graphics::DeltaTime());
                ////Hierarchy::allScripts[1]->color -= Vector4(0, 1.f * Graphics::DeltaTime(), 0, 0.09f * Graphics::DeltaTime());
                //Hierarchy::allScripts[1]->transform.rotation += Vector3(45.0f * Graphics::DeltaTime(), 0, 45.0f * Graphics::DeltaTime());

                /*models[2]->position -= Vector3(0, 0, 20 * Graphics::DeltaTime());
                models[2]->color -= Vector4(1.f * Graphics::DeltaTime(), 0, 0, 0.09f * Graphics::DeltaTime());
                models[2]->rotation += Vector3(0, 0, 45.0f * Graphics::DeltaTime());

                models[7]->position -= Vector3(0, 0, 5 * Graphics::DeltaTime());
                models[7]->color -= Vector4(0, 0, 1.f * Graphics::DeltaTime(), 0.09f * Graphics::DeltaTime());
                models[7]->rotation += Vector3(0, 45.0f * Graphics::DeltaTime(), 45.0f * Graphics::DeltaTime());

                models[8]->position -= Vector3(0, 0, 10 * Graphics::DeltaTime());
                models[8]->color -= Vector4(0, 1.f * Graphics::DeltaTime(), 0, 0.09f * Graphics::DeltaTime());
                models[8]->rotation += Vector3(45.0f * Graphics::DeltaTime(), 0, 45.0f * Graphics::DeltaTime());

                models[9]->position -= Vector3(0, 0, 20 * Graphics::DeltaTime());
                models[9]->color -= Vector4(1.f * Graphics::DeltaTime(), 0, 0, 0.09f * Graphics::DeltaTime());
                models[9]->rotation += Vector3(0, 0, 45.0f * Graphics::DeltaTime());
                */
            }
        }

        Hierarchy::UpdateScripts();

        RendererCore::Render();

        Graphics::Update();

        Broadcast::Update();

        firstFrame = false;
    }

    Graphics::Exit();
    exit(0);
    return 0;
}
