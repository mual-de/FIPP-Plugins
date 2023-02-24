add_library(ImageExporter SHARED ImageExporter.cpp)

find_package(yaml-cpp)
set(PipelineElements_DIR "/usr/local/lib/cmake/FIPP/PipelineElements")
find_library(PIPELINEELEMENTS_LIBRARY PipelineElements Point HINTS /usr/local/lib/FIPP)
set(ImageContainer_DIR "/usr/local/lib/cmake/FIPP/ImageContainer")
find_library(IMAGECONTAINER_LIBRARY ImageContainer HINTS /usr/local/lib/FIPP)
set(Logging_DIR "/usr/local/lib/cmake/FIPP/Logging")
find_library(LOGGING_LIBRARY Logging HINTS /usr/local/lib/FIPP)
set(Point_DIR "/usr/local/lib/cmake/FIPP/Point")
find_library(POINT_LIBRARY Point HINTS /usr/local/lib/FIPP)


find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_link_libraries(ImageExporter PRIVATE yaml-cpp ${PIPELINEELEMENTS_LIBRARY} ${IMAGECONTAINER_LIBRARY} ${POINT_LIBRARY} ${LOGGING_LIBRARY} ${OpenCV_LIBS})

install(TARGETS ImageExporter 
            RUNTIME DESTINATION bin/FIPP-Plugins
            LIBRARY DESTINATION lib/FIPP-Plugins)