#ifndef __SESSION_MANAGER_HEADER__
#define __SESSION_MANAGER_HEADER__

#define GL_VERSION_MINOR    3
#define GL_VERSION_MAJOR    3
#define GL_MSAA_SAMPLES     8

#define FUSION_WORLD_EMPTY "WORLDEMPTY"
#define NOMINMAX

#include <windows.h>

#include "../../Resources/glad/glad.h"
#include "../../Resources/GLFW/glfw3.h"
#include "../../Resources/glm/glm.hpp"

#include "../Common/Console.h"
#include "../Common/Parser.h"
#include "../Common/Shader.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Skybox.h"

#include "../Editor/Window/ComponentsWindow.h"
#include "ModelSystem.h"

struct EditorShaderCollection
{
    Shader          SkyboxShader;
    Shader          SceneShader;
    Shader          EntityShader;
    Shader          DebugShader;
};

struct EditorSceneComponents
{
    Parser          EngineParser;
    Camera          EngineCamera;
    Skybox          EngineSkybox;
};

struct EditorFrameBufferView
{
    unsigned int    FrameBufferIdentifer;
    unsigned int    TextureIdentifer;
    unsigned int    RenderIdentifer;
};

void MouseViewCallback(GLFWwindow* EngineWindow,int Button,int Action,int Mods);

class SessionManager
{
    public:
                                        SessionManager(void);
        void                            CreateSession(void);
        void                            CalculateDeltaTime(void);
        void                            UpdateWorld(const std::string& WorldDirectory);
        void                            CreateEngine(void);
        void                            UpdateEngineValue(void);
        void                            RenderEngine(void);
        void                            DestroyEngine(void);

        void                            SetViewportResolution(ImVec2 Resolution);
        void                            CreateFrameBuffer(void);
        void                            BindFrameBuffer(void);
        void                            UnbindFrameBuffer(void);
        void                            RescaleFrameBuffer(float Width,float Height);

        glm::vec2                       GetWindowResolution(void);
        glm::vec2                       GetCurrentMousePosition(void);
        GLFWwindow*                     ReturnEngineWindow(void);

        EditorSceneComponents&          GetComponents(void);
        EditorShaderCollection&         GetShaders(void);
        EditorFrameBufferView&          GetFrameBuffer(void);
        ComponentsWindowRenderHandler&  GetEditorToolbar(void);
    private:

        int                             FramesPerSecond;
        double                          DeltaTime;

        EditorSceneComponents           EditorComponents;
        EditorShaderCollection          EditorShaders;
        ComponentsWindowRenderHandler   EditorToolbar;
        EditorFrameBufferView           EditorFramebuffer;

        ModelSystem                     NewEntityService;
        glm::vec3                       PointerPosition;

        std::string 			        CurrentWorldFile;
        bool                            FirstTimeInitilized = true;

        float                           Near                = 0.001;
        float                           Far                 = 1000;
        float                           FieldOfView =        90;

        int                             ViewportWidth;
        int                             ViewportHeight;

        double                          MousePositionX;
        double                          MousePositionY;

        int                             Width               = 1920;
        int                             Height              = 1080;
        int                             Counter;
        double                          CurrentTime;
        double                          PreviousTime;
        double                          EngineDelayInput;

        bool                            Fullscreen          = false;
        bool                            UseVSync            = false;

        GLFWwindow*                     EngineWindow;
};

#endif