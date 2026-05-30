#include "Console.h"
#include <iostream>

Console::Console(void) {}

bool Console::ConsoleMouseInputDetected                                             = false;

bool Console::LockMouse                                                             = false;
bool Console::ForceConsoleOpen                                                      = true;
bool Console::ConsoleToBottom                                                       = false;
bool Console::ConsoleReclaimFocus                                                   = false;
bool Console::IsDebugReferenceOpen                                                  = false;
bool Console::EnableVertex                                                          = false;
bool Console::EnableCollision                                                       = false;
bool Console::GridViewEnable                                                        = false;
float Console::MenuBarHeight                                                        = 0.0f;

float Console::IncrementValue[3]                                                    = {0.5f,0.5f,0.5f};

char Console::ConsoleInputBuffer[128];
std::unordered_map<std::string,CommandFunction> Console::ConsoleCommandFunctions    = {};
std::vector<ConsoleLogTemplate> Console::ConsoleCommandTexts                        = {};
std::vector<FunctionCommand> Console::ConsoleCommands                               = {};
ImVector<char*> Console::ConsoleRenderedItems;

void Console::AddToRenderedConsole(const char* Format, ...)
{
    char Buffer[1024];
    va_list Arguments;
    va_start(Arguments,Format);
    vsnprintf(Buffer,IM_ARRAYSIZE(Buffer),Format,Arguments);
    Buffer[IM_ARRAYSIZE(Buffer)-1] = 0;
    va_end(Arguments);
    ConsoleRenderedItems.push_back(strdup(Buffer));
}

void Console::SysPrint(const std::string& NewTextLog,int ErrorCode)
{
    glm::vec3               Colour;
    ConsoleLogTemplate      NewConsoleTemplate;
    Console::AddToRenderedConsole(((ErrorCode != 6 && ErrorCode != 7 && ErrorCode != 8 ? "[" + std::to_string(glfwGetTime()) + "]: " : "") + NewTextLog).c_str());

    if      (ErrorCode == 1) {Colour = CONSOLE_INFO;}
    else if (ErrorCode == 2) {Colour = CONSOLE_WARN;}
    else if (ErrorCode == 3) {Colour = CONSOLE_ERROR;}
    else if (ErrorCode == 4) {Colour = CONSOLE_CLIENT;}
    else if (ErrorCode == 5) {Colour = CONSOLE_SERVER;}
    else if (ErrorCode == 6) {Colour = CONSOLE_COMPILE;}
    else if (ErrorCode == 7) { Colour = CONSOLE_COMPILE_WARN;}
    else if (ErrorCode == 8) { Colour = CONSOLE_COMPILE_ERROR; }

    std::cout << NewTextLog << "\n";

    NewConsoleTemplate.CommandContents  = NewTextLog;
    NewConsoleTemplate.CommandColour    = Colour;
    Console::ConsoleCommandTexts.push_back(NewConsoleTemplate);
}

template <typename T>
void Console::SysPrintSafe(const T& NewLog,int ErrorCode)
{
    if constexpr(std::is_convertible_v<T,std::string>) {Console::SysPrint(std::string(NewLog),ErrorCode);}
    else 
    {
        std::ostringstream NewStringStream;
        NewStringStream << NewLog;
        Console::SysPrint(NewStringStream.str(),ErrorCode);
    }
}

void Console::AddCommand(const std::string& NewCommand,CommandFunction Function)
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 

    for (int Index = 0; Index < Console::ConsoleCommands.size(); Index++) 
    {if (Console::ConsoleCommands[Index].CommandString == NewCommand) {Console::SysPrint("Command '"+NewCommand+"' already exists.",2);}}

    FunctionCommand NewCommandFunction;
    NewCommandFunction.CommandString            = NewCommand;
    NewCommandFunction.ExecutableFunction       = Function;
    Console::ConsoleCommands.push_back(NewCommandFunction);
}

