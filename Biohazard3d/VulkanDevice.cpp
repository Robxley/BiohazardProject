#include "VulkanDevice.hpp"

using namespace bhd;


std::vector<VkPhysicalDevice> VulkanDevices::inquireAvailablePhysicalDevices(VkInstance instance)
{
	BHD_ASSERT(instance != VK_NULL_HANDLE);
	uint32_t count = 0;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(count);
	vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());
	return std::move(physicalDevices);
}

std::vector<VkPhysicalDeviceProperties> VulkanDevices::inquirePhysicalDeviceProperties(const std::vector<VkPhysicalDevice> & devices)
{
	BHD_ASSERT(!devices.empty());
	std::vector<VkPhysicalDeviceProperties> properties(devices.size());
	for (int i = 0; i < devices.size(); i++)
		vkGetPhysicalDeviceProperties(devices[i], &properties[i]);
	return std::move(properties);
}

std::vector<std::string> VulkanDevices::inquirePhysicalDeviceNames(const std::vector<VkPhysicalDeviceProperties> & properties)
{
	BHD_ASSERT(!properties.empty());
	std::vector<std::string> names;
	names.reserve(properties.size());
	for (auto & one : properties)
		names.push_back(one.deviceName);
	return names;
}

//Return a vector of all features for each physical devices
std::vector<VkPhysicalDeviceFeatures> VulkanDevices::inquirePhysicalDeviceFeatures(const std::vector<VkPhysicalDevice> & devices)
{
	BHD_ASSERT(!devices.empty());
	std::vector<VkPhysicalDeviceFeatures> features(devices.size());
	for (int i = 0; i < devices.size(); i++)
		vkGetPhysicalDeviceFeatures(devices[i], &features[i]);
	return std::move(features);
}

std::vector<QueueFamilyProperties> VulkanDevices::inquirePhysicalDeviceQueueFamilyProperties(const std::vector<VkPhysicalDevice> & devices)
{
	BHD_ASSERT(!devices.empty());
	std::vector<QueueFamilyProperties> queues(devices.size());
	for (int i = 0; i < devices.size(); i++)
	{
		auto & device = devices[i];
		auto & queue = queues[i];
		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
		queue.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, &queue[0]);
	}
	return std::move(queues);
}

//Inquire physical Device extension names
std::vector<ExtensionProperties> VulkanDevices::inquirePhysicalDeviceExtensionProperties(const std::vector<VkPhysicalDevice> & devices)
{
	BHD_ASSERT(!devices.empty());
	std::vector<ExtensionProperties> deviceExtensionProperties(devices.size());
	for (int i = 0; i < devices.size(); i++)
	{
		auto & device = devices[i];
		auto & extensions = deviceExtensionProperties[i];
		uint32_t count = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
		extensions.resize(count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data());
	}
	return std::move(deviceExtensionProperties);
}

std::vector<ExtensionNames> VulkanDevices::inquirePhysicalDeviceExtensionNames(const std::vector<ExtensionProperties> & extensionProperties)
{
	BHD_ASSERT(!extensionProperties.empty());
	std::vector<ExtensionNames> deviceExtensionNames; 
	deviceExtensionNames.resize(extensionProperties.size());
	int i = 0;
	for (const auto & oneDevice : extensionProperties)
	{
		auto & extensionNames = deviceExtensionNames[i++];
		extensionNames.reserve(oneDevice.size());
		for (const auto & properties : oneDevice){
			extensionNames.push_back(properties.extensionName);
		}
	}
	return deviceExtensionNames;
}

std::vector<SwapChainFeatures> VulkanDevices::inquirePhysicalDeviceSwapChainFeatures(const std::vector<VkPhysicalDevice> & devices, VkSurfaceKHR surface)
{
	BHD_ASSERT(!devices.empty());
	BHD_ASSERT(surface != VK_NULL_HANDLE);

	std::vector<SwapChainFeatures> swapChainFeatures(devices.size());

	//for each device
	std::size_t i = 0;
	for (const auto & device : devices)
	{
		//Inquire the Surface capabilities
		auto & scf = swapChainFeatures[i++];
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &scf.surfaceCapabilities);

		//Inquire the surface formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			scf.surfaceFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, scf.surfaceFormats.data());
		}

		//Inquire the surface present modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			scf.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, scf.presentModes.data());
		}

	}

	return std::move(swapChainFeatures);

}


VkBool32 VulkanDevices::inquirePhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32_t queueIndex, VkSurfaceKHR surface)
{
	BHD_ASSERT(queueIndex >= 0);
	BHD_ASSERT(surface != VK_NULL_HANDLE);
	VkBool32 support = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueIndex, surface, &support);
	return support;
}

