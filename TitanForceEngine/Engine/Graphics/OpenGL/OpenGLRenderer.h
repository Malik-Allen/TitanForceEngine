#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "../../RenderCore/Renderer.h"

#include <glad/glad.h>

class OpenGLRenderer : public IRenderer
{
public:

	OpenGLRenderer();
	~OpenGLRenderer();

	// Initializes Renderer
	virtual bool OnCreate(
		const char* applicationName,
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window ) override final;
	virtual void OnDestroy() override final;

	virtual void RenderScene( IScene* scene ) override final;

private:

	virtual void BeginScene( IScene* scene ) override final;
	virtual void EndScene() override final;

	virtual void Begin() override final;
	virtual void Present() override final;
	virtual void End() override final;

	virtual void SubmitModel( Model* model ) override final;

	void GetInstalledOpenGLInfo( int* major, int* minor );

	

};


#endif // !OPENGLRENDERER_H

