cmake_minimum_required(VERSION 3.5)

find_package(Eigen3 REQUIRED NO_MODULE)
include_directories(${Eigen_INCLUDE_DIRS})
link_directories(${Eigen_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-fopenmp")

project(lagrange-sim)

add_executable(satellite
    src/execs/satellite.cpp
    src/lagrange-sim.cpp
)

target_link_libraries(satellite
    ${Eigen_LIBRARIES}
)

add_executable(lagrange-stability
    src/execs/lagrange-stability.cpp
    src/lagrange-sim.cpp
)