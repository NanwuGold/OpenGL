SET(target ABuffer)

find_package(glfW3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

FILE(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

ADD_EXECUTABLE(${target} ${SOURCES})

TARGET_LINK_LIBRARIES(${target} PRIVATE glfw 
                                        glad
                                        imgui::imgui 
                                        OpenGLBase)

TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE 
                                        ${CMAKE_SOURCE_DIR}/src/
                                        ${CMAKE_SOURCE_DIR}/vendor/imgui
                                        ${CMAKE_SOURCE_DIR}/vendor/glad/include)

FILE(INSTALL Shaders DESTINATION ${CMAKE_BINARY_DIR}/bin/)

