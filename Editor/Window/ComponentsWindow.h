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
#include "../../Utilites/ImageUtilities.h"
#include "../EditorWorkspace.h"
#include "../../../Resources/imgui/ImGuizmo.h"

#include "GridEditor/GridEditorWindow.h"
#include "PropertiesWindow.h"
#include "CodeEditor.h"

#include "../../Common/Parser.h"

#define MENU_OFFSET		20.0f
#define MENU_SIZE		50.0f
#define BUILD_VERSION	0.20f
#define RELEASE_VERSION 0.25f
#define MAX_TEXT_EDITORS 256
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

struct ScriptContainer
{
	int	ScriptIndex;
	bool ScriptActive;
	std::string ScriptName;
	std::string ScriptContents;
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

struct AssetViewModule
{
	unsigned int	AssetImage;
	std::string		AssetUnicodeImage;
};

class ComponentsWindowRenderHandler
{
	public:
												ComponentsWindowRenderHandler(void);
		void									LoadComponentWindow(void);
		void									LoadIconFonts(void);
		void									LoadDescriptiveImage(void);
		void									ApplyLocalTheme(void);
		void									SubmitParserDataToEditor(const std::vector<ParserMap>& ParserContents);
		void									ShowEditGizmo(Camera* EngineCamera, int ViewportWidth, int ViewportHeight);
		void									DrawComponentWindow(GLFWwindow* EngineWindow, Camera* EngineCamera, Shader* EngineShader, unsigned int FrameBuffer);
		void									DrawTextureViewer(void);
		void									DrawStatusBar(void);
		void									DrawRenderSceneWindow(GLFWwindow* EngineWindow, Camera* EngineCamera, InterfaceRenderModule CurrentModule);
		void									DrawMainToolbar(Camera* EngineCamera, Shader* EngineShader);
		void									DrawAddComponentsMenu(Camera* EngineCamera, Shader* EngineShader);
		void									DrawDropdownMenu(std::vector<ModelObjectProperties>& DropdownContents);
		void									DrawSettings(InterfaceValues CurrentValues);
		void									DrawToolbarWidget(InterfaceValues& CurrentValues, GLFWwindow* EngineWindow, const glm::mat4& Projection, const glm::mat4& View, const glm::vec3& Position);
		void									Destroy(int Index);

		int										SelectedIndex;
		EditorWorkspace&						ReturnEditorWorkspaceObject(void);
		ImVec2									ReturnRegionContentAvail(void);
		ImVec2									ReturnPositionViewport(void);
		ImVec2									ReturnViewportSafeSpace(void);
		ImVec2									ReturnViewportLocalScreenPosition(void);
		ImVec2									ReturnViewportRectMin(void);
		ImVec2									ReturnViewportRectSize(void);

		InterfaceTooltipValues					CurrentToolValue;
		InterfaceValues							CurrentInterfaceValue;
		InterfaceStatusValues					CurrentStatus;
		GridEditor								CurrentGridEditor;

	private:

		void									SpawnScriptInstance(void);
		void									SpawnCubeInstance(Camera* EngineCamera, Shader* EngineShader);
		void									RenderScripts(void);
		
		void									DrawFileImporter(void);
		void									DrawAudioToolpack(void);
		void									DrawDebugToolpack(void);
		void									DrawModelToolpack(Camera* EngineCamera, Shader* EngineShader);
		void									DrawGeometryToolpack(Camera* EngineCamera, Shader* EngineShader);
		void									ModeButton(const char* Icon, InterfaceTooltipValues Mode, const ImVec4& ActiveColor);
		void									ManageAssetBeingPicked(const std::string& FolderSelectedName);
		void									FeedbackMenuSystem(void);

		unsigned int							UnknownImageID;
		unsigned int							SpawnID;
		unsigned int							ThumbnailID;

		int										InstanceIndex;
		bool									IsFileImporterOpen;
		bool									FeedbackToggle;
		ImVec2									ViewportSafeSpace;
		ImVec2									CurrentViewportSize;
		ImVec2									CurrentLocalScreenPosition;
		ImVec2									CurrentViewportPosition;
		ImVec2									CurrentViewportRectMin;
		ImVec2									CurrentViewportRectSize;

		std::string								PromptFileImport(const char* Filter, const char* FileName);
		std::string								StatusToMessage(void);
		std::map<std::string, AssetViewModule>	TextureGroup;
		std::map<std::string, AssetViewModule>	SkyboxGroup;
		std::map<std::string, AssetViewModule>	ModelGroup;
		std::map<std::string, AssetViewModule>	ShaderGroup;

		std::string								CurrentWidget			= "Geometry";
		std::vector<std::string>				ToolsList				= {"None"};
		std::vector<std::string>				GroupFolders			= { "Textures","Skybox","Models","Shader" };
		std::vector<std::string>				WidgetTabs				= { "Geometry","Model","Debug","Audio" };
		std::vector<InterfaceAdditionMenu>		AdditionComponentMenu	= { InterfaceAdditionMenu("Add Mesh",ICON_FA_CUBE), InterfaceAdditionMenu("Add Cube",ICON_FA_CUBE), InterfaceAdditionMenu("Add Lights",ICON_FA_LIGHTBULB), InterfaceAdditionMenu("Add Player",ICON_FA_MAGIC),InterfaceAdditionMenu("Add Script",ICON_FA_SCROLL) };
		std::vector<ModelObjectProperties>		ObjectProperties;
		std::vector<ScriptContainer>			ScriptList;
		std::vector<CodeEditor>					NewTextEditor;
		std::map<int, int>						RedirectionList;

		glm::vec3								ObjectScale;	
		glm::vec3								ObjectTranslation;
		glm::quat								ObjectRotation;
		glm::vec3								Skew;
		glm::vec4								Perspective;

		EditorWorkspace							NewWorkspace;
		InterfaceSearchGroup					NewSearchGroup;
		GradientBlock							DefaultButtonHovered	= { IM_COL32(40, 40, 40, 255), IM_COL32(20, 20, 20, 255) };
		GradientBlock							DefaultButtonActive		= { IM_COL32(70, 70, 70, 255), IM_COL32(20, 20, 20, 255) };
		GradientBlock							DefaultButtonNormal		= { IM_COL32(60, 60, 60, 255), IM_COL32(20, 20, 20, 255) };
		GradientBlock							DefaultButtonUsed		= { IM_COL32(60, 120, 60, 255), IM_COL32(20, 20, 20, 255) };

		GradientBlock							GreenButtonHovered		= { IM_COL32(20, 20, 20, 255), IM_COL32(60, 120, 60, 255) };
		GradientBlock							GreenButtonNormal		= { IM_COL32(40, 40, 40, 255), IM_COL32(20, 20, 20, 255) };

		GradientBlock							BlueButtonHovered		= { IM_COL32(20, 20, 20, 255), IM_COL32(107, 179, 255, 255) };

		GradientBlock							GreenShinyButtonActive	= { IM_COL32(21, 112, 0, 255), IM_COL32(4, 23, 0, 255) };
		GradientBlock							GreenShinyButtonNormal	= { IM_COL32(40, 40, 40, 255), IM_COL32(20, 20, 20, 255) };
};

#endif