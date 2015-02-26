BINARY          := mallard
SRCS            := $(wildcard src/*.cpp)
OBJS            := $(SRCS:.cpp=.o)

#DEBUG           := -g

SDL_INCLUDE     := `sdl2-config --cflags`
SDL_LIB         := `sdl2-config --libs` -lSDL2_ttf -lSDL2_mixer -lSDL2_image

CPPFLAGS        += $(SDL_INCLUDE)
CXXFLAGS        += $(DEBUG) -Wall -std=c++11
LDFLAGS         += $(SDL_LIB)
CXXFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib

.PHONY: all clean

all: $(BINARY)

$(BINARY): $(OBJS)
	$(LINK.cc) $(OBJS) -o $(BINARY) $(LDFLAGS)

.depend: $(SRCS)
	@- $(RM) .depend
	@- $(g++) $(CXXFLAGS) -MM $^ | sed -E 's|^([^ ])|src/\1|' > .depend;

-include .depend

clean:
	@- $(RM) $(BINARY)
	@- $(RM) $(OBJS)
	@- $(RM) .depend