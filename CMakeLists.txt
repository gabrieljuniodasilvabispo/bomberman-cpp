cmake_minimum_required(VERSION 3.22)

project(Bomberman CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# O jogo atual nao utiliza audio nem rede.
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
)

FetchContent_MakeAvailable(SFML)

include(CTest)
option(BOMBERMAN_COVERAGE "Instrumentar codigo proprio com GCC/gcov" OFF)

# Jogo e testes usam a mesma implementacao das regras, compilada uma unica vez.
add_library(BombermanLogica STATIC
    src/Mapa.cpp src/Bloco.cpp src/Bomba.cpp src/Explosao.cpp
    src/Jogador.cpp src/Inimigo.cpp src/Menu.cpp src/Melhoria.cpp src/Partida.cpp)
target_include_directories(BombermanLogica PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(BombermanLogica PUBLIC SFML::System)

add_executable(Bomberman src/main.cpp)

# Caminho absoluto da pasta assets, usado pelo main.cpp para achar a fonte.
target_compile_definitions(Bomberman PRIVATE
    BOMBERMAN_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

target_link_libraries(Bomberman PRIVATE
    BombermanLogica
    SFML::Graphics
    SFML::Window
)

if(BOMBERMAN_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "Use GCC para instrumentacao com gcov.")
    endif()
    target_compile_options(BombermanLogica PRIVATE --coverage -O0 -g)
    # O runtime de cobertura tambem e necessario aos executaveis consumidores.
    target_link_options(BombermanLogica PUBLIC --coverage)
    target_compile_options(Bomberman PRIVATE --coverage -O0 -g)
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Bomberman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Bomberman>
            $<TARGET_FILE_DIR:Bomberman>
        COMMAND_EXPAND_LISTS
    )
endif()

if(BUILD_TESTING)
    add_executable(BombermanTests
        tests/main.cpp tests/test_posicao.cpp tests/test_mapa.cpp
        tests/test_bloco.cpp tests/test_jogador.cpp tests/test_inimigo.cpp
        tests/test_bomba.cpp tests/test_explosao.cpp tests/test_melhoria.cpp
        tests/test_menu.cpp tests/test_partida.cpp tests/test_defensiva.cpp)
    target_include_directories(BombermanTests PRIVATE third_party)
    target_link_libraries(BombermanTests PRIVATE BombermanLogica)
    if(WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET BombermanTests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:BombermanTests>
                $<TARGET_FILE_DIR:BombermanTests>
            COMMAND_EXPAND_LISTS)
    endif()
    add_test(NAME testes_iniciais COMMAND BombermanTests)
    # O alvo usado por make test-build tambem prepara os testes da entrada da aplicacao.
    add_dependencies(BombermanTests Bomberman)
    foreach(CASO ajuda help invalido excesso)
        add_test(NAME cli_${CASO}
            COMMAND ${CMAKE_COMMAND}
                "-DEXECUTAVEL=$<TARGET_FILE:Bomberman>"
                "-DCASO=${CASO}"
                -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/verificar_cli.cmake")
    endforeach()
endif()
