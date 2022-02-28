#include "Model.h"
#include "../Cores/Renderer/RendererCore.h"
#include "SceneAndDrawCall.h"
#include "../Cores/Texture/Texture.h"
#include "../../Editor/Editor.h"

void Model::OnDelete()
{
	DeleteModel();
}

void Model::DeleteModel()
{
	DrawCall& d = *RendererCore::FindDrawCall(SceneBelongsTo, DWBelongsTo);

	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].isDeleted = true;
		vertices[i].position = glm::vec4(0);
		vertices[i].color = glm::vec4(0);

		d.allVerts[indexVertices[i]].isDeleted = vertices[i].isDeleted;
		d.allVerts[indexVertices[i]].position = vertices[i].position;
		d.allVerts[indexVertices[i]].color = vertices[i].color;
	}
	isDeleted = true;
	d.deleteProcess = true;

	RendererCore::ModifyVerticesOnBuffer(indexVertices, false, d);

	vertices.clear();
	std::vector<Vertex>().swap(vertices);

	indexVertices.clear();
	std::vector<int>().swap(indexVertices);
}

Matrix Model::SetTransformAndGetWorldMatrix(Vector3 position, Vector3 rotation, Vector3 scale)
{
	this->transform.position = Vector4(position);
	this->transform.position.w = 1;

	this->transform.rotation = Vector4(rotation);
	this->transform.rotation.w = 1;

	this->transform.scale = Vector4(scale);
	this->transform.scale.w = 1;

	translationMatrix = Matrix(1);
	translationMatrix = Matrix::Translate(Vector3(-position.x, -position.y, -position.z));

	rotationMatrix = Matrix::Rotate(Vector3(rotation.x, rotation.y, rotation.z + 180.0f));

	Matrix finalMatrix = translationMatrix * rotationMatrix * -translationMatrix;
	return finalMatrix;
}

void Model::SetVertices(Matrix matrix, std::vector<float> vertsToSet)
{
	for (int i = 0; i < vertsToSet.size(); i++)
	{
		modelVertexData.push_back(vertsToSet[i]);
	}

	vertices.clear();
	std::vector<Vertex>().swap(vertices);
	vertices.resize(vertsToSet.size() / 4);

	for (int i = 0; i < vertices.size(); i++)
	{
		Vector4 set = matrix * Vector4(transform.position.x + modelVertexData[i * 4] * transform.scale.x, transform.position.y + modelVertexData[i * 4 + 1] * transform.scale.y, transform.position.z, 1.0f);

		vertices[i].position = glm::vec4(set.x, set.y, set.z, set.w);
		vertices[i].color = glm::vec4(color.r, color.g, color.b, color.a);
		
		if (texture != nullptr)
			vertices[i].textureGroupId = texture->texGroupId;
		else
			vertices[i].textureGroupId = -1;
	}
}

void Model::SetTextureIntoModel(Model* model, std::string* path)
{
	std::cout << "Uploading to: " << model->objectClassName << std::endl;
	std::cout << "Texture Path: " << *path << std::endl;
	Texture* newT = new Texture(path->c_str(), Texture::Type::Default);
	TextureManager::UploadToGPU();
	model->texture = newT;

	DrawCall& d = *RendererCore::FindDrawCall(model->SceneBelongsTo, model->DWBelongsTo);
	d.Refresh();
}

void Model::OnStartup()
{
	//std::cout << "Model Applied!" << std::endl;
	ClassType = Class::Pointer;
}

