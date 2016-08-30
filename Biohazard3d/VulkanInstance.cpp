#include "VulkanInstance.hpp"


using namespace bhd;

VulkanInstance::VulkanInstance()
{
};

VkResult VulkanInstance::init()
{
	release();

	auto result = validation();
	if (result != VK_SUCCESS) return result;

	VulkanPPMaker ppExtensions{ extensions };
	VulkanPPMaker ppLayer{ layers };

	instanceCreateInfo.enabledLayerCount = ppLayer.count();
	instanceCreateInfo.ppEnabledLayerNames = ppLayer;
	instanceCreateInfo.enabledExtensionCount = ppExtensions.count();
	instanceCreateInfo.ppEnabledExtensionNames = ppExtensions;

	return vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

}
void VulkanInstance::release()
{
	if(instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(instance, nullptr);
	}
	instance = VK_NULL_HANDLE;
}
VkResult VulkanInstance::validation()
{
	VkResult avaiblable = VK_SUCCESS;
	BHD_LOG_NEW_SECTION
	BHD_LOG("Extension Validation :");
	BHD_LOG_PUSH;
	if (!checkAvailability(extensions, getAvailableExtensions()))	//Check if all required extension are available
	{
		avaiblable = VK_ERROR_LAYER_NOT_PRESENT;
		BHD_THROW_WITH_LOG("Some layers requests are not available!")
	}
	BHD_LOG_POP;

	if (!layers.empty())											//Check if all required layers are available
	{
		BHD_LOG("Layer Validation :");
		BHD_LOG_PUSH;
		if (!checkAvailability(layers, getAvailableLayers()))
		{
			avaiblable = VK_ERROR_EXTENSION_NOT_PRESENT;
			BHD_THROW_WITH_LOG("Some layers requests are not available!")
		}
		BHD_LOG_POP;
	}
	return avaiblable;
}

std::vector<std::string> VulkanInstance::getAvailableExtensions()
{
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	std::vector<VkExtensionProperties> extensions(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
	std::vector<std::string> extensionNames;
	extensionNames.reserve(count);
	for (const auto& ext : extensions) {
		extensionNames.push_back((const char*)ext.extensionName);
	}
	return std::move(extensionNames);
}

std::vector<std::string> VulkanInstance::getAvailableLayers()
{
	uint32_t count;
	vkEnumerateInstanceLayerProperties(&count, nullptr);
	std::vector<VkLayerProperties> layers(count);
	vkEnumerateInstanceLayerProperties(&count, layers.data());
	std::vector<std::string> layerNames;
	layerNames.reserve(count);
	for (const auto& layer : layers) {
		layerNames.push_back((const char*)layer.layerName);
	}
	return std::move(layerNames);
}


void VulkanInstance::info()
{
	BHD_LOG_NEW_SECTION
	BHD_LOG("About Vulkan:");
	BHD_LOG_PUSH;
		
	auto extensionNames = getAvailableExtensions();
	BHD_LOG_LIST("Extensions:", extensionNames);

	auto layerNames = getAvailableLayers();
	BHD_LOG_LIST("Layers:", extensionNames);

	BHD_LOG_POP;
}