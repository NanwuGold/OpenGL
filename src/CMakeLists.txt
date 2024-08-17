set(target OpenGLBase)

FIND_PACKAGE(glm CONFIG REQUIRED)
FIND_PACKAGE(glfw3 CONFIG REQUIRED)
FIND_PACKAGE(imgui CONFIG REQUIRED)

FILE(GLOB_RECURSE SOURCES_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

ADD_LIBRARY(${target} STATIC ${SOURCES_FILES})

SET(LINKER_LIBRARIES
        glfw
        glad
        glm::glm
        imgui::imgui
)

TARGET_LINK_LIBRARIES(${target} PRIVATE ${LINKER_LIBRARIES})

TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}
                                              ${CMAKE_SOURCE_DIR}/vendor/imgui
                                              ${CMAKE_SOURCE_DIR}/vendor/glad/include
)