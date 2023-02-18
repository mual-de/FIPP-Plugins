add_library(TestVideoSink SHARED TestVideoSink.cpp)
find_package(yaml-cpp)
find_library(PIPELINEELEMENTS_LIBRARY PipelineElements ImageContainer HINTS /usr/local/lib/FIPP)
target_include_directories(TestVideoSink PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(TestVideoSink PRIVATE yaml-cpp ${PIPELINEELEMENTS_LIBRARY})

install(TARGETS TestVideoSink 
            RUNTIME DESTINATION bin/FIPP-Plugins
            LIBRARY DESTINATION lib/FIPP-Plugins)