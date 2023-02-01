TARGET = Vk_Application

SOURCES = 	main.cpp private/vkApplication.cpp private/debugMessenger.cpp private/window.cpp private/swapChain.cpp private/physicalDevice.cpp private/logicalDevice.cpp
CC = g++
CFLAGS = -std=c++17 -g -O3
LD = g++
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)