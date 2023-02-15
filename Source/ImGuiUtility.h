#pragma once


Vector2 SliderVector2(Vector2 initial, float min, float max, const char* label);

Vector3 SliderVector3(Vector3 initial, float min, float max, const char* label);

Vector4 SliderVector4(Vector4 initial, const char* label);

Vector4 ColorPickerVector4(Vector4 initial, const char* label);

Vector4 ColorEditor(Vector4 color, const char* label);

Vector2 SizeEditor(Vector2 sz, bool& divisionFlag, const char* label);