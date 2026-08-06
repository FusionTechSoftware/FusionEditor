#include "SplashScreen.h"

SplashScreen::SplashScreen(void) {}

void SplashScreen::GenerateSplashScreen(void)
{
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_SPLASH_SCREEN_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_SPLASH_SCREEN_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->SplashScreenFrame = glfwCreateWindow(this->SplashScreenWidth, this->SplashScreenHeight, "FusionLoader", NULL, NULL);
    glfwMakeContextCurrent(this->SplashScreenFrame);
    gladLoadGL();
    glViewport(0, 0, this->SplashScreenWidth, this->SplashScreenHeight);

    if (this->SplashScreenFrame)                                { printf("GLFW failed to initilise the window for FusionEditor Splash screen"); glfwTerminate(); }
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))     { printf("Failed to load GL!");  glfwTerminate(); }
}

void SplashScreen::RenderSplashScreen(void)
{
    while (!glfwWindowShouldClose(this->SplashScreenFrame))
    {
        glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(this->SplashScreenFrame);
        glfwPollEvents();
    }
}

void SplashScreen::DestroySplashScreen(void)
{
    glfwTerminate();
}