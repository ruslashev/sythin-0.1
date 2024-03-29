CXX = clang++
OBJS = $(patsubst ./%.cpp, .objs/%.o, $(shell find . -type f -name '*.cpp' ))
CXXFLAGS = -Wall -Werror -g -std=c++0x
LDFLAGS = -lSDL2 -lGLEW -lGL
EXECNAME = sythin

all: objdir $(EXECNAME)
	./$(EXECNAME)

.objs/%.o: %.cpp
	@$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo "Compiling $<"

$(EXECNAME): $(OBJS)
	@$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "Linking to $@"

valgrind: objdir $(EXECNAME)
	valgrind --leak-check=full ./$(EXECNAME)

callgrind: objdir $(EXECNAME)
	@valgrind --tool=callgrind ./$(EXECNAME)
	@kcachegrind callgrind.out.$!

objdir:
	@mkdir -p .objs
	@mkdir -p .objs/opengl
	@mkdir -p .objs/imgui

clean:
	-rm -f $(EXECNAME)
	-rm -rf $(OBJS)

