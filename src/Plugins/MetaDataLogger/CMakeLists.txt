add_library(MetaDataLogger SHARED metaDataLogger.cpp)
find_package(yaml-cpp)
set(PipelineElements_DIR "/usr/local/lib/cmake/FIPP/PipelineElements")
find_library(PIPELINEELEMENTS_LIBRARY PipelineElements Point HINTS /usr/local/lib/FIPP)
set(ImageContainer_DIR "/usr/local/lib/cmake/FIPP/ImageContainer")
find_library(IMAGECONTAINER_LIBRARY ImageContainer HINTS /usr/local/lib/FIPP)
set(Logging_DIR "/usr/local/lib/cmake/FIPP/Logging")
find_library(LOGGING_LIBRARY Logging HINTS /usr/local/lib/FIPP)
set(Point_DIR "/usr/local/lib/cmake/FIPP/Point")
find_library(POINT_LIBRARY Point HINTS /usr/local/lib/FIPP)

target_link_libraries(MetaDataLogger PUBLIC yaml-cpp ${PIPELINEELEMENTS_LIBRARY} ${IMAGECONTAINER_LIBRARY} ${POINT_LIBRARY} ${LOGGING_LIBRARY})

install(TARGETS MetaDataLogger 
            RUNTIME DESTINATION bin/FIPP-Plugins
            LIBRARY DESTINATION lib/FIPP-Plugins)