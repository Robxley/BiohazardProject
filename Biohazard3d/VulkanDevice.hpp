#ifndef _BHD_VULKANDEVICE_H
#define _BHD_VULKANDEVICE_H
#pragma once

#include <map>
#include "VulkanTools.hpp"
#include "VulkanSwapChain.h"

namespace bhd
{

	typedef struct PhysicalDeviceStuffs
	{
		const std::string *					name;
		const VkPhysicalDevice *			physicalDevice;
		const VkPhysicalDeviceProperties *	properties;
		const VkPhysicalDeviceFeatures *	features;
		const QueueFamilyProperties *		queueFamilyProperties;
		const ExtensionProperties *			extensionProperties;
		const ExtensionNames *				extensionNames;

		operator std::string() const { return (name != nullptr) ? *name : std::string(); }
		operator VkPhysicalDevice () const { return (physicalDevice != nullptr) ? *physicalDevice : VK_NULL_HANDLE; }
		operator VkPhysicalDeviceProperties () const { return (properties != nullptr) ? *properties : VkPhysicalDeviceProperties(); }
		operator VkPhysicalDeviceFeatures () const { return (features != nullptr) ? *features : VkPhysicalDeviceFeatures(); }
		operator QueueFamilyProperties () const { return (queueFamilyProperties != nullptr) ? *queueFamilyProperties : QueueFamilyProperties(); }
		operator ExtensionProperties () const { return (extensionProperties != nullptr) ? *extensionProperties : ExtensionProperties(); }
		operator ExtensionNames () const { return (extensionNames != nullptr) ? *extensionNames : ExtensionNames(); }

		ExtensionNames getExtensionNames(){
			if (extensionProperties == nullptr) return ExtensionNames();
			ExtensionNames extensionNames; extensionNames.reserve(extensionProperties->size());
			for (const auto & one : *extensionProperties) extensionNames.push_back(one.extensionName);
		}

		void info()
		{
			extensionNames->info("Extension Names");
		}

	} PhysicalDeviceStuffs;



	class PhysicalDeviceStuffList
	{
		friend class VulkanPhysicalDevices;
	public:

		//get physical device stuffs by index
		PhysicalDeviceStuffs getPhysicalDeviceStuffs(int i) const {
			BHD_ASSERT_LOG(i < count(), "out of range");
			return{
				validVectorPtr(physicalDeviceNames,i),
				validVectorPtr(physicalDevices,i),
				validVectorPtr(physicalDeviceProperties,i),
				validVectorPtr(physicalDeviceFeatures,i),
				validVectorPtr(physicalDeviceQueueFamilyProperties,i),
				validVectorPtr(physicalDeviceExtensionProperties,i),
				validVectorPtr(physicalDeviceExtensionNames,i)
				};
		}

		//give the number of physical devices
		int count() const { 
			return (int)physicalDevices.size(); 
		}

		//clear all data
		void release() {
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
		PhysicalDeviceStuffList & operator <<(const std::vector<ExtensionProperties> & extensionProperties) { physicalDeviceExtensionProperties = extensionProperties; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<ExtensionNames> & extensions) { physicalDeviceExtensionNames = extensions; return *this; };
		PhysicalDeviceStuffList & operator <<(const std::vector<SwapChainFeatures> & swapChainFeatures) { physicalDeviceSwapChainFeatures = swapChainFeatures; return *this; };


		operator const std::vector<std::string>&() const { return physicalDeviceNames; }
		operator const std::vector<VkPhysicalDevice>&() const { return physicalDevices; }
		operator const std::vector<VkPhysicalDeviceProperties>&() const { return physicalDeviceProperties; }
		operator const std::vector<VkPhysicalDeviceFeatures>&() const { return physicalDeviceFeatures; }
		operator const std::vector<QueueFamilyProperties>&() const { return physicalDeviceQueueFamilyProperties; }
		operator const std::vector<ExtensionProperties>&() const { return physicalDeviceExtensionProperties; }
		operator const std::vector<ExtensionNames>&() const { return physicalDeviceExtensionNames; }
		operator const std::vector<SwapChainFeatures>&() const { return physicalDeviceSwapChainFeatures; }

		PhysicalDeviceStuffs operator [] (unsigned int i) const { return getPhysicalDeviceStuffs(i); }

	protected:

		std::vector<std::string> physicalDeviceNames;
		std::vector<VkPhysicalDevice> physicalDevices;
		std::vector<VkPhysicalDeviceProperties> physicalDeviceProperties;
		std::vector<VkPhysicalDeviceFeatures> physicalDeviceFeatures;
		std::vector<QueueFamilyProperties> physicalDeviceQueueFamilyProperties;
		std::vector<ExtensionProperties> physicalDeviceExtensionProperties;
		std::vector<ExtensionNames> physicalDeviceExtensionNames;
		std::vector<SwapChainFeatures> physicalDeviceSwapChainFeatures;

		//Not implemented yet
		//std::vector<VkPhysicalDeviceLimits> physicalDeviceLimits;
		//std::vector<VkPhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties;
	private:
		template< typename T>
		const T* validVectorPtr(const std::vector<T> & data, int i) const
		{
			return i < data.size() ? &data[i] : nullptr;
		}
	};


