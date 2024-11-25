cmake_minimum_required(VERSION 3.20)

# 设置项目名称
set(PROJECT_NAME "TinyEngineCore")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD_REQUIRED 20)						# C++标准
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)	# 可执行文件输出路径
include_directories(${PROJECT_SOURCE_DIR}/include)		# 头文件路径

file(GLOB_RECURSE CXX_FILES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.c)		#寻找PROJECT_SOURCE_DIR下所有cxx文件
add_executable(${PROJECT_NAME} ${CXX_FILES})	# 可执行文件

link_directories(${PROJECT_SOURCE_DIR}/lib)					# 库文件路径
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/glfw3.lib)		# 目标链接文件
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/assimp-vc143-mt.lib)	