#ifndef MODEL_H
#define MODEL_H

#include "../../Graphics/Graphics.h"
#include "../Shader/ShaderLinker.h"
#include "../Material/Material.h"
#include "../Texture/Texture.h"
#include "../../Components/TransformComponent.h"

class IMesh;
class CameraComponent;

class Model
{

public:

	Model(
		const char* objFileName, 
		const char* materialFileName, 
		ShaderLinker* shaderLinker, 
		Texture* texture, 
		TransformComponent* transformComponent
	);
	~Model();

	bool OnCreate();
	void Render( CameraComponent* camera );

private:

	// TODO:
	// Replace a single mesh reference with a dynamic array of meshes
	// OBJLoader Will need to support loading multiple meshes
	IMesh*				m_mesh;	
	Material*			m_material;
	ShaderLinker*		m_shaderLinker;
	Texture*			m_texture;
	TransformComponent*	m_transform;

	void OnDestroy();

};


#endif // !MODEL_H

