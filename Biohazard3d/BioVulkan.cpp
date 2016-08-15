#include "BioVulkan.hpp"
#include "VulkanTools.hpp"

using namespace bhd;

std::vector<std::string> BioVulkan::getAvailableExtensions()
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

std::vector<std::string> BioVulkan::getAvailableLayers()
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

VkResult BioVulkan::init()
{
#ifdef _glfw3_h_
	if (glfwVulkanSupported() != GLFW_TRUE)
	{
		BHD_THROW_WITH_LOG("Vuklan is not available with GLFW" << std::endl);
	}
	else
	{
		BHD_LOG("Vuklan is available with GLFW" << std::endl);
	}
#endif

	BHD_LOG("Checking required Vulkan stuffs:");

	//Check if all required extension are available
	BHD_LOG("Extension Validation :");
	if (!checkAvailability(extensions, getAvailableExtensions()))
	{
		BHD_THROW_WITH_LOG("Some layers requests are not available!")
	}

	//Check if all required layers are available
	if (!layers.empty())
	{
		BHD_LOG("Layer Validation :")
		if(!checkAvailability(layers, getAvailableLayers()))
		{BHD_THROW_WITH_LOG("Some layers requests are not available!") }
	}



	//vector<string> to const char**
	PPVulkanMaker ppExtensions{ extensions };
	PPVulkanMaker ppLayer{ layers };

	//Instance Create info struct
	VkInstanceCreateInfo instanceCreateInfo = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,				//sType
		nullptr,											//pNext
		0,													//flags
		&appInfo,											//pApplicationInfo
		ppLayer.count(),									//enabledLayerCount
		ppLayer,											//ppEnabledLayerNames
		ppExtensions.count(),								//enabledExtensionCount
		ppExtensions										//ppEnabledExtensionNames
	};

	BHD_LOG("Vulkan Create Instance.")
	if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS)
	{
		BHD_THROW_WITH_LOG("Failed to create instance!");
	}
	return VK_SUCCESS;
}


void BioVulkan::info()
{
	BHD_LOG("About Vulkan:");
	
	auto print = [](const std::vector<std::string> & names)
	{
		for (auto & name : names) {
			BHD_LOG("\t\t"<<name)
		}
		BHD_LOG(std::endl);
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

	BHD_LOG(std::endl);
}

bool BioVulkan::checkAvailability(const std::vector<std::string> & exts, const std::vector<std::string> & vulkanExts)
{
	bool availability = true;
	for (const auto & ext : exts)
	{
		bool isHere = false;
		for (const auto & vext : vulkanExts)
		{
			if (std::string(ext).compare(std::string(vext)) == 0)
			{
				BHD_LOG("\t"<<ext<<": OK");
				isHere = true;
				continue;
			}
		}
		if (!isHere)
		{
			BHD_LOG_WARNING(ext << ": Not available");
			availability = false;
		}
	}
	return availability;
}

#ifdef _glfw3_h_
 std::vector<std::string> BioVulkan::getGlfwRequiredInstanceExtensions()
{
	uint32_t n = 0;
	std::vector<std::string> exts;
	if (glfwInit() != GLFW_TRUE) { BHD_THROW_WITH_LOG("Failed on glfwInit"); }
	const char ** glwfexts = glfwGetRequiredInstanceExtensions(&n);
	exts.reserve(n + 1);
	BHD_LOG("GLFW required instance extensions : ");
	for (uint32_t i = 0; i < n; i++)
	{
		exts.push_back(glwfexts[i]);
		BHD_LOG("\t" << glwfexts[i]);
	}

	BHD_LOG("Debug extension : ");
	BHD_LOG("\t" << VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	exts.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	return std::move(exts);
}
#endif