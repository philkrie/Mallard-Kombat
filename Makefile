CXX = clang++
SDL = -framework SDL2
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
INCLUDES = -I/Library/Frameworks/SDL2_ttf.framework/Headers
LIBS = -L/full/path/dylib
CXXFLAGS = -Wall -c -std=c++11 
LDFLAGS = $(SDL) $(TTF)
EXE = SDL_Lesson0

all: $(EXE)

$(EXE): main.o
	$(CXX) $(LDFLAGS) $(INCLUDES) $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)