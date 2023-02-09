TARGET = Application
OBJDIR = obj

SOURCES = $(wildcard private/*.cpp)
OBJECTS = $(patsubst private/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

CC = g++
CPPFLAGS = -std=c++17 -g -O2
LD = g++
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

all: $(TARGET)

$(TARGET): main.cpp $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $(TARGET) main.cpp $(OBJECTS) $(LDFLAGS)

$(OBJDIR)/%.o: private/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)
	rm -f shaders/spirv/*