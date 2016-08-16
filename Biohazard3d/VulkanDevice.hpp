#ifndef _BHD_VULKANDEVICE_H
#define _BHD_VULKANDEVICE_H
#pragma once

#include "VulkanTools.hpp"

namespace bhd
{
	class PhysicalDeviceStuffList
	{
		friend class VulkanPhysicalDevice;
	public:
		typedef struct PhysicalDeviceStuffs
		{
			const std::string & name;
			const VkPhysicalDevice & physicalDevice;
			const VkPhysicalDeviceProperties & properties;
			const VkPhysicalDeviceFeatures & features;
		}PhysicalDeviceStuffs;

	public:

		//get physical device stuffs by index
		PhysicalDeviceStuffs getPhysicalDeviceStuffs(int i) {
			BHD_ASSERT_LOG(i < count(), "out of range");
			return { 
				physicalDeviceNames[i], 
				physicalDevices[i], 
				physicalDeviceProperties[i], 
				physicalDeviceFeatures[i] };
		}

		int count() { 
			return (int)physicalDevices.size(); 
		}

	public:

		//some useful operator
		PhysicalDeviceStuffList & operator <<(const std::vector<std::string> & names) { physicalDeviceNames = names; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<VkPhysicalDevice> & devices) { physicalDevices = devices; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<VkPhysicalDeviceProperties> & properties) { physicalDeviceProperties = properties; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<VkPhysicalDeviceFeatures> & features) { physicalDeviceFeatures = features; return *this; };

		operator const std::vector<std::string>&() { return physicalDeviceNames; }
		operator const std::vector<VkPhysicalDevice>&() { return physicalDevices; }
		operator const std::vector<VkPhysicalDeviceProperties>&() { return physicalDeviceProperties; }
		operator const std::vector<VkPhysicalDeviceFeatures>&() { return physicalDeviceFeatures; }

	protected:

		std::vector<std::string> physicalDeviceNames;
		std::vector<VkPhysicalDevice> physicalDevices;
		std::vector<VkPhysicalDeviceProperties> physicalDeviceProperties;
		std::vector<VkPhysicalDeviceFeatures> physicalDeviceFeatures;

		//Not implemented yet
		//std::vector<VkPhysicalDeviceLimits> physicalDeviceLimits;
		//std::vector<VkPhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties;
	};


	class VulkanPhysicalDevice : public VulkanObject<VkPhysicalDevice>
	{
	public:
		//Physical devices useful functions
		//--------------------------------

		//Return a vector of available physical devices
		static std::vector<VkPhysicalDevice> getAvailablePhysicalDevices(VkInstance instance);

		//Return a vector of all properties for each physical devices
		static std::vector<VkPhysicalDeviceProperties> getPhysicalDeviceProperties(const std::vector<VkPhysicalDevice> & devices);
	
		//Get all physical device names from their properties
		static std::vector<std::string> getPhysicalDeviceNames(const std::vector<VkPhysicalDeviceProperties> & properties);
	
		//Return a vector of all features for each physical devices
		static std::vector<VkPhysicalDeviceFeatures> getPhysicalDeviceFeatures(const std::vector<VkPhysicalDevice> & devices);

		static std::vector<std::string> getPhysicalDeviceNames(VkInstance instance)
		{
			return getPhysicalDeviceNames(
				getPhysicalDeviceProperties(
					getAvailablePhysicalDevices(instance)));
		}


	public:
		//Functions with a trace of physical device features
		//--------------------------------------------------

		//call getAvailablePhysicalDevices and save the result in the static variable and return a reference on it
		static const std::vector<VkPhysicalDevice> & saveAvailablePhysicalDevices(VkInstance instance){
			return physicalDeviceStuffs << getAvailablePhysicalDevices(instance);
		}

		//call getPhysicalDeviceProperties and save the result in the static variable and return a reference on it
		static const std::vector<VkPhysicalDeviceProperties> & savePhysicalDeviceProperties(const std::vector<VkPhysicalDevice> & devices){
			return physicalDeviceStuffs << getPhysicalDeviceProperties(devices);
		}

		//call getPhysicalDeviceName and save the result in the static variable and return a reference on it
		static const std::vector<std::string> & savePhysicalDeviceNames(const std::vector<VkPhysicalDeviceProperties> & properties) {
			return physicalDeviceStuffs << getPhysicalDeviceNames(properties);
		}

		//Return a vector of all features for each physical devices
		static const std::vector<VkPhysicalDeviceFeatures> & savePhysicalDeviceFeatures(const std::vector<VkPhysicalDevice> & devices) {
			return physicalDeviceStuffs << getPhysicalDeviceFeatures(devices);
		}

		//call getPhysicalDeviceName, getPhysicalDeviceProperties and getAvailablePhysicalDevices saving all result in the static variable and return a reference on it
		static const PhysicalDeviceStuffList & savePhysicalDeviceStuffs(VkInstance instance)
		{
			physicalDeviceStuffs << getAvailablePhysicalDevices(instance);
			physicalDeviceStuffs << getPhysicalDeviceProperties(physicalDeviceStuffs);
			physicalDeviceStuffs << getPhysicalDeviceFeatures(physicalDeviceStuffs);
			physicalDeviceStuffs << getPhysicalDeviceNames(physicalDeviceStuffs);
			return physicalDeviceStuffs;
		}

	public:
		//Pick up functions to select a physical device
		//---------------------------------------------

		//virtual pickByScore(const PhysicalDeviceStuffList & );


	public:
		//Variables
		//----------------------------------

		// List of the Physical device stuffs
		static PhysicalDeviceStuffList physicalDeviceStuffs;
	};
}

#endif