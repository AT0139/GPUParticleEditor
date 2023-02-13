#pragma once

// Vectorの非侵入型シリアライズ定義
namespace DirectX::SimpleMath
{
	template<class Archive>
	void serialize(Archive& archive, Vector2& vector)
	{
		archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y));
	}

	template<class Archive>
	void serialize(Archive& archive, Vector3& vector)
	{
		archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y), cereal::make_nvp("z", vector.z));
	}

	template<class Archive>
	void serialize(Archive& archive, Vector4& vector)
	{
		archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y), cereal::make_nvp("z", vector.z), cereal::make_nvp("w", vector.w));
	}
}