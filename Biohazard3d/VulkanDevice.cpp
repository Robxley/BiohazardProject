#include "VulkanDevice.hpp"

using namespace bhd;


std::vector<VkPhysicalDevice> VulkanPhysicalDevices::inquireAvailablePhysicalDevices(VkInstance instance)
{
	uint32_t count = 0;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(count);
	vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());
	return std::move(physicalDevices);
}

std::vector<VkPhysicalDeviceProperties> VulkanPhysicalDevices::inquirePhysicalDeviceProperties(const std::vector<VkPhysicalDevice> & devices)
{
	std::vector<VkPhysicalDeviceProperties> properties(devices.size());
	for (int i = 0; i < devices.size(); i++)
		vkGetPhysicalDeviceProperties(devices[i], &properties[i]);
	return std::move(properties);
}

std::vector<std::string> VulkanPhysicalDevices::inquirePhysicalDeviceNames(const std::vector<VkPhysicalDeviceProperties> & properties)
{
	std::vector<std::string> names;
	names.reserve(properties.size());
	for (auto & one : properties)
		names.push_back(one.deviceName);
	return names;
}

//Return a vector of all features for each physical devices
std::vector<VkPhysicalDeviceFeatures> VulkanPhysicalDevices::inquirePhysicalDeviceFeatures(const std::vector<VkPhysicalDevice> & devices)
{
	std::vector<VkPhysicalDeviceFeatures> features(devices.size());
	for (int i = 0; i < devices.size(); i++)
		vkGetPhysicalDeviceFeatures(devices[i], &features[i]);
	return std::move(features);
}

std::vector<QueueFamilyProperties> VulkanPhysicalDevices::inquirePhysicalDeviceQueueFamilyProperties(const std::vector<VkPhysicalDevice> & devices)
{
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



PhysicalDeviceStuffs VulkanPhysicalDevices::pickByScore(std::function<int(const PhysicalDeviceStuffs &)> scoreMaker, int * score)
{
	std::map<int, PhysicalDeviceStuffs> physicalDeviceScores;
	auto count = physicalDeviceStuffs.count();
	for (auto i = 0; i < count; i++)
	{
		PhysicalDeviceStuffs deviceStuff = physicalDeviceStuffs[i];
		int score = scoreMaker(deviceStuff);
		physicalDeviceScores.insert(std::pair<int, PhysicalDeviceStuffs>(score, deviceStuff));
	}
	auto bestPhysicalDevice = (--physicalDeviceScores.end());

	if (score) *score = bestPhysicalDevice->first;
	return  bestPhysicalDevice->second;
}

int VulkanPhysicalDevices::scoreMaker(const PhysicalDeviceStuffs & physicalDeviceStuffs)
{
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
		const auto & queueFamilyProperties = *physicalDeviceStuffs.queueFamilyProperties;
		for (auto queueFamily : queueFamilyProperties)
		{
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				scoreFamily++;
			}
		}
		if (scoreFamily == 0)
			score = 0;
	}

	return score;
}