PhysicalDeviceStuffs VulkanDevices::pickByScore(std::function<int(const PhysicalDeviceStuffs &, VkSurfaceKHR, std::vector<std::string>)> scoreMaker, VkSurfaceKHR surface, std::vector<std::string> extensionNames, int * score) const
{
	std::map<int, PhysicalDeviceStuffs> physicalDeviceScores;
	auto count = physicalDeviceStuffs.count();
	for (auto i = 0; i < count; i++)
	{
		PhysicalDeviceStuffs deviceStuff = physicalDeviceStuffs[i];
		int score = scoreMaker(deviceStuff, surface, extensionNames);
		physicalDeviceScores.insert(std::pair<int, PhysicalDeviceStuffs>(score, deviceStuff));
	}
	auto bestPhysicalDevice = (--physicalDeviceScores.end());

	if (score) *score = bestPhysicalDevice->first;
	return  bestPhysicalDevice->second;
}

int VulkanDevices::scoreMaker(const PhysicalDeviceStuffs & physicalDeviceStuffs, VkSurfaceKHR surface, std::vector<std::string> extensionNames)
{
	BHD_LOG("Score maker >> " << *physicalDeviceStuffs.name);
	BHD_LOG_PUSH;
	int score = 0;
	if (physicalDeviceStuffs.properties != nullptr)
	{
		auto properties = physicalDeviceStuffs.properties;
		// Discrete GPUs have a significant performance advantage
		if (properties->deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		// Maximum possible size of textures affects graphics quality
		score += properties->limits.maxImageDimension2D;
	}
	else
	{
		BHD_LOG_WARNING("The score Maker need the physical device properties!")
	}

	if (physicalDeviceStuffs.queueFamilyProperties != nullptr)
	{
		int scoreFamily = 0;
		int queueIndex = 0;
		
		const auto & queueFamilyProperties = *physicalDeviceStuffs.queueFamilyProperties;
		for (auto queueFamily : queueFamilyProperties)
		{
			VkBool32 surfaceSupport = true;
			if (surface != VK_NULL_HANDLE)
				surfaceSupport = inquirePhysicalDeviceSurfaceSupport(*physicalDeviceStuffs.physicalDevice, queueIndex, surface);

			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && surfaceSupport)
			{
				scoreFamily++;
			}
			queueIndex++;
		}
		if (scoreFamily == 0)
			score = -1;
	}
	else
	{
		BHD_LOG_WARNING("The score Maker need the queue family properties!")
	}

	if (!extensionNames.empty())
	{
		if (physicalDeviceStuffs.extensionNames!=nullptr)
		{
			if (!checkAvailability(extensionNames, *physicalDeviceStuffs.extensionNames)) //Check if all functions are availables for the device
				score = 0;
		}
		else
		{
			BHD_LOG_WARNING("The score Maker need the device extension names!")
		}
	}

	if (surface == VK_NULL_HANDLE)
	{
		BHD_LOG_WARNING("Surface is set to VK_NULL_HANDLE! - Score was evaluated without the surface")
	}

	BHD_LOG(">> Score:" << score);
	if (score == -1)
		BHD_LOG("VK_QUEUE_GRAPHICS_BIT with this surface/device is not supported");
	
	BHD_LOG_POP;
	return score;
}

