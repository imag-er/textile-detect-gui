cmake_minimum_required (VERSION 3.8)

project(texile-detect-gui)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -Werror -mwindows -municode -g") # 设置C++ 工程的 CXX_FLAGS

foreach(v SRCS)
	message(${v})
endforeach()
aux_source_directory(./src/ SRCS)
include_directories(${ROOT_DIR}/src/include)



add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} vfw32)
target_link_libraries(${PROJECT_NAME} Gdiplus)



set(CMAKE_INSTALL_PREFIX ./install)  # 自定义安装目录，打包使用
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin) # 打包二进制文件