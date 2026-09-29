#include "EditorMain.h"

EditorMain::EditorMain(void)
{
    this->LastNormalTrigger     = 0.0;
    this->ViewportHeight        = 0.0f;
    this->ViewportWidth         = 0.0f;
    this->IsActive              = false;
};

void MouseViewCallback(GLFWwindow* EngineWindow, int Button, int Action, int Mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(EngineWindow, Button, Action, Mods);

    if (Button == GLFW_MOUSE_BUTTON_RIGHT && Action == GLFW_PRESS)  { Console::LockMouse = false; Console::ConsoleMouseInputDetected = false; }
    else                                                            { Console::LockMouse = true; Console::ConsoleMouseInputDetected = true; }
}

void EditorMain::SpecialKeyFunction(void)
{
    //Key functions to edit geometry data.
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
                Console::SysPrint("Pasted Object", 1);
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

void EditorMain::RunBackgroundProcess(void)
{
    this->NewSplashScreen.GenerateSplashScreen();

    while (this->IsActive)
    {
        this->NewSplashScreen.RenderSplashScreen();
    }

    this->NewSplashScreen.DestroySplashScreen();
}

void EditorMain::StartEngine(void)
{
    this->IsActive = true;
    this->CurrentThreadPool.CreateThreads(1);
    //this->CurrentThreadPool.SubmitJob([this]() {this->RunBackgroundProcess(); });

    this->NewSessionManager.CreateSession();

    Commands::SetFromRegister("world", "../Fusion/DebugData.zip");

    this->NewSessionManager.UpdateWorld(std::get<std::string>(Commands::GetFromRegister("world")));
    this->NewSessionManager.CreateEngine();
    this->NewSessionManager.CreateFrameBuffer();
}

void EditorMain::DrawDebugBoxes(const glm::mat4& Projection, const glm::mat4& View, const glm::vec3& Minimum, const glm::vec3& Maximum)
{
    // This essentially would be copied from the main engine branch of the system and would be a debug tool needed to select objects.

    glUseProgram(0);

    // Submit the projection and view to te intermidate renderer.
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(Projection));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(View));
    glBegin(GL_LINES);
    // Draw the bounding box, just as how you would normally check the bounding box

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(Minimum.x, Minimum.y, Minimum.z);
    glVertex3f(Maximum.x, Minimum.y, Minimum.z);

    glVertex3f(Maximum.x, Minimum.y, Minimum.z);
    glVertex3f(Maximum.x, Maximum.y, Minimum.z);

    glVertex3f(Maximum.x, Maximum.y, Minimum.z);
    glVertex3f(Minimum.x, Maximum.y, Minimum.z);

    glVertex3f(Minimum.x, Maximum.y, Minimum.z);
    glVertex3f(Minimum.x, Minimum.y, Minimum.z);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(Minimum.x, Minimum.y, Maximum.z);
    glVertex3f(Maximum.x, Minimum.y, Maximum.z);

    glVertex3f(Maximum.x, Minimum.y, Maximum.z);
    glVertex3f(Maximum.x, Maximum.y, Maximum.z);

    glVertex3f(Maximum.x, Maximum.y, Maximum.z);
    glVertex3f(Minimum.x, Maximum.y, Maximum.z);

    glVertex3f(Maximum.x, Maximum.y, Maximum.z);
    glVertex3f(Minimum.x, Minimum.y, Maximum.z);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(Minimum.x, Minimum.y, Minimum.z);
    glVertex3f(Minimum.x, Minimum.y, Maximum.z);

    glVertex3f(Maximum.x, Minimum.y, Minimum.z);
    glVertex3f(Maximum.x, Minimum.y, Maximum.z);

    glVertex3f(Maximum.x, Maximum.y, Minimum.z);
    glVertex3f(Maximum.x, Maximum.y, Maximum.z);

    glVertex3f(Minimum.x, Maximum.y, Minimum.z);
    glVertex3f(Maximum.x, Maximum.y, Maximum.z);

    glEnd();
    glFlush();
}

glm::vec3 GlobalBoundMin;
glm::vec3 GlobalBoundMax;


