CXXFLAGS = -g -std=c++20 -Wall -Werror -Wextra -Wpedantic

EXT_INCLUDE = externals/include/
EXT_LIBS 		= externals/libs/

GLAD_STATIC_LIB 	= ${EXT_LIBS}glad.a
GLFW_STATIC_LIB 	= ${EXT_LIBS}libglfw3.a
SPDLOG_STATIC_LIB = ${EXT_LIBS}libspdlog.a

STATIC_LIBS = ${GLAD_STATIC_LIB} ${GLFW_STATIC_LIB} ${SPDLOG_STATIC_LIB}

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

TARGET = exe

TARGET: ${OBJECTS}
	g++ $^ ${STATIC_LIBS} -o ${TARGET} -lX11 

%.o: %.cpp
	g++ -c $< -I${EXT_INCLUDE} ${CXXFLAGS} -o $@

.PHONY: clean
clean:
	rm -f ${OBJECTS} ${target}