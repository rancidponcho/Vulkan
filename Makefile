# c++ version, optimization, debug
INPUT = main.cpp private/vkApplication.cpp
OUTPUT = -o VulkanTest
CXXFLAGS = -std=c++17 -g # -O2 (optimization)
# GLFW, VK function loader, low level systems for GLFW 
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
VulkanTest: main.cpp
	g++ $(INPUT) $(OUTPUT) $(CXXFLAGS) $(LDFLAGS)