#include "SerializeManager.h"
#include "SerializeComponent.h"
#include "Manager.h"
#include "Scene.h"

SerializeManager::SerializeManager()
{
}

SerializeManager::~SerializeManager()
{
}

void SerializeManager::ToSerialize()
{
	//todo : マスタ化
	std::list<SerializeInfo> serializeList;

	//シリアライズコンポーネントを付けたオブジェクト情報の取得
	auto& objects = Manager::GetInstance().GetScene()->GetAllObject();
	for (auto& obj : objects)
	{
		auto pSerial = obj->GetComponent<SerializeComponent>();
		if (pSerial)
		{
			SerializeInfo info;
			info.data = pSerial->GetSerializeData();

			serializeList.push_back(info);
		}
	}

	std::ofstream os("objects.json", std::ios::out);
	cereal::JSONOutputArchive archiveFile(os);

	//シリアライズ
	serialize(archiveFile, serializeList);
}

void SerializeManager::ToDeserialize()
{
	std::ifstream os("objects.json", std::ios::in);
	cereal::JSONInputArchive archive(os);

	std::list<SerializeInfo> inputList;
	//デシリアライズ
	serialize(archive, inputList);

	auto scene = Manager::GetInstance().GetScene();

	//オブジェクトの作成、座標、回転代入
	for (auto input : inputList)
	{
		if (input.type == SerializeObjectType::WALL)
		{
			//auto objTrans = scene->AddGameObject<Wall>(scene->OBJECT)->GetComponent<Transform>();
			//Vector3 pos(input.data.positionX, input.data.positionY, input.data.positionZ);
			//Quaternion rot(input.data.rotateX, input.data.rotateY, input.data.rotateZ, input.data.rotateW);
			//objTrans->SetPosition(pos);
			//objTrans->SetQuaternion(rot);
		}
		else
		{
			//auto objTrans = scene->AddGameObject<Floor>(scene->OBJECT)->GetComponent<Transform>();
			//Vector3 pos(input.data.positionX, input.data.positionY, input.data.positionZ);
			//Quaternion rot(input.data.rotateX, input.data.rotateY, input.data.rotateZ, input.data.rotateW);
			//objTrans->SetPosition(pos);
			//objTrans->SetQuaternion(rot);
		}
	}



}

