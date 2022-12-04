add_library(TestVideoSrc SHARED TestVideoSrc.cpp)
find_package(yaml-cpp)
set(PipelineElements_DIR "/usr/local/lib/cmake/FIPP/PipelineElements")
find_library(PIPELINEELEMENTS_LIBRARY PipelineElements ImageContainer HINTS /usr/local/lib/FIPP)
target_include_directories(TestVideoSrc PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(TestVideoSrc PRIVATE yaml-cpp ${PIPELINEELEMENTS_LIBRARY})

install(TARGETS TestVideoSrc 
            RUNTIME DESTINATION bin/FIPP-Plugins
            LIBRARY DESTINATION lib/FIPP-Plugins)