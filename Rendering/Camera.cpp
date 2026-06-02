#include "Camera.h"

Camera::Camera(void) 
{
	this->Position  = glm::vec3(0.0f, 0.0f, 0.0f);
    this->Width     = 0;
	this->Height    = 0;
    this->Near      = 0;
    this->Far       = 0;
}

void LocalScrollCallback(GLFWwindow* EngineWindow, double XOffset, double YOffset)
{
    // @EDIT - Just a basic wrapper from GLFW -> Camera object system.
    Camera* CameraObject = static_cast<Camera*>(glfwGetWindowUserPointer(EngineWindow));
    CameraObject->MoveFunctionScroll(EngineWindow, XOffset, YOffset);
}

glm::vec3& Camera::ReturnCameraPosition(void)       {return this->Position;}
glm::mat4& Camera::ReturnProjectionMatrix(void)     {return this->ProjectionMatrix;}
glm::mat4& Camera::ReturnViewMatrix(void)           {return this->ViewMatrix;}

void Camera::UpdateCameraPosition(const glm::vec3& NewPosition)         {this->Position = NewPosition;}
void Camera::UpdateCameraResolution(int WindowWidth,int WindowHeight)   {this->Width = WindowWidth; this->Height = WindowHeight;}

void Camera::SetViewportSize(float Width, float Height,float FieldOfView,float Near,float Far)
{
    // @EDIT - Why is this here? Is this called or something at the start? I need to figure out!
    float AspectRatio   = Width / Height;
    this->Width         = Width;
    this->Height        = Height;

    this->ProjectionMatrix = glm::perspective(glm::radians(FieldOfView),AspectRatio,Near,Far);
}

void Camera::SpawnCamera(int WindowWidth,int WindowHeight,const glm::vec3& CameraPosition)
{
    this->Width         = WindowWidth;
    this->Height        = WindowHeight;
    this->Position      = CameraPosition;
}

void Camera::UpdateCameraMatrix(float FieldOfView,float NearPlane,float FarPlane,int WindowWidth,int WindowHeight)
{
    float AspectRatio       = this->Width / this->Height;
    AspectRatio             = glm::clamp(AspectRatio, 0.3f, 5.0f);

    this->Near              = NearPlane;
    this->Far               = FarPlane;
    this->ProjectionMatrix  = glm::perspective(glm::radians(FieldOfView),(float)(AspectRatio), NearPlane,FarPlane);
    this->ViewMatrix        = glm::lookAt(this->Position,this->Position+this->Orientation,this->UpPosition);
}

void Camera::BindCameraMatrix(float FieldOfView,float NearPlane,float FarPlane,const glm::vec3& BindedPosition)
{
    // @EDIT - This function was primarily used for 3rd person camera view when rendering something or when 
    // we are following an object, bind the position to the object. We need to add new stuff from UpdateCameraMatrix() 
    // as thats the main function that we are focused on

    // @EDIT - Perhaps use that function with a parameter for perspective view?

    this->Near              = NearPlane;
    this->Far               = FarPlane;
    this->ProjectionMatrix  = glm::perspective(glm::radians(FieldOfView),(float)(this->Width/this->Height),NearPlane,FarPlane);
    this->ViewMatrix        = glm::lookAt(BindedPosition,BindedPosition+this->Orientation,this->UpPosition);
}

void Camera::MoveCamera(void)
{
    if (Console::LockMouse) {return;}

    float RotationX;
    float RotationY;
    double MouseX = 0.0;
    double MouseY = 0.0;
    RotationX = this->Sensitivity * (float)(MouseY - (this->Height/2)) / this->Height;
    RotationY = this->Sensitivity * (float)(MouseX - (this->Width/2)) / this->Width;

    glm::vec3 NewOrientation = glm::rotate(this->Orientation,glm::radians(-RotationX),glm::normalize(glm::cross(this->Orientation,this->UpPosition)));

    if (!((glm::angle(NewOrientation,UpPosition) <= glm::radians(5.0f)) || (glm::angle(NewOrientation,-UpPosition) <= glm::radians(5.0f))))
    {this->Orientation  = NewOrientation;}
    this->Orientation   = glm::rotate(this->Orientation,glm::radians(-RotationY),this->UpPosition);
}

void Camera::UpdateCameraView(GLFWwindow* EngineWindow,double XPosition,double YPosition)
{
    if (Console::LockMouse) {this->FirstMouse = true; return;}
    if (this->FirstMouse)
    {
        this->LastX         = XPosition;
        this->LastY         = YPosition;
        this->FirstMouse    = false;
    }

    this->XOffset   = (float)XPosition - this->LastX;
    this->YOffset   = (float)this->LastY - YPosition;
    this->LastX     = XPosition;
    this->LastY     = YPosition;

    this->XOffset *= this->Sensitivity;
    this->YOffset *= this->Sensitivity;
    this->RotateX += this->XOffset;
    this->RotateY += this->YOffset;

    if (this->RotateY > 89.0f)  {this->RotateY = 89.0f;}
    if (this->RotateY < -89.0f) {this->RotateY = -89.0f;}

    this->Orientation.x = cos(glm::radians(this->RotateY)) * cos(glm::radians(this->RotateX));
    this->Orientation.y = sin(glm::radians(this->RotateY));
    this->Orientation.z = cos(glm::radians(this->RotateY)) * sin(glm::radians(this->RotateX));
    this->Orientation   = glm::normalize(this->Orientation);
}

void Camera::MoveFunctionScroll(GLFWwindow* EngineWindow,double XOffset,double YOffset)
{
    // @TODO - Maybe you could also capture the focus on windows rather than mostly text input when using scrolls. We might need scrollwheel to
    // scroll down elements.
    if (!(ImGui::GetIO().WantTextInput))
    {
        if (YOffset > 0)    { this->Position += 5.0f * this->Orientation; }
        else                { this->Position += 5.0f * -this->Orientation; } // We could speed this up with the duration of scroll wheel?
    }
}

void Camera::MoveFunctionCamera(GLFWwindow* EngineWindow)
{
    // @EDIT - These would be normal inputs for which would be bounded upon the normal keys and locked when text is needed / ctrl is used.
    // @TODO - Make the keys more modular and interchangeable (perhaps a key system using register?)
    if (glfwGetKey(EngineWindow, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && !(ImGui::GetIO().WantTextInput))
    {
        if (glfwGetKey(EngineWindow, GLFW_KEY_W) == GLFW_PRESS) { this->Position += 5.0f * this->Orientation; }
        if (glfwGetKey(EngineWindow, GLFW_KEY_A) == GLFW_PRESS) { this->Position += 5.0f * -glm::normalize(glm::cross(Orientation, glm::vec3(0.0f, 1.0f, 0.0f))); }
        if (glfwGetKey(EngineWindow, GLFW_KEY_S) == GLFW_PRESS) { this->Position += 5.0f * -this->Orientation; }
        if (glfwGetKey(EngineWindow, GLFW_KEY_D) == GLFW_PRESS) { this->Position += 5.0f * glm::normalize(glm::cross(Orientation, glm::vec3(0.0f, 1.0f, 0.0f))); }
    }

    glfwSetWindowUserPointer(EngineWindow, this);
    glfwSetScrollCallback(EngineWindow, LocalScrollCallback);
}