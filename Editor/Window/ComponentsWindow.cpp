#include "ComponentsWindow.h"

#include "../../Common/Console.h"
#include "../../Common/Commands.h"
#include "../Templates/CubeObjectTemplate.h"

ComponentsWindowRenderHandler::ComponentsWindowRenderHandler(void) 
{
	this->CurrentInterfaceValue.IsImageCompilerOpen = false;
	this->CurrentInterfaceValue.IsSettingsMenuOpen	= false;
	this->CurrentInterfaceValue.IsTextureMenuOpen	= false;
	this->CurrentInterfaceValue.LogoImageID			= 0;

	this->ObjectProperties		= {};
	this->ScriptList			= {};
	this->NewTextEditor			= {};
	this->RedirectionList		= {};

	this->CurrentStatus			= STATUS_READY;
	this->CurrentToolValue		= VALUE_POINTER;
	this->FeedbackToggle		= false;
	this->IsFileImporterOpen	= false;
	this->SelectedIndex			= -1;
	this->ScriptInstanceIndex	= 0;
	this->InstanceIndex			= 0;
}

std::string ComponentsWindowRenderHandler::StatusToMessage(void)
{
	switch (this->CurrentStatus)
	{
		case STATUS_ERROR:
			return "Errors Found!";
		case STATUS_WARNING:
			return "Warning Found Within Editor";
		case STATUS_READY:
			return "Ready";
		case STATUS_UNKNOWN:
			return "Current Engine Status Is Unknown";
		default:
			return "ComponentsWindowRenderHandler::StatusToMessage Failed To Retrieve Message.";
	}
}

std::string ComponentsWindowRenderHandler::PromptFileImport(const char* Filter, const char* FileName)
{
	OPENFILENAME OpenFileName;
	char File[260];
	HWND WindowHandle;
	HANDLE HandleFrame;

	ZeroMemory(&OpenFileName, sizeof(OpenFileName));
	OpenFileName.lStructSize = sizeof(OpenFileName);
	OpenFileName.lpstrFile = File;

	OpenFileName.lpstrFile[0] = '\0';
	OpenFileName.nMaxFile = sizeof(File);
	OpenFileName.lpstrFilter = Filter;
	OpenFileName.nFilterIndex = 1;
	OpenFileName.lpstrFileTitle = NULL;
	OpenFileName.nMaxFileTitle = 0;
	OpenFileName.lpstrInitialDir = NULL;
	OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&OpenFileName) == TRUE)
	{
		if (std::string(File).find(FileName) != std::string::npos)
		{
			return std::string(File);
		}
	}

	return "invalid";
}

std::string ComponentsWindowRenderHandler::ReturnEditMode(void)						{ return this->CurrentEditMode; }
EditorWorkspace& ComponentsWindowRenderHandler::ReturnEditorWorkspaceObject(void)	{ return this->NewWorkspace; }
ImVec2 ComponentsWindowRenderHandler::ReturnRegionContentAvail(void)				{ return this->CurrentViewportSize; }
ImVec2 ComponentsWindowRenderHandler::ReturnPositionViewport(void)					{ return this->CurrentViewportPosition; }
ImVec2 ComponentsWindowRenderHandler::ReturnViewportSafeSpace(void)					{ return this->ViewportSafeSpace; }
ImVec2 ComponentsWindowRenderHandler::ReturnViewportLocalScreenPosition(void)		{ return this->CurrentLocalScreenPosition; }
ImVec2 ComponentsWindowRenderHandler::ReturnViewportRectMin(void)					{ return this->CurrentViewportRectMin; }
ImVec2 ComponentsWindowRenderHandler::ReturnViewportRectSize(void)					{ return this->CurrentViewportRectSize; }

ScriptContainer ComponentsWindowRenderHandler::GetScriptFromName(const std::string& ItemName)
{
	// Checks if the script is active using the scripts name (The script should be stored by the name)

	for (int Index = 0; Index < this->ScriptList.size(); Index++)
	{
		if (this->ScriptList[Index].ScriptName == ItemName)
		{
			return this->ScriptList[Index];
		}
	}

	return {};
}

int ComponentsWindowRenderHandler::GetScriptIndexFromName(const std::string& ItemName)
{
	for (int Index = 0; Index < this->ScriptList.size(); Index++)
	{
		if (this->ScriptList[Index].ScriptName == ItemName)
		{
			return Index;
		}
	}

	return -1;
}

void ComponentsWindowRenderHandler::ApplyLocalTheme(void)
{
	ImGuiStyle& CurrentStyle	= ImGui::GetStyle();
	ImVec4* CurrentColours		= CurrentStyle.Colors;

	CurrentColours[ImGuiCol_WindowBg]				= ImVec4(0.10f, 0.105f, 0.10f, 1.00f);
	CurrentColours[ImGuiCol_ChildBg]				= ImVec4(0.12f, 0.125f, 0.12f, 1.00f);
	CurrentColours[ImGuiCol_PopupBg]				= ImVec4(0.14f, 0.145f, 0.14f, 1.00f);
	CurrentColours[ImGuiCol_DockingEmptyBg]			= ImVec4(0.08f, 0.085f, 0.08f, 1.00f);
	CurrentColours[ImGuiCol_DockingPreview]			= ImVec4(0.35f, 0.45f, 0.35f, 0.65f);

	CurrentColours[ImGuiCol_Border]					= ImVec4(0.20f, 0.23f, 0.20f, 0.60f);
	CurrentColours[ImGuiCol_BorderShadow]			= ImVec4(0, 0, 0, 0);

	CurrentColours[ImGuiCol_FrameBg]				= ImVec4(0.16f, 0.18f, 0.16f, 1.00f);
	CurrentColours[ImGuiCol_FrameBgHovered]			= ImVec4(0.20f, 0.25f, 0.20f, 1.00f);
	CurrentColours[ImGuiCol_FrameBgActive]			= ImVec4(0.0784f, 0.6275f, 0.0784f, 0.4706f);

	CurrentColours[ImGuiCol_TitleBg]				= ImVec4(0.08f, 0.09f, 0.08f, 1.00f);
	CurrentColours[ImGuiCol_TitleBgActive]			= ImVec4(0.0784f, 0.6275f, 0.0784f, 0.4706f);
	CurrentColours[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.06f, 0.07f, 0.06f, 1.00f);

	CurrentColours[ImGuiCol_MenuBarBg]				= ImVec4(0.12f, 0.13f, 0.12f, 1.00f);

	CurrentColours[ImGuiCol_Button]					= ImVec4(0.20f, 0.22f, 0.20f, 1.00f);
	CurrentColours[ImGuiCol_ButtonHovered]			= ImVec4(0.26f, 0.32f, 0.26f, 1.00f);
	CurrentColours[ImGuiCol_ButtonActive]			= ImVec4(0.0784f, 0.6275f, 0.0784f, 0.4706f);

	CurrentColours[ImGuiCol_Header]					= ImVec4(0.18f, 0.20f, 0.18f, 1.00f);
	CurrentColours[ImGuiCol_HeaderHovered]			= ImVec4(0.24f, 0.30f, 0.24f, 1.00f);
	CurrentColours[ImGuiCol_HeaderActive]			= ImVec4(0.0784f, 0.6275f, 0.0784f, 0.4706f);

	CurrentColours[ImGuiCol_Tab]					= ImVec4(0.14f, 0.15f, 0.14f, 1.00f);
	CurrentColours[ImGuiCol_TabHovered]				= ImVec4(0.24f, 0.30f, 0.24f, 1.00f);
	CurrentColours[ImGuiCol_TabActive]				= ImVec4(0.0784f, 0.6275f, 0.0784f, 0.4706f);
	CurrentColours[ImGuiCol_TabUnfocused]			= ImVec4(0.12f, 0.13f, 0.12f, 1.00f);
	CurrentColours[ImGuiCol_TabUnfocusedActive]		= ImVec4(0.16f, 0.18f, 0.16f, 1.00f);

	CurrentColours[ImGuiCol_ResizeGrip]				= ImVec4(0.30f, 0.35f, 0.30f, 0.30f);
	CurrentColours[ImGuiCol_ResizeGripHovered]		= ImVec4(0.40f, 0.45f, 0.40f, 0.80f);
	CurrentColours[ImGuiCol_ResizeGripActive]		= ImVec4(0.0784f, 0.6275f, 0.0784f, 0.4706f);

	CurrentColours[ImGuiCol_ScrollbarBg]			= ImVec4(0.08f, 0.09f, 0.08f, 1.00f);
	CurrentColours[ImGuiCol_ScrollbarGrab]			= ImVec4(0.20f, 0.22f, 0.20f, 1.00f);
	CurrentColours[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.28f, 0.32f, 0.28f, 1.00f);
	CurrentColours[ImGuiCol_ScrollbarGrabActive]	= ImVec4(0.0784f, 0.6275f, 0.0784f, 0.4706f);

	CurrentColours[ImGuiCol_CheckMark]				= ImVec4(0.55f, 0.65f, 0.50f, 1.00f);
	CurrentColours[ImGuiCol_SliderGrab]				= ImVec4(0.45f, 0.50f, 0.45f, 1.00f);
	CurrentColours[ImGuiCol_SliderGrabActive]		= ImVec4(0.0784f, 0.6275f, 0.0784f, 0.4706f);

	CurrentColours[ImGuiCol_Text]					= ImVec4(0.90f, 0.92f, 0.90f, 1.00f);
	CurrentColours[ImGuiCol_TextDisabled]			= ImVec4(0.50f, 0.55f, 0.50f, 1.00f);

	CurrentStyle.WindowRounding		= 6.0f;
	CurrentStyle.FrameRounding		= 4.0f;
	CurrentStyle.GrabRounding		= 4.0f;
	CurrentStyle.ScrollbarRounding	= 6.0f;
	CurrentStyle.TabRounding		= 4.0f;

	CurrentStyle.WindowBorderSize	= 1.0f;
	CurrentStyle.FrameBorderSize	= 0.0f;
}

