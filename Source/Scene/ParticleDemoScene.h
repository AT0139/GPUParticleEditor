#pragma once

#include "Scene.h"
#include "ParticleEmitterManager.h"
#include "EmitterGui.h"

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

struct ParticleSerializeData
{
	EmitterInitData data;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(data.filePath),
			CEREAL_NVP(data.gravity), CEREAL_NVP(data.maxLife),
			CEREAL_NVP(data.velocity), CEREAL_NVP(data.velocityType), CEREAL_NVP(data.initialSize), CEREAL_NVP(data.finalSize),
			CEREAL_NVP(data.initialColor), CEREAL_NVP(data.finalColor), CEREAL_NVP(data.offsetPosition));
	}
};

class ParticleDemoScene :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();

private:
	void AddEmitter();
	void AddEmitter(EmitterInitData initData);

	void ToSerialize();
	void ToDeserialize();

	template<class Archive>
	void serialize(Archive& archive, std::list<ParticleSerializeData>& objects)
	{
		archive(CEREAL_NVP(objects));
	}

	ParticleEmitterManager* m_emitterManager;
	std::list<std::shared_ptr<EmitterGui>> m_emitterList;
};