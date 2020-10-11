cmake_minimum_required(VERSION 3.17)
project(PySubInterpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(PySubInterpreter main.cpp interface.cpp interface.h lexicalanalyzer.cpp lexicalanalyzer.h expevaluator.cpp expevaluator.h)