void ComponentsWindowRenderHandler::LoadIconFonts(void)
{
	ImGuiIO& NewIO = ImGui::GetIO();
	NewIO.Fonts->AddFontDefault();

	float BaseFontSize = 20.0f;
	float IconFontSize = BaseFontSize;

	static const ImWchar IconRange[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

	ImFontConfig NewFontConfig;
	NewFontConfig.MergeMode			= true;
	NewFontConfig.PixelSnapH		= true;
	NewFontConfig.GlyphMinAdvanceX	= IconFontSize;
	NewFontConfig.GlyphOffset		= ImVec2(0.0f, 4.0f);

	NewIO.Fonts->AddFontFromFileTTF("../Fusion/Fonts/fa-regular-400.ttf", IconFontSize, &NewFontConfig, IconRange);
	NewIO.Fonts->AddFontFromFileTTF("../Fusion/Fonts/fa-solid-900.ttf", IconFontSize, &NewFontConfig, IconRange);
}

void ComponentsWindowRenderHandler::LoadDescriptiveImage(void)
{
	this->CurrentInterfaceValue.LogoImageID		= ImageUtility::LoadTextureIntoMemory(ImageUtility::ReadImageBuffer("../Fusion/Images/Logo.imgbuf"));
	this->UnknownImageID						= ImageUtility::LoadTextureIntoMemory(ImageUtility::ReadImageBuffer("../Fusion/Images/Camera.imgbuf"));
	this->SpawnID								= ImageUtility::LoadTextureIntoMemory(ImageUtility::ReadImageBuffer("../Fusion/Images/Spawn.imgbuf"));
	this->ThumbnailID							= ImageUtility::LoadTextureIntoMemory(ImageUtility::ReadImageBuffer("../Fusion/Images/Thumbnail.imgbuf"));
	this->CodeID								= ImageUtility::LoadTextureIntoMemory(ImageUtility::ReadImageBuffer("../Fusion/Images/Code.imgbuf"));
}

void ComponentsWindowRenderHandler::LoadComponentWindow(void)
{
	this->ApplyLocalTheme();
	this->LoadIconFonts();
	this->LoadDescriptiveImage();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	this->CurrentStatus			= STATUS_READY;
}

void ComponentsWindowRenderHandler::SubmitParserDataToEditor(const std::vector<ParserMap>& ParserContents)
{
	for (int Index = 0; Index < ParserContents.size(); Index++)
	{
		// This might be a very crap way of sorting the files based on the contents but this should do
		// We could add verification to these so if we cant read a file, just ignore it or add a question mark.
		AssetViewModule NewModule;
		ImageBufferFormatLocal NewLocalFormat;
		std::string CurrentFileName = ParserContents[Index].LabelName;
		bool ImageAvailable = (CurrentFileName.find("Textures/") != std::string::npos || CurrentFileName.find("Skybox/") != std::string::npos);

		if (ImageAvailable)
		{
			// We would have to apply to a local format for it to work.
			NewLocalFormat.TextureWidth		= ParserContents[Index].ImageFormat.TextureWidth;
			NewLocalFormat.TextureHeight	= ParserContents[Index].ImageFormat.TextureHeight;
			NewLocalFormat.TextureData		= ParserContents[Index].ImageFormat.TextureData;
			NewLocalFormat.TextureChannel	= ParserContents[Index].ImageFormat.TextureChannel;
		}

		NewModule.AssetImage = (ImageAvailable ? ImageUtility::LoadTextureIntoMemory(NewLocalFormat) : (CurrentFileName.find("Shader/") != std::string::npos ? this->CodeID : this->UnknownImageID));
		NewModule.AssetUnicodeImage = "Asset";

		if (CurrentFileName.find("Textures/") != std::string::npos)
		{ 
			this->TextureGroup[CurrentFileName] = NewModule;
		}
		if (CurrentFileName.find("Models/") != std::string::npos)
		{
			this->ModelGroup[CurrentFileName] = NewModule;
		}
		if (CurrentFileName.find("Skybox/") != std::string::npos)
		{
			this->SkyboxGroup[CurrentFileName] = NewModule;
		}
		if (CurrentFileName.find("Shader/") != std::string::npos)
		{
			ScriptInfomation NewInfomation;

			NewInfomation.FileName		= ParserContents[Index].LabelName;
			NewInfomation.FileContents	= ParserContents[Index].DataContents;

			this->ShaderGroup[CurrentFileName]		= NewModule;
			this->ScriptContents[CurrentFileName]	= NewInfomation;
		}
	}
}

void ComponentsWindowRenderHandler::DrawToolbarWidget(InterfaceValues& CurrentValues, GLFWwindow* EngineWindow, const glm::mat4& Projection, const glm::mat4& View, const glm::vec3& Position)
{
	const float ButtonWidth = 30.0f;
	const float Spacing		= 5.0f;
	bool IsEditorFullScreen = std::get<bool>(Commands::GetFromRegister("editorfullscreen"));

	static bool Dragging = false;
	static double OffsetX, OffsetY = 0.0;
	double MouseX, MouseY;

	// @TODO: Change the window fullscreen mode system, where if the user enters a new monitor region, it should adhere and resize to that 
	// window fullscreen (Could have some problems with resolution) 

	if (glfwGetMouseButton(EngineWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (glfwGetWindowAttrib(EngineWindow, GLFW_MAXIMIZED))
		{
			double MouseX, MouseY;
			glfwGetCursorPos(EngineWindow, &MouseX, &MouseY);

			// We could've used the GLFW_RELEASE command to reset dragging, however its unneeded extra work.
			if (!Dragging)
			{
				Dragging = true;
				int CurrentWindowX, CurrentWindowY;

				// Gather the current window position and the offsets.
				glfwGetWindowPos(EngineWindow, &CurrentWindowX, &CurrentWindowY);
				OffsetX = MouseX;
				OffsetY = MouseY;
			}


			// We would have used an adjustment system to gather the cursor pos and offset the window based upon the window position
			double GlobalX, GlobalY;
			POINT WindowPoint;
			glfwGetCursorPos(EngineWindow, &GlobalX, &GlobalY);
			GetCursorPos(&WindowPoint);

			glfwSetWindowPos(EngineWindow,
				WindowPoint.x - (int)OffsetX,
				WindowPoint.y - (int)OffsetY);
		}
	}
	else
	{
		Dragging = false;
	}

	if (ImGui::BeginMainMenuBar())
	{
		ImGuiUtilites::GradientRect(
			ImGui::GetWindowPos(),
			ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y),
			{ IM_COL32(40, 40, 40, 255), IM_COL32(10, 10, 10, 255) }
		);

		ImGui::Image((ImTextureRef)(CurrentValues.LogoImageID),ImVec2(25.0f,25.0f),ImVec2(0,1),ImVec2(1,0));
		ImGui::SameLine();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New","Ctrl+N"))					{ }
			ImGui::Separator();

			if (ImGui::MenuItem("Open", "Ctrl+O"))					{ }
			ImGui::Separator();

			if (ImGui::MenuItem("Save","Ctrl+S"))					{ }
			ImGui::Separator();

			if (ImGui::MenuItem("Save As","Ctrl+Shift+S"))			{ }
			ImGui::Separator();

			if (ImGui::MenuItem("Import World", "Ctrl+Shift+I"))	{ }
			ImGui::Separator();

			if (ImGui::MenuItem("Export World", "Ctrl+Shift+X"))	{ }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
			ImGui::Separator();

			if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Texture Editor"))
		{
			if (ImGui::MenuItem((CurrentValues.IsImageCompilerOpen ? "Close Image Editor" : "Open Image Editor"), "Ctrl+T"))			{ }
			ImGui::Separator();
		
			if (ImGui::MenuItem((CurrentValues.IsTextureMenuOpen ? "Close Texture Editor" : "Open Texture Editor"), "Ctrl+U"))			{ }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Fusion Editor Settings", "Ctrl+A")) { CurrentValues.IsSettingsMenuOpen = !CurrentValues.IsSettingsMenuOpen; }
			ImGui::EndMenu();
		}

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (ButtonWidth * 3) - 1500);
		for (int Index = 0; Index < this->WidgetTabs.size(); Index++)
		{
			if (ImGuiUtilites::ButtonGradient(this->WidgetTabs[Index].c_str(), ImVec2(ImGui::CalcTextSize(("  "+this->WidgetTabs[Index]+"  ").c_str()).x, ImGui::GetFrameHeight()), ImVec4(20.0f, 20.0f, 20.0f, 120.0f), (this->CurrentWidget == this->WidgetTabs[Index] ? this->GreenButtonHovered : this->GreenButtonNormal), (this->CurrentWidget == this->WidgetTabs[Index] ? this->GreenButtonHovered : this->GreenButtonNormal), (this->CurrentWidget == this->WidgetTabs[Index] ? this->GreenButtonHovered : this->GreenButtonNormal)))
			{ 
				this->CurrentWidget = this->WidgetTabs[Index];
			}
			ImGui::SameLine(0, Spacing);
		}

#ifdef ALLOW_FEEDBACK
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (ButtonWidth * 3) - 400);
		if (ImGuiUtilites::ButtonGradient("Got Feedback?", ImVec2(ImGui::CalcTextSize(" Got Feedback? ").x, ImGui::GetFrameHeight()), ImVec4(20.0f, 20.0f, 20.0f, 120.0f), this->BlueButtonHovered, this->BlueButtonHovered, this->BlueButtonHovered))
		{
			this->FeedbackToggle = !this->FeedbackToggle;
		}
#endif

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (ButtonWidth * 3) - (Spacing * 3));

		if (ImGuiUtilites::ButtonGradient("_",ImVec2(ButtonWidth, ImGui::GetFrameHeight()), ImVec4(0.0f, 0.0f, 0.0f, 120.0f), this->DefaultButtonHovered,this->DefaultButtonActive,this->DefaultButtonNormal)) { glfwIconifyWindow(EngineWindow); }
		ImGui::SameLine(0, Spacing);

		if (ImGuiUtilites::ButtonGradient((glfwGetWindowAttrib(EngineWindow, GLFW_MAXIMIZED) ? "[]" : "[ ]"), ImVec2(ButtonWidth, ImGui::GetFrameHeight()), ImVec4(0.0f, 0.0f, 0.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal))
		{ 
			if (glfwGetWindowAttrib(EngineWindow, GLFW_MAXIMIZED))	{ glfwRestoreWindow(EngineWindow); Commands::SetFromRegister("editorfullscreen", false); }
			else													{ glfwMaximizeWindow(EngineWindow); Commands::SetFromRegister("editorfullscreen", true); }
		}
		ImGui::SameLine(0, Spacing);

		if (ImGuiUtilites::ButtonGradient("X", ImVec2(ButtonWidth, ImGui::GetFrameHeight()), ImVec4(0.0f, 0.0f, 0.0f, 120.0f),{ IM_COL32(200, 0, 0, 255), IM_COL32(30, 0, 0, 255) }, { IM_COL32(255, 10, 10, 255), IM_COL32(40, 0, 0, 255) }, { IM_COL32(60, 60, 60, 255), IM_COL32(20, 20, 20, 255) }))
		{ 
			glfwTerminate();
			exit(0);
		}

		ImGui::EndMainMenuBar();
	}
}