void Console::UpdateCommandExecution(std::string& CommandString)
{
    int Index = -1;
    for (int NewIndex = 0; NewIndex < Console::ConsoleCommands.size(); NewIndex++)  {if (Console::ConsoleCommands[NewIndex].CommandString == CommandString) {Index=NewIndex;}}
    if (Index == -1)                                                                {Console::ConsoleCommands[Index].ExecutableFunction(CommandString);}
    else if (CommandString.find("+set") != std::string::npos)                       {Commands::EditFromRegister(CommandString);}
    else if (CommandString.find("+add") != std::string::npos)                       {Commands::CreateLocalRegister(CommandString);}
    else                                                                            {Console::SysPrint("Invalid/Unknown Command",3);}
}

void Console::RenderConsoleFrame(GLFWwindow* EngineWindow)
{
    if (Console::LockMouse) {glfwSetInputMode(EngineWindow,GLFW_CURSOR,GLFW_CURSOR_NORMAL);}
    else                        {glfwSetInputMode(EngineWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);}
 
    ImGuiDockNodeFlags DockspaceFlag =
        ImGuiDockNodeFlags_NoWindowMenuButton |
        ImGuiDockNodeFlags_PassthruCentralNode |
        ImGuiDockNodeFlags_NoCloseButton;

    ImGui::SetNextWindowBgAlpha(1.0f);
    ImGui::DockSpaceOverViewport(0,ImGui::GetMainViewport(), DockspaceFlag);

    ImGui::Begin("Console",&Console::ForceConsoleOpen);
    ImGui::Separator();

    Console::ConsoleReclaimFocus    = std::get<bool>(Commands::GetFromRegister("consolefocuswhenactive"));
    const float HeightToReserve     = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

    if (ImGui::BeginChild("ScrollingRegion",ImVec2(0,-HeightToReserve),ImGuiChildFlags_None,ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1));
        for (int Index = 0; Index < Console::ConsoleRenderedItems.size(); Index++)
        {
            ImVec4  Colour;
            if (Console::ConsoleCommandTexts[Index].CommandColour == CONSOLE_INFO)              {Colour = ImVec4(1.0f,1.0f,1.0f,1.0f);}
            else if (Console::ConsoleCommandTexts[Index].CommandColour == CONSOLE_WARN)         {Colour = ImVec4(1.0f,1.0f,0.4f,1.0f);}
            else if (Console::ConsoleCommandTexts[Index].CommandColour == CONSOLE_ERROR)        {Colour = ImVec4(1.0f,0.4f,0.4f,1.0f);}
            else if (Console::ConsoleCommandTexts[Index].CommandColour == CONSOLE_SERVER)       {Colour = ImVec4(0.4f,1.0f,0.4f,1.0f);}
            else if (Console::ConsoleCommandTexts[Index].CommandColour == CONSOLE_CLIENT)       {Colour = ImVec4(0.4f,0.4f,1.0f,1.0f);}
            else if (Console::ConsoleCommandTexts[Index].CommandColour == CONSOLE_COMPILE)      {Colour = ImVec4(1.0f,1.0f,1.0f,1.0f);}
            else if (Console::ConsoleCommandTexts[Index].CommandColour == CONSOLE_COMPILE_ERROR) {Colour = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);}
            else if (Console::ConsoleCommandTexts[Index].CommandColour == CONSOLE_COMPILE_WARN) { Colour = ImVec4(1.0f, 1.0f, 0.4f, 1.0f); }
            ImGui::PushStyleColor(ImGuiCol_Text,Colour);
            ImGui::TextUnformatted(ConsoleRenderedItems[Index]);
            ImGui::PopStyleColor();
        }
        if (ConsoleToBottom || (std::get<bool>(Commands::GetFromRegister("consoleactivescroll")) && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) 
        {ImGui::SetScrollHereY(1.0f);}
        ConsoleToBottom = false;

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::Separator();

    ImGuiInputTextFlags TextFlags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (ImGui::InputText("Input",ConsoleInputBuffer,IM_ARRAYSIZE(ConsoleInputBuffer),TextFlags))
    {
        SysPrint(ConsoleInputBuffer,1);
        if (std::string(ConsoleInputBuffer).find("+exec") == std::string::npos) 
        {
            std::string ConsoleBufferString(ConsoleInputBuffer);
            Console::UpdateCommandExecution(ConsoleBufferString);
            ConsoleInputBuffer[0]   = '\0';
            ConsoleReclaimFocus     = true;
        }
    }

    ImGui::SetItemDefaultFocus();
    if (ConsoleReclaimFocus) {ImGui::SetKeyboardFocusHere(-1);}
    ImGui::End();

}

