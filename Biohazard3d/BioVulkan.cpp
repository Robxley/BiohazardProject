#include "BioVulkan.hpp"



using namespace bhd;


std::vector<const char*>&& BioVulkan::checkInstanceExtensions()
{
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	std::vector<VkExtensionProperties> extensions(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
	std::vector<const char*> strExtensions;
	strExtensions.reserve(count);
	BHD_LOG("Vulkan available extensions ("<< count <<"):");
	for (const auto& extension : extensions) {
		BHD_LOG("\t" << extension.extensionName);
		strExtensions.push_back(extension.extensionName);
	}
	return std::move(strExtensions);
}

std::vector<const char*>&& BioVulkan::checkValidationLayers()
{
	uint32_t count;
	vkEnumerateInstanceLayerProperties(&count, nullptr);
	std::vector<VkLayerProperties> layers(count);
	vkEnumerateInstanceLayerProperties(&count, layers.data());
	std::vector<const char*> strLayers;
	strLayers.reserve(count);
	BHD_LOG("Vulkan available layers (" << count << "):");
	for (const auto& layer : layers) {
		BHD_LOG("\t" << layer.layerName);
		strLayers.push_back(layer.layerName);
	}
	return std::move(strLayers);
}

VkResult BioVulkan::init()
{
	BHD_LOG("Vulkan Create Instance.")
	if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS)
	{
		BHD_THROW_WITH_LOG("Failed to create instance!");
	}
	return VK_SUCCESS;
}
void BioVulkan::info()
{
	BHD_LOG("About Vulkan:"<< std::endl);
	checkInstanceExtensions();
	BHD_LOG(std::endl);
	checkValidationLayers();
	BHD_LOG(std::endl);
}

bool BioVulkan::checkSupport(const std::vector<const char *> & exts, const std::vector<const char *> & vulkanExts)
{
	bool allAreHere = true;
	for (const auto & ext : exts)
	{
		bool isHere = false;
		for (const auto & vext : vulkanExts)
		{
			if (std::string(ext).compare(std::string(vext)) == 0)
			{
				isHere = true;
				continue;
			}
		}
		if (!isHere)
		{
			BHD_LOG_WARNING(ext << " is not supported by vulkan");
			allAreHere = false;
		}
	}
	return allAreHere;
}