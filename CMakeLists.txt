cmake_minimum_required(VERSION 3.17)

project(cache)

set(SRC_FILES src/driver.cpp)

set(CMAKE_CXX_STANDARD 17)

set(COMPILE_FLAGS
    -Wall
    -Wextra
    -Werror
    -O3
)

add_executable(release-lfu ${SRC_FILES})
target_include_directories(release-lfu PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(release-lfu PUBLIC ${COMPILE_FLAGS})
target_compile_definitions(release-lfu PRIVATE LFU_CACHE=1)

add_executable(release-ideal ${SRC_FILES})
target_include_directories(release-ideal PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(release-ideal PUBLIC ${COMPILE_FLAGS})
target_compile_definitions(release-ideal PRIVATE IDEAL_CACHE=1)

add_executable(test ${SRC_FILES})
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(test PUBLIC ${COMPILE_FLAGS})
target_compile_definitions(test PRIVATE LFU_CACHE=1)

add_executable(debug ${SRC_FILES})
target_include_directories(debug PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(debug PUBLIC ${COMPILE_FLAGS})
target_compile_definitions(debug PRIVATE LFU_CACHE=1)
target_compile_definitions(debug PRIVATE IDEAL_CACHE=1)

add_custom_target(tests
    COMMAND bash ${CMAKE_SOURCE_DIR}/tests/tester.sh
    DEPENDS test
    COMMENT "Running tests..."
)




