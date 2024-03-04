cmake_minimum_required (VERSION 3.8)

project(textile-detect-gui)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -Werror -mwindows -municode -g ") # 设置C++ 工程的 CXX_FLAGS
set(OpenCV_DIR D:/MyDocuments/opencv/build/install/x64/mingw/lib)

aux_source_directory(./src/ SRCS)
include_directories(./src/)



add_executable(${PROJECT_NAME} ${SRCS})


target_link_libraries(${PROJECT_NAME} Gdiplus)
target_link_libraries(${PROJECT_NAME} ole32)
target_link_libraries(${PROJECT_NAME} strmiids)
target_link_libraries(${PROJECT_NAME} oleaut32)
target_link_libraries(${PROJECT_NAME} Comctl32)


FIND_PACKAGE(OpenCV REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})

set(CMAKE_INSTALL_PREFIX ./install)  # 自定义安装目录，打包使用
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin) # 打包二进制文件