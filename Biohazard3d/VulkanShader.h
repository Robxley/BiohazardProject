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
		VulkanShader(VkDevice _device = VK_NULL_HANDLE) : device(_device){}
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


	private:
		//Vulkan variables

		VkDevice device;
		VkShaderModule shaderModule;
		VkPipelineShaderStageCreateInfo shaderStageInfo;
		
	private:
		//Tmp variables used to create the vulkan shader

		std::vector<uint32_t> sharderCode;	//raw data (can contain some zero padding at the end)
		std::size_t sharderCodeSize;		//the useful data size (without the zero padding)
		

#ifdef _DEBUG
		std::string debug_filename;
#endif

	};
}
#endif

