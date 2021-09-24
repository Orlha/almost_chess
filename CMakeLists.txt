cmake_minimum_required(VERSION 3.9)
project(almost_chess CXX)

set(CHESS_SRC
	src/main.cpp
	src/render.cpp
	src/field.cpp
	src/ai.cpp
)

add_executable(almost_chess
	${CHESS_SRC}
)

add_subdirectory(SFML)
target_link_libraries(almost_chess
	sfml-system
	sfml-window
	sfml-graphics
	sfml-audio
)
if(UNIX)
target_link_libraries(almost_chess
	pthread
)
target_compile_options(almost_chess PRIVATE
	-Werror -Wall -Wextra -Wpedantic
	-Wfatal-errors
	-Wno-sign-compare
	-Wno-unused-parameter
	-Wno-unused-variable
	-Wno-unused-function
)
endif()

set_property(TARGET almost_chess PROPERTY CXX_STANDARD 20)
