#ifndef _BHD_VULKANDEVICE_H
#define _BHD_VULKANDEVICE_H
#pragma once

#include "VulkanTools.hpp"
#include <map>

namespace bhd
{
	typedef std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
	
	typedef struct PhysicalDeviceStuffs
	{
		const std::string *name;
		const VkPhysicalDevice *physicalDevice;
		const VkPhysicalDeviceProperties *properties;
		const VkPhysicalDeviceFeatures *features;
		const QueueFamilyProperties *queueFamilyProperties;
	} PhysicalDeviceStuffs;

	class PhysicalDeviceStuffList
	{
		friend class VulkanPhysicalDevices;
	public:

		//get physical device stuffs by index
		PhysicalDeviceStuffs getPhysicalDeviceStuffs(int i) {
			BHD_ASSERT_LOG(i < count(), "out of range");
			return{
				validVectorPtr(physicalDeviceNames,i),
				validVectorPtr(physicalDevices,i),
				validVectorPtr(physicalDeviceProperties,i),
				validVectorPtr(physicalDeviceFeatures,i),
				validVectorPtr(physicalDeviceQueueFamilyProperties,i)
				};
		}

		//give the number of physical devices
		int count() { 
			return (int)physicalDevices.size(); 
		}

		//clear all data
		void clear() {
			physicalDeviceNames.clear();
			physicalDevices.clear();
			physicalDeviceProperties.clear();
			physicalDeviceFeatures.clear();
			physicalDeviceQueueFamilyProperties.clear();
		}

	public:

		//some useful operator
		PhysicalDeviceStuffList & operator <<(const std::vector<std::string> & names) { physicalDeviceNames = names; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<VkPhysicalDevice> & devices) { physicalDevices = devices; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<VkPhysicalDeviceProperties> & properties) { physicalDeviceProperties = properties; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<VkPhysicalDeviceFeatures> & features) { physicalDeviceFeatures = features; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<QueueFamilyProperties> & queueFamilyProperties) { physicalDeviceQueueFamilyProperties = queueFamilyProperties; return *this; };

		operator const std::vector<std::string>&() { return physicalDeviceNames; }
		operator const std::vector<VkPhysicalDevice>&() { return physicalDevices; }
		operator const std::vector<VkPhysicalDeviceProperties>&() { return physicalDeviceProperties; }
		operator const std::vector<VkPhysicalDeviceFeatures>&() { return physicalDeviceFeatures; }
		operator const std::vector<QueueFamilyProperties>&() { return physicalDeviceQueueFamilyProperties; }

		PhysicalDeviceStuffs operator [] (unsigned int i) { return getPhysicalDeviceStuffs(i); }

	protected:

		std::vector<std::string> physicalDeviceNames;
		std::vector<VkPhysicalDevice> physicalDevices;
		std::vector<VkPhysicalDeviceProperties> physicalDeviceProperties;
		std::vector<VkPhysicalDeviceFeatures> physicalDeviceFeatures;
		std::vector<QueueFamilyProperties> physicalDeviceQueueFamilyProperties;

		//Not implemented yet
		//std::vector<VkPhysicalDeviceLimits> physicalDeviceLimits;
		//std::vector<VkPhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties;
	private:
		template< typename T>
		const T* validVectorPtr(const std::vector<T> & data, int i)
		{
			return i < data.size() ? &data[i] : nullptr;
		}
	};


	class VulkanPhysicalDevices
	{
	public:
		VulkanPhysicalDevices(const VkInstance & instance) { getPhysicalDeviceStuffs(instance); };
		//Physical devices useful functions
		//--------------------------------

		//Return a vector of available physical devices
		static std::vector<VkPhysicalDevice> inquireAvailablePhysicalDevices(VkInstance instance);

		//Return a vector of all properties for each physical devices
		static std::vector<VkPhysicalDeviceProperties> inquirePhysicalDeviceProperties(const std::vector<VkPhysicalDevice> & devices);
	