void Model::OnSave()
{
	SaveNewPointer(Model, Model::Square(),
		Vector4(SaveReadFloat(CurrentObject::transform.position.x), SaveReadFloat(CurrentObject::transform.position.y), SaveReadFloat(CurrentObject::transform.position.z), SaveReadFloat(CurrentObject::transform.position.w)),
		Vector4(SaveReadFloat(CurrentObject::transform.rotation.x), SaveReadFloat(CurrentObject::transform.rotation.y), SaveReadFloat(CurrentObject::transform.rotation.z), SaveReadFloat(CurrentObject::transform.rotation.w)),
		Vector4(SaveReadFloat(CurrentObject::transform.scale.x), SaveReadFloat(CurrentObject::transform.scale.y), SaveReadFloat(CurrentObject::transform.scale.z), SaveReadFloat(CurrentObject::transform.scale.w)));

	SaveValue(transform.position.x);
	SaveValue(transform.position.y);
	SaveValue(transform.position.z);
	SaveValue(transform.position.w);

	SaveValue(transform.rotation.x);
	SaveValue(transform.rotation.y);
	SaveValue(transform.rotation.z);
	SaveValue(transform.rotation.w);

	SaveValue(transform.scale.x);
	SaveValue(transform.scale.y);
	SaveValue(transform.scale.z);
	SaveValue(transform.scale.w);

	if (texture != nullptr)
	{
		SaveResource(texture, Texture, Texture(SaveReadResourceString(CurrentObject::texture->filename), Texture::Type::Default), "Texture Initialization");
		SaveValue(texture->filename);
	}

	SaveInstruction(CurrentObject::color = Color(SaveReadFloat(CurrentObject::color.r), SaveReadFloat(CurrentObject::color.g), SaveReadFloat(CurrentObject::color.b), SaveReadFloat(CurrentObject::color.a)););
	SaveValue(color.r);
	SaveValue(color.g);
	SaveValue(color.b);
	SaveValue(color.a);


	SaveResourceInstructionAtEnd(
		TextureManager::UploadToGPU(),
		"Texture Initialization");

	SaveObjectOnlyInstruction(RendererCore::AddModel, *);

	SaveEnd();
}

void Model::OnInspector()
{
	VisualAccess_Title(Model);

	VisualAccess_AddValue(ColorEditorButtonRGBA, Color, &color);
	VisualAccess_AddEditUIValue(textureButton, FileSearch, Texture, "No Texture Selected");

	if(texture != nullptr)
		textureButton->storedPath = texture->filename;

	textureButton->OnClick(std::bind(Editor::FileBrowser_ChooseFileMode, textureButton->text, objectClassName, textureButton));
	textureButton->OnFileChange(std::bind(SetTextureIntoModel, this, &textureButton->storedPath));
}

Model::Model(std::vector<float> vertices, Vector3 position, Vector3 rotation, Vector3 scale)
{
	Matrix worldMatrix = SetTransformAndGetWorldMatrix(position, rotation, scale);
	SetVertices(worldMatrix, vertices);

	this->transform.position.sharedModels.push_back(this);
	this->transform.rotation.sharedModels.push_back(this);
	this->transform.scale.sharedModels.push_back(this);

	this->color.sharedModels.push_back(this);
}

Model Model::Square(Model& square, Vector3 position, Vector3 rotation, Vector3 scale)
{
	Matrix worldMatrix = square.SetTransformAndGetWorldMatrix(position, rotation, scale);
	square.SetVertices(worldMatrix, Square());

	square.transform.position.sharedModels.push_back(&square);
	square.transform.rotation.sharedModels.push_back(&square);
	square.transform.scale.sharedModels.push_back(&square);

	square.color.sharedModels.push_back(&square);

	return square;
}

void Model::OnChange()
{
	OnChange(true, false);
}

void Model::ChangeModelIndex(int index, int result)
{
	indexVertices[index] = result;
}

void Model::SetTexture(Texture& tex)
{
	texture = &tex;
	if (indexVertices.size() != 0)
		OnChange(false, false);
}

void Model::FlipTexture(bool x, bool y)
{
	flipXTexture = x;
	flipYTexture = y;
	if (indexVertices.size() != 0)
		OnChange(false, false);
}

