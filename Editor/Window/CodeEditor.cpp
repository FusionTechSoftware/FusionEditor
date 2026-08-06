#include "CodeEditor.h"
#include <iostream>

CodeEditor::CodeEditor(void) {}

bool CodeEditor::IsFileTypeCorrect(const std::string& FileName,const std::string& FileExtension)
{
	if (FileName.substr(FileName.find_last_of(".") + 1) == FileExtension) {
		return true;
	}
	return false;
}

void CodeEditor::SetupCodeEditor(const std::string& FileName)
{
	std::string NewExtension = FileName.substr(FileName.find_last_of('.') + 1);

	if (NewExtension == "fs")		{ this->SyntaxDefinition = TextEditor::LanguageDefinition::FusionScript(); }
	else if (NewExtension == "c")	{ this->SyntaxDefinition = TextEditor::LanguageDefinition::C(); }
	else if (NewExtension == "cpp")	{ this->SyntaxDefinition = TextEditor::LanguageDefinition::CPlusPlus(); }

	this->SyntaxHighlight.SetLanguageDefinition(this->SyntaxDefinition);
}

void CodeEditor::DrawCodeEditor(InterfaceTextModule CurrentModule, bool* WindowActive)
{
	if (!(*WindowActive)) { return; }
	ImGuiUtilites::BeginGradient((CurrentModule.TextTitle+"##"+std::to_string(CurrentModule.TextIndex)).c_str(), this->CodeGradientBlock, WindowActive);

	if (this->SyntaxDefinition.mExtension == "unknown" || !this->IsFileTypeCorrect(CurrentModule.TextTitle, this->SyntaxDefinition.mExtension))
	{
		this->SetupCodeEditor(CurrentModule.TextTitle);
	}

	ImGui::Text("%6d/%-6d %6d lines | %s | %s | %s",
		this->SyntaxHighlight.GetCursorPosition().mLine + 1,
		this->SyntaxHighlight.GetCursorPosition().mColumn + 1,
		this->SyntaxHighlight.GetTotalLines(),
		this->SyntaxHighlight.IsOverwrite() ? "OVR" : "INS",
		this->SyntaxHighlight.CanUndo() ? "*" : " ",
		this->SyntaxHighlight.GetLanguageDefinition().mName.c_str()
	);
	this->SyntaxHighlight.Render(CurrentModule.TextTitle.c_str());
	ImGui::End();
}

void CodeEditor::ParseValidSyntax(SyntaxType CurrentType, std::string& EditorContents)
{
	int WordIndex;
	std::stringstream NewStringStream(EditorContents);
	std::string CurrentWord;

	while (NewStringStream >> CurrentWord)
	{
		if (HighlightToken.find(CurrentWord) != HighlightToken.end())
		{

		}
	}
}