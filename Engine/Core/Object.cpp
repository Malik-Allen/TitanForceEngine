#include "Object.h"


DemoObject::DemoObject( Mesh* mesh_ ) :
	mesh( mesh_ )
{}

DemoObject::~DemoObject() {}

bool DemoObject::OnCreate() { return true; } /// Just a stub
void DemoObject::OnDestroy() {}				  /// Just a stub
void DemoObject::Update( float deltaTime_ ) {} /// Just a stub

void DemoObject::Render(Camera* camera) const
{

	/// This is some fancy code.  Assigning a 4x4 matrix to a 3x3 matrix
	/// just steals the upper 3x3 of the 4x4 and assigns thoses values 
	/// to the 3x3.  
	Kobe::Matrix3 normalMatrix = Kobe::Math::Transpose( Kobe::Math::Inverse( modelMatrix ) );

	mesh->Render( camera, modelMatrix );

	
}

void DemoObject::HandleEvents( ) {} /// Just a stub