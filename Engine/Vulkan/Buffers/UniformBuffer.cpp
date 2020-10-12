#include "UniformBuffer.h"

namespace Vulkan {

	UniformBuffer::UniformBuffer(uint64_t size, void* data) :
		Buffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, data)
	{}


	UniformBuffer::~UniformBuffer() {}


	void UniformBuffer::OnCreate() {}


	void UniformBuffer::OnDestroy() {}


}