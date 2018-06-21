
cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME CellCreator)
project(${PROJECT_NAME} VERSION 0.2.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui XmlPatterns)

# Produces a target 'cellml'
find_package(libCellML REQUIRED)

add_subdirectory(src/lib)
add_subdirectory(src/app)
