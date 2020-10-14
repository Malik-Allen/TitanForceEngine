#ifndef DESCRIPTORPOOL_H
#define DESCRIPTORPOOL_H

#include <vulkan.h>

namespace Vulkan {

	class DescriptorPool {

	public:

		DescriptorPool(const DescriptorPool&) = delete;
		DescriptorPool& operator=(const DescriptorPool&) = delete;
		DescriptorPool(DescriptorPool&&) = delete;
		DescriptorPool& operator=(DescriptorPool&&) = delete;

		DescriptorPool(VkDescriptorType type, uint32_t descriptorCount);
		~DescriptorPool();

		void OnCreate(VkDescriptorType type, uint32_t descriptorCount);
		void OnDestroy();

		const VkDescriptorPool& GetVkDescriptorPool() const { return descriptorPool; }


	private:

		VkDescriptorPool descriptorPool;


	};



}

#endif // !DESCRIPTORPOOL_H

