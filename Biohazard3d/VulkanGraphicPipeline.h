
#ifndef _BHD_VULKANGRAPHICPIPELINE_H
#define _BHD_VULKANGRAPHICPIPELINE_H

#pragma once

#include <vector>
#include "vulkan\vulkan.h"


namespace bhd
{
	class VulkanGraphicPipeline
	{
	public:
		VulkanGraphicPipeline(VkDevice _device) : device(_device) {};
		~VulkanGraphicPipeline() { release(); }

		void initVertexInput();

		void initInputAssembly();

		void initViewport(float width, float height, float x = 0.0f, float y = 0.0f, float minDepth = 1.0f, float maxDepth = 1.0f);

		void initRasterizer();

		void initMultisampling();

		void initDepthAndStencilTesting();

		void initColorBlending();

		void initDynamicState();

		void initLayout();

		void initRenderPass(VkFormat format);

		void createGraphicPipeline(std::vector<VkPipelineShaderStageCreateInfo> & shaderStages);


		void init(float width, float height)
		{
			initVertexInput();
			initInputAssembly();
			initViewport(width, height);
			initRasterizer();
			initMultisampling();
			initDepthAndStencilTesting();
			initColorBlending();
			initDynamicState();
			initLayout();
		}

		void release();

	private:
		//Render pass states
		//--------------------------------------------

		VkAttachmentDescription colorAttachment = {};
		VkAttachmentReference colorAttachmentRef = {};
		VkSubpassDescription subpass = {};
		VkRenderPassCreateInfo renderPassInfo = {};
		VkRenderPass renderPass;



		//Pipeline states
		//--------------------------------------------

		//vertex input states
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

		//Input assembly states
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};

		//Viewport stuffs
		VkViewport viewport = {};
		VkRect2D scissor = {};
		VkPipelineViewportStateCreateInfo viewportState = {};

		//Rasteriser
		VkPipelineRasterizationStateCreateInfo rasterizer = {};

		//Multisample States
		VkPipelineMultisampleStateCreateInfo multisampling = {};

		//Blending States
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		VkPipelineColorBlendStateCreateInfo colorBlending = {};

		//Layout States
		VkPipelineLayout pipelineLayout;
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};


		//Dynamic states
		std::vector<VkDynamicState> dynamicStates;
		VkPipelineDynamicStateCreateInfo dynamicState = {};

		
		VkPipeline graphicsPipeline;
	private:
		VkDevice device;

	};
}

#endif
