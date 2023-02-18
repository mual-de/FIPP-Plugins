find_package(GTest REQUIRED)
set(Point_DIR "/usr/local/lib/cmake/FIPP/Point")
set(ImageContainer_DIR "/usr/local/lib/cmake/FIPP/ImageContainer")
find_package(Point CONFIG REQUIRED)
find_package(ImageContainer CONFIG REQUIRED)
add_subdirectory(Plugins)
add_subdirectory(Sources)
add_subdirectory(Sinks)

function(do_test target result)
  add_test(NAME ${target}_test COMMAND ${target})
  set_tests_properties(${target}_test
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
  )
endfunction()