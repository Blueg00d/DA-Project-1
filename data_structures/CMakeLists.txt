cmake_minimum_required(VERSION 3.10)
project(DataStructures CXX)

set(CMAKE_CXX_STANDARD 17)

# Added FileManager.cpp and FileManager.h to the compilation process
add_library(DataStructures STATIC
        ConferenceManager.cpp
        ConferenceManager.h
        FileManager.cpp
        FileManager.h
        Graph.h
        MatchResult.h
        MatchResult.cpp
        MissingReviewsResult.cpp
        MissingReviewsResult.h
        Parameters.h
        Parser.cpp
        Parser.h
        Reviewer.cpp
        Reviewer.h
        Submission.cpp
        Submission.h
        Utils.h
        Tester.cpp
        Tester.h
        Debugger.cpp
        Debugger.h
)

target_include_directories(DataStructures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})