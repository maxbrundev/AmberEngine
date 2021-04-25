#pragma once

#include <AmberEngine/Core/Context.h>
#include <AmberEngine/Core/Editor.h>
#include <AmberEngine/Core/ALayer.h>

#include <AmberEngine/Core/UIManager.h>
#include <AmberEngine/Resources/AssimpModel.h>
#include <AmberEngine/Resources/AssimpParser.h>

class ExampleLayer : public AmberEngine::Core::ALayer
{
public:
	ExampleLayer(AmberEngine::Core::Context& p_context, AmberEngine::Core::Editor& p_editor);
	~ExampleLayer() = default;


	void OnStart() override;
	void OnUpdate(float p_deltaTime) override;
	void OnDestroy() override;

private:
	//TODO: Remove + pass by Scene Manager + Model should be a mesh..
	glm::vec3 lighDir = glm::vec3(1, 1, 1);
	AmberEngine::Resources::AssimpModel* model;
	AmberEngine::Core::UIManager ui;

	// Quick and Dirty Test Voxelizer.
	std::vector<unsigned int> vertexArrays;
	std::vector<unsigned int> meshIndices;

	static AmberEngine::Resources::AssimpParser PARSER;
};

