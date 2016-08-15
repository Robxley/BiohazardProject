#include "VulkanInstance.hpp"


using namespace bhd;

VulkanInstance::VulkanInstance()
{
	create = [&](VkInstance & instance) {

		auto result = validation();
		if (result != VK_SUCCESS) return result;

		PPVulkanMaker ppExtensions{ extensions };
		PPVulkanMaker ppLayer{ layers };

		instanceCreateInfo.enabledLayerCount = ppLayer.count();
		instanceCreateInfo.ppEnabledLayerNames = ppLayer;
		instanceCreateInfo.enabledExtensionCount = ppExtensions.count();
		instanceCreateInfo.ppEnabledExtensionNames = ppExtensions;

		return vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
	};
	destroy = [&](VkInstance instance) { vkDestroyInstance(instance, nullptr); };
};


VkResult VulkanInstance::validation()
{
	VkResult avaiblable = VK_SUCCESS;
	BHD_LOG("Extension Validation :");
	if (!checkAvailability(extensions, getAvailableExtensions()))	//Check if all required extension are available
	{
		avaiblable = VK_ERROR_LAYER_NOT_PRESENT;
		BHD_THROW_WITH_LOG("Some layers requests are not available!")
	}


	if (!layers.empty())											//Check if all required layers are available
	{
		BHD_LOG("Layer Validation :");
		if (!checkAvailability(layers, getAvailableLayers()))
		{
			avaiblable = VK_ERROR_EXTENSION_NOT_PRESENT;
			BHD_THROW_WITH_LOG("Some layers requests are not available!")
		}
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
	BHD_LOG("About Vulkan:");
	BHD_LOG_PUSH
		auto print = [](const std::vector<std::string> & names)
	{
		BHD_LOG_PUSH
			for (auto & name : names) {
				BHD_LOG(name)
			}
		BHD_LOG(std::endl);
		BHD_LOG_POP
	};

	{
		BHD_LOG("\tExtensions:");
		auto extensionNames = getAvailableExtensions();
		print(extensionNames);
	}

	{
		BHD_LOG("\tLayers:");
		auto layerNames = getAvailableLayers();
		print(layerNames);
	}

	BHD_LOG_POP
}