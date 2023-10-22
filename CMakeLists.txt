cmake_minimum_required(VERSION 3.27)
project("Botanic-Resurgance")
add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} -lvulkan -lglfw -ldl -lpthread)
