cmake_minimum_required(VERSION 3.30)
project(bee_solve_vrp)

set(CMAKE_CXX_STANDARD 20)

find_package(Python3 REQUIRED COMPONENTS Development NumPy)

include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(bee_solve_vrp
        main.cpp
        utils/problem.cpp
        utils/problem.h
        utils/solution.cpp
        utils/solution.h
        algorithm/local_search.cpp
        algorithm/local_search.h
        algorithm/neighbor_operator.cpp
        algorithm/neighbor_operator.h
        algorithm/bee_colony.cpp
        algorithm/bee_colony.h
        utils/visualize.cpp
        utils/visualize.h
)

target_link_libraries(bee_solve_vrp
        ${Python3_LIBRARIES}
)
