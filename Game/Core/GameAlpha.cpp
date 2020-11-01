#include "GameAlpha.h"

#include "../../Engine/Debug/Debug.h"

#include "../../Engine/Rendering/3D/Mesh.h"

#include "../../Engine/Core/ObjLoader.h"

#include "../../Engine/Vulkan/VulkanRenderer.h"



GameAlpha::GameAlpha() :
	GameInterface(),
	sceneInterface(nullptr),
	currentSceneNum(0),
	m_camera(nullptr),
	test(nullptr),
	objLoader(nullptr)
{}

GameAlpha::~GameAlpha() {}

bool GameAlpha::OnCreate() {
	Debug::Info("Created Game!", __FILE__, __LINE__);

	m_camera = new Camera();

	objLoader = new ObjLoader();

	objLoader->LoadModel( "meshes/F-16C.obj");

	SubMesh subMesh;

	subMesh.vertexList = objLoader->GetVerts();

	subMesh.meshIndices = objLoader->GetIndices();


	Mesh* mesh = new Mesh( subMesh, Vulkan::VulkanRenderer::GetInstance()->GetGraphicsCommandBuffer() );
	
	mesh->GenerateBuffers();

	test = new DemoObject( mesh );


	return true;
}

void GameAlpha::Update(const float deltaTime) {}

void GameAlpha::Render() {

	test->Render( m_camera );
}

void GameAlpha::BuildScene() {}