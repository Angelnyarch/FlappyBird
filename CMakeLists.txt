cmake_minimum_required(VERSION 3.16)
project(FlappyBird LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(BUILD_SHARED_LIBS FALSE)
FetchContent_MakeAvailable(SFML)

add_executable(FlappyBird
    main.cpp
    Bird.cpp
    Parallax.cpp
    Obstacle.cpp
    UISound.cpp
)

target_link_libraries(FlappyBird PRIVATE SFML::Graphics SFML::Audio)

# Copia la carpeta resources junto al ejecutable después de compilar
add_custom_command(TARGET FlappyBird POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:FlappyBird>/resources)