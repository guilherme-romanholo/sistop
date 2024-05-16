#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

//LISTA
typedef struct node {
    void *content;
    struct node *next;
    struct node *prev;
} Node;

typedef struct list {
    Node *head;
    Node *tail;
    int size;
} List;

List* List__create() {
    List *list = malloc(sizeof(List));

    if (list == NULL) {
        printf("Not memory to allocate list.");
        exit(1);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

Node* Node__create(void *content) {
    Node *node = malloc(sizeof(Node));

    if (node == NULL) {
        printf("Not memory to allocate node.");
        return NULL;
    }

    node->content = content;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void List__append(List *list, void *content) {
    Node *node = Node__create(content);

    if (node == NULL)
        return;

    if (list->head == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
    }

    list->tail = node;
    list->size++;
}

//SEMÁFORO

typedef struct Semaphore {
    char name[2];
    int val;
    List *waiters;
    sem_t  mutex;
} semaphore_t;

typedef struct SemaphoreTable {
    semaphore_t *table;
    int len;
} semaphore_table_t;

void semaphore_init(semaphore_t *sem, const char *name, const int val) {
    strcpy(sem->name,name);
    sem->val       = val;
    sem->waiters = list_init();
    sem_init(&sem->mutex, 0, 1);
}



//INSTRUÇÕES
typedef struct Instruction {
    char op[10];
    int value;
    //char* sem;
} instr_t;

//PROCESSO
typedef struct {
    int pid;
    int state;
    char name[10];
    //int segment_id;
    int priority;
    //int segment_size;
    int pc;
    instr_t* code;
} process_t;

//ESCALONADOR
typedef struct Scheduler {
    process_t* scheduled_proc;
    process_t* next_proc_to_schedule;
} scheduler_t;

//KERNEL

typedef struct Kernel {
    /* Process Table Information (aka PCB) */
    process_t* proc_table;

    int next_proc_id;
    /* Scheduler Information */
    scheduler_t scheduler;

    /* Semaphore Table Information */
    semaphore_table_t sem_table;
} kernel_t;

instr_t* read_process_code(FILE* fp){
    instr_t* code = NULL;
    char op_code[20];
    int tempo, i = 0;

    while (fscanf(fp, "%s %d", op_code, &tempo) == 2) {
        // Realocar memória para o vetor e adicionar um novo elemento
        code = (instr_t *)realloc(code, (i + 1) * sizeof(instr_t));
        if (code == NULL) {
            perror("Erro ao realocar memória");
            return NULL;
        }

        // Atribuir os valores lidos à estrutura inst_t e adicionar ao vetor

       
        strcpy(code[i].op,op_code);
        code[i].value = tempo;
        i++;
    }

    fclose(fp);

    return code;
}

void eliminar_processo(process_t* processos, int* tamanho, int pid) {
    int encontrado = 0;
    for (int i = 0; i < *tamanho; i++) {
        if (processos[i].pid == pid) {
            encontrado = 1;
        }
        if (encontrado && i < *tamanho - 1) {
            processos[i] = processos[i + 1];
        }
    }
    if (encontrado) {
        (*tamanho)--;
    }
}

int exec_process(process_t *process){
    int quantum = 5000/process->priority;
    int process_end = 0, pc = process->pc;
    while(quantum > 0 && !(process_end)){
        if(strcmp(process->code[pc].op,"exec") == 0){
            printf("Processo %d executando: %d u.t.; restam %d u.t.\n", process->pid,process->code[pc].value, quantum);
            sleep(3);
        }else if(strcmp(process->code[pc].op,"read") == 0){
            printf("Processo %d lendo: %d u.t.; restam %d u.t.\n", process->pid,process->code[pc].value, quantum);
            sleep(3);
        }else if(strcmp(process->code[pc].op,"write") == 0){
            printf("Processo %d escrevendo: %d u.t.; restam %d u.t.\n", process->pid,process->code[pc].value, quantum);
            sleep(3);
        }else if(strcmp(process->code[pc].op,"print") == 0){
            printf("Processo %d printando: %d u.t.; restam %d u.t.\n", process->pid,process->code[pc].value, quantum);
            sleep(3);
        }

        if(quantum < process->code[pc].value){
            process->code[pc].value -= quantum;
            quantum = 0;
        }else{
            quantum = quantum - process->code[pc].value;
            pc++;
        }

        if(pc > 11)
            process_end = 1;
    }
    process->pc = pc;
    return process_end;
}


int main()
{   
    FILE *fp1,*fp2,*fp3;
    kernel_t* Kernel = (kernel_t *)malloc(sizeof(kernel_t));
    process_t *processTable;
    List *procTable = List__create();
    int num_process =0, process_scheduled = 0;

    processTable = (process_t *)malloc(3 * sizeof(process_t));

    if((fp1 = fopen("/home/luanbonizi/VS code/QuintoSemestre/synt1.txt", "r"))== NULL){
        printf("Arquivo não foi lido");
        return 1;
    }

    if((fp2 = fopen("/home/luanbonizi/VS code/QuintoSemestre/synt2.txt", "r"))== NULL){
        printf("Arquivo não foi lido");
        return 1;
    }

    if((fp3 = fopen("/home/luanbonizi/VS code/QuintoSemestre/synt3.txt", "r"))== NULL){
        printf("Arquivo não foi lido");
        return 1;
    }

    //Inicializando o kernel
    Kernel->next_proc_id = 1;
    Kernel->proc_table = (process_t *) malloc(sizeof(process_t) * 5);
    Kernel->scheduler.scheduled_proc = NULL;
    Kernel->scheduler.next_proc_to_schedule = NULL;

    //Inicializando processos
    strcpy(processTable[0].name, "synt1");
    processTable[0].pc = 0;
    processTable[0].pid = 1;
    processTable[0].priority = 1;
    processTable[0].state = 1; //READY
    processTable[0].code = read_process_code(fp1);
    num_process++;

    strcpy(processTable[1].name, "synt2");
    processTable[1].pc = 0;
    processTable[1].pid = 2;
    processTable[1].priority = 1;
    processTable[1].state = 1; //READY
    processTable[1].code = read_process_code(fp2);
    num_process++;

    strcpy(processTable[2].name, "synt3");
    processTable[2].pc = 0;
    processTable[2].pid = 3;
    processTable[2].priority = 1;
    processTable[2].state = 1; //READY
    processTable[2].code = read_process_code(fp3);
    num_process++;

    List__append(procTable,(void *)&processTable[0]);
    List__append(procTable,(void *)&processTable[1]);
    List__append(procTable,(void *)&processTable[2]);

    if(Kernel->scheduler.scheduled_proc == NULL && num_process != 0){
            //pegar o processo que está na cabeça da lista e colocá-lo para executar
            Kernel->scheduler.scheduled_proc = &processTable[process_scheduled];
            Kernel->scheduler.next_proc_to_schedule = &processTable[process_scheduled + 1];
        }
    
    do{

        if(exec_process(Kernel->scheduler.scheduled_proc)){
            printf("\nProcesso %d terminou a sua execução\n", Kernel->scheduler.scheduled_proc->pid);
            //Chama o process finish para tirar o processo do PCB
            eliminar_processo(processTable,&num_process,Kernel->scheduler.scheduled_proc->pid);
            process_scheduled++;
            num_process--;

            Kernel->scheduler.scheduled_proc = &processTable[(process_scheduled) % num_process];
            Kernel->scheduler.next_proc_to_schedule = &processTable[(process_scheduled + 1) % num_process];

        }else{
            printf("\nProcesso %d estourou o quantum time\n", Kernel->scheduler.scheduled_proc->pid);
            //Chama o process block para bloquear o processo e colocar o próximo para executar
            process_scheduled++;
            Kernel->scheduler.scheduled_proc = &processTable[(process_scheduled) % num_process];
            Kernel->scheduler.next_proc_to_schedule = &processTable[(process_scheduled + 1) % num_process];
        }


    }while(num_process != 0);
    return 0;

}