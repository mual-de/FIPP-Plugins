add_library(TestVideoSrc SHARED TestVideoSrc.cpp)
find_package(yaml-cpp)
target_include_directories(TestVideoSrc PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(TestVideoSrc PRIVATE yaml-cpp pipelineElements)