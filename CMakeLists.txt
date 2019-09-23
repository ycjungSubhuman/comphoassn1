project(comphoassn1)
cmake_minimum_required(VERSION 3.15)

find_package(OpenCV 4.1.1 REQUIRED COMPONENTS core)

add_executable(main.out src/main.cpp)
target_include_directories(main.out PRIVATE
    ${OpenCV_INCLUDE_DIRS})
target_link_libraries(main.out PRIVATE
    ${OpenCV_LIBS})

