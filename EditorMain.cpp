#include "EditorMain.h"

EditorMain::EditorMain(void)
{
    this->LastNormalTrigger     = 0.0;
    this->ViewportHeight        = 0.0f;
    this->ViewportWidth         = 0.0f;

    this->ObjectScale           = glm::vec3(0.0f);
    this->ObjectTranslation     = glm::vec3(0.0f);
    this->ObjectRotation        = glm::vec3(0.0f);
};

void MouseViewCallback(GLFWwindow* EngineWindow, int Button, int Action, int Mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(EngineWindow, Button, Action, Mods);

    if (Button == GLFW_MOUSE_BUTTON_RIGHT && Action == GLFW_PRESS)  { Console::LockMouse = false; Console::ConsoleMouseInputDetected = false; }
    else                                                            { Console::LockMouse = true; Console::ConsoleMouseInputDetected = true; }
}

void EditorMain::SpecialKeyFunction(void)
{
    if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (this->NewSessionManager.GetEditorToolbar().SelectedIndex == -1) { return; }
        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)      {this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex, this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex) + glm::vec3(-Console::IncrementValue[0], 0.0f, 0.0f)); }
        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)     {this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex, this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex) + glm::vec3(Console::IncrementValue[0], 0.0f, 0.0f));}
        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_UP) == GLFW_PRESS)        {this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex, this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex) + glm::vec3(0.0f, Console::IncrementValue[1], 0.0f));}
        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)      {this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex, this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex) + glm::vec3(0.0f, -Console::IncrementValue[1], 0.0f));}

        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_PAGE_UP) == GLFW_PRESS)   {this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex, this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex) + glm::vec3(0.0f, 0.0f, Console::IncrementValue[2]));}
        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex, this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex) + glm::vec3(0.0f, 0.0f, -Console::IncrementValue[2]));}

        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_R) == GLFW_PRESS)
        {
            if (glfwGetTime() - this->LastNormalTrigger >= 0.4f)
            {
                glm::quat Rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f));

                this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetRotation(
                    this->NewSessionManager.GetEditorToolbar().SelectedIndex,
                    this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetRotation(this->NewSessionManager.GetEditorToolbar().SelectedIndex) * Rotation
                );
                this->LastNormalTrigger = glfwGetTime();
            }
        }

        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_T) == GLFW_PRESS)
        {
            if (glfwGetTime() - this->LastNormalTrigger >= 0.4f)
            {
                glm::quat Rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f,0.0f,0.0f));

                this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetRotation(
                    this->NewSessionManager.GetEditorToolbar().SelectedIndex,
                    this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetRotation(this->NewSessionManager.GetEditorToolbar().SelectedIndex) * Rotation
                );
                this->LastNormalTrigger = glfwGetTime();
            }
        }

        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_C) == GLFW_PRESS)
        {
            if (glfwGetTime() - this->LastNormalTrigger >= 0.4f)
            {
                Console::SysPrint("Copied Object", 1);
                this->LastNormalTrigger = glfwGetTime();
            }
        }

        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_V) == GLFW_PRESS)
        {
            if (glfwGetTime() - this->LastNormalTrigger >= 0.4f)
            {
                Console::SysPrint("Pased Object", 1);
                this->LastNormalTrigger = glfwGetTime();
            }
        }

        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_D) == GLFW_PRESS)
        {
            if (glfwGetTime() - this->LastNormalTrigger >= 0.4f)
            {
                Console::SysPrint("Duplicated Object", 1);
                this->LastNormalTrigger = glfwGetTime();
            }
        }
    }

    if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_DELETE) == GLFW_PRESS)
    {
        if (glfwGetTime() - this->LastNormalTrigger >= 0.4f)
        {
            this->NewSessionManager.GetEditorToolbar().Destroy(this->NewSessionManager.GetEditorToolbar().SelectedIndex);
            this->LastNormalTrigger = glfwGetTime();
        }
    }

    if (!ImGui::IsAnyItemActive())
    {
        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_2) == GLFW_PRESS) { this->NewSessionManager.GetEditorToolbar().CurrentToolValue = VALUE_ROTATE; }
        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_1) == GLFW_PRESS) { this->NewSessionManager.GetEditorToolbar().CurrentToolValue = VALUE_TRANSLATE; }
        if (glfwGetKey(this->NewSessionManager.ReturnEngineWindow(), GLFW_KEY_3) == GLFW_PRESS) { this->NewSessionManager.GetEditorToolbar().CurrentToolValue = VALUE_SCALE; }
    }
}

