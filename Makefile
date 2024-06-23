# Makefile for GNU Linux
CXX := g++
CPPFLAGS := -O3
LDFLAGS := -lOpenCL
SRCDIR := source
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
BUILDDIR := build
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

all: raytrace

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

%.d: %.cpp | $(BUILDDIR)
	$(CXX) -MMD -MP $<
	
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CPPFLAGS) -c $< -o $@

-include $(OBJECTS:.o=.d)

raytrace: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o raytrace

clean:
	rm -f raytrace
	rm -rf build
	