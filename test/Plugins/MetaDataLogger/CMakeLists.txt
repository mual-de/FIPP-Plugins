add_executable(MetaDataLoggerTest
    MetaDataLoggerTest.cpp
)
find_package(yaml-cpp)
target_link_libraries(MetaDataLoggerTest
    PRIVATE
    GTest::GTest
    pipelineElements
    MetaDataLogger
    ImageContainer
    Point
    Logging
    yaml-cpp
)

add_test(
    NAME MetaDataLoggerTest
    COMMAND MetaDataLoggerTest
)