add_library(MetaDataLogger SHARED metaDataLogger.cpp)
find_package(yaml-cpp)
set(PipelineElements_DIR "/usr/local/lib/cmake/FIPP/PipelineElements")
find_library(PIPELINEELEMENTS_LIBRARY PipelineElements HINTS /usr/local/lib/FIPP)
target_include_directories(MetaDataLogger PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(MetaDataLogger PUBLIC yaml-cpp ${PIPELINEELEMENTS_LIBRARY})

install(TARGETS MetaDataLogger 
            RUNTIME DESTINATION bin/FIPP-Plugins
            LIBRARY DESTINATION lib/FIPP-Plugins)