void EditorMain::SelectObjectFromWorld(ImVec2 ViewportCursorPosition)
{
    if (!ImGui::IsMouseClicked(ImGuiMouseButton_Left))  { return; } //Return if not selected at all

    Camera NewCamera = this->NewSessionManager.GetComponents().EngineCamera;

    this->CurrentGeometryData =
        this->NewSessionManager
        .GetEditorToolbar()
        .ReturnEditorWorkspaceObject()
        .GetGeometryList();

    // @EDIT - It seems as if the mouse position is fetched within ImGuis perspective, hence we can use this to our advantage with the ReturnRegionContentAvail() function for which would produce a bound system 
    // across the viewport window, hence if the window escapes that bounds, the input is invalid. 
    ImVec2 CurrentMousePosition     = ImGui::GetMousePos();
    ImVec2 ViewportContentRegion    = this->NewSessionManager.GetEditorToolbar().ReturnRegionContentAvail();
    ImVec2 ViewportContentCorner    = this->NewSessionManager.GetEditorToolbar().ReturnViewportLocalScreenPosition(); // This would be used for the region above the window, the spaces the first region cannot get.
    ImVec2 ViewportItemRectMin      = this->NewSessionManager.GetEditorToolbar().ReturnViewportRectMin();
    ImVec2 ViewportItemRectSize     = this->NewSessionManager.GetEditorToolbar().ReturnViewportRectSize();

    // Check if the mouse position is within bounds from the viewport system. Ignores if out of bounds.
    if (CurrentMousePosition.x > ViewportContentRegion.x || CurrentMousePosition.y > ViewportContentRegion.y)   { return; }
    if (CurrentMousePosition.x < ViewportContentCorner.x || CurrentMousePosition.y < ViewportContentCorner.y)   { return; } // Do the same for the region the first statement cannot touch.

    //Transfer viewport (ImGui) coordinates into Normalised Device Coordinates (NDC)
    float MouseX = (2.0f * (CurrentMousePosition.x - ViewportItemRectMin.x)) / ViewportItemRectSize.x - 1.0f;
    float MouseY = 1.0f - (2.0f * (CurrentMousePosition.y - ViewportItemRectMin.y)) / ViewportItemRectSize.y;
    float MouseZ = 1.0f;

    glm::mat4 Projection =
        NewCamera.ReturnProjectionMatrix();

    glm::mat4 View =
        NewCamera.ReturnViewMatrix();

    glm::mat4 InverseViewProjection =
        glm::inverse(Projection * View);


    // Near point
    glm::vec4 NearPoint =
        InverseViewProjection *
        glm::vec4(MouseX, MouseY, -1.0f, 1.0f);

    NearPoint /= NearPoint.w;


    // Far point
    glm::vec4 FarPoint =
        InverseViewProjection *
        glm::vec4(MouseX, MouseY, 1.0f, 1.0f);

    FarPoint /= FarPoint.w;


    // ------------------------------------------------------------
    // World ray
    // ------------------------------------------------------------#

    std::cout
        << "Mouse NDC: "
        << MouseX << ", "
        << MouseY
        << "\n";

    std::cout
        << "Near: "
        << NearPoint.x << ", "
        << NearPoint.y << ", "
        << NearPoint.z
        << "\n";

    std::cout
        << "Far: "
        << FarPoint.x << ", "
        << FarPoint.y << ", "
        << FarPoint.z
        << "\n";

    std::cout
        << "Ray: "
        << RayDirection.x << ", "
        << RayDirection.y << ", "
        << RayDirection.z
        << "\n";

    glm::vec3 RayOrigin =
        glm::vec3(NearPoint);

    glm::vec3 RayDirection =
        glm::normalize(
            glm::vec3(FarPoint) -
            glm::vec3(NearPoint)
        );


    // Store it
    this->RayOrigin = RayOrigin;
    this->RayDirection = RayDirection;

    float ClosestValue  = FLT_MAX;
    int Selected        = -1;

    for (int Index = 0; Index < this->CurrentGeometryData.size(); Index++)
    {
        std::cout << "Position Index " << Index << ": " << this->CurrentGeometryData[Index].Translation.x << "," << this->CurrentGeometryData[Index].Translation.y << "," << this->CurrentGeometryData[Index].Translation.z << "\n";

        float T;
        GeometryData& CurrentObject = this->CurrentGeometryData[Index];
        glm::mat4 ModelMatrix       = CurrentObject.TranslationMatrix * CurrentObject.RotationMatrix * CurrentObject.ScaleMatrix;
        glm::mat4 InverseModel      = glm::inverse(ModelMatrix);

        glm::vec3 LocalOrigin       = glm::vec3(InverseModel * glm::vec4(this->RayOrigin, 1.0f));
        glm::vec3 LocalDirection    = glm::normalize(glm::vec3(InverseModel * glm::vec4(this->RayDirection, 0.0f)));

        glm::vec3 LocalBoundingMin = glm::vec3((CurrentObject.TranslationMatrix * CurrentObject.RotationMatrix * CurrentObject.ScaleMatrix) * glm::vec4(CurrentObject.GeometryBoundingBox.MinimumBounds, 1.0f));
        glm::vec3 LocalBoundingMax = glm::vec3((CurrentObject.TranslationMatrix * CurrentObject.RotationMatrix * CurrentObject.ScaleMatrix) * glm::vec4(CurrentObject.GeometryBoundingBox.MaximumBounds, 1.0f));

        GlobalBoundMin = LocalBoundingMin;
        GlobalBoundMax = LocalBoundingMax;

        std::cout << LocalBoundingMin.x << LocalBoundingMin.y << LocalBoundingMin.z << "\n";
        std::cout << LocalBoundingMax.x << LocalBoundingMax.y << LocalBoundingMax.z << "\n";

        if (EditorUtilites::RayIntersectsAABB(this->RayOrigin, this->RayDirection, LocalBoundingMin, LocalBoundingMax, T))
        {
            if (T < ClosestValue)
            {
                ClosestValue    = T;
                Selected        = Index;

                std::cout << "Selected item" << "\n";

                glm::vec3 LocalHit = LocalOrigin + LocalDirection * T;
                glm::vec3 WorldHit = glm::vec3(ModelMatrix * glm::vec4(LocalHit, 1.0f));

                this->DragDelta = CurrentObject.Translation - WorldHit;
            }
        }
    }

    this->NewSessionManager.GetEditorToolbar().SelectedIndex = Selected;
}

