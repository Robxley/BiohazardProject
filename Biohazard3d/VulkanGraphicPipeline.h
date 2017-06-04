
#ifndef _BHD_VULKANGRAPHICPIPELINE_H
#define _BHD_VULKANGRAPHICPIPELINE_H

#pragma once

#include <vector>
#include "vulkan\vulkan.h"
#include "VulkanShader.h"
#include "VulkanSwapChain.h"

namespace bhd
{
	class VulkanGraphicPipeline
	{
	public:
		VulkanGraphicPipeline(VkDevice _device = VK_NULL_HANDLE) : device(_device) {};
		~VulkanGraphicPipeline() { release(); }

		void initVertexInput();

		void initInputAssembly();

		void initViewport(float width, float height, float x = 0.0f, float y = 0.0f, float minDepth = 1.0f, float maxDepth = 1.0f);
		void initViewport(const VkExtent2D & extent2D) {
			initViewport((float)extent2D.width, (float)extent2D.height);
		}


		void initRasterizer();

		void initMultisampling();

		void initDepthAndStencilTesting();

		void initColorBlending();

		void initDynamicState();

		void initLayout();

		void initRenderPass(VkFormat format);

		void initGraphicPipeline(const VulkanSwapChain & swapChain)
		{
			initVertexInput();
			initInputAssembly();
			initViewport(swapChain);
			initRasterizer();
			initMultisampling();
			initDepthAndStencilTesting();
			initColorBlending();
			initDynamicState();
			initLayout();
			initRenderPass(swapChain);
		}

		VkResult createGraphicPipeline(VkDevice _device, const std::vector<VkPipelineShaderStageCreateInfo> & shaderStages);
		VkResult createGraphicPipeline(VkDevice _device, const std::vector<VulkanShader> & shaderStages);



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
