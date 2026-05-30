#ifndef __CODE_EDITOR_HEADER__
#define __CODE_EDITOR_HEADER__

#include <map>
#include <algorithm>
#include <sstream>

#include "../../../Resources/imgui/imgui.h"
#include "../../../Resources/imgui/imgui_impl_opengl3.h"
#include "../../../Resources/imgui/imgui_impl_glfw.h"
#include "../../../Resources/imgui/imgui_stdlib.h"

#include "../../Utilites/ImGuiUtilites.h"
#include "SyntaxHighlightTokens/FusionScriptHighlight.h"
#include "SyntaxResources/TextEditor.h"

enum SyntaxType
{
	IGNORE_SYNTAX = 0,
	FUSION_SYNTAX
};

struct InterfaceTextModule
{
	std::string		TextContainer;
	std::string		TextTitle;
};

class CodeEditor
{
	public:
											CodeEditor(void);
		void								DrawCodeEditor(InterfaceTextModule CurrentModule);
		void								ParseValidSyntax(SyntaxType CurrentType, std::string& EditorContents);
		void								SetupCodeEditor(const std::string& FileName);

	private:
		bool								IsFileTypeCorrect(const std::string& FileName, const std::string& FileExtension);

		GradientBlock						CodeGradientBlock = { IM_COL32(40, 40, 40, 255), IM_COL32(20, 20, 20, 255) };
		std::map<int, std::string>			VisitedSyntax = {};
		TextEditor							SyntaxHighlight;
		TextEditor::LanguageDefinition		SyntaxDefinition;
};

#endif
