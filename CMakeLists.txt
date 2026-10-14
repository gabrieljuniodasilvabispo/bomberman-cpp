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

# O prototipo atual possui somente main.cpp; Posicao e um struct no cabecalho.
add_executable(Bomberman src/main.cpp)

# Informa ao CMake onde estão os arquivos .hpp (na pasta include)
target_include_directories(Bomberman PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Bomberman PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Bomberman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Bomberman>
            $<TARGET_FILE_DIR:Bomberman>
        COMMAND_EXPAND_LISTS
    )
endif()

include(CTest)
option(BOMBERMAN_TDD_STUBS "Permitir auxiliares que falham para classes ainda ausentes" ON)
option(BOMBERMAN_COVERAGE "Instrumentar codigo proprio com GCC/gcov" OFF)

if(BUILD_TESTING)
    set(CLASSES Mapa Bloco Bomba Explosao Player Enemy Menu Melhoria Partida)
    set(FONTES_TESTADAS)
    set(CLASSES_PENDENTES)
    foreach(CLASSE IN LISTS CLASSES)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${CLASSE}.cpp")
            list(APPEND FONTES_TESTADAS "src/${CLASSE}.cpp")
        elseif(BOMBERMAN_TDD_STUBS)
            list(APPEND FONTES_TESTADAS "tests/tdd/${CLASSE}.cpp")
            list(APPEND CLASSES_PENDENTES "${CLASSE}")
        else()
            message(FATAL_ERROR "Falta src/${CLASSE}.cpp. A fase RED usa BOMBERMAN_TDD_STUBS=ON.")
        endif()
    endforeach()
    if(CLASSES_PENDENTES)
        message(STATUS "TDD RED - classes nao implementadas: ${CLASSES_PENDENTES}")
        if(BOMBERMAN_COVERAGE)
            message(FATAL_ERROR "Cobertura de auxiliares TDD nao representa o jogo. Implemente as classes antes de medir cobertura.")
        endif()
    endif()
    add_executable(BombermanTests
        tests/main.cpp tests/test_posicao.cpp tests/test_mapa.cpp
        tests/test_bloco.cpp tests/test_jogador.cpp tests/test_inimigo.cpp
        tests/test_bomba.cpp tests/test_explosao.cpp tests/test_melhoria.cpp
        tests/test_menu.cpp tests/test_partida.cpp ${FONTES_TESTADAS})
    target_include_directories(BombermanTests PRIVATE include third_party)
    target_link_libraries(BombermanTests PRIVATE SFML::System)
    if(BOMBERMAN_COVERAGE)
        if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            message(FATAL_ERROR "Use GCC para instrumentacao com gcov.")
        endif()
        target_compile_options(BombermanTests PRIVATE --coverage -O0 -g)
        target_link_options(BombermanTests PRIVATE --coverage)
    endif()
    if(WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET BombermanTests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:BombermanTests>
                $<TARGET_FILE_DIR:BombermanTests>
            COMMAND_EXPAND_LISTS)
    endif()
    add_test(NAME testes_iniciais COMMAND BombermanTests)
endif()
