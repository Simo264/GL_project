CXXFLAGS 	 = -g -std=c++20 -Wall -Werror -Wextra -Wpedantic -fopenmp
LDFLAGS 	 = -lX11 -lassimp -fopenmp

EXECUTABLE = main.exe

INCLUDE_DIR 		= externals/include/
STATIC_LIBS_DIR = externals/libs/static/
SHARED_LIBS_DIR = externals/libs/shared/

STATIC_LIBS = ${STATIC_LIBS_DIR}libglad.a \
							${STATIC_LIBS_DIR}libglfw3.a \
							${STATIC_LIBS_DIR}libspdlog.a \
							${STATIC_LIBS_DIR}libglm.a 


SOURCES 	= 	$(wildcard src/*.cpp)
SOURCES  += 	$(wildcard src/imgui/*.cpp)

OBJDIR 		= debug/
OBJECTS 	= $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	g++ -o ${EXECUTABLE} $^ ${STATIC_LIBS} \
	-L${SHARED_LIBS_DIR} -Wl,-rpath=${SHARED_LIBS_DIR} ${LDFLAGS}


$(OBJDIR)%.o: %.cpp
	@mkdir -p '$(@D)'
	g++ -o $@ -c $< -I${INCLUDE_DIR} ${CXXFLAGS}


.PHONY: clean
clean:
	find . -name "*.o" -delete
	rm -f ${EXECUTABLE}