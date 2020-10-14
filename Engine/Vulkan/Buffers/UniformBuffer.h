#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <vulkan.h>

#include "Buffer.h"

namespace Vulkan{


	class UniformBuffer : public Buffer {

	public:

		UniformBuffer(uint64_t size, void* data = nullptr);

		~UniformBuffer();

		void UpdateDescriptorSet(VkDescriptorType type, VkPipelineBindPoint pipelineBindPoint);


	private:

		VkDescriptorBufferInfo bufferInfo = {};
		VkWriteDescriptorSet descriptorWrite = {};

		class DescriptorSet* descriptorSet;


	};


}


#endif // !UNIFORMBUFFER_H
