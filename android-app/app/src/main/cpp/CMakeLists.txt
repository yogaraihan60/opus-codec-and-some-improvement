cmake_minimum_required(VERSION 3.22.1)

project("audio-share-app")

# Add Opus subdirectory
add_subdirectory(opus-1.5.2)

add_library(
    audio-share-lib
    SHARED
    native-lib.cpp
)

find_library(
    log-lib
    log
)

target_link_libraries(
    audio-share-lib
    ${log-lib}
    opus
)
