
cmake_minimum_required(VERSION 3.16)
project(OpenGL)
set(CMAKE_CXX_STANDARD 17)


# 源文件
# 搞这么麻烦主要是因为有个glad.c，
# 不能${PROJECT_SOURCE_DIR}/*.cpp 是因为会把临时生成的CMakeCXXCompilerId.cpp也拿去编译，这样就有两个main函数了
file(GLOB_RECURSE SRC 
${PROJECT_SOURCE_DIR}/src/*.cpp 
${PROJECT_SOURCE_DIR}/include/*.cpp 
${PROJECT_SOURCE_DIR}/include/*.c
)

add_executable(OpenGL ${SRC})

# 设置可执行文件输出目录out
 set_target_properties(OpenGL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"  )
# 设置库目录和要链接的库
target_link_directories(OpenGL PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(OpenGL PUBLIC glfw3 opengl32 assimp-vc142-mt)

# dll拷贝到输出目录
add_custom_command(TARGET OpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/assimp-vc142-mt.dll" $<TARGET_FILE_DIR:OpenGL>
)

# 包含目录
target_include_directories(OpenGL PUBLIC 	
${PROJECT_SOURCE_DIR}/src/test 
${PROJECT_SOURCE_DIR}/src/tools
${PROJECT_SOURCE_DIR}/include
)
