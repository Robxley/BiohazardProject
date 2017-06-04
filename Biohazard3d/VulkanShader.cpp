#include "VulkanShader.h"
#include "Logger.hpp"

using namespace bhd;

VulkanShader::VulkanShader(VulkanShader && _shader)
{
	this->device = _shader.device;
	this->shaderModule = _shader.shaderModule;
	this->sharderCodeSize = _shader.sharderCodeSize;

	this->shaderStageInfo = _shader.shaderStageInfo;
	this->sharderCode = std::move(_shader.sharderCode);
	this->invokeFunction = std::move(_shader.invokeFunction);
	this->shaderStageInfo.pName = invokeFunction.c_str();

#ifdef _DEBUG
	this->debug_filename = std::move(_shader.debug_filename);
	_shader.debug_filename.clear();
#endif

	_shader.device = VK_NULL_HANDLE;
	_shader.shaderModule = VK_NULL_HANDLE;
	_shader.sharderCodeSize = 0;
	_shader.shaderStageInfo = {};
	_shader.sharderCode.clear();
	_shader.invokeFunction.clear();
}


void VulkanShader::loadFromFile(const std::string & filename)
{
	//Try to open the file at the end (to get the file size)
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		BHD_THROW_WITH_LOG("Can't read the sader " << filename);
	}
	sharderCodeSize = (std::size_t)file.tellg();

	//Buffer allocation in uint32_t (format used by vulkan to describe a sharder code)
	sharderCode.resize(sharderCodeSize / sizeof(uint32_t) + 1);
	file.seekg(0);

	//read the file
	file.read((char*)sharderCode.data(), sharderCodeSize);
	file.close();

#ifdef _DEBUG
	debug_filename = filename;
#endif

}


void VulkanShader::loadFromRaw(const std::vector<char> & rawCode)
{
	sharderCode.resize(rawCode.size() / sizeof(uint32_t) + 1);
	sharderCodeSize = rawCode.size();
	std::memcpy(sharderCode.data(), rawCode.data(), sharderCodeSize);
}


void VulkanShader::createShader(VkShaderStageFlagBits stageFlag, const std::string & invok_fct)
{
	if (device == VK_NULL_HANDLE) {
		BHD_THROW_WITH_LOG("failted to create the shader, the device is undefined");
	}

	invokeFunction = invok_fct;

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = sharderCodeSize;
	createInfo.pCode = sharderCode.data();

	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		const std::string msg = "failed to create shader module! ";
#ifdef _DEBUG
		BHD_THROW_WITH_LOG(msg <<debug_filename);
#else
		BHD_THROW_WITH_LOG(msg);
#endif
	}

#ifdef _DEBUG
	BHD_LOG("Shader module created : " << debug_filename);
#endif

	sharderCode.clear();
	sharderCodeSize = 0;

	//Set some info about the shader (use by the pypeline)

	shaderStageInfo = {};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = stageFlag;
	shaderStageInfo.module = shaderModule;
	shaderStageInfo.pName = invokeFunction.c_str();
}

void VulkanShader::release()
{
	if(shaderModule != VK_NULL_HANDLE)
		vkDestroyShaderModule(device, shaderModule, nullptr);

	shaderModule = VK_NULL_HANDLE;
	device = VK_NULL_HANDLE;
	sharderCode.clear();
	sharderCodeSize = 0;

	shaderStageInfo = {};
}