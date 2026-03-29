cmake_minimum_required(VERSION 3.10)
project(DataStructures CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(DataStructures STATIC
        core/Brainer.cpp
        models/Reviewer.cpp
        models/Submission.cpp
        io/FileManager.cpp
        io/Parser.cpp
        models/MatchResult.cpp
        models/MissingReviewsResult.cpp
        tests/Tester.cpp
        utils/Debugger.cpp
)

#include all folders
target_include_directories(DataStructures PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/io
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_SOURCE_DIR}/structures
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)