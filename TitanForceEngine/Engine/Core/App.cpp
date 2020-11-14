#include "App.h"

#include "../Graphics/OpenGL/OpenGLRenderer.h"
#include "../Graphics/Vulkan/VulkanRenderer.h"

bool App::CreateRenderer( 
    const char* engineName,
    int version,
    bool enableValidationLayers,
    Window* window)
{

    switch ( m_graphicsAPI )
    {
    case GraphicsAPI::OpenGL:

        m_renderer = new OpenGLRenderer();

        break;
    case GraphicsAPI::Vulkan:

        m_renderer = new VulkanRenderer();

        break;

    case GraphicsAPI::Metal:
        break;

    default:
        break;
    }

    if ( m_renderer == nullptr )
    {
        return false;
    }
    else
    {
        m_renderer->OnCreate( m_appName, engineName, version, enableValidationLayers, window );

        return true;
    }

}
