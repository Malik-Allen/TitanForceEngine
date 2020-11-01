#ifndef OBJECT_H
#define OBJECT_H

#include "../Rendering/3D/Mesh.h"
#include"../Kobe/Math.h"

#include "Camera.h"



using namespace Kobe;


class DemoObject 
{

private:
	Kobe::Matrix4 modelMatrix;
	Mesh* mesh;

public:
	DemoObject( Mesh* mesh_);
	~DemoObject();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update( const float deltaTime_ );
	virtual void Render( Camera* camera ) const;
	virtual void HandleEvents(  );

	inline void setModelMatrix( const Kobe::Matrix4& modelMatrix_ ) { modelMatrix = modelMatrix_; }
	inline const Kobe::Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif
