#ifndef __SPLASH_SCREEN_HEADER__
#define __SPLASH_SCREEN_HEADER__

#include <stdio.h>

#include "../../Resources/glad/glad.h"
#include "../../Resources/GLFW/glfw3.h"

#define GL_SPLASH_SCREEN_VERSION_MAJOR 3
#define GL_SPLASH_SCREEN_VERSION_MINOR 3

class SplashScreen
{
	public:
					SplashScreen(void);
		void		GenerateSplashScreen(void);
		void		RenderSplashScreen(void);
		void		DestroySplashScreen(void);
	private:
		GLFWwindow* SplashScreenFrame;

		const int	SplashScreenWidth = 100;
		const int	SplashScreenHeight = 100;

};

#endif
