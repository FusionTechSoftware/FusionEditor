#include "CodeEditor.h"
#include "../../Common/Console.h"

#include <iostream>

CodeEditor::CodeEditor(void) {}

bool CodeEditor::IsFileTypeCorrect(const std::string& FileName,const std::string& FileExtension)
{
	if (FileName.substr(FileName.find_last_of(".") + 1) == FileExtension) {
		return true;
	}
	return false;
}

void CodeEditor::SetupCodeEditor(const std::string& FileName, const std::string& FileContents)
{
	std::string NewExtension = FileName.substr(FileName.find_last_of('.') + 1);

	if (NewExtension == "fs")		{ this->SyntaxDefinition = TextEditor::LanguageDefinition::FusionScript(); }
	else if (NewExtension == "c")	{ this->SyntaxDefinition = TextEditor::LanguageDefinition::C(); }
	else if (NewExtension == "cpp")	{ this->SyntaxDefinition = TextEditor::LanguageDefinition::CPlusPlus(); }
	else if (NewExtension == "vert" || NewExtension == "frag") { this->SyntaxDefinition = TextEditor::LanguageDefinition::GLSL(); }

	this->SyntaxHighlight.SetText(FileContents);
	this->SyntaxHighlight.SetLanguageDefinition(this->SyntaxDefinition);
}

void CodeEditor::CopyTextToClipboard(const std::string& TextString)
{
	// @TODO: Just found out there is a function built into ImGui called ImGui::SetClipboardText() *FACEPALM*
	OpenClipboard(0);
	EmptyClipboard();

	HGLOBAL NewGlobal = GlobalAlloc(GMEM_MOVEABLE, TextString.size());
	if (!NewGlobal) { Console::SysPrint("Failed to copy text to clipboard.", 2); CloseClipboard(); return; } // We failed to copy to clipboard;

	memcpy(GlobalLock(NewGlobal), TextString.c_str(), TextString.size());
	GlobalUnlock(NewGlobal);

	SetClipboardData(CF_TEXT, NewGlobal);
	CloseClipboard();
	GlobalFree(NewGlobal);
}

void CodeEditor::DrawCodeEditor(InterfaceTextModule CurrentModule, bool* WindowActive)
{
	if (!(*WindowActive)) { return; }
	ImGuiUtilites::BeginGradient((CurrentModule.TextTitle+"##"+std::to_string(CurrentModule.TextIndex)).c_str(), this->CodeGradientBlock, WindowActive);

	ImGui::Text("%6d/%-6d %6d lines | %s | %s | %s",
		this->SyntaxHighlight.GetCursorPosition().mLine + 1,
		this->SyntaxHighlight.GetCursorPosition().mColumn + 1,
		this->SyntaxHighlight.GetTotalLines(),
		this->SyntaxHighlight.IsOverwrite() ? "OVR" : "INS",
		this->SyntaxHighlight.CanUndo() ? "*" : " ",
		this->SyntaxHighlight.GetLanguageDefinition().mName.c_str()
	);
	this->SyntaxHighlight.Render(CurrentModule.TextTitle.c_str());
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Copy Text")) { this->CopyTextToClipboard(CurrentModule.TextContainer); }; // @TODO: So far this copies the whole text to clipboard. In the future this should only copy the selected text to clipboard. If nothing selected then the whole text.
		ImGui::Separator();
		if (ImGui::MenuItem("Paste Text")) { }
		ImGui::Separator();
		ImGui::MenuItem("Cut Text");
		ImGui::Separator();
		ImGui::MenuItem("Save File Copy");
		ImGui::EndPopup();
	}
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