	class VulkanDevices
	{
	public:
		VulkanDevices(){}
		VulkanDevices(VkInstance instance) { getPhysicalDeviceStuffs(instance); };
		~VulkanDevices() { release(); }
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

		//Inquire physical Device extension properties
		static std::vector<ExtensionProperties> inquirePhysicalDeviceExtensionProperties(const std::vector<VkPhysicalDevice> & devices);

		//Inquire physical Device extension names
		static std::vector<ExtensionNames> inquirePhysicalDeviceExtensionNames(const std::vector<ExtensionProperties> & extensionProperties);

		//Inquire physical device surface support
		static VkBool32 inquirePhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32_t queueIndex, VkSurfaceKHR surface);


	public:
		//surcharge for useful inquire (not optimal)
		//-----------------------------------------------------------------------------
		static std::vector<std::string> inquirePhysicalDeviceNames(VkInstance instance)
		{
			return inquirePhysicalDeviceNames(
				inquirePhysicalDeviceProperties(
					inquireAvailablePhysicalDevices(instance)));
		}

		static std::vector<ExtensionNames> inquirePhysicalDeviceExtensionNames(VkInstance instance)
		{
			return inquirePhysicalDeviceExtensionNames(
				inquirePhysicalDeviceExtensionProperties(
					inquireAvailablePhysicalDevices(instance)));
		}

	public:
		//Functions keeping a trace of physical device stuffs in the member variable -> physicalDeviceStuffs 
		//--------------------------------------------------

		//call inquireAvailablePhysicalDevices and update the private variable physicalDeviceStuffs (see PhysicalDeviceStuffList)
		const std::vector<VkPhysicalDevice> & getAvailablePhysicalDevices(VkInstance instance){
			physicalDeviceStuffs.release();
			return physicalDeviceStuffs << inquireAvailablePhysicalDevices(instance);
		}

		//call inquirePhysicalDeviceProperties and update the private variable physicalDeviceStuffs (see PhysicalDeviceStuffList)
		const std::vector<VkPhysicalDeviceProperties> & getPhysicalDeviceProperties(){
			return physicalDeviceStuffs << inquirePhysicalDeviceProperties(physicalDeviceStuffs);
		}

		//call inquirePhysicalDeviceNames and update the private variable physicalDeviceStuffs (see PhysicalDeviceStuffList)
		const std::vector<std::string> & getPhysicalDeviceNames() {
			return physicalDeviceStuffs << inquirePhysicalDeviceNames(physicalDeviceStuffs);
		}

		//call inquirePhysicalDeviceFeatures and update the private variable physicalDeviceStuffs (see PhysicalDeviceStuffList)
		const std::vector<VkPhysicalDeviceFeatures> & getPhysicalDeviceFeatures() {
			return physicalDeviceStuffs << inquirePhysicalDeviceFeatures(physicalDeviceStuffs);
		}

		//call inquirePhysicalDeviceQueueFamilyProperties and update the private variable physicalDeviceStuffs (see PhysicalDeviceStuffList)
		const std::vector<QueueFamilyProperties> & getPhysicalDeviceQueueFamilyProperties() {
			return physicalDeviceStuffs << inquirePhysicalDeviceQueueFamilyProperties(physicalDeviceStuffs);
		}

		//call inquirePhysicalDeviceExtensionProperties and update the private variable physicalDeviceStuffs (see PhysicalDeviceStuffList)
		const std::vector<ExtensionProperties> & getPhysicalDeviceExtensionProperties() {
			return physicalDeviceStuffs << inquirePhysicalDeviceExtensionProperties(physicalDeviceStuffs);
		}