void ComponentsWindowRenderHandler::FeedbackMenuSystem(void)
{
#ifdef ALLOW_FEEDBACK
	if (!this->FeedbackToggle) { return; }
	ImGui::Begin("Feedback Menu");

	// @TODO: do a feedback menu here (could link to gmail or smth)
	ImGui::Text("No feedback menu yet :(");

	ImGui::End();
#endif
}


void ComponentsWindowRenderHandler::SpawnCubeInstance(Camera* EngineCamera,Shader* EngineShader)
{
	// Moved the creation instance to a seperate function for multiple instance creations within seperate windows
	// reduces redundancy.
	ModelObjectProperties NewObject;

	// + glm::normalize(EngineCamera->Orientation) * SpawnDistance;
	float SpawnDistance		= 50.0f;
	glm::vec3 Translation	= EngineCamera->ReturnCameraPosition();
	glm::quat Rotation		= glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 Scale			= glm::vec3(10.0f, 10.0f, 10.0f);

	std::cout << "TRANS: " << Translation.x << "," << Translation.y << "," << Translation.z << "\n";
	
	NewObject.Translate		= Translation;
	NewObject.Rotation		= Rotation;
	NewObject.Scale			= Scale;

	NewObject.Index			= this->InstanceIndex; // @EDIT - We could use this as an index selection system using SelectedIndex perhaps?
	NewObject.LabelName		= "NewObject" + (this->InstanceIndex >= 1 ? " (" + std::to_string(this->InstanceIndex) + ")" : "");
	NewObject.LabelPrefix	= ICON_FA_CUBE; // @TODO - We could edit this and add different predetermined objects (spheres,cubes,cones ect.)
	
	NewObject.ObjectDataType = DATATYPE_BLOCK; // Used to determine the type of object we are seeing.

	this->ObjectProperties.push_back(NewObject);
	this->CurrentGridEditor.SubmitWorldGeometry(EditorUtilites::GroupVector3(CubeVertices), CubeIndices);
	this->NewWorkspace.CreateGeometryData(EngineShader, CubeVertices, CubeIndices, Translation, Rotation, Scale, "", SHAPE_BLOCK, -1);

	this->InstanceIndex++;
}

