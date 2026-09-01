cmake_minimum_required(VERSION 3.14)

project(Bomberman CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
)

FetchContent_MakeAvailable(SFML)

add_executable(Bomberman
    src/main.cpp
)

target_link_libraries(Bomberman PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# Copiar DLLs automaticamente no Windows
if(WIN32)
    add_custom_command(TARGET Bomberman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Bomberman>
            $<TARGET_FILE_DIR:Bomberman>
        COMMAND_EXPAND_LISTS
    )
endif()