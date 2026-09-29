cmake_minimum_required(VERSION 3.22)

project(Bomberman CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# Desativação de módulos do SFML que não serão usados para acelerar a compilação. ATIVAR DEPOIS!!!!!
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
)

FetchContent_MakeAvailable(SFML)

# Declara o executável listando o main e todas as implementações em src/
add_executable(Bomberman
    src/main.cpp
    src/Map.cpp
    src/Block.cpp
    src/Bomb.cpp
    src/Explosion.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Menu.cpp
    src/Effects.cpp
    src/Game.cpp
)

# Informa ao CMake onde estão os arquivos .hpp (na pasta include)
target_include_directories(Bomberman PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Bomberman PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(WIN32)
    add_custom_command(TARGET Bomberman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Bomberman>
            $<TARGET_FILE_DIR:Bomberman>
        COMMAND_EXPAND_LISTS
    )
endif()