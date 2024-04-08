set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (Raytracer)

file(GLOB SOURCES "src/*.cpp")

add_executable (${PROJECT_NAME} ${SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC include
    )