VkResult VulkanDevices::createLogicalDevice(const PhysicalDeviceStuffs & physicalDeviceStuffs, VkSurfaceKHR surface, VkQueueFlags flag, std::vector<std::string> extensions, std::vector<std::string> layers)
{
	BHD_ASSERT(physicalDeviceStuffs.physicalDevice != nullptr);

	BHD_LOG_NEW_SECTION;
	BHD_LOG("Try to create a logical device...");
	BHD_LOG_PUSH;
	//Defaut queue info
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, //sType
		nullptr,									//pNext
		0,											//flags
		0,											//queueFamilyIndex
		1,											//queueCount
		&queuePriority								//pQueuePriorities
	};

	//Queue list to attach with the device
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	//Check the queue flag
	auto addQueueFamily = [&](VkQueueFlags queue, std::string info)
	{
		if (flag & queue)
		{
			int queueFamilyIndex = getQueueFamilyIndex(physicalDeviceStuffs, queue);
			if (queueFamilyIndex >= 0)
			{
				queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
				queueCreateInfos.push_back(queueCreateInfo);
				BHD_LOG("Add a queue from the family: " << info);
			}
			else
			{
				BHD_LOG_WARNING("Family queue not supported : "<< info);
			}
		}
	};
	addQueueFamily(VK_QUEUE_GRAPHICS_BIT, "VK_QUEUE_GRAPHICS_BIT");
	addQueueFamily(VK_QUEUE_COMPUTE_BIT, "VK_QUEUE_COMPUTE_BIT");
	addQueueFamily(VK_QUEUE_TRANSFER_BIT, "VK_QUEUE_TRANSFER_BIT");
	addQueueFamily(VK_QUEUE_SPARSE_BINDING_BIT, "VK_QUEUE_SPARSE_BINDING_BIT");


	if (surface != VK_NULL_HANDLE)
	{
		BHD_LOG("Check the surface support :")
		VkBool32 surfaceSupport = 0;
		//Check if the surface is supported by a queue
		for (auto queueInfo : queueCreateInfos)
		{
			if (surfaceSupport = inquirePhysicalDeviceSurfaceSupport(physicalDeviceStuffs, queueInfo.queueFamilyIndex, surface))
			{
				BHD_LOG("The surface is already supported by one queue!")
				break;
			}
		}
		
		//Find a queue family supporting the surface
		if (!surfaceSupport)
		{
			uint32_t queueFamilyIndex = 0;
			BHD_LOG("Try to find a queue family supporting the surface...")
			if (physicalDeviceStuffs.queueFamilyProperties != nullptr)
			{	
				for (auto propertie : *physicalDeviceStuffs.queueFamilyProperties)
				{
					surfaceSupport = inquirePhysicalDeviceSurfaceSupport(physicalDeviceStuffs, queueFamilyIndex, surface);
					if (surfaceSupport) break;
					queueFamilyIndex++;
				}
			}
			else
			{
				BHD_LOG_ERROR("Queue family properties is needed for that!")
			}

			if (surfaceSupport)
			{
				queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
				queueCreateInfos.push_back(queueCreateInfo);
				BHD_LOG("Surface support OK ( by the queue family with the index: " << queueFamilyIndex << ")");
			}
			else
			{
				BHD_LOG_ERROR("The surface support is not available")
			}
		}

	}

	if (!extensions.empty())
	{
		BHD_LOG("Check the extension support for this device...");
		BHD_LOG_PUSH;
		ExtensionNames extensionNames;
		if(physicalDeviceStuffs.extensionNames != nullptr)
		{
			if (!physicalDeviceStuffs.extensionNames->empty())
				extensionNames = *physicalDeviceStuffs.extensionNames;
		}
		else if (physicalDeviceStuffs.extensionProperties != nullptr)
		{
			for (const auto & one : *physicalDeviceStuffs.extensionProperties)
				extensionNames.push_back(one.extensionName);
		}
		else
		{
			auto physicalDeviceExtensionProperties = inquirePhysicalDeviceExtensionProperties({ *physicalDeviceStuffs.physicalDevice });
			if (!physicalDeviceExtensionProperties.empty())
			{
				auto extensionProperties = physicalDeviceExtensionProperties[0];
				extensionNames.reserve(extensionProperties.size());
				for (const auto & one : extensionProperties)
					extensionNames.push_back(one.extensionName);
			}
		}
		
		if (!extensionNames.empty() && checkAvailability(extensions, extensionNames))
		{
			BHD_LOG("All extensions are supported by this device - you are lucky...");
		}
		else
		{
			BHD_LOG_ERROR("Some/all extensions are not supported! - that is so sad...");
		}
		BHD_LOG_POP;
	}


	//VkPhysicalDeviceFeatures deviceFeatures;

	VkDeviceCreateInfo deviceCreationInfo;
	deviceCreationInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreationInfo.pNext = nullptr;
	deviceCreationInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreationInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
	deviceCreationInfo.pEnabledFeatures = physicalDeviceStuffs.features;
	deviceCreationInfo.flags = 0;

	VulkanPPMaker ppExtensions(extensions);
	deviceCreationInfo.enabledExtensionCount = ppExtensions.count();
	deviceCreationInfo.ppEnabledExtensionNames = ppExtensions.data();

	VulkanPPMaker ppLayers(layers);
	deviceCreationInfo.enabledLayerCount = ppLayers.count();
	deviceCreationInfo.ppEnabledLayerNames = ppLayers.data();
	
	VkResult result = vkCreateDevice(*physicalDeviceStuffs.physicalDevice, &deviceCreationInfo, nullptr, &device);
	if (result != VK_SUCCESS) {
		BHD_LOG_ERROR("Can't create the logical device - VK ERROR: "<< vkresultToString(result));
	}
	else
	{
		logicalQueues.reserve(queueCreateInfos.size());
		for (auto queueInfo : queueCreateInfos)
		{
			VkQueue queue;
			vkGetDeviceQueue(device, queueInfo.queueFamilyIndex, 0, &queue);
			logicalQueues.push_back(queue);
		}
	}

	return result;
}

int VulkanDevices::getQueueFamilyIndex(const QueueFamilyProperties & queueFamilyProperties, VkQueueFlags flag) const
{
	uint32_t index = 0;
	for (auto queueFamily : queueFamilyProperties)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & flag)
		{
			return index;
		}
		index++;
	}
	BHD_LOG_WARNING("Flag not found");
	return -1;
}
