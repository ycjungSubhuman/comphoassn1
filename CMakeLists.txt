project(comphoassn1)
cmake_minimum_required(VERSION 3.15)

find_package(OpenCV 4.1.1 REQUIRED COMPONENTS core imgcodecs imgproc)
find_package(OpenMP REQUIRED)

add_library(comphoassn1
    src/align.cpp
    src/match_score.cpp
    src/multi_scale.cpp
    src/pyramid.cpp
    src/single_scale.cpp
    src/util.cpp)
target_link_libraries(comphoassn1 PUBLIC ${OpenCV_LIBS} OpenMP::OpenMP_CXX)
target_include_directories(comphoassn1 PUBLIC ${OpenCV_INCLUDE_DIRS})
target_compile_options(comphoassn1 PUBLIC -Wall)

add_executable(main.out src/main.cpp)
target_link_libraries(main.out PRIVATE comphoassn1)