void EditorMain::StartEngine(void)
{
    this->NewSessionManager.CreateSession();

    Commands::SetFromRegister("world", "../Fusion/DebugData.zip");

    this->NewSessionManager.UpdateWorld(std::get<std::string>(Commands::GetFromRegister("world")));
    this->NewSessionManager.CreateEngine();
    this->NewSessionManager.CreateFrameBuffer();
}

void EditorMain::ShowEditGizmo(int ViewportWidth, int ViewportHeight)
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    ImVec2 ViewportPos = ImGui::GetWindowPos();
    ImGuizmo::SetRect(ViewportPos.x, ViewportPos.y, ViewportWidth, ViewportHeight);

    if (this->NewSessionManager.GetEditorToolbar().SelectedIndex != -1 && (this->NewSessionManager.GetEditorToolbar().CurrentToolValue != VALUE_NULL && this->NewSessionManager.GetEditorToolbar().CurrentToolValue != VALUE_POINTER))
    {
        ImGuizmo::Style& NewStyle = ImGuizmo::GetStyle();

        NewStyle.Colors[ImGuizmo::DIRECTION_X] = ImVec4(0.666f, 0.000f, 0.000f, 1.000f);
        NewStyle.Colors[ImGuizmo::DIRECTION_Y] = ImVec4(0.000f, 0.666f, 0.000f, 1.000f);
        NewStyle.Colors[ImGuizmo::DIRECTION_Z] = ImVec4(0.000f, 0.000f, 0.666f, 1.000f);

        glm::mat4 ModelMatrix = this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetPositionMatrix(this->NewSessionManager.GetEditorToolbar().SelectedIndex) *
            this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetRotationMatrix(this->NewSessionManager.GetEditorToolbar().SelectedIndex) *
            this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().GetScaleMatrix(this->NewSessionManager.GetEditorToolbar().SelectedIndex);


        ImGuizmo::Manipulate(
            glm::value_ptr(this->NewSessionManager.GetComponents().EngineCamera.ReturnViewMatrix()),
            glm::value_ptr(this->NewSessionManager.GetComponents().EngineCamera.ReturnProjectionMatrix()),
            (
                this->NewSessionManager.GetEditorToolbar().CurrentToolValue == VALUE_TRANSLATE ? ImGuizmo::TRANSLATE : (this->NewSessionManager.GetEditorToolbar().CurrentToolValue == VALUE_ROTATE ? ImGuizmo::ROTATE : (this->NewSessionManager.GetEditorToolbar().CurrentToolValue == VALUE_SCALE ? ImGuizmo::SCALE : ImGuizmo::SCALE))),
            ImGuizmo::LOCAL,
            glm::value_ptr(ModelMatrix),
            nullptr,
            Console::IncrementValue
        );

        if (ImGuizmo::IsUsing())
        {
            if (glm::decompose(ModelMatrix, this->ObjectScale, this->ObjectRotation, this->ObjectTranslation, Skew, Perspective))
            {
                if (this->ObjectScale.x <= 0.1 || this->ObjectScale.y <= 0.1 || this->ObjectScale.z <= 0.1) { return; }

                this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetPosition(this->NewSessionManager.GetEditorToolbar().SelectedIndex,this->ObjectTranslation);
                this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetRotation(this->NewSessionManager.GetEditorToolbar().SelectedIndex, this->ObjectRotation);
                this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().SetScale(this->NewSessionManager.GetEditorToolbar().SelectedIndex, this->ObjectScale);
            }
        }
    }

    this->SpecialKeyFunction();
}

