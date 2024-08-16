SET(target_ glad)

FILE(GLOB_RECURSE FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.*)

# 生成可执行文件
ADD_LIBRARY(${target_} STATIC ${FILES})

TARGET_INCLUDE_DIRECTORIES(
        ${target_}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)