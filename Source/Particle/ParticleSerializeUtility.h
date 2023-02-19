#pragma once

#include "ParticleEmitter.h"
#include "EmitterGui.h"

const std::string PARTICLE_FOLDER_PATH = "./Particles/";
const std::string PARTICLE_LIST_PATH = PARTICLE_FOLDER_PATH + "Particles.json";

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
	std::string emitterName;
	EmitterInitData data;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(emitterName), CEREAL_NVP(data.filePath),
			CEREAL_NVP(data.gravity), CEREAL_NVP(data.maxLife),
			CEREAL_NVP(data.velocity), CEREAL_NVP(data.velocityType), CEREAL_NVP(data.initialSize), CEREAL_NVP(data.finalSize),
			CEREAL_NVP(data.initialColor), CEREAL_NVP(data.finalColor), CEREAL_NVP(data.offsetPosition),
			CEREAL_NVP(data.createInterval), CEREAL_NVP(data.createOnceNum));
	}
};

namespace ParticleSerialize
{
	//シリアライズ用テンプレート関数
	template<class Archive>
	void serialize(Archive& archive, std::vector<ParticleSerializeData>& particles)
	{
		archive(CEREAL_NVP(particles));
	}

	template<class Archive>
	void serialize(Archive& archive, std::vector<std::string>& particles)
	{
		archive(CEREAL_NVP(particles));
	}


	void ParticleSerialize(std::string particleName, std::vector<std::shared_ptr<EmitterGui>> emitterList, std::vector<std::string>& savedList);
	std::vector<ParticleSerializeData> ParticleDeserialize(std::string particleName);
}