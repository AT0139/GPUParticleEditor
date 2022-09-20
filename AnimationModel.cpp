#include "main.h"
#include "renderer.h"
#include "AnimationModel.h"



AnimationModel::AnimationModel(const char* fileName)
{
	Load(fileName);
}

void AnimationModel::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_aiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_aiScene);

	m_vertexBuffer = new ID3D11Buffer * [m_aiScene->mNumMeshes];
	m_indexBuffer = new ID3D11Buffer * [m_aiScene->mNumMeshes];

	//�ό`�㒸�_�z�񐶐�
	m_deformVertex = new std::vector<DEFORM_VERTEX>[m_aiScene->mNumMeshes];

	//�ċA�I�Ƀ{�[������
	CreateBone(m_aiScene->mRootNode);



	for (unsigned int m = 0; m < m_aiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_aiScene->mMeshes[m];

		//���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetInstance()->GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer[m]);

			delete[] vertex;
		}

		//�C���f�b�N�X�o�b�t�@����
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetInstance()->GetDevice()->CreateBuffer(&bd, &sd, &m_indexBuffer[m]);

			delete[] index;
		}

		//�ό`�㒸�_�f�[�^������
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX deformVertex;
			deformVertex.Position = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			m_deformVertex[m].push_back(deformVertex);
		}

		//�{�[���f�[�^������
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//�ό`��Ƀ{�[���f�[�^�i�[
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_deformVertex[m][weight.mVertexId].BoneNum;

				m_deformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_deformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
				m_deformVertex[m][weight.mVertexId].BoneNum++;

				assert(m_deformVertex[m][weight.mVertexId].BoneNum <= 4);
			}
		}
	}

	//�e�N�X�`���ǂݍ���
	{
		for (unsigned int m = 0; m < m_aiScene->mNumMaterials; m++)
		{
			aiString path;
			if (m_aiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				if (path.data[0] == '*')
				{
					//FBX�t�@�C��������ǂݍ���
					if (m_texture[path.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&path.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							Renderer::GetInstance()->GetDevice(),
							(const unsigned char*)m_aiScene->mTextures[id]->pcData,
							m_aiScene->mTextures[id]->mWidth,
							NULL, NULL, &texture, NULL);
						
						m_texture[path.data] = texture;
					}
				}
				else
				{
					if (m_texture[path.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
				
						char temp[1024] = { "asset/model/" };
						(void)strcat(temp, path.data);

						//�O���t�@�C������ǂݍ���
						D3DX11CreateShaderResourceViewFromFile(
							Renderer::GetInstance()->GetDevice(),
							temp,
							NULL, NULL, &texture, NULL
						);

						m_texture[temp] = texture;
					}
				}
			}
			else
			{
				m_texture[path.data] = NULL;
			}
		}
	}
}

void AnimationModel::LoadAnimation(const char* fileName, const char* animarionName)
{
	m_animation[animarionName] = aiImportFile(fileName, aiProcess_ConvertToLeftHanded);
	assert(m_animation[animarionName]);
}

void AnimationModel::Unload()
{
	for (unsigned int m = 0; m < m_aiScene->mNumMeshes; m++)
	{
		m_vertexBuffer[m]->Release();
		m_indexBuffer[m]->Release();
	}
	delete[] m_vertexBuffer;
	delete[] m_indexBuffer;

	delete[] m_deformVertex;

	for (auto pair : m_texture)
	{
		if (pair.second)
			pair.second->Release();
	}

	aiReleaseImport(m_aiScene);

	for (auto pair : m_animation)
	{
		aiReleaseImport(pair.second);
	}
}

