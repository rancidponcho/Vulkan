# Vulkan

You've found my issues branch! I'm getting a segmentation fault on line 55 of private/physicalDevice.cpp.

GDB says:
Thread 1 "Vk_Application" received signal SIGSEGV, Segmentation fault.
0x00007ffff7f00e0e in vkGetPhysicalDeviceQueueFamilyProperties () from /lib/x86_64-linux-gnu/libvulkan.so.1

libvulkan.so.1 appears to be a binary file, so I don't know what further debugging steps to take. Help is much appreciated!

Built using g++ on Ubuntu. Run "make" in terminal to create Vk_Application executable.