void EditorMain::DebugSelectionLine(void)
{
    // This would use the immediate mode of OpenGL rather than the retained mode. This is for easier debugging, since
    // I dont need to submit to the renderer (VAO,VBO ect.) 
    glUseProgram(0);
    //Show to projection mode, we use projection matrix from the camera.
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(this->NewSessionManager.GetComponents().EngineCamera.ReturnProjectionMatrix()));

    //Submit model matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(this->NewSessionManager.GetComponents().EngineCamera.ReturnViewMatrix()));

    //Just draw using lines. This is the ray direction.
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(RayOrigin.x, RayOrigin.y, RayOrigin.z);
    glVertex3f(
        RayOrigin.x + RayDirection.x * 1000.0f,
        RayOrigin.y + RayDirection.y * 1000.0f,
        RayOrigin.z + RayDirection.z * 1000.0f); // These would appear to fire far far away as a debug.
    glEnd();


    // Draw points as a debug. This is still using the weaker intermediate mode of rendering.
    // Just a debug!
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(RayOrigin.x, RayOrigin.y, RayOrigin.z);
    glEnd();


    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(
        RayOrigin.x + RayDirection.x,
        RayOrigin.y + RayDirection.y,
        RayOrigin.z + RayDirection.z);
    glEnd();
    glFlush(); // Remember to clean up!
}

void EditorMain::RenderEngine(void)
{
    while (!glfwWindowShouldClose(this->NewSessionManager.ReturnEngineWindow()))
    {
        if (this->EngineViewportSize.x != this->ViewportWidth || this->EngineViewportSize.y != this->ViewportHeight)
        {
            // @EDIT - This would only be called when the frame size would be updated. Uses the captured viewport size from each frame compared to the one we have right now.
            // There seems to be slight delay with the rescaling of the framebuffer and the movement of the window and this is the culprit. 
            // We could move this eariler or do this every frame but thats very inefficent.

            this->ViewportWidth         = this->EngineViewportSize.x;
            this->ViewportHeight        = this->EngineViewportSize.y;

            this->NewSessionManager.GetComponents().EngineCamera.UpdateCameraResolution(this->ViewportWidth, this->ViewportHeight);
            this->NewSessionManager.SetViewportResolution(ImVec2(this->ViewportWidth, this->ViewportHeight));
            this->NewSessionManager.RescaleFrameBuffer(this->ViewportWidth, this->ViewportHeight);
            glViewport(0, 0, this->ViewportWidth, this->ViewportHeight);
        }
        
        // @TODO - Perhaps make this more efficent by not fetching per frame? Maybe a timer system with set intervals or something.
        this->EngineViewportSize = this->NewSessionManager.GetEditorToolbar().ReturnRegionContentAvail();
        this->NewSessionManager.BindFrameBuffer();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        this->NewSessionManager.GetEditorToolbar().ReturnEditorWorkspaceObject().DrawGeometryScene(&this->NewSessionManager.GetShaders().EntityShader, this->NewSessionManager.GetComponents().EngineCamera.ReturnProjectionMatrix(), this->NewSessionManager.GetComponents().EngineCamera.ReturnViewMatrix(), this->NewSessionManager.GetComponents().EngineCamera.ReturnCameraPosition());
        this->NewSessionManager.RenderEngine();

        this->SpecialKeyFunction();
        this->SelectObjectFromWorld(this->NewSessionManager.GetEditorToolbar().ReturnPositionViewport());

        this->DrawDebugBoxes(this->NewSessionManager.GetComponents().EngineCamera.ReturnProjectionMatrix(), this->NewSessionManager.GetComponents().EngineCamera.ReturnViewMatrix(), GlobalBoundMin, GlobalBoundMax);
        this->DebugSelectionLine();

        this->NewSessionManager.UnbindFrameBuffer();
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