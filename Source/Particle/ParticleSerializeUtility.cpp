#include "ParticleSerializeUtility.h"


namespace ParticleSerialize
{
	//シリアライズ
	void ParticleSerialize(std::string particleName, std::list<std::shared_ptr<EmitterGui>> emitterList, std::vector<std::string>& savedList)
	{
		std::list<ParticleSerializeData> serializeList;

		for (auto emitter : emitterList)
		{
			ParticleSerializeData serializeData;
			serializeData.data = emitter->GetEmitter()->GetSerializeData();
			serializeData.emitterName = emitter->GetEmitterName();
			serializeList.push_back(serializeData);
		}

		{
			std::ofstream os(PARTICLE_LIST_PATH, std::ios::out);
			cereal::JSONOutputArchive archiveFile(os);

			savedList.push_back(particleName);
			serialize(archiveFile, savedList);
		}

		{
			std::ofstream os(PARTICLE_FOLDER_PATH + particleName + ".json", std::ios::out);
			cereal::JSONOutputArchive archiveFile(os);
			serialize(archiveFile, serializeList);
		}
	}


	//デシリアライズ
	std::list<ParticleSerializeData> ParticleDeserialize(std::string particleName)
	{
		std::ifstream os(PARTICLE_FOLDER_PATH + particleName + ".json", std::ios::in);
		cereal::JSONInputArchive archive(os);

		std::list<ParticleSerializeData> inputList;

		serialize(archive, inputList);

		return inputList;
	}
}