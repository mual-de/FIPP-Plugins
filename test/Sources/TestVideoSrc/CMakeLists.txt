add_executable(TestVideoSrcTest
TestVideoSrcTest.cpp
)
find_package(yaml-cpp)
set(PipelineElements_DIR "/usr/local/lib/cmake/FIPP/PipelineElements")
find_library(PIPELINEELEMENTS_LIBRARY PipelineElements Logging Point HINTS /usr/local/lib/FIPP)
set(ImageContainer_DIR "/usr/local/lib/cmake/FIPP/ImageContainer")
find_library(IMAGECONTAINER_LIBRARY ImageContainer HINTS /usr/local/lib/FIPP)
set(Logging_DIR "/usr/local/lib/cmake/FIPP/Logging")
find_library(LOGGING_LIBRARY Logging HINTS /usr/local/lib/FIPP)
set(Point_DIR "/usr/local/lib/cmake/FIPP/Point")
find_library(POINT_LIBRARY Point HINTS /usr/local/lib/FIPP)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_include_directories(TestVideoSrcTest PUBLIC "/usr/local/include/FIPP")

target_link_libraries(TestVideoSrcTest
    PRIVATE
    TestVideoSrc
    GTest::GTest
    yaml-cpp
    ${OpenCV_LIBS}
    ${PIPELINEELEMENTS_LIBRARY} ${IMAGECONTAINER_LIBRARY} ${LOGGING_LIBRARY} ${POINT_LIBRARY}
)

add_test(
    NAME TestVideoSrcTest
    COMMAND TestVideoSrcTest
)