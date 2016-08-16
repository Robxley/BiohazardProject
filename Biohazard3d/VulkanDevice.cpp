#include "VulkanDevice.hpp"

using namespace bhd;

PhysicalDeviceStuffList VulkanPhysicalDevice::physicalDeviceStuffs;

std::vector<VkPhysicalDevice> VulkanPhysicalDevice::getAvailablePhysicalDevices(VkInstance instance)
{
	uint32_t count = 0;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(count);
	vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());
	return std::move(physicalDevices);
}

std::vector<VkPhysicalDeviceProperties> VulkanPhysicalDevice::getPhysicalDeviceProperties(const std::vector<VkPhysicalDevice> & devices)
{
	std::vector<VkPhysicalDeviceProperties> properties(devices.size());
	for (int i = 0; i < devices.size(); i++)
		vkGetPhysicalDeviceProperties(devices[i], &properties[i]);
	return std::move(properties);
}

std::vector<std::string> VulkanPhysicalDevice::getPhysicalDeviceNames(const std::vector<VkPhysicalDeviceProperties> & properties)
{
	std::vector<std::string> names;
	names.reserve(properties.size());
	for (auto & one : properties)
		names.push_back(one.deviceName);
	return names;
}

//Return a vector of all features for each physical devices
std::vector<VkPhysicalDeviceFeatures> VulkanPhysicalDevice::getPhysicalDeviceFeatures(const std::vector<VkPhysicalDevice> & devices)
{
	std::vector<VkPhysicalDeviceFeatures> features(devices.size());
	for (int i = 0; i < devices.size(); i++)
		vkGetPhysicalDeviceFeatures(devices[i], &features[i]);
	return std::move(features);
}