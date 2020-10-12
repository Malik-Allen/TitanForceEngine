#ifndef DESCRIPTORSET_H
#define DESCRIPTORSET_H

#include <vulkan.h>


namespace Vulkan {

	class DescriptorSet {

	public:

		DescriptorSet(const DescriptorSet&) = delete;
		DescriptorSet& operator=(const DescriptorSet&) = delete;
		DescriptorSet(DescriptorSet&&) = delete;
		DescriptorSet& operator = (DescriptorSet&&) = delete;

		DescriptorSet(VkPipelineBindPoint bindPoint);
		~DescriptorSet();

		void OnCreate();
		void OnDestroy();

		void BindDescriptor(class CommandBuffer* commandBuffer) const;

	private:

		VkDescriptorSet descriptorSet;
		VkDescriptorPool descriptorPool;
		VkPipelineLayout pipelineLayout;
		VkPipelineBindPoint pipelineBindPoint;


	};


}

#endif // !DESCRIPTORSET_H