void ComponentsWindowRenderHandler::SpawnScriptInstance(void)
{
	// This would be the place where we would create a new script, alongside initilising the script contents. We should use an interpreter to 
	// help read and "recompile" the script into bytecode.

	if (this->NewTextEditor.size() >= MAX_TEXT_EDITORS) { Console::SysPrint("Cannot spawn any more code editors. Consider closing text editors to fix this.", 2); return; } // Try not to lag the editor.

	ScriptContainer NewContainer;
	CodeEditor NewCodeWidget;
	ModelObjectProperties NewObject; // We could rename the thing?

	std::string NewScriptName	= "NewScript" + (this->ScriptInstanceIndex >= 1 ? " (" + std::to_string(this->ScriptInstanceIndex) + ")" : "") + ".fs";

	NewContainer.ScriptContents = "";
	NewContainer.ScriptName		= NewScriptName;
	NewContainer.ScriptIndex	= this->ScriptInstanceIndex;

	NewObject.Translate		= glm::vec3(0.0f, 0.0f, 0.0f);
	NewObject.Rotation		= glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
	NewObject.Scale			= glm::vec3(0.0f, 0.0f, 0.0f);

	NewObject.Index			= this->ScriptInstanceIndex;
	NewObject.LabelName		= NewScriptName;
	NewObject.LabelPrefix	= ICON_FA_SCROLL;

	NewObject.ObjectDataType = DATATYPE_SCRIPT; // A new addition to the object data type value.
	NewCodeWidget.SetupCodeEditor(NewScriptName,""); // Start the code syntax system.

	this->NewTextEditor.push_back(NewCodeWidget); // This should be the same as the ScriptList vector.
	this->ScriptList.push_back(NewContainer);
	this->ObjectProperties.push_back(NewObject);

	this->RedirectionList[this->ScriptInstanceIndex] = this->ScriptList.size() - 1;
	this->ScriptInstanceIndex++; // We are changing the index system to a seperate script index to keep track.
}

void ComponentsWindowRenderHandler::ModeButton(const char* Icon, InterfaceTooltipValues Mode, const ImVec4& ActiveColor)
{
	bool IsActive = (this->CurrentToolValue == Mode);
	if (ImGuiUtilites::ButtonGradient(Icon, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), (IsActive ? this->DefaultButtonUsed : this->DefaultButtonHovered), this->DefaultButtonActive, (IsActive ? this->DefaultButtonUsed : this->DefaultButtonNormal)))
	{ this->CurrentToolValue = Mode; }
}