void AnimationModel::Update(const char* animationName,int frame)
{
	if (!m_animation[animationName]->HasAnimations())
		return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation = m_animation[animationName]->mAnimations[0];

	for (unsigned int c = 0; c < animation->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[c];
		BONE* bone = &m_bone[nodeAnim->mNodeName.C_Str()];

		int f;
		f = frame % nodeAnim->mNumRotationKeys;//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = frame % nodeAnim->mNumPositionKeys;
		aiVector3D pos;
		//�A�j���[�V�����f�[�^��1���傫���ꍇ(�A�j���[�V�����f�[�^������)(�N���[�`���[�ɂȂ�ꍇ�̓R�����g�A�E�g)
		//if (nodeAnim->mNumPositionKeys > 1)
		//	pos = nodeAnim->mPositionKeys[f].mValue;
		
		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	UpdateBoneMatrix(m_aiScene->mRootNode, aiMatrix4x4());

	//���_�ϊ�(CPU�X�L�j���O)
	for (unsigned int m = 0; m < m_aiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_aiScene->mMeshes[m];

		D3D11_MAPPED_SUBRESOURCE ms;

		Renderer::GetInstance()->GetDeviceContext()->
			Map(m_vertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX* deformVertex = &m_deformVertex[m][v];

			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;
			matrix[0] = m_bone[deformVertex->BoneName[0]].Matrix;
			matrix[1] = m_bone[deformVertex->BoneName[1]].Matrix;
			matrix[2] = m_bone[deformVertex->BoneName[2]].Matrix;
			matrix[3] = m_bone[deformVertex->BoneName[3]].Matrix;

			//�E�F�C�g���l�����ă}�g���N�X�Z�o
			//outMatrix =	matrix[0] * deformVertex->BoneWeight[0]
			//		+	matrix[1] * deformVertex->BoneWeight[1]
			//		+	matrix[2] * deformVertex->BoneWeight[2]
			//		+	matrix[3] * deformVertex->BoneWeight[3]
			{
				outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0] +
					matrix[1].a1 * deformVertex->BoneWeight[1] +
					matrix[2].a1 * deformVertex->BoneWeight[2] +
					matrix[3].a1 * deformVertex->BoneWeight[3];

				outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0] +
					matrix[1].a2 * deformVertex->BoneWeight[1] +
					matrix[2].a2 * deformVertex->BoneWeight[2] +
					matrix[3].a2 * deformVertex->BoneWeight[3];

				outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0] +
					matrix[1].a3 * deformVertex->BoneWeight[1] +
					matrix[2].a3 * deformVertex->BoneWeight[2] +
					matrix[3].a3 * deformVertex->BoneWeight[3];

				outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0] +
					matrix[1].a4 * deformVertex->BoneWeight[1] +
					matrix[2].a4 * deformVertex->BoneWeight[2] +
					matrix[3].a4 * deformVertex->BoneWeight[3];

				outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0] +
					matrix[1].b1 * deformVertex->BoneWeight[1] +
					matrix[2].b1 * deformVertex->BoneWeight[2] +
					matrix[3].b1 * deformVertex->BoneWeight[3];

				outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0] +
					matrix[1].b2 * deformVertex->BoneWeight[1] +
					matrix[2].b2 * deformVertex->BoneWeight[2] +
					matrix[3].b2 * deformVertex->BoneWeight[3];

				outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0] +
					matrix[1].b3 * deformVertex->BoneWeight[1] +
					matrix[2].b3 * deformVertex->BoneWeight[2] +
					matrix[3].b3 * deformVertex->BoneWeight[3];

				outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0] +
					matrix[1].b4 * deformVertex->BoneWeight[1] +
					matrix[2].b4 * deformVertex->BoneWeight[2] +
					matrix[3].b4 * deformVertex->BoneWeight[3];

				outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0] +
					matrix[1].c1 * deformVertex->BoneWeight[1] +
					matrix[2].c1 * deformVertex->BoneWeight[2] +
					matrix[3].c1 * deformVertex->BoneWeight[3];

				outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0] +
					matrix[1].c2 * deformVertex->BoneWeight[1] +
					matrix[2].c2 * deformVertex->BoneWeight[2] +
					matrix[3].c2 * deformVertex->BoneWeight[3];

				outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0] +
					matrix[1].c3 * deformVertex->BoneWeight[1] +
					matrix[2].c3 * deformVertex->BoneWeight[2] +
					matrix[3].c3 * deformVertex->BoneWeight[3];

				outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0] +
					matrix[1].c4 * deformVertex->BoneWeight[1] +
					matrix[2].c4 * deformVertex->BoneWeight[2] +
					matrix[3].c4 * deformVertex->BoneWeight[3];

				outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0] +
					matrix[1].d1 * deformVertex->BoneWeight[1] +
					matrix[2].d1 * deformVertex->BoneWeight[2] +
					matrix[3].d1 * deformVertex->BoneWeight[3];

				outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0] +
					matrix[1].d2 * deformVertex->BoneWeight[1] +
					matrix[2].d2 * deformVertex->BoneWeight[2] +
					matrix[3].d2 * deformVertex->BoneWeight[3];

				outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0] +
					matrix[1].d3 * deformVertex->BoneWeight[1] +
					matrix[2].d3 * deformVertex->BoneWeight[2] +
					matrix[3].d3 * deformVertex->BoneWeight[3];

				outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0] +
					matrix[1].d4 * deformVertex->BoneWeight[1] +
					matrix[2].d4 * deformVertex->BoneWeight[2] +
					matrix[3].d4 * deformVertex->BoneWeight[3];
			}

			deformVertex->Position = mesh->mVertices[v];
			deformVertex->Position *= outMatrix;

			//�@���Ԋ҂悤�Ɉړ��������폜
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			deformVertex->Normal = mesh->mNormals[v];
			deformVertex->Normal *= outMatrix;

			//���_�o�b�t�@�֏�������
			vertex[v].Position.x = deformVertex->Position.x;
			vertex[v].Position.y = deformVertex->Position.y;
			vertex[v].Position.z = deformVertex->Position.z;

			vertex[v].Normal.x = deformVertex->Normal.x;
			vertex[v].Normal.y = deformVertex->Normal.y;
			vertex[v].Normal.z = deformVertex->Normal.z;

			vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
			vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

			vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		Renderer::GetInstance()->GetDeviceContext()->Unmap(m_vertexBuffer[m], 0);
	}
}

void AnimationModel::Draw()
{
	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�}�e���A���ݒ� FBX������o���Ă��Ȃ��̂ł����@���ݒ萔
	MATERIAL material;
	ZeroMemory(&material,sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::GetInstance()->SetMaterial(material);

	for (unsigned int m = 0; m < m_aiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_aiScene->mMeshes[m];

		aiMaterial* material = m_aiScene->mMaterials[mesh->mMaterialIndex];

		//�e�N�X�`���ݒ�
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (path.data[0] != '*')
		{
			char temp[1024] = { "asset/model/" };
			(void)strcat(temp, path.data);
			strcpy(path.data, temp);
		}
		Renderer::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture[path.data]);

		//���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer[m], &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�ݒ�
		Renderer::GetInstance()->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//�|���S���`��
		Renderer::GetInstance()->GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);

	}
}

void AnimationModel::CreateBone(aiNode* node)
{
	BONE bone;

	bone.TransMatrix = node->mTransformation;

	m_bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;

	worldMatrix = matrix;
	worldMatrix *= bone->TransMatrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}
