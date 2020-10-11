#ifndef GRAPHICSPIPELINE_H
#define GRAPGICSPIPELINE_H

#include <vulkan.h>
#include <vector>

namespace Vulkan {

	class GraphicsPipeline {

	public:

		GraphicsPipeline(const GraphicsPipeline&) = delete;
		GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
		GraphicsPipeline(GraphicsPipeline&&) = delete;
		GraphicsPipeline& operator = (GraphicsPipeline&&) = delete;



		GraphicsPipeline(const VkDevice& device_,
			const VkRenderPass& renderPass_,
			const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
			const std::vector<VkVertexInputBindingDescription>& vertexBinding,
			const std::vector<VkVertexInputAttributeDescription>& attributeBinding,
			const std::uint32_t windowWidth, const std::uint32_t windowHeight, const std::string& name);
		
		~GraphicsPipeline();

		void OnDestroy();

		const VkPipeline GetVkPipeline() const { return pipeline; }

	private:

		void OnCreate(const VkDevice& device_,
			const VkRenderPass& renderPass_,
			const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
			const std::vector<VkVertexInputBindingDescription>& vertexBinding,
			const std::vector<VkVertexInputAttributeDescription>& attributeBinding,
			const uint32_t windowWidth, const uint32_t windowHeight, const std::string& name);


		void CreatePipelineLayout();
		void CreateDescriptorSetLayout();


		VkDevice device;
		VkRenderPass renderPass;

		VkPipeline pipeline;
		VkPipelineCache pipelineCache;
		VkPipelineLayout pipelineLayout;

		VkDescriptorSetLayout descriptorSetLayout;

	};


}

#endif // !GRAPHICSPIPELINE_H