void ComponentsWindowRenderHandler::DrawGeometryToolpack(Camera* EngineCamera, Shader* EngineShader)
{
	static const char* SelectionItems[] = { "Instance Mode","Vertex Edit Mode" };
	static int SelectionMode			= 0;

	this->ModeButton(ICON_FA_MOUSE_POINTER, VALUE_POINTER, ImVec4(0.10f, 0.65f, 0.35f, 1.00f));
	ImGui::SameLine();
	this->ModeButton(ICON_FA_ARROWS_ALT, VALUE_TRANSLATE, ImVec4(0.10f, 0.65f, 0.35f, 1.00f));
	ImGui::SameLine();
	this->ModeButton(ICON_FA_SYNC, VALUE_ROTATE, ImVec4(0.10f, 0.65f, 0.35f, 1.00f));
	ImGui::SameLine();
	this->ModeButton(ICON_FA_EXPAND, VALUE_SCALE, ImVec4(0.10f, 0.65f, 0.35f, 1.00f));

	ImGui::SetCursorPos(ImVec2(200.0f + (47.0f * 0), 5.0f));
	if (ImGuiUtilites::ButtonGradient(ICON_FA_CUBE, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal))
	{
		this->SpawnCubeInstance(EngineCamera, EngineShader);
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Spawn Cube");
		ImGui::EndTooltip();
	}

	ImGui::SetCursorPos(ImVec2(200.0f + (47.0f * 1), 5.0f));
	if (ImGuiUtilites::ButtonGradient(ICON_FA_LIGHTBULB, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Spawn Light");
		ImGui::EndTooltip();
	}

	ImGui::SetCursorPos(ImVec2(200.0f + (47.0f * 2), 5.0f));
	if (ImGuiUtilites::ButtonGradient(ICON_FA_CODE, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Compile Map");
		ImGui::EndTooltip();
	}

	ImGui::SetCursorPos(ImVec2(200.0f + (47.0f * 3), 5.0f));
	if (ImGuiUtilites::ButtonGradient(ICON_FA_TRASH, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Delete Object");
		ImGui::EndTooltip();
	}

	ImGui::SetCursorPos(ImVec2(200.0f + (47.0f * 4), 5.0f));
	if (ImGuiUtilites::ButtonGradient(ICON_FA_MAGIC, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Spawn PlayerSpawn");
		ImGui::EndTooltip();
	}

	ImGui::SetCursorPos(ImVec2(200.0f + (47.0f * 5), 5.0f));
	if (ImGuiUtilites::ButtonGradient(ICON_FA_ATOM, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Open Component Menu");
		ImGui::EndTooltip();
	}

	ImGui::PushItemWidth(100);
	ImGui::SetCursorPos(ImVec2(485.0f, 5.0f));

	ImGui::InputFloat(" ", &Console::IncrementValue[0], 0.5f, 100.0f);
	ImGui::SetCursorPos(ImVec2(495.0f, 30.0f));
	ImGui::Text("Increment X");

	ImGui::SetCursorPos(ImVec2(595.0f, 5.0f));

	ImGui::InputFloat("  ", &Console::IncrementValue[1], 0.5f, 100.0f);
	ImGui::SetCursorPos(ImVec2(605.0f, 30.0f));
	ImGui::Text("Increment Y");

	ImGui::SetCursorPos(ImVec2(705.0f, 5.0f));

	ImGui::InputFloat("   ", &Console::IncrementValue[2], 0.5f, 100.0f);
	ImGui::SetCursorPos(ImVec2(715.0f, 30.0f));
	ImGui::Text("Increment Z");

	ImGui::SetCursorPos(ImVec2(825.0f, 3.0f));
	ImGui::Checkbox("Toggle Grid Edit", &Console::GridViewEnable);

	ImGui::SetCursorPos(ImVec2(825.0f, 25.0f));
	ImGui::Checkbox("Toggle Collisions", &Console::EnableCollision);

	ImGui::SetCursorPos(ImVec2(990.0f, 3.0f));
	ImGui::SetNextItemWidth(200.0f);
	ImGui::Combo("Geometry Mode",&SelectionMode, SelectionItems,IM_ARRAYSIZE(SelectionItems));
	this->CurrentEditMode = SelectionItems[SelectionMode]; // Update the selection mode for geometry editing.

	ImGui::PopItemWidth();
}

void ComponentsWindowRenderHandler::DrawModelToolpack(Camera* EngineCamera, Shader* EngineShader) 
{
	if (ImGuiUtilites::ButtonGradient(ICON_FA_CUBE, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) { this->SpawnCubeInstance(EngineCamera, EngineShader); }
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Spawn Cube");
		ImGui::EndTooltip();
	}

	ImGui::SameLine();
	if (ImGuiUtilites::ButtonGradient(ICON_FA_FILE_IMPORT, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) { this->IsFileImporterOpen = !this->IsFileImporterOpen; }
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Import Meshes");
		ImGui::EndTooltip();
	}
}

void ComponentsWindowRenderHandler::DrawDebugToolpack(void) 
{
	if (ImGuiUtilites::ButtonGradient(ICON_FA_FAST_FORWARD, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Fast World Compile");
		ImGui::EndTooltip();
	}

	ImGui::SameLine();
	if (ImGuiUtilites::ButtonGradient(ICON_FA_HOURGLASS_START, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Full World Compile");
		ImGui::EndTooltip();
	}

	ImGui::SameLine();
	if (ImGuiUtilites::ButtonGradient(ICON_FA_DRAW_POLYGON, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Visibility World Compile");
		ImGui::EndTooltip();
	}
}

void ComponentsWindowRenderHandler::DrawAudioToolpack(void) 
{
	if (ImGuiUtilites::ButtonGradient(ICON_FA_LIGHTBULB, ImVec2(40.0f, 40.0f), ImVec4(20.0f, 160.0f, 20.0f, 120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) {}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Spawn Light");
		ImGui::EndTooltip();
	}
}

void ComponentsWindowRenderHandler::ShowEditGizmo(Camera* EngineCamera, int ViewportWidth, int ViewportHeight)
{
	// @TODO - Perhaps make some parts persistant, this is especially when updating resolution and changing parts of the resolution
	// would make it more efficent as it changes per user frame update than every frame.
	
	// This is moved from EditorMain.cpp -> ComponentsWindow.cpp
	ImGuizmo::BeginFrame();
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

	ImVec2 CurrentMousePosition = ImGui::GetMousePos();
		
	ImVec2 ViewportPos				= ImGui::GetWindowPos();
	ImVec2 ViewportMinimumRegion	= ImGui::GetWindowContentRegionMin();
	ImVec2 ViewportMaximumRegion	= ImGui::GetWindowContentRegionMax();
	this->ViewportSafeSpace			= ImVec2(ViewportMaximumRegion.x - ViewportMinimumRegion.x, ViewportMaximumRegion.y - ViewportMinimumRegion.y); //Capture this per frame for any updates

	ImGuizmo::SetRect(
		ViewportPos.x + ViewportMinimumRegion.x,
		ViewportPos.y + ViewportMinimumRegion.y,
		this->ViewportSafeSpace.x,
		this->ViewportSafeSpace.y
	);

	if (this->SelectedIndex != -1 && (this->CurrentToolValue != VALUE_NULL && this->CurrentToolValue != VALUE_POINTER))
	{
		ImGuizmo::Style& NewStyle = ImGuizmo::GetStyle();

		NewStyle.Colors[ImGuizmo::DIRECTION_X] = ImVec4(0.666f, 0.000f, 0.000f, 1.000f);
		NewStyle.Colors[ImGuizmo::DIRECTION_Y] = ImVec4(0.000f, 0.666f, 0.000f, 1.000f);
		NewStyle.Colors[ImGuizmo::DIRECTION_Z] = ImVec4(0.000f, 0.000f, 0.666f, 1.000f);

		glm::mat4 ModelMatrix = this->NewWorkspace.GetPositionMatrix(this->SelectedIndex) *
			this->NewWorkspace.GetRotationMatrix(this->SelectedIndex) *
			this->NewWorkspace.GetScaleMatrix(this->SelectedIndex);

		ImGuizmo::Manipulate(
			glm::value_ptr(EngineCamera->ReturnViewMatrix()),
			glm::value_ptr(EngineCamera->ReturnProjectionMatrix()),
			(
				this->CurrentToolValue == VALUE_TRANSLATE ? ImGuizmo::TRANSLATE : (this->CurrentToolValue == VALUE_ROTATE ? ImGuizmo::ROTATE : (this->CurrentToolValue == VALUE_SCALE ? ImGuizmo::SCALE : ImGuizmo::SCALE))
			),
			ImGuizmo::LOCAL,
			glm::value_ptr(ModelMatrix),
			nullptr,
			Console::IncrementValue
		);

		if (ImGuizmo::IsUsing())
		{
			std::cout << "Using" << "\n";
			// Change the properties, Skew and perspective
			// @FIX: We dont need skew or perspective. Perhaps use a float store or something? (Just get rid of it)
			

			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(ModelMatrix), &this->ObjectTranslation.x, &this->ObjectRotation.x, &this->ObjectScale.x); // This was and should not produce garbage values if one tool is being used in opposition to the other.#
			this->NewWorkspace.SetPosition(this->SelectedIndex, this->ObjectTranslation);

			//if (glm::decompose(ModelMatrix, this->ObjectScale, this->ObjectRotation, this->ObjectTranslation, Skew, Perspective))
			//{
			//	//if (this->ObjectScale.x <= 0.1 || this->ObjectScale.y <= 0.1 || this->ObjectScale.z <= 0.1) { return; }

			//	std::cout << "LOL" << this->ObjectTranslation.x << "," << this->ObjectTranslation.y << "," << this->ObjectTranslation.z << "\n";

			//	this->NewWorkspace.SetPosition(this->SelectedIndex, this->ObjectTranslation); // Moved the values of Translation, Rotation and Scale,
			//	this->NewWorkspace.SetRotation(this->SelectedIndex, this->ObjectRotation);
			//	this->NewWorkspace.SetScale(this->SelectedIndex, this->ObjectScale);
			//}
		}
	}
}

void ComponentsWindowRenderHandler::DrawMainToolbar(Camera* EngineCamera, Shader* EngineShader)
{
	// @FIX: This reduces the mirror effect from borders within IMGUI windows. Perhaps there is a unknown border control that produces this?
	// Maybe in the future, we could fix this by clearing once, both for framebuffer and the IMGUI rendering.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	static ImGuiWindowFlags CurrentFlags	= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
	static ImGuiViewport* CurrentViewport	= ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(CurrentViewport->Pos.x, CurrentViewport->Pos.y + Console::MenuBarHeight + MENU_OFFSET));
	ImGui::SetNextWindowSize(ImVec2(CurrentViewport->Size.x, MENU_SIZE));
	ImGui::SetNextWindowViewport(CurrentViewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Toolbar", NULL, CurrentFlags);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7.0f, 8.0f));

	ImGuiUtilites::GradientRect(
		ImGui::GetWindowPos(),
		ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y),
		{ IM_COL32(50, 50, 50, 255), IM_COL32(20, 20, 20, 255) }
	);

	ImGui::SetCursorPos(ImVec2(8.0f, 5.0f));
	if (this->CurrentWidget == WidgetTabs[0])			{ this->DrawGeometryToolpack(EngineCamera, EngineShader); }
	else if (this->CurrentWidget == WidgetTabs[1])		{ this->DrawModelToolpack(EngineCamera, EngineShader); }
	else if (this->CurrentWidget == WidgetTabs[2])		{ this->DrawDebugToolpack(); }
	else if (this->CurrentWidget == WidgetTabs[3])		{ this->DrawAudioToolpack(); }

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::End();
}

void ComponentsWindowRenderHandler::DrawAddComponentsMenu(Camera* EngineCamera, Shader* EngineShader)
{
	std::string Label;
	int Count;
	int Columns;
	float AvailX;

	const float ThumbnailSize	= 128.0f;
	const float Padding			= 16.0f;
	const float CellSize		= ThumbnailSize + Padding;

	ImGuiUtilites::BeginGradient("Component Menu", this->DefaultButtonHovered);

	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_HorizontalScrollbar))
	{
		Count	= 0;
		AvailX	= ImGui::GetContentRegionAvail().x;
		Columns = static_cast<int>(AvailX / CellSize);

		if (Columns < 1) { Columns = 1; }

		for (int Index = 0; Index < this->AdditionComponentMenu.size(); Index++)
		{
			Label = this->AdditionComponentMenu[Index].Icons + " " + this->AdditionComponentMenu[Index].Name;
			
			if (ImGuiUtilites::ButtonGradient(Label.c_str(), ImVec2(120.0f, 30.0f), ImVec4(0.0f,0.0f,0.0f,120.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) 
			{
				// @TODO: so far these are hardcoded, perhaps we could use a function pointer (void*) to create a new function storage? 

				if (this->AdditionComponentMenu[Index].Icons == ICON_FA_CUBE)	{ this->SpawnCubeInstance(EngineCamera,EngineShader); }
				if (this->AdditionComponentMenu[Index].Icons == ICON_FA_SCROLL)	{ this->SpawnScriptInstance(); }
			}

			if (++Count % Columns != 0) { ImGui::SameLine(); }
		}
	}
	ImGui::EndChild();
	ImGui::End();
}

void ComponentsWindowRenderHandler::DrawStatusBar(void)
{
	const float Height				= 24.0f;
	ImGuiViewport* CurrentViewport	= ImGui::GetMainViewport();
	ImGuiWindowFlags CurrentFlags	= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking;

	ImGui::SetNextWindowPos(ImVec2(CurrentViewport->Pos.x, CurrentViewport->Pos.y + CurrentViewport->Size.y - Height));
	ImGui::SetNextWindowSize(ImVec2(CurrentViewport->Size.x, Height));
	ImGui::SetNextWindowViewport(CurrentViewport->ID);

	ImGuiUtilites::BeginGradient("##StatusBar", this->DefaultButtonHovered, nullptr, CurrentFlags);
	ImGui::Text(this->StatusToMessage().c_str());
	ImGui::SameLine();
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	ImGui::SameLine();
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::End();
}

void ComponentsWindowRenderHandler::DrawFileImporter(void)
{
	if (this->IsFileImporterOpen)
	{
		std::string ImportDestination = "invalid";
		ImGui::SetNextWindowSize(ImVec2(535.0f, 380.0f));
		ImGui::Begin("Mesh File Importer");

		ImGui::Text("Please choose what mesh file you would like to import.");
		ImGui::Separator();		

		if (ImGuiUtilites::ButtonGradient("\xef\x95\xb4  Import .OBJ", ImVec2(160.0f, 25.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) { ImportDestination = this->PromptFileImport("Wavefront OBJ\0*.obj\0", ".obj"); }	
		if (ImGuiUtilites::ButtonGradient("\xef\x95\xb4  Import .GLTF", ImVec2(160.0f, 25.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) { ImportDestination = this->PromptFileImport("Khronos GLTF\0*.gltf\0", ".gltf"); }
		if (ImGuiUtilites::ButtonGradient("\xef\x95\xb4  Import .FBX", ImVec2(160.0f, 25.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) { ImportDestination = this->PromptFileImport("Autodesk FBX\0*.fbx\0", ".fbx"); }
		if (ImGuiUtilites::ButtonGradient("\xef\x95\xb4  Import .FMD", ImVec2(160.0f, 25.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), this->DefaultButtonHovered, this->DefaultButtonActive, this->DefaultButtonNormal)) { ImportDestination = this->PromptFileImport("Fusion Model Data\0*.fmd\0", ".fmd"); }

		//We can draw the image to the side as a preview. Otherwise we just generate a default thumbnail.
		ImGui::SetCursorPos(ImVec2(200.0f, 50.0f));
		ImGui::Image((ImTextureRef)(this->UnknownImageID), ImVec2(300.0f, 300.0f));

		ImGui::End();

		if (ImportDestination != "invalid") // Just avoid any return values that doesnt contain the file that we want (basically "invalid")
		{

		}
	}
}

void ComponentsWindowRenderHandler::OpenScriptsInstance(const std::string& ItemName)
{
	
	if (this->GetScriptIndexFromName(ItemName) != -1)
	{
		bool ScriptActiveQuery = this->GetScriptFromName(ItemName).ScriptActive;

		if (ScriptActiveQuery) { Console::SysPrint("Script is already active", 1); }
		else
		{
			this->ScriptList[this->GetScriptIndexFromName(ItemName)].ScriptActive = true;
			return; //Dont need to create another script instance. We already have one.
		}
	}
	else
	{
		ScriptContainer NewScriptContainer;
		CodeEditor NewCodeWidget;
		ModelObjectProperties NewObject;

		NewScriptContainer.ScriptName = ItemName;
		NewScriptContainer.ScriptContents = this->ScriptContents[ItemName].FileContents;
		NewScriptContainer.ScriptIndex = this->ScriptInstanceIndex;

		NewCodeWidget.SetupCodeEditor(ItemName,this->ScriptContents[ItemName].FileContents);

		NewObject.Translate = glm::vec3(0.0f, 0.0f, 0.0f);
		NewObject.Rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
		NewObject.Scale = glm::vec3(0.0f, 0.0f, 0.0f);

		NewObject.Index = this->ScriptInstanceIndex;
		NewObject.LabelName = this->ScriptContents[ItemName].FileName;
		NewObject.LabelPrefix = ICON_FA_SCROLL;

		this->ObjectProperties.push_back(NewObject);
		this->NewTextEditor.push_back(NewCodeWidget);
		this->ScriptList.push_back(NewScriptContainer);

		this->RedirectionList[this->ScriptInstanceIndex] = this->ScriptList.size() - 1;
		this->ScriptInstanceIndex++;
	}
}

void ComponentsWindowRenderHandler::ManageAssetBeingPicked(const std::string& FolderSelectedName,const std::string& ItemName)
{
	if (FolderSelectedName == this->GroupFolders[0])		{} //Texture
	else if (FolderSelectedName == this->GroupFolders[1])	{} //Skybox
	else if (FolderSelectedName == this->GroupFolders[2])	{} //Model
	else if (FolderSelectedName == this->GroupFolders[3])	{ this->OpenScriptsInstance(ItemName); } //Shader
	else													{ Console::SysPrint("ComponentsWindowRenderHandler::ManageAssetBeingPicked got invalid folder name.", 3); }
}

void ComponentsWindowRenderHandler::DrawTextureViewer(void)
{
	const float TextPadding					= 20.0f;
	int CurrentIndex						= 0;
	static std::string FolderSelectedName	= "Textures";

	ImGuiUtilites::BeginGradient("Asset Viewer", this->DefaultButtonHovered);

	for (int Index = 0; Index < this->GroupFolders.size(); Index++)
	{
		// We should select the active option that we have to display the user what section they are in.
		if (ImGuiUtilites::ButtonGradient(this->GroupFolders[Index].c_str(),ImVec2(ImGui::CalcTextSize(this->GroupFolders[Index].c_str()).x + TextPadding, 20.0f), ImVec4(0.0f, 0.0f, 0.0f, 120.0f), 
			(FolderSelectedName == this->GroupFolders[Index] ? this->GreenShinyButtonActive : this->GreenShinyButtonNormal),
			(FolderSelectedName == this->GroupFolders[Index] ? this->GreenShinyButtonActive : this->GreenShinyButtonNormal),
			(FolderSelectedName == this->GroupFolders[Index] ? this->GreenShinyButtonActive : this->GreenShinyButtonNormal))
		) // Weird naming sceme but essentially would glow the button based upon if its selected.
		{ FolderSelectedName = this->GroupFolders[Index]; }
		ImGui::SameLine();
	}

	ImGui::SameLine();

	ImGui::PushItemWidth(145);
	ImGui::InputText("Search", &this->NewSearchGroup.SearchInputText);
	ImGui::PopItemWidth();

	ImGui::NewLine(); // Why do we need newline if we are setting the cursor position?
	ImGui::SetCursorPos(ImVec2(10.0f,55.0f)); 

	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_HorizontalScrollbar))
	{
		std::string NewString;
		static std::map<std::string, AssetViewModule> ObjectMapIndex = {};

		// @TODO: Perhaps make this constant? Will this affect anything?
		int Count				= 0;
		float ThumbnailSize		= 128.0f;
		float Padding			= 16.0f;
		float CellSize			= ThumbnailSize + Padding;

		float AvailX			= ImGui::GetContentRegionAvail().x;
		int Columns				= static_cast<int>(AvailX / CellSize);

		if (FolderSelectedName != "")
		{
			//Basic system, when selecting the actual thing we use, we just change the data group (They should be within the same "format")
			if (FolderSelectedName == this->GroupFolders[0])		{ ObjectMapIndex = this->TextureGroup; }
			else if (FolderSelectedName == this->GroupFolders[1])	{ ObjectMapIndex = this->SkyboxGroup; }
			else if (FolderSelectedName == this->GroupFolders[2])	{ ObjectMapIndex = this->ModelGroup; }
			else if (FolderSelectedName == this->GroupFolders[3])	{ ObjectMapIndex = this->ShaderGroup; }
		}

		if (Columns < 1) { Columns = 1; }

		for (std::map<std::string, AssetViewModule>::iterator NewIterator = ObjectMapIndex.begin(); NewIterator != ObjectMapIndex.end(); NewIterator++)
		{
			if (this->NewSearchGroup.SearchInputText.size() != 0)
			{
				//Store them as new objects to be compared so we dont affect the actual name.
				std::string ItemName	= NewIterator->first;
				std::string ToBeFound	= this->NewSearchGroup.SearchInputText;

				EditorUtilites::ConvertStringToLowercase(ItemName);
				EditorUtilites::ConvertStringToLowercase(ToBeFound);

				if (ItemName.find(ToBeFound) == std::string::npos)
				{
					// If we cant find the element that matches the current string. just ignore and actually use the things found.
					continue; 
				}
			}

			NewString = "";
			ImGui::BeginGroup();

			if (NewIterator->first.size() > 15) { NewString = NewIterator->first.substr(0, 15) + "..."; } // Does the string length exceed 15 characters? Cap it!
			else								{ NewString = NewIterator->first; }
	
			if (NewIterator->second.AssetImage == -1)
			{
				if (ImGui::Button(NewIterator->second.AssetUnicodeImage.c_str(), ImVec2(ThumbnailSize, ThumbnailSize)))
				{
					this->ManageAssetBeingPicked(FolderSelectedName, NewIterator->first);
				}
			}
			else
			{
				if (ImGui::ImageButton(NewIterator->first.c_str(), (ImTextureID)(intptr_t)(NewIterator->second.AssetImage), ImVec2(ThumbnailSize, ThumbnailSize), ImVec2(0, 0), ImVec2(1, 1)))
				{
					this->ManageAssetBeingPicked(FolderSelectedName, NewIterator->first);
				}
			}


			ImGui::TextWrapped("%s", NewString.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(NewIterator->first.c_str()); // Actually show the full string if the user wants to see
				ImGui::EndTooltip();
			}
			ImGui::EndGroup();

			if (++Count % Columns != 0) { ImGui::SameLine(); }
		}
	}

	ImGui::EndChild();
	ImGui::End();
}

void ComponentsWindowRenderHandler::DrawSettings(InterfaceValues CurrentValues)
{
	if (CurrentValues.IsSettingsMenuOpen)
	{
		ImGuiUtilites::BeginGradient("Fusion Settings", this->DefaultButtonHovered);

		if (ImGui::BeginTabBar("MainTabs"))
		{
			if (ImGui::BeginTabItem("Editor Variables"))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.92f, 0.79f, 0.18f, 1.0f));
				ImGui::Text("\xef\x81\xb1 Warning - The values will not change unless this specific value is used within the register.");
				ImGui::PopStyleColor();
				ImGui::Separator();

				if (ImGui::BeginTable("KeyValueTable", 2))
				{
					for (int Index = 0; Index < Commands::RegisterList.size(); Index++)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text(Commands::RegisterList[Index].Key.c_str());

						ImGui::TableSetColumnIndex(1);
						if (std::holds_alternative<bool>(Commands::RegisterList[Index].Value))				{ ImGui::Checkbox(Commands::RegisterList[Index].About.c_str(), &std::get<bool>(Commands::RegisterList[Index].Value)); }
						else if (std::holds_alternative<int>(Commands::RegisterList[Index].Value))			{ ImGui::InputInt(Commands::RegisterList[Index].About.c_str(), &std::get<int>(Commands::RegisterList[Index].Value)); }
						else if (std::holds_alternative<float>(Commands::RegisterList[Index].Value))		{ ImGui::InputFloat(Commands::RegisterList[Index].About.c_str(), &std::get<float>(Commands::RegisterList[Index].Value)); }
						else if (std::holds_alternative<std::string>(Commands::RegisterList[Index].Value))	{ ImGui::InputText(Commands::RegisterList[Index].About.c_str(), &std::get<std::string>(Commands::RegisterList[Index].Value)); }
					}
					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("About"))
			{
				ImGui::Image((ImTextureRef)(-1), ImVec2(550.0f, 150.0f));
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::Text("Build Version: %f\n", BUILD_VERSION);
				ImGui::Text("Release Version: %f\n", RELEASE_VERSION);

			#ifdef SET_AS_ALPHA
				ImGui::Text("This product is in early alpha!");
			#endif
				ImGui::NewLine();
				ImGui::Text("Dependancies: \nImGui\nImGuizmo\nNlohmann json\nGlad\nminiz\nGLM\nGLFW\nImGuizmo\nSTB\n(Depdancies Licences Found Within Source/ReadMe)");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

void ComponentsWindowRenderHandler::DrawDropdownMenu(std::vector<ModelObjectProperties>& DropdownContents)
{
	ImGuiUtilites::BeginGradient("WorldEditor", this->DefaultButtonHovered);
	ImGuiTreeNodeFlags CurrentFlags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (ImGui::BeginTable("EnviromentHirachy", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerV))
	{
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 60.0f);
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);

		for (int Index = 0; Index < DropdownContents.size(); Index++)
		{
			const bool IsSelectable = DropdownContents[Index].ObjectDataType != DATATYPE_ENVIROMENT_NOINTERACT;

			ImGui::PushID(Index);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 4.0f));

			if (IsSelectable && this->SelectedIndex == Index)	{ ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.65f, 0.35f, 1.00f)); }
			else												{ ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.22f, 0.16f, 1.00f)); }

			if (ImGui::Button(DropdownContents[Index].LabelPrefix.c_str(),ImVec2(50.0f,20.0f)))
			{
				if (DropdownContents[Index].LabelName == "Skybox") {}
				else if (DropdownContents[Index].LabelName == "Sun") {}
				else if (DropdownContents[Index].LabelName == "Camera") {}
				else {}

				// @TODO: Again another hardcoded system. Maybe just edit this so its automatic?
				if (DropdownContents[Index].LabelPrefix == ICON_FA_SCROLL)
				{
					this->ScriptList[this->RedirectionList[Index]].ScriptActive = !this->ScriptList[this->RedirectionList[Index]].ScriptActive;
					this->SelectedIndex = -1;
				}
				else
				{
					this->SelectedIndex = Index;
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleVar(2);
			ImGui::TableSetColumnIndex(1);

			if (DropdownContents[Index].Selected) { CurrentFlags |= ImGuiTreeNodeFlags_Selected; }

			ImGui::InputText("##TextInputSystem", &DropdownContents[Index].LabelName);
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
	ImGui::Separator();
	ImGui::End();
}

void ComponentsWindowRenderHandler::DrawRenderSceneWindow(GLFWwindow* EngineWindow,Camera* EngineCamera,InterfaceRenderModule CurrentModule)
{
	ImGuiUtilites::BeginGradient(CurrentModule.RenderTitle.c_str(), this->DefaultButtonHovered);
	this->CurrentViewportSize			= ImGui::GetContentRegionAvail();
	this->CurrentViewportSize.x			= std::max(1.0f, this->CurrentViewportSize.x);
	this->CurrentViewportSize.y			= std::max(1.0f, this->CurrentViewportSize.y);

	this->CurrentLocalScreenPosition	= ImGui::GetWindowPos(); // Apparantly this would return the top corner of the window as its position.
	this->CurrentViewportRectMin		= ImGui::GetItemRectMin();
	this->CurrentViewportRectSize		= ImGui::GetItemRectSize();

	ImGui::Image((void*)(CurrentModule.FrameBufferTextureIndex), this->CurrentViewportSize, ImVec2(0, 1), ImVec2(1, 0));
	this->ShowEditGizmo(EngineCamera, EngineCamera->Width, EngineCamera->Height); //Moved inside look to update region update
	ImGui::End();

	if (glfwGetMouseButton(EngineWindow,GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		if (ImGui::IsMouseHoveringRect(ImVec2(0.0f, 0.0f), this->CurrentViewportSize))
		{
			glfwSetInputMode(EngineWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Console::LockMouse = false;
		}
	}
	else
	{
		glfwSetInputMode(EngineWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		Console::LockMouse = true;
	}
}

void ComponentsWindowRenderHandler::RenderScripts(void)
{
	for (int Index = 0; Index < this->ScriptList.size(); Index++)
	{
		// This should have the same number as the code editor widget for now...
		// We could add more for shaders ect.
		
		this->NewTextEditor[Index].DrawCodeEditor({ this->ScriptList[Index].ScriptIndex,this->ScriptList[Index].ScriptContents,this->ScriptList[Index].ScriptName},&this->ScriptList[Index].ScriptActive);
	}
}

void ComponentsWindowRenderHandler::DrawComponentWindow(GLFWwindow* EngineWindow, Camera* EngineCamera,Shader* EngineShader,unsigned int FrameBuffer)
{
	const float ToolbarHeight	= 40.0f;
	const float StatusBarHeight	= 55.0f;
	static ImGuiDockNodeFlags DockspaceFlags	= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
	static ImGuiWindowFlags DockViewportFlags	= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiViewport* CurrentViewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(CurrentViewport->Pos.x, CurrentViewport->Pos.y + ToolbarHeight + Console::MenuBarHeight + 28));
	ImGui::SetNextWindowSize(ImVec2(CurrentViewport->Size.x, CurrentViewport->Size.y - ToolbarHeight - StatusBarHeight));
	ImGui::SetNextWindowViewport(CurrentViewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("DockspaceHost", nullptr, DockViewportFlags);
	ImGui::PopStyleVar();

	ImGuiID CurrentDockspaceID = ImGui::GetID("MyDockspace");
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::DockSpace(CurrentDockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);
	ImGui::PopStyleVar();

	this->CurrentViewportPosition = ImGui::GetCursorScreenPos();
	this->DrawToolbarWidget(this->CurrentInterfaceValue, EngineWindow, EngineCamera->ReturnProjectionMatrix(), EngineCamera->ReturnViewMatrix(), EngineCamera->ReturnCameraPosition());
	this->DrawSettings(this->CurrentInterfaceValue);
	this->DrawStatusBar();
	this->DrawMainToolbar(EngineCamera, EngineShader);
	this->DrawDropdownMenu(this->ObjectProperties);
	this->NewWorkspace.SetSelectedIndex(this->SelectedIndex);

	if (Console::GridViewEnable) { this->CurrentGridEditor.DrawGrids(EngineWindow); }

	this->FeedbackMenuSystem();
	this->DrawFileImporter();
	this->DrawAddComponentsMenu(EngineCamera,EngineShader);
	this->RenderScripts();
	this->DrawTextureViewer();
	this->DrawRenderSceneWindow(EngineWindow,EngineCamera,{FrameBuffer,static_cast<int>(CurrentViewport->Size.x),static_cast<int>(CurrentViewport->Size.y),"MyScene"});

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ComponentsWindowRenderHandler::Destroy(int Index)
{
	this->NewWorkspace.DestroyBlocks(Index);

	for (int Index = 0; Index < this->NewTextEditor.size(); Index++)
	{
		// @TODO: We should destroy the object here!
	}
}