		//Get all physical device names from their properties
		static std::vector<std::string> inquirePhysicalDeviceNames(const std::vector<VkPhysicalDeviceProperties> & properties);
	
		//Return a vector of all features for each physical devices
		static std::vector<VkPhysicalDeviceFeatures> inquirePhysicalDeviceFeatures(const std::vector<VkPhysicalDevice> & devices);

		//Inquire the queue family properties 
		static std::vector<QueueFamilyProperties> inquirePhysicalDeviceQueueFamilyProperties(const std::vector<VkPhysicalDevice> & devices);

		static std::vector<std::string> inquirePhysicalDeviceNames(VkInstance instance)
		{
			return inquirePhysicalDeviceNames(
				inquirePhysicalDeviceProperties(
					inquireAvailablePhysicalDevices(instance)));
		}


	public:
		//Functions with a trace of physical device features
		//--------------------------------------------------

		//call getAvailablePhysicalDevices and save the result in the static variable and return a reference on it
		const std::vector<VkPhysicalDevice> & getAvailablePhysicalDevices(VkInstance instance){
			clear();
			return physicalDeviceStuffs << inquireAvailablePhysicalDevices(instance);
		}

		//call getPhysicalDeviceProperties and save the result in the static variable and return a reference on it
		const std::vector<VkPhysicalDeviceProperties> & getPhysicalDeviceProperties(){
			return physicalDeviceStuffs << inquirePhysicalDeviceProperties(physicalDeviceStuffs);
		}

		//call getPhysicalDeviceName and save the result in the static variable and return a reference on it
		const std::vector<std::string> & getPhysicalDeviceNames() {
			return physicalDeviceStuffs << inquirePhysicalDeviceNames(physicalDeviceStuffs);
		}

		//Return a vector of all features for each physical devices
		const std::vector<VkPhysicalDeviceFeatures> & getPhysicalDeviceFeatures() {
			return physicalDeviceStuffs << inquirePhysicalDeviceFeatures(physicalDeviceStuffs);
		}

		const std::vector<QueueFamilyProperties> & getPhysicalDeviceQueueFamilyProperties() {
			return physicalDeviceStuffs << inquirePhysicalDeviceQueueFamilyProperties(physicalDeviceStuffs);
		}

		//call getPhysicalDeviceName, getPhysicalDeviceProperties and getAvailablePhysicalDevices saving all result in the static variable and return a reference on it
		const PhysicalDeviceStuffList & getPhysicalDeviceStuffs(VkInstance instance)
		{
			getAvailablePhysicalDevices(instance);
			getPhysicalDeviceProperties();
			getPhysicalDeviceFeatures();
			getPhysicalDeviceNames();
			getPhysicalDeviceQueueFamilyProperties();
			return physicalDeviceStuffs;
		}

	public:
		//Pick up functions to select a physical device
		//---------------------------------------------

		//give the better physical device based on the score function
		PhysicalDeviceStuffs pickByScore(std::function<int(const PhysicalDeviceStuffs &)> scoreMaker, int * score = nullptr);

		//default score function (only the properties and queue family properties are used)
		static int scoreMaker(const PhysicalDeviceStuffs & physicalDeviceStuffs);
		
		PhysicalDeviceStuffs pickByScore(int * score = nullptr) {
			return pickByScore(scoreMaker, score);
		}

		PhysicalDeviceStuffs getBestPhysicalDevice(VkInstance instance = VK_NULL_HANDLE)
		{
			if (instance != VK_NULL_HANDLE) clear();
			if (physicalDeviceStuffs.count() == 0)
			{
				getAvailablePhysicalDevices(instance);
				getPhysicalDeviceProperties();
				getPhysicalDeviceNames();
				getPhysicalDeviceQueueFamilyProperties();
			}
			return pickByScore();
		}

		void clear() { physicalDeviceStuffs.clear(); }
	public:
		//Variables
		//----------------------------------

		// List of the Physical device stuffs
		PhysicalDeviceStuffList physicalDeviceStuffs;
	};
}

#endif