# Parte 1 :::

- Na primeira parte do projeto devem ser implementadas a interface de controle do sistema e as funções indicadas a seguir:

    1. processInterrupt (1) -- interrupção gerada pelo final do quantum-time de um processo
    2. semaphoreP (10) -- tratamento de bloqueio de processo
    3. semaphoreV (11) -- tratamento de desbloqueio de processo
    4. memLoadReq (6) -- chamada de operação de carregamento na memória
    5. memLoadFinish (7) -- sinalização de final de carregamento
    6. processCreate (2) -- chamada para iniciar a criação de um processo no BCP *
    7. processFinish (3) -- chamada para terminar a existência de um processo no BCP * 

- BCP (Bloco de Controle de Processos) é a estrutura que armazena os dados dos processos em execução 
 
### Algoritmo de Escalonamento

- Deve ser implementado uma variação do Round-Robin, em que o quantum de cada processo será proporcional à sua prioridade. O valor base, para processos com prioridade igual a 1, será de 5000 unidades de tempo. O quantum específico de um processo será igual ao valor base dividido pela prioridade, ou seja, um processo com prioridade 1 terá quantum-time de 5000, enquanto outro com prioridade 3 terá quantum-time de 1666.
- O processo que estiver em execução será interrompido sempre que solicitar algo diferente de exec, ou o usuário solicitar a execução de um novo processo (processCreate).
- Chamadas para P(s) e V(s), quando não gerarem bloqueio do processo, contabilizarão 200 unidades de tempo e manterão o processo que as executou na CPU, exceto se a operação seguinte for de entrada/saída.
 
### Gerenciamento de memória

    1. O espaço total da memória é de 1 Gbyte (já descontado o espaço ocupado pelo SO);
    2. Deve ser simulada uma memória com alocação em blocos, com páginas de 8 kbytes;
    3. A ocupação da memória deve ser gerenciada através de uma estrutura baseada em segmentos;
    4. O mecanismo de memória virtual deve ser implementado usando o algoritmo da segunda chance. 

 
### Interface de controle

- A interface de controle deverá permitir que sejam submetidos novos processos, visualizados dados sobre o estado dos processos em execução e sobre o estado de ocupação da memória.
 
**Deve ser observado que as visualizações devem ser solicitadas explicitamente, ficando normalmente visível apenas informações sobre o processo em execução.**

### Prazo de entrega

- 26 de maio
