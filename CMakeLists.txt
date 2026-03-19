cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(./data_structures)
include_directories(./Project1SampleDatasets)

add_executable(MyProject
        main.cpp
        data_structures/Parser.cpp
        data_structures/ConferenceManager.cpp
        data_structures/Reviewer.cpp
        data_structures/Submission.cpp)