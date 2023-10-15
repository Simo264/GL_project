CXXFLAGS = -g -std=c++20 -Wall -Werror -Wextra -Wpedantic

INCLUDE_DIR 		= externals/include/
STATIC_LIBS_DIR = externals/libs/static/
SHARED_LIBS_DIR = externals/libs/shared/

STATIC_LIBS = ${STATIC_LIBS_DIR}libglad.a \
							${STATIC_LIBS_DIR}libglfw3.a \
							${STATIC_LIBS_DIR}libspdlog.a \
							${STATIC_LIBS_DIR}libglm.a \
							${STATIC_LIBS_DIR}libimgui.a 

SHARED_LIBS = -lX11 \
							-lassimp

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

TARGET = exe

TARGET: ${OBJECTS}
	g++ -o ${TARGET} $^ ${STATIC_LIBS}  \
	-L${SHARED_LIBS_DIR} -Wl,-rpath=${SHARED_LIBS_DIR} ${SHARED_LIBS}
	 

%.o: %.cpp
	g++ -c $< -I${INCLUDE_DIR} ${CXXFLAGS} -o $@

.PHONY: clean
clean:
	rm -f ${OBJECTS} ${target}