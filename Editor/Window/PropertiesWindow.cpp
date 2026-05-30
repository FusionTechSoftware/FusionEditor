#include "PropertiesWindow.h"

BaseProperties::BaseProperties(void) {}

void BaseProperties::DrawSkyboxProperties(SkyboxObjectProperties NewProperties)
{
	ImGui::SeparatorText("Skybox Properties");

	for (int Index = 0; Index < NewProperties.SkyboxValues.size(); Index++)
	{
		ImGui::NewLine();
		//ImGui::Image((ImTextureID)(NewProperties.SkyboxTextureSystem.find(NewProperties[Index])->second), ImVec2(50.0f, 50.0f));
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (NewProperties.ChangeSkyboxValue && NewProperties.ChangeSkyboxIndex == Index) ? ACTIVE_SELECTION_COLOUR : ImGui::GetStyleColorVec4(ImGuiCol_Button));

		ImGui::BeginGroup();
		{
			ImGui::Text(NewProperties.SkyboxValues[Index].c_str());
			//if (ImGui::Button(("Change Image " + SkyboxDimensions[Index]).c_str()))
			//{
			//	if (!NewProperties.ChangeSkyboxValue)	{ }
			//}
		}
		ImGui::EndGroup();
		ImGui::PopStyleColor();
	}
}

void BaseProperties::DrawCameraProperties(void)
{
	ImGui::SeparatorText("Camera Properties");
}

void BaseProperties::DrawSunProperties(void)
{
	ImGui::SeparatorText("Sun Properties");
}

void BaseProperties::DrawGeometryProperties(ModelObjectProperties NewProperties)
{
	ImGui::SeparatorText("Model Properties");

	ImGui::Text("Name:");
	ImGui::InputText("    ", &NewProperties.LabelName);
	if (NewProperties.LabelName.size() == 0) { NewProperties.LabelName = this->LastKnownModelName; }

	ImGui::Text("DataType %s", (NewProperties.ObjectDataType == DATATYPE_BLOCK ? "Block" : (NewProperties.ObjectDataType == DATATYPE_LIGHT ? "Light" : "?")));
	ImGui::Separator();

	ImGui::Text("Material");
	//ImGui::Combo("    ", &NewProperties.SelectedMaterial, this->MaterialItems.data(), this->MaterialItems.size());
	ImGui::Separator();

	ImGui::Text("Transform");
	ImGui::DragFloat3("Position", &NewProperties.Translate.x, 0.1f);
	ImGui::DragFloat4("Rotation", (float*) & NewProperties.Rotation, 0.1f);
	ImGui::DragFloat3("Scale", &NewProperties.Scale.x, 0.1f);
	ImGui::Separator();

	ImGui::Checkbox("Physics Locked", &NewProperties.PhysicsLocked);
	ImGui::Separator();

	ImGui::Checkbox("Visible", &NewProperties.Visible);
	ImGui::Separator();

	ImGui::Checkbox("Use Colour", &NewProperties.UseColour);
	ImGui::ColorEdit4("Colour", (float*) &NewProperties.Colour);
}