#ifndef _BHD_VULKANSHADER_H
#define _BHD_VULKANSHADER_H

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "vulkan/vulkan.h"

namespace bhd
{

	class VulkanShader
	{
	public:

		VulkanShader(VkDevice _device, const std::string & filename, VkShaderStageFlagBits stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT, const std::string & invok_fct = "main") :
			device(_device) 
		{
			loadFromFile(filename);
			createShader(stageFlag, invok_fct);
		}
		VulkanShader(VkDevice _device, const std::vector<char> & rawCode, VkShaderStageFlagBits stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT, const std::string & invok_fct = "main") :
			device(_device)
		{
			loadFromRaw(rawCode);
			createShader(stageFlag, invok_fct);
		}

		VulkanShader(VulkanShader && _shader);
		VulkanShader(const VulkanShader&) = delete;
		VulkanShader& operator=(const VulkanShader&) = delete;

		~VulkanShader() { release(); };

		//Load the shader code from a file (fill sharderCode and sharderCodeSize with the appropriate data and size)
		void loadFromFile(const std::string & filename);

		//Load the shader code from a raw data (fill sharderCode and sharderCodeSize with the appropriate data and size)
		void loadFromRaw(const std::vector<char> & rawCode);

		//Create a shader module from shaderCode 
		//(sharderCode are released)
		void createShader(VkShaderStageFlagBits stageFlag, const std::string & invok_fct = "main");

		//release all shader stuffs
		void release();



	public:
		//Inline functions

		void SetDevice(VkDevice _device) { device = _device; }
		VkShaderModule getShaderModule() { return shaderModule; }

		//Overload the create shader function
		void createFragmentShader(const std::string & invok_fct = "main") { createShader(VK_SHADER_STAGE_FRAGMENT_BIT, invok_fct); }
		void createVertexShader(const std::string & invok_fct = "main") { createShader(VK_SHADER_STAGE_VERTEX_BIT, invok_fct); }
		void createGeometryShader(const std::string & invok_fct = "main") { createShader(VK_SHADER_STAGE_GEOMETRY_BIT, invok_fct); }

		operator VkPipelineShaderStageCreateInfo() const {
			return shaderStageInfo;
		}

	private:
		//Vulkan variables

		VkDevice device = VK_NULL_HANDLE;
		VkShaderModule shaderModule = VK_NULL_HANDLE;
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		std::string invokeFunction = "main";

	private:
		//Tmp variables used to create the vulkan shader

		std::vector<uint32_t> sharderCode;	//raw data (can contain some zero padding at the end)
		std::size_t sharderCodeSize = 0;		//the useful data size (without the zero padding)


#ifdef _DEBUG
		std::string debug_filename;
#endif

	};
}
#endif

