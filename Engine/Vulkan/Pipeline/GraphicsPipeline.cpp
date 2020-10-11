#include "GraphicsPipeline.h"


namespace Vulkan{


	GraphicsPipeline::GraphicsPipeline(const VkDevice& device_,
		const VkRenderPass& renderPass_,
		const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
		const std::vector<VkVertexInputBindingDescription>& vertexBinding,
		const std::vector<VkVertexInputAttributeDescription>& attributeBinding,
		const std::uint32_t windowWidth, const std::uint32_t windowHeight, const std::string& name) :
		device(device_),
		pipelineLayout(VK_NULL_HANDLE),
		renderPass(renderPass_)
	{}

	GraphicsPipeline::~GraphicsPipeline() {}

	
	void GraphicsPipeline::OnCreate(const VkDevice& device_,
		const VkRenderPass& renderPass_,
		const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
		const std::vector<VkVertexInputBindingDescription>& vertexBinding,
		const std::vector<VkVertexInputAttributeDescription>& attributeBinding,
		const uint32_t windowWidth, const uint32_t windowHeight, const std::string& name) {


		device = device_;
		renderPass = renderPass_;

		if (device == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null device passed to graphics pipeline!");
		}

		if (renderPass == VK_NULL_HANDLE) {
			throw std::runtime_error("Error! Null renderpass passed to graphics pipeline!");
		}

		CreateDescriptorSetLayout();



		// The Vertex Input Data describes the format of the vertex data that will be passed to the vertex shader
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexBinding.size());
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeBinding.size());
		vertexInputInfo.pVertexBindingDescriptions = vertexBinding.data();
		vertexInputInfo.pVertexAttributeDescriptions = attributeBinding.data();


		// SThe Input Assembly describes what kind of geometry will be drawn and if primitive restart is enabled
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;


		// Describes the region of the frame buffer that the output willl be rendered to
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)windowWidth;
		viewport.height = (float)windowHeight;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;


		// We just want to make sure that this scissor rector covers everything
		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		VkExtent2D extent = { windowWidth, windowHeight };
		scissor.extent = extent;


		// This is where we combine the scissors and the viewport, this is an opptunity to asign multiple viewports as welll
		VkPipelineViewportStateCreateInfo viewportStateInfo = {};
		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.viewportCount = 1;
		viewportStateInfo.pViewports = &viewport;
		viewportStateInfo.scissorCount = 1;
		viewportStateInfo.pScissors = &scissor;



		// The rasterizer takes gemotry that is shaped by the vertices from the vertex shader and turns it into fragmets to be coloured by the fragment shagderS
		VkPipelineRasterizationStateCreateInfo rasterizerStateInfo = {};
		rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerStateInfo.depthClampEnable = VK_FALSE;
		rasterizerStateInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizerStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizerStateInfo.lineWidth = 1.0f;
		rasterizerStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizerStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizerStateInfo.depthBiasEnable = VK_FALSE;
		rasterizerStateInfo.depthBiasClamp = 0.0f;
		rasterizerStateInfo.depthBiasConstantFactor = 0.0f;
		rasterizerStateInfo.depthBiasSlopeFactor = 0.0f;



		// Multisanpling is one way to perform anti-aliasing
		VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
		multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisamplingStateInfo.sampleShadingEnable = VK_FALSE;
		multisamplingStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisamplingStateInfo.minSampleShading = 1.0f;	//optional
		multisamplingStateInfo.pSampleMask = nullptr;	//optional
		multisamplingStateInfo.alphaToCoverageEnable = VK_FALSE;	//optional
		multisamplingStateInfo.alphaToOneEnable = VK_FALSE;	//optional



		// After the fragment shader returns a color, it needs to be combined with the color that is already in the framebuffer
			// The two ways to do this are:
				// - Mix the old and new value to produce a final color
				// - Combine the old and new value using a bitwise operation

		// Setting up the configuration per attached framebuffer 
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;


		// Setting up global color blending settings
		VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {};
		colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendingInfo.logicOpEnable = VK_FALSE;
		colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlendingInfo.attachmentCount = 1;
		colorBlendingInfo.pAttachments = &colorBlendAttachment;
		colorBlendingInfo.blendConstants[0] = 0.0f; // Optional
		colorBlendingInfo.blendConstants[1] = 0.0f; // Optional
		colorBlendingInfo.blendConstants[2] = 0.0f; // Optional
		colorBlendingInfo.blendConstants[3] = 0.0f; // Optional

		
		// A limited amoint of thhe state we have specififed in the above settings can actually be changed without recreating the pipeline, this is the seet up for that
		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

		VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount = 2;
		dynamicStateInfo.pDynamicStates = dynamicStates;


		// Now we can setting Uniform Buffer expectations
		CreatePipelineLayout();
		

		// Creating the graphics pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportStateInfo;
		pipelineInfo.pRasterizationState = &rasterizerStateInfo;
		pipelineInfo.pMultisampleState = &multisamplingStateInfo;
		pipelineInfo.pDepthStencilState = nullptr;	//Optional
		pipelineInfo.pColorBlendState = &colorBlendingInfo;
		pipelineInfo.pDynamicState = nullptr;	//Optional
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;	//Optional

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create graphics pipeline");
		}

	}


	void GraphicsPipeline::OnDestroy() {

		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

		vkDestroyPipeline(device, pipeline, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

	}

	void GraphicsPipeline::CreatePipelineLayout() {

		// Creating multiple descriptors is possible, using pools and descriptor sets *Later*
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout!");
		}

	}

	void GraphicsPipeline::CreateDescriptorSetLayout() {

		// Every uniform binding needs to be described using the DescriptorSetLayoutBinding Struct
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;	// Flagging which shader stage this uniform will take plave
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional // Relevant for image sampling *Later*

		// We need to specify descriptor set layout during pipeline creation to tell Vulkan which descriptors the shaders will be using
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}

	}

}