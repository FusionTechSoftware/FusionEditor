#ifndef __COMPONENTS_WINDOWS_HEADER__
#define __COMPONENTS_WINDOWS_HEADER__

#define GLM_ENABLE_EXPERIMENTAL

#include <map>

#include "../../../Resources/glad/glad.h"
#include "../../../Resources/glm/glm.hpp"
#include "../../../Resources/GLFW/glfw3.h"
#include "../../../Resources/glm/gtx/matrix_decompose.hpp"

#include "../../../Resources/IconsFontAwesome5.h"
#include "../../Rendering/Camera.h"
#include "../../Utilites/ImGuiUtilites.h"
#include "../EditorWorkspace.h"
#include "../../../Resources/imgui/ImGuizmo.h"

#include "PropertiesWindow.h"
#include "CodeEditor.h"

#define MENU_OFFSET		20.0f
#define MENU_SIZE		50.0f
#define BUILD_VERSION	0.20f
#define RELEASE_VERSION 0.25f
#define SET_AS_ALPHA

#define ALLOW_FEEDBACK

enum InterfaceTooltipValues
{
	VALUE_NULL = 0,
	VALUE_POINTER,
	VALUE_TRANSLATE,
	VALUE_ROTATE,
	VALUE_SCALE
};

enum InterfaceStatusValues
{
	STATUS_UNKNOWN,
	STATUS_ERROR,
	STATUS_WARNING,
	STATUS_READY
};

struct InterfaceValues
{
	bool			IsTextureMenuOpen;
	bool			IsSettingsMenuOpen;
	bool			IsImageCompilerOpen;
	unsigned int	LogoImageID;
};

struct InterfaceSearchGroup
{
	std::string SearchInputText;
};

struct InterfaceAdditionMenu
{
	InterfaceAdditionMenu(const std::string& Name, const std::string& Icons) : Name(Name), Icons(Icons) {};

	std::string Name;
	std::string Icons;
};

struct InterfaceRenderModule
{
	unsigned int	FrameBufferTextureIndex;
	int				ViewportWidth;
	int				ViewportHeight;
	std::string		RenderTitle;
};

class ComponentsWindowRenderHandler
{
	public:
												ComponentsWindowRenderHandler(void);
		void									LoadComponentWindow(void);
		void									LoadIconFonts(void);
		void									ApplyLocalTheme(void);
		void									ShowEditGizmo(Camera* EngineCamera, int ViewportWidth, int ViewportHeight);
		void									DrawComponentWindow(GLFWwindow* EngineWindow, Camera* EngineCamera, Shader* EngineShader, unsigned int FrameBuffer);
		void									DrawTextureViewer(void);
		void									DrawStatusBar(void);
		void									DrawRenderSceneWindow(GLFWwindow* EngineWindow, Camera* EngineCamera, InterfaceRenderModule CurrentModule);
		void									DrawMainToolbar(Camera* EngineCamera, Shader* EngineShader);
		void									DrawAddComponentsMenu(void);
		void									DrawDropdownMenu(const std::vector< ModelObjectProperties>& DropdownContents);
		void									DrawSettings(InterfaceValues CurrentValues);
		void									DrawToolbarWidget(InterfaceValues& CurrentValues, GLFWwindow* EngineWindow, const glm::mat4& Projection, const glm::mat4& View, const glm::vec3& Position);
		void									Destroy(int Index);

		int										SelectedIndex;
		EditorWorkspace&						ReturnEditorWorkspaceObject(void);
		ImVec2									ReturnRegionContentAvail(void);
		ImVec2									ReturnPositionViewport(void);
		InterfaceTooltipValues					CurrentToolValue;
		InterfaceValues							CurrentInterfaceValue;
		InterfaceStatusValues					CurrentStatus;

	private:

		void									DrawAudioToolpack(void);
		void									DrawDebugToolpack(void);
		void									DrawModelToolpack(void);
		void									DrawGeometryToolpack(Camera* EngineCamera, Shader* EngineShader);
		void									ModeButton(const char* Icon, InterfaceTooltipValues Mode, const ImVec4& ActiveColor);
		void									FeedbackMenuSystem(void);

		bool									FeedbackToggle;
		ImVec2									CurrentViewportSize;
		ImVec2									CurrentViewportPosition;
		std::string								StatusToMessage(void);

		std::map<std::string, unsigned int>		TextureGroup;
		std::map<std::string, unsigned int>		SkyboxGroup;
		std::map<std::string, unsigned int>		ModelGroup;

		std::string								CurrentWidget			= "Geometry";
		std::vector<std::string>				ToolsList				= {"None"};
		std::vector<std::string>				GroupFolders			= { "Textures","Skybox","Models" };
		std::vector<std::string>				WidgetTabs				= { "Geometry","Model","Debug","Audio" };
		std::vector<InterfaceAdditionMenu>		AdditionComponentMenu	= { InterfaceAdditionMenu("Add Mesh",ICON_FA_CUBE), InterfaceAdditionMenu("Add Cube",ICON_FA_CUBE), InterfaceAdditionMenu("Add Lights",ICON_FA_LIGHTBULB), InterfaceAdditionMenu("Add Player",ICON_FA_MAGIC) };
	
		glm::vec3								ObjectScale;
		glm::vec3								ObjectTranslation;
		glm::quat								ObjectRotation;
		glm::vec3								Skew;
		glm::vec4								Perspective;

		CodeEditor								NewTextEditor;
		EditorWorkspace							NewWorkspace;
		InterfaceSearchGroup					NewSearchGroup;
		GradientBlock							DefaultButtonHovered	= { IM_COL32(40, 40, 40, 255), IM_COL32(20, 20, 20, 255) };
		GradientBlock							DefaultButtonActive		= { IM_COL32(70, 70, 70, 255), IM_COL32(20, 20, 20, 255) };
		GradientBlock							DefaultButtonNormal		= { IM_COL32(60, 60, 60, 255), IM_COL32(20, 20, 20, 255) };
		GradientBlock							DefaultButtonUsed		= { IM_COL32(60, 120, 60, 255), IM_COL32(20, 20, 20, 255) };

		GradientBlock							GreenButtonHovered		= { IM_COL32(20, 20, 20, 255), IM_COL32(60, 120, 60, 255) };
		GradientBlock							GreenButtonNormal		= { IM_COL32(40, 40, 40, 255), IM_COL32(20, 20, 20, 255) };

		GradientBlock							BlueButtonHovered		= { IM_COL32(20, 20, 20, 255), IM_COL32(107, 179, 255, 255) };
};

#endif