void EditorMain::SelectObjectFromWorld(ImVec2 ViewportCursorPosition)
{
    if (Console::LockMouse)                             { return; }
    if (!ImGui::IsMouseClicked(ImGuiMouseButton_Left))  { return; }

    Camera NewCamera = this->NewSessionManager.GetComponents().EngineCamera;

    this->CurrentGeometryData =
        this->NewSessionManager
        .GetEditorToolbar()
        .ReturnEditorWorkspaceObject()
        .GetGeometryList();

    this->RayOrigin     = NewCamera.ReturnCameraPosition();
    this->RayDirection  = glm::normalize(
        EditorUtilites::GetRayFromMouse(
            NewCamera.ReturnProjectionMatrix(),
            NewCamera.ReturnViewMatrix(),
            this->NewSessionManager.GetCurrentMousePosition().x - 5.0f,
            this->NewSessionManager.GetCurrentMousePosition().y - 5.0f,
            this->ViewportWidth,
            this->ViewportHeight
        )
    );

    float ClosestValue  = FLT_MAX;
    int Selected        = -1;

    for (int Index = 0; Index < this->CurrentGeometryData.size(); Index++)
    {
        float T;
        GeometryData& CurrentObject = this->CurrentGeometryData[Index];
        glm::mat4 ModelMatrix       = CurrentObject.TranslationMatrix * CurrentObject.RotationMatrix * CurrentObject.ScaleMatrix;
        glm::mat4 InverseModel      = glm::inverse(ModelMatrix);

        glm::vec3 LocalOrigin       = glm::vec3(InverseModel * glm::vec4(this->RayOrigin, 1.0f));
        glm::vec3 LocalDirection    = glm::normalize(glm::vec3(InverseModel * glm::vec4(this->RayDirection, 0.0f)));

        if (EditorUtilites::DoesRayIntersectBounds(LocalOrigin, LocalDirection, CurrentObject.GeometryBoundingBox.MinimumBounds, CurrentObject.GeometryBoundingBox.MaximumBounds, T))
        {
            if (T < ClosestValue)
            {
                ClosestValue    = T;
                Selected        = Index;

                glm::vec3 LocalHit = LocalOrigin + LocalDirection * T;
                glm::vec3 WorldHit = glm::vec3(ModelMatrix * glm::vec4(LocalHit, 1.0f));

                this->DragDelta = CurrentObject.Translation - WorldHit;
            }
        }
    }

    this->NewSessionManager.GetEditorToolbar().SelectedIndex = Selected;
}

void EditorMain::RenderEngine(void)
{
    while (!glfwWindowShouldClose(this->NewSessionManager.ReturnEngineWindow()))
    {
        if (this->EngineViewportSize.x != this->ViewportWidth || this->EngineViewportSize.y != this->ViewportHeight)
        {
            this->ViewportWidth     = static_cast<int>(this->EngineViewportSize.x);
            this->ViewportHeight    = static_cast<int>(this->EngineViewportSize.y);

            this->NewSessionManager.SetViewportResolution(ImVec2(this->ViewportWidth, this->ViewportHeight));
            this->NewSessionManager.RescaleFrameBuffer(this->ViewportWidth, this->ViewportHeight);
            glViewport(0, 0, this->ViewportWidth, this->ViewportHeight);
        }

        this->NewSessionManager.BindFrameBuffer();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().DrawGeometryScene(&this->NewSessionManager.GetShaders().EntityShader, this->NewSessionManager.GetComponents().EngineCamera.ReturnProjectionMatrix(), this->NewSessionManager.GetComponents().EngineCamera.ReturnViewMatrix(), this->NewSessionManager.GetComponents().EngineCamera.ReturnCameraPosition());
        this->NewSessionManager.RenderEngine();

        this->SelectObjectFromWorld(this->NewSessionManager.GetEditorToolbar().ReturnPositionViewport());
        //this->ShowEditGizmo(this->ViewportWidth, this->ViewportHeight);

        this->NewSessionManager.UnbindFrameBuffer();

        this->EngineViewportSize = this->NewSessionManager.GetEditorToolbar().ReturnRegionContentAvail();
        this->NewSessionManager.GetEditorToolbar().DrawComponentWindow(this->NewSessionManager.ReturnEngineWindow(), &this->NewSessionManager.GetComponents().EngineCamera, &this->NewSessionManager.GetShaders().DebugShader,this->NewSessionManager.GetFrameBuffer().TextureIdentifer);

        glfwSwapBuffers(this->NewSessionManager.ReturnEngineWindow());
        glfwPollEvents();
    }
}

void EditorMain::CloseEngine(void)
{
    this->NewSessionManager.DestroyEngine();
}

int main()
{
    EditorMain NewEngine;
    NewEngine.StartEngine();
    NewEngine.RenderEngine();
    NewEngine.CloseEngine();
}