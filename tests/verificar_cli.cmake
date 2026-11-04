if(NOT DEFINED EXECUTAVEL OR NOT EXISTS "${EXECUTAVEL}")
    message(FATAL_ERROR "Executavel Bomberman ausente. Compile o projeto antes dos testes.")
endif()

if(CASO STREQUAL "ajuda")
    set(ARGUMENTOS --ajuda)
    set(CODIGO_ESPERADO 0)
    set(TEXTO_ESPERADO "Uso: Bomberman")
elseif(CASO STREQUAL "help")
    set(ARGUMENTOS --help)
    set(CODIGO_ESPERADO 0)
    set(TEXTO_ESPERADO "Uso: Bomberman")
elseif(CASO STREQUAL "invalido")
    set(ARGUMENTOS --opcao-inexistente)
    set(CODIGO_ESPERADO 2)
    set(TEXTO_ESPERADO "Erro de entrada: Opcao desconhecida")
elseif(CASO STREQUAL "excesso")
    set(ARGUMENTOS --ajuda extra)
    set(CODIGO_ESPERADO 2)
    set(TEXTO_ESPERADO "Erro de entrada: Numero de argumentos invalido")
else()
    message(FATAL_ERROR "Caso de teste CLI desconhecido: ${CASO}")
endif()

execute_process(
    COMMAND "${EXECUTAVEL}" ${ARGUMENTOS}
    RESULT_VARIABLE RESULTADO
    OUTPUT_VARIABLE SAIDA
    ERROR_VARIABLE ERRO
    TIMEOUT 10)

# Comparacao textual tambem rejeita encerramentos por sinal, falha de processo e timeout.
if(NOT "${RESULTADO}" STREQUAL "${CODIGO_ESPERADO}")
    message(FATAL_ERROR "Codigo esperado ${CODIGO_ESPERADO}; recebido ${RESULTADO}. ${SAIDA}${ERRO}")
endif()
if(CODIGO_ESPERADO EQUAL 0)
    if(NOT SAIDA MATCHES "${TEXTO_ESPERADO}" OR NOT "${ERRO}" STREQUAL "")
        message(FATAL_ERROR "Ajuda nao foi apresentada corretamente: ${SAIDA}${ERRO}")
    endif()
else()
    if(NOT ERRO MATCHES "${TEXTO_ESPERADO}" OR NOT "${SAIDA}" STREQUAL "")
        message(FATAL_ERROR "Erro de entrada nao foi tratado corretamente: ${SAIDA}${ERRO}")
    endif()
endif()