		//call inquirePhysicalDeviceExtensionNames and update the private variable physicalDeviceStuffs (see PhysicalDeviceStuffList)
		const std::vector<ExtensionNames> & getPhysicalDeviceExtensionNames() {
			return physicalDeviceStuffs << inquirePhysicalDeviceExtensionNames(physicalDeviceStuffs);
		}

		//call all get* functions and update the private variable physicalDeviceStuffs (see PhysicalDeviceStuffList)
		const PhysicalDeviceStuffList & getPhysicalDeviceStuffs(VkInstance instance)
		{
			getAvailablePhysicalDevices(instance);
			getPhysicalDeviceProperties();
			getPhysicalDeviceFeatures();
			getPhysicalDeviceNames();
			getPhysicalDeviceQueueFamilyProperties();
			getPhysicalDeviceExtensionProperties();
			getPhysicalDeviceExtensionNames();
			return physicalDeviceStuffs;
		}

	public:
		//Pick up functions to select a physical device
		//---------------------------------------------

		//give the better physical device based on the score function
		PhysicalDeviceStuffs pickByScore(std::function<int(const PhysicalDeviceStuffs &, VkSurfaceKHR, std::vector<std::string> extensionNames)> scoreMaker, VkSurfaceKHR surface, std::vector<std::string> extensionNames, int * score = nullptr) const;

		//default score function (only the properties and queue family properties are used)
		static int scoreMaker(const PhysicalDeviceStuffs & physicalDeviceStuffs, VkSurfaceKHR surface = VK_NULL_HANDLE, std::vector<std::string> extensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME });
		
		PhysicalDeviceStuffs pickByScore(VkSurfaceKHR surface, std::vector<std::string> extensionNames, int * score = nullptr) const {
			return pickByScore(scoreMaker, surface, extensionNames, score);
		}

		PhysicalDeviceStuffs getBestPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, std::vector<std::string> extensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME })
		{
			release();
			getAvailablePhysicalDevices(instance);
			getPhysicalDeviceProperties();
			getPhysicalDeviceNames();
			getPhysicalDeviceQueueFamilyProperties();
			if (!extensionNames.empty())
			{
				getPhysicalDeviceExtensionProperties();
				getPhysicalDeviceExtensionNames();
			}
			return pickByScore(surface,extensionNames);
		}

		VkResult createLogicalDevice(const PhysicalDeviceStuffs & physicalDeviceStuffs, VkSurfaceKHR surface = VK_NULL_HANDLE, VkQueueFlags flag = VK_QUEUE_GRAPHICS_BIT,  std::vector<std::string> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }, std::vector<std::string> layers = std::vector<std::string>());

		int getQueueFamilyIndex(const QueueFamilyProperties & queueFamilyProperties, VkQueueFlags flag = VK_QUEUE_GRAPHICS_BIT) const;
		int getQueueFamilyIndex(const PhysicalDeviceStuffs & physicalDeviceStuffs, VkQueueFlags flag = VK_QUEUE_GRAPHICS_BIT) const {
			BHD_ASSERT_LOG(physicalDeviceStuffs.queueFamilyProperties != nullptr, "Empty pointer");
			return getQueueFamilyIndex(*physicalDeviceStuffs.queueFamilyProperties, flag);
		}

		VkBool32 getQueueFamilySurfaceSupport(PhysicalDeviceStuffs physicalDeviceStuffs, VkSurfaceKHR surface, VkQueueFlags flag = VK_QUEUE_GRAPHICS_BIT, int * queueFamilyIndex = nullptr) const
		{
			int index = getQueueFamilyIndex(physicalDeviceStuffs, flag);
			if (index == -1) return 0;
			if (queueFamilyIndex != nullptr) *queueFamilyIndex = index;
			return inquirePhysicalDeviceSurfaceSupport(physicalDeviceStuffs, index, surface);
		}

		void release() {
			physicalDeviceStuffs.release();
			if (device != VK_NULL_HANDLE)
				vkDestroyDevice(device,nullptr);
			device = VK_NULL_HANDLE;
		}
	
	private:
		//Variables
		//----------------------------------

		// List of the Physical device stuffs
		PhysicalDeviceStuffList physicalDeviceStuffs;

		//logical Device
		VkDevice device = VK_NULL_HANDLE;
		std::vector<VkQueue>  logicalQueues;
	
	};

}

#endif