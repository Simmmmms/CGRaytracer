set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (Raytracer)

find_package(OpenMP)

file(GLOB SOURCES "src/*.cpp")

add_executable (${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC external
)

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
    