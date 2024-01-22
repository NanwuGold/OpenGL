SET(target OpenGL_FXAA)

FIND_PACKAGE(glfw3 CONFIG REQUIRED)
FIND_PACKAGE(glad CONFIG REQUIRED)

add_executable(${target} main.cpp)

target_link_libraries(${target} PRIVATE glfw glad::glad)