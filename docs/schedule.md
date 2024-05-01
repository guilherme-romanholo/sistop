# Main Functions :::

- Specs: https://www.dcce.ibilce.unesp.br/~aleardo/cursos/sisop/projeto.php
- Requires (C): "semaphore.h", "pthreads.h"

- Main (Funções Principais)
    1. VirtualClock
    3. BCP -- Bloco de Controle de Processo

- Virtual Clock
    - Avança conforme duração de operação realizada
    1. interruptControl -- Sinalização de ocorrência de evento externo (Atua conforme Programa Sintético)
    2. sysCalls -- Sinalização de ocorrência de evento interno (Atua conforme Programa Sintético)

- Programa Sintético
    - Example: https://www.dcce.ibilce.unesp.br/~aleardo/cursos/sisop/synthetic_2
    - HEADER
        1. Nome -- (do programa, uma palavra)
        2. Identificador -- (do segmento, um inteiro)
        3. Prioridade -- (original do programa, um inteiro)
        4. Tamanho -- (do segmento, em kbytes)
        5. Lista de Semáforos -- (usados pelo programa, separados por espaços em branco)

    - COMMANDS
        1. exec t -- (indicando que o programa executará por t unidades de tempo)
        2. read k -- (indicando que o programa lerá do disco, na trilha k)
        3. write k -- (indicando que o programa escreverá no disco, na trilha k)
        4. P(s) -- (indicando que o programa acessará a região crítica guardada por s)
        5. V(s) -- (indicando que o programa liberará a região crítica guardada por s)
        6. print t -- (indicando que o programa imprimirá resultados por t unidades de tempo)

- Process
    1. processInterrupt (1) -- Interrupção gerada pela execução da função processCreate e pelo término de operações de E/S
    2. processCreate (2) -- Chamada para iniciar a criação de um processo no BCP
    3. processFinish (3) -- Chamada para terminar a existência de um processo no BCP

- Disk
    1. DiskRequest (4) -- Chamada de operação de entrada/saída para disco
    2. DiskFinish (5) -- Sinalização de final de E/S para disco

- Memory
    1. memLoadReq (6) -- Chamada de operação de carregamento na memória
    2. memLoadFinish (7) -- Sinalização de final de carregamento

- Files
    1. fsRequest (8) -- Chamada para operação no sistema de arquivos
    2. fsFinish (9) -- Sinalização de final de operação no sistema de arquivos

- Semaphore
    1. semaphoreP (10) -- Bloqueio de processo
    2. semaphoreV (11) -- Desbloqueio de processo

- Print
    1. PrintRequest (14) -- Chamada de operação de entrada/saída para impressão
    2. PrintFinish (15) -- Sinalização de final de E/S para impressão