void Model::OnChange(bool modifyTransform, bool reCheck)
{
	//YO
	/*if (modifyTransform)
	{*/

	if (modifyTransform)
	{
		translationMatrix = Matrix(1);
		translationMatrix = Matrix::Translate(Vector3(-transform.position.x, -transform.position.y, -transform.position.z));

		rotationMatrix = Matrix::Rotate(Vector3(transform.rotation.x, transform.rotation.y, transform.rotation.z + 180.0f));

		finalMatrix = translationMatrix * rotationMatrix * -translationMatrix;
	}

	if (indexVertices.size() != 0)
	{
		DrawCall& d = *RendererCore::FindDrawCall(SceneBelongsTo, DWBelongsTo);

		int texCount = 0;
		for (int i = 0; i < vertices.size(); i++)
		{
			Vector4 finalPosition;
			int xmodel = i * 4,
				ymodel = i * 4 + 1;

			if (d.type != DrawCall::Type::UI) finalPosition = finalMatrix * Vector4(transform.position.x + modelVertexData[xmodel] * transform.scale.x, transform.position.y + modelVertexData[ymodel] * transform.scale.y, transform.position.z, 1.0f);
			else {
				// Scale 1:1px
				Vector2 _corner = Vector2(-RendererCore::WindowWidth() / 2, -RendererCore::WindowHeight() / 2);

				//// Making your life easier
				//// If an anchor axis equals on min and max, it converts the position, size and pivot to a UI Box
				//// If not, it use the corresponding UI Box sides
				if (uitransform.anchors.min.x == uitransform.anchors.max.x) {
					uitransform.left = uitransform.position.x - (uitransform.pivot.x * uitransform.size.x);
					uitransform.right = uitransform.position.x + uitransform.size.x;
				}
				if (uitransform.anchors.min.y == uitransform.anchors.max.y) {
					uitransform.bottom = uitransform.position.y - (uitransform.pivot.y * uitransform.size.y);
					uitransform.top = uitransform.position.y + uitransform.size.y;
				}

				//// Converting percentage to pixels
				float widthRatio = ((float)RendererCore::WindowWidth() / (float)d.uiWidth) * ((float)d.uiHeight / (float)RendererCore::WindowHeight());
				//float heightRatio = 1;

				Vector4 fixedAnchors = Vector4(
					ClampRange(uitransform.anchors.min.x - 0.5f, -0.5f, uitransform.anchors.max.x - 0.5f) * (float)RendererCore::WindowWidth(),
					ClampRange(uitransform.anchors.min.y - 0.5f, -0.5f, uitransform.anchors.max.y - 0.5f) * (float)RendererCore::WindowHeight(),
					ClampRange(uitransform.anchors.max.x - 0.5f, -0.5f, 0.5f) * (float)RendererCore::WindowWidth(),
					ClampRange(uitransform.anchors.max.y - 0.5f, -0.5f, 0.5f) * (float)RendererCore::WindowHeight()
				);

				Vector4 fixedBox = Vector4(
					fixedAnchors.x + uitransform.left,
					fixedAnchors.y + uitransform.bottom,
					fixedAnchors.z + uitransform.top,
					fixedAnchors.w + uitransform.right
				);


				// Now time to convert it to position and scale
				Vector4 posAndScale = Vector4(
					fixedBox.x, // position.x
					fixedBox.y, // position.y
					fixedBox.w - fixedBox.y, // scale.y
					fixedBox.z - fixedBox.x // scale.x
				);

				// Replace this to local variables if you want
				Vector4 pos = Vector4(posAndScale.x, posAndScale.y, transform.position.z, 1.0f);
				Vector4 sca = Vector4(fabs(posAndScale.z), fabs(posAndScale.w), transform.scale.z, transform.scale.w);

				//std::cout << pos.x << " | " << pos.y << " | " << sca.x << " | " << sca.y << std::endl;
				//std::cout << RendererCore::WindowWidth() << " | " << RendererCore::WindowHeight() << " | " << d.uiWidth << " | " << d.uiHeight << std::endl;

				// And that's all
				finalPosition = finalMatrix * Vector4(pos.x + modelVertexData[xmodel] * sca.x, pos.y + modelVertexData[ymodel] * sca.y, transform.position.z, 1.0f);
				//std::cout << finalPosition.x << " | " << finalPosition.y << std::endl;

				/*switch (uiPosition) {
					case Center:
						finalPosition = finalMatrix * Vector4(position.x + modelVertexData[xmodel] * scale.x, position.y + modelVertexData[ymodel] * scale.y, position.z, 1.0f);
						break;
					case Left:
						Vector3 pos = position;

						// Nacho: Hacelo aca xd
						// Roboto: Haceme esta XDDDDDDDDDDDDDDDDDDDDDD

						finalPosition = finalMatrix * Vector4(pos.x + modelVertexData[xmodel] * scale.x, position.y + modelVertexData[ymodel] * scale.y, position.z, 1.0f);
						break;
				}*/
			}

			vertices[i].modelId = scriptId;
			vertices[i].color = glm::vec4(color.r, color.g, color.b, color.a);
			vertices[i].position = glm::vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);

			d.allVerts[indexVertices[i]].modelId = vertices[i].modelId;
			d.allVerts[indexVertices[i]].color = vertices[i].color;
			d.allVerts[indexVertices[i]].position = vertices[i].position;

			if (texture != nullptr)
			{
				if (texture->width != 0 || texture->height != 0)
				{
					d.allVerts[indexVertices[i]].textureGroupId = texture->texGroupId;

					float x = (float)texture->finalRect.x,
						y = (float)texture->finalRect.y,
						width = (float)texture->finalRect.x + (float)texture->finalRect.width,
						height = (float)texture->finalRect.y + (float)texture->finalRect.height;

					if (flipXTexture)
						std::swap(x, width);

					if (flipYTexture)
						std::swap(y, height);

					switch (texCount)
					{
					case 0:
						d.allVerts[indexVertices[i]].texCoords =
							glm::vec2(x / (float)TextureManager::textureGroups[texture->texGroupId].width,
								y / (float)TextureManager::textureGroups[texture->texGroupId].height);

						texCount++;
						break;
					case 1:
						d.allVerts[indexVertices[i]].texCoords =
							glm::vec2(x / (float)TextureManager::textureGroups[texture->texGroupId].width,
								height / (float)TextureManager::textureGroups[texture->texGroupId].height);

						texCount++;
						break;
					case 2:

						d.allVerts[indexVertices[i]].texCoords =
							glm::vec2(width / (float)TextureManager::textureGroups[texture->texGroupId].width,
								height / (float)TextureManager::textureGroups[texture->texGroupId].height);

						texCount++;
						break;
					case 3:
						d.allVerts[indexVertices[i]].texCoords =
							glm::vec2(width / (float)TextureManager::textureGroups[texture->texGroupId].width,
								y / (float)TextureManager::textureGroups[texture->texGroupId].height);

						texCount = 0;
						break;
					}
				}
				else
				{
					d.allVerts[indexVertices[i]].textureGroupId = -1;
				}

				/*std::cout << "Verts: " << d.allVerts[indexVertices[i]].texCoords.x << " " << d.allVerts[indexVertices[i]].texCoords.y << std::endl;
				std::cout << "Texture Coords: " << texture->finalRect.x << " " << texture->finalRect.y << std::endl;*/
			}
			else
			{
				d.allVerts[indexVertices[i]].textureGroupId = -1;
			}

			RendererCore::ModifyVerticesOnBuffer(indexVertices, reCheck, d);
		}
	}

	//std::cout << "Changed! " << RendererCore::allVerts[indexVertices[0]].modelId << std::endl;
//}
//else
//{
//	if (indexVertices.size() != 0)
//	{
//		for (int i = 0; i < vertices.size(); i++)
//		{
//			vertices[i].color = color;

//			RendererCore::allVerts[indexVertices[i]].color = vertices[i].color;
//			//RendererCore::allVerts[indexVertices[i]].position = vertices[i].position;

//			RendererCore::ModifyVerticesOnBuffer(indexVertices, reCheck);
//		}
//	}
//}
}

//Skunks <3
