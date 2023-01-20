CXXFLAGS = -std=c++17 -O2 -g

# GLFW, VK function loader, low level systems for GLFW 
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
VulkanTest: main.cpp
	g++ $(CXXFLAGS) main.cpp -o VulkanTest $(LDFLAGS)