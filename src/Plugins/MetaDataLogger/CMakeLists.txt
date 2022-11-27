add_library(MetaDataLogger SHARED metaDataLogger.cpp)
find_package(yaml-cpp)
find_package(pipelineElements)
target_include_directories(MetaDataLogger PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(MetaDataLogger PRIVATE yaml-cpp pipelineElements)

install(TARGETS MetaDataLogger 
            RUNTIME DESTINATION bin/FIPP-Plugins
            LIBRARY DESTINATION lib/FIPP-Plugins)