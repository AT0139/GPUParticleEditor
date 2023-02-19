#include "stdafx.h"
#include "ImGuiUtility.h"

Vector2 SliderVector2(Vector2 initial, float min, float max, const char* label)
{
	float slider[2] = { initial.x,initial.y };
	ImGui::SliderFloat2(label, slider, min, max);

	return Vector2(slider[0], slider[1]);
}

Vector3 SliderVector3(Vector3 initial, float min, float max, const char* label)
{
	float slider[3] = { initial.x,initial.y,initial.z };
	ImGui::SliderFloat3(label, slider, min, max);

	return Vector3(slider[0], slider[1], slider[2]);
}

Vector4 SliderVector4(Vector4 initial, const char* label)
{
	float slider[4] = { initial.x,initial.y,initial.z,initial.w };
	ImGui::SliderFloat4(label, slider, 0.0f, 1.0f);

	return Vector4(slider[0], slider[1], slider[2], slider[3]);
}

Vector4 ColorPickerVector4(Vector4 initial, const char* label)
{
	float color[4] = { initial.x,initial.y,initial.z,initial.w };
	ImGui::ColorPicker4(label, color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float);
	return Vector4(color[0], color[1], color[2], color[3]);
}

Vector4 ColorEditor(Vector4 color, const char* label)
{
	Vector4 initialColor = color;
	if (ImGui::TreeNode(label))
	{
		initialColor = SliderVector4(initialColor, "");

		ImVec4 colorVec4(initialColor.x, initialColor.y, initialColor.z, initialColor.w); ImGui::SameLine();
		ImGui::ColorButton("##color", colorVec4);

		//ピッカー
		if (ImGui::Button("OpenColorPicker"))
			ImGui::OpenPopup("ColorPicker", ImGuiPopupFlags_MouseButtonLeft);

		if (ImGui::BeginPopup("ColorPicker"))
		{
			initialColor = ColorPickerVector4(initialColor, "");
			ImGui::EndPopup();
		}

		ImGui::TreePop();
	}
	return initialColor;
}

Vector2 SizeEditor(Vector2 sz, bool& divisionFlag, const char* label)
{
	Vector2 size = sz;
	if (ImGui::TreeNode(label))
	{
		ImGui::Spacing(); ImGui::SameLine(0.0f, 50.0f);
		ImGui::Checkbox("division", &divisionFlag);
		if (!divisionFlag)
		{
			ImGui::SliderFloat(label, &size.x, 0.01f, 5.0f);
			size.y = size.x;
		}
		else
		{
			size = SliderVector2(sz, 0.01f, 5.0f, label);
		}
		ImGui::TreePop();
	}
	return size;
}
