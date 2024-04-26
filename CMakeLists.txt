set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (Raytracer)

find_package(Eigen3 REQUIRED)
find_package(OpenMP)


include_directories(${EIGEN3_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp")

add_executable (${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)


target_include_directories(
    ${PROJECT_NAME}
    PUBLIC include
    )
# add_custom_command(
#     TARGET ${PROJECT_NAME}
#     POST_BUILD
#     COMMAND "../run.sh"
# )
    