cmake_minimum_required(VERSION 3.25)
project(simplex)

set(CMAKE_CXX_STANDARD 20)
set(wxBUILD_SHARED OFF)

# Set entry point to WinMain for Windows
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:WinMainCRTStartup")
endif()

include(cmake/CPM.cmake)

CPMAddPackage("gh:wxWidgets/wxWidgets@3.2.4")

set(SOURCES main.cpp
        App.cpp
        App.hpp
        MainFrame.cpp
        MainFrame.hpp
        SimplexSolver.cpp)

if (WIN32)
    add_executable(simplex WIN32 ${SOURCES})
else()
    add_executable(simplex ${SOURCES})
endif()

target_link_libraries(simplex PRIVATE wxcore wxnet)
