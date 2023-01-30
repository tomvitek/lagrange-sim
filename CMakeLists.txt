cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-fopenmp")

project(lagrange-sim)

add_executable(satellite
    src/execs/satellite.cpp
    src/lagrange-sim.cpp
    src/input-args-parser.cpp
)

target_link_libraries(satellite
    ${Eigen_LIBRARIES}
)

add_executable(lagrange-rings
    src/execs/lagrange-rings.cpp
    src/lagrange-sim.cpp
    src/input-args-parser.cpp
)

add_executable(lagrange-span
    src/execs/lagrange-span.cpp
    src/lagrange-sim.cpp
    src/input-args-parser.cpp
)