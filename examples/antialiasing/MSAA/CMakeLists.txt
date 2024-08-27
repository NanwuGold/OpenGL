SET(target MSAA)

FIND_PACKAGE(glfw3 CONFIG REQUIRED)
FIND_PACKAGE(glad CONFIG REQUIRED)
FIND_PACKAGE(glm CONFIG REQUIRED)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_executable(${target} main.cpp)

target_link_libraries(${target} PRIVATE glfw glad::glad OpenGLBase)
target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/src)

FILE(INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_BINARY_DIR}/bin)