void Console::RenderDebugOverlay(const std::vector<DebugData>& LocalDebugData)
{
    ImGuiWindowFlags NewDebugFlags          = ImGuiWindowFlags_AlwaysAutoResize;
    ImGuiIO& WindowInput                    = ImGui::GetIO();

    const float DebugPadding                = 10.0f;
    const ImGuiViewport* RenderViewport     = ImGui::GetMainViewport();
    ImVec2 ViewportWorkPosition             = RenderViewport->WorkPos;
    ImVec2 ViewportWorkScale                = RenderViewport->WorkSize;
    ImVec2 WindowPosition                   = ImVec2(ViewportWorkPosition.x+DebugPadding,ViewportWorkPosition.y+DebugPadding);
    ImVec2 WindowPivort                     = ImVec2(0.0f,0.0f);

    ImGui::SetNextWindowBgAlpha(0.35f);
    if (ImGui::Begin("Debug Values",&Console::IsDebugReferenceOpen,NewDebugFlags))
    {
        for (int Index = 0; Index < LocalDebugData.size(); Index++)
        {
            ImGui::Text((LocalDebugData[Index].LabelName+" | "+LocalDebugData[Index].Data).c_str());
            ImGui::Separator();
        }
    }
    ImGui::End();
}

bool Console::ShowExitMessage(void)
{
    if (std::get<bool>(Commands::GetFromRegister("exit")))
    {
        ImGui::OpenPopup("Quit?");
        ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(Center,ImGuiCond_Appearing,ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to leave without saving?\nPress 'Yes' to continue.");
            ImGui::Separator();

            if (ImGui::Button("Yes", ImVec2(120, 0))) {Commands::SetFromRegister("exitconfirm",true); Commands::SetFromRegister("exit",false);}
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();  
            if (ImGui::Button("Save", ImVec2(120, 0))) { Commands::SetFromRegister("exit", false); ImGui::CloseCurrentPopup(); return true;}
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { Commands::SetFromRegister("exit", false); ImGui::CloseCurrentPopup();}
            ImGui::EndPopup();
        }
    }
    return false;
}

void Console::GenerateValues(void)
{
    Console::AddCommand("exit",Commands::ExitGameFunction);
    Console::AddCommand("-dump",Commands::DumpConsole);
    Console::AddCommand("-update",Commands::UpdateGLValues);
    Console::AddCommand("-print",Commands::PrintAllRegisters);
    Console::AddCommand("-clear",Commands::ClearConsole);

    Commands::CreateFromRegister("width",1920.0f);
    Commands::CreateFromRegister("height",1080.0f);
    Commands::CreateFromRegister("showhud",true);
    Commands::CreateFromRegister("consolefocuswhenactive",false);
    Commands::CreateFromRegister("consoleactivescroll",true);
    Commands::CreateFromRegister("gravity",-9.81f);
    Commands::CreateFromRegister("accel",15.0f);
    Commands::CreateFromRegister("decel",0.0005f);
    Commands::CreateFromRegister("speed",35.0f);
    Commands::CreateFromRegister("frict",0.006f);
    Commands::CreateFromRegister("mass",200.0f);
    Commands::CreateFromRegister("debugreference",true);
}
