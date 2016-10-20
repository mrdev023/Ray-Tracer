CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS :=
CC_FLAGS := -std=gnu++11

ifeq ($(OS),Windows_NT)
EXEC := main.exe
else
EXEC := main
endif

$(EXEC): $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	mkdir -p obj
	g++ $(CC_FLAGS) -c -o $@ $<

clean:
	rm obj/*
	rm $(EXEC)
