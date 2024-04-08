SET(target  WeightedAveraging_1st)

FIND_PACKAGE(glfw3 CONFIG REQUIRED)
FIND_PACKAGE(glad CONFIG REQUIRED)
FIND_PACKAGE(glm CONFIG REQUIRED)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

FILE(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${target} ${SOURCES})

target_link_libraries(${target} PRIVATE glfw glad::glad OpenGLBase)
target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/src)

FILE(INSTALL Shaders DESTINATION ${CMAKE_BINARY_DIR}/bin)