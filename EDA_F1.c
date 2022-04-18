#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_LINE_LENGTH 128

/*
 ----------------------------- Estrutura de Armazenamento de Dados --------------------------------
*/

/*!
 A estrutura das operações é uma lista em que cada node contém informação sobre quantas e quais máquinas  pode ser executada a tarefa e a sua duração.
*/
typedef struct node_s 
{
    int *machines;
    int *times;
    int length;
    struct node_s *next;
} Tasks;

/*!
 A estrutura do Job contém uma lista de operações e informação sobre quantas operações tem que efetuar.
*/
typedef struct list_s {
    int n_operations;
    struct node_s * head;
} Jobs;

/*
---------------------------- Funções de modificação dos dados --------------------------------------
*/


/*!
 Função de inicialização da estrutura de armazenamento de dados
*/
void init_list(Jobs *list) {
        list->n_operations = 0;
        list->head = NULL;
}

void init_node(Tasks *task,int size, int *m, int *t)
{
	task->length = size;
	task->machines = malloc(size * sizeof(int));
	task->times = malloc(size * sizeof(int));
	
	for(int i = 0;i < size;i++)
	{
		task->machines[i] = m[i];
		task->times[i] = t[i];
	}
	task->next = NULL;
}

/*!
 Função para a inserção de uma tarefa. Recebe como argumentos o Job e a nova tarefa.
 Se a lista estiver vazia, adiciona a nova tarefa como cabeça da lista
 Senão irá percorrer a lista até ao fim e adiciona a nova tarefa como fim da lista
*/
void add(Jobs *list, Tasks *new_node) 
{ 
    if(list->head == NULL)
    {
        list->head = new_node;
        new_node->next = NULL;
    }
    
    Tasks *node = list->head;
    while(node->next != NULL)
        node = node->next;       
    
    node->next = new_node;
    new_node->next = NULL;
    ++list->n_operations;
}

/*!
Função para a inserção de uma tarefa. Recebe como argumentos um Job e a nova tarefa.
Se a lista estiver vazia, adiciona a nova tarefa como cabeça da lista
Senão irá percorrer a lista até ao fim e adiciona a nova tarefa como fim da lista

Esta função distingue-se da anterior por não alterar o numero de tarefas na estrutura Job, uma vez que é utilizada durante a leitura do ficheiro, e essa informação já se encontra atualizada
*/
void add_from_file(Jobs *list, Tasks *new_node) 
{ 
	printf("A adicionar task..\n");
    if(list->head == NULL)
    {
        list->head = new_node;
        new_node->next = NULL;
    }
    Tasks *node = list->head;
    
    while(node->next != NULL)
        node = node->next;   
	    
    node->next = new_node;
    new_node->next = NULL;
}

/*!
 Função para remover a cabeça da lista
*/
void pop(Jobs *list) 
{
  Tasks *head = list->head;
  if (head) 
  {
    list->head = head->next;
    head->next = NULL;
  }
  --list->n_operations;
  free(head);
}


/*!
Função que remove uma das tarefas pelo indice. Recebe como argumentos um Job e o indice da tarefa a remover.
Se o indice for 0 significa que é necessário remover a cabeça então é feito pop da lista.
Senão percorre a lista até encontrar o indice anterior ao pretendido e remove o node seguinte mantendo a continuação da lista.
*/
int remove_by_index(Jobs *list, int n)
{
    if (list->head == NULL)
        return 1;
    if (n==0)
    {
        pop(list);
        return 0;
    }
        
    Tasks *p_node = list->head;
    for (int i = 0; i<n-1; i++)
    {
        p_node = p_node->next;
        if(p_node == NULL)
            return 1;
    }
    Tasks *node = p_node->next;
    p_node->next = node->next;
    free(node);
    --list->n_operations;
    
    return 0;
}

/*!
Função que substitui uma das tarefas pelo indice. Recebe como argumentos um Job e o indice da tarefa a remover.
Se o indice for 0 significa que é necessário remover a cabeça então é feita a alteração da cabeça de lista-
Senão percorre a lista até encontrar o indice anterior ao pretendido e substitui o node seguinte mantendo a continuação da lista.
É utilizada na alteração de informação de uma tarefa já existente.
*/

int substitute(Jobs *list, int n, Tasks *new_node)
{
    if (list->head == NULL)
        return 1;
    if(n==0)
    {
       new_node->next = list->head->next;
       list->head->next = NULL;
       list->head = new_node;
    }
    else
    {
        Tasks *p_node = list->head;
        for (int i = 0; i<n-1; i++)
        {
            p_node = p_node->next;
            if(p_node == NULL)
                return 1;
        }
        Tasks *node = p_node->next;
        Tasks *n_node = node->next;
        node->next = NULL;
        p_node->next = new_node;
        new_node->next = n_node;
        free(node);
    }
    return 0;
}

/*
 --------------------------------- Funções para calculos de tempo -----------------------------
*/

/*!
Função que calcula a quantidade mínima de unidades de tempo necessárias para completar um job. Recebe como argumentos um Job.
São percorridos todas as tarefas e selecionadas as máquinas com menos tempo de execução
*/
void minimum(Jobs *list)
{
    if(list->head == NULL)
    {
        printf("Lista vazia\n");
        return;
    }
    Tasks* node = list->head;
    int total_time = 0;
    int i = 0;
    printf("Operações: \n");
    while(node != NULL)
    {
        int min_time = find_minimum(node->times,node->length);
        total_time += node->times[min_time];
        printf("Operação %d: Maquina %d com duraçao %d\n",i,node->machines[min_time],node->times[min_time]);
        i++;
        node = node->next;
    }
    
    printf("Tempo minimo: %d \n",total_time);
}


/*!
Função que calcula a quantidade máxima de unidades de tempo necessárias para completar um job. Recebe como argumentos um Job.
São percorridos todas as tarefas e selecionadas as máquinas com mais tempo de execução
*/
void maximum(Jobs *list)
{
    if(list->head == NULL)
    {
        printf("Lista vazia\n");
        return;
    }
    Tasks* node = list->head;
    int total_time = 0;
    int i=0;
    printf("Operações: \n");
    while(node != NULL)
    {
        int max_time = find_maximum(node->times,node->length);
        total_time += node->times[max_time];
        printf("Operação %d: Maquina %d com duraçao %d\n",i,node->machines[max_time],node->times[max_time]);
        i++;
        node = node->next;
    }
    
    printf("Tempo maximo: %d \n",total_time);
}


/*!
Função que calcula a quantidade média de unidades de tempo necessárias para completar um job tendo em conta todas as combinações possiveis. Recebe como argumentos um Job.
São percorridos todas as tarefas e calculado o tempo médio para cada tarefa, e posteriormente efetuada a média desse valor, obtendo assim uma média de todos os tempos possiveis.
*/
void average(Jobs *list)
{
    if(list->head == NULL)
    {
        printf("Lista vazia\n");
        return;
    }   
    Tasks* node = list->head;
    int total_time = 0;
    int task_average = 0;
    int i = 0;
    printf("Operações: \n");
    while(node != NULL)
    {  
        int average = 0;
        for(int j = 0; j < node->length; j++)
            average += node->times[j];
		
		task_average = average/node->length;
        printf("Operação %d Media: %d\n",i,task_average);
        total_time += task_average;
        i++;
        node = node->next;
    }
    printf("Tempo medio: %d\n",total_time);
}

/*!
Funções Auxiliares para o calculo da execução dos jobs
*/
int find_minimum(int a[], int n) {
  int c, index = 0,min = a[0];
 
  for (c = 1; c < n; c++)
    if (a[c] < min)
      index = c;

  return index;
}

int find_maximum(int a[], int n) {
  int c, index = 0,max = a[0];
 
  for (c = 1; c < n; c++)
    if (a[c] > max)
      index = c;

  return index;
}

/*
 ------------------------------------- Funções para ficheiros -------------------------------------
*/

/*!
Função que percorre um ficheiro de forma a obter a informação contida nele. O ficheiro necessita ter a formatação esperada. Sem esta formatação não é garantido que seja feita a leitura correta.
*/

int read_file(Jobs *list)
{

    FILE *fp=NULL;
    char s[50];
    printf("Indique a localização do ficheiro: \n");
    scanf("%s",s);


    fp = fopen(s, "r");
    if(fp == NULL) {
        perror("Erro a abrir ficheiro!\n");
        return 1;
    }
    else
        printf("Ficheiro aberto com sucesso\n");
    
    char line[MAX_LINE_LENGTH];
    char *parse;
    printf("A ler ficheiro...\n");
    
    int i = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
    	printf("Lido: %s\n",line);
        if(strcmp(line,"Plan\n") == 0)
        {
           fgets(line, MAX_LINE_LENGTH, fp);
           printf("Lido: %s\n",line);
           parse = strtok(line,",");
           
            while(parse)
            {
                printf("Guardado: %s\n",parse);
                parse = strtok(NULL,",");
            }
        }
        else if(strcmp(line,"Process\n") == 0 )
        {
        	i++;
            fgets(line, MAX_LINE_LENGTH, fp);
            printf("Lido: %s\n",line);
            list->n_operations = atoi(line);
            printf("Guardado: %d\n",atoi(line));
            printf("Processo %d\n",i);
        }
        
        else if(strcmp(line,"Operation\n") == 0 )
        {
            fgets(line, MAX_LINE_LENGTH, fp);
            printf("Lido: %s\n",line);
            int n_machines = atoi(line);
            
            int task_mach[n_machines];
            int task_time[n_machines];
            
            fgets(line, MAX_LINE_LENGTH, fp);
            printf("Lido: %s\n",line);
            parse = strtok(line,",");
            for(int i = 0; i < n_machines; i++)
            {
            	task_mach[i] = atoi(parse);
				printf("Guardado: %d\n",task_mach[i]);
				parse = strtok(NULL,",");
			}
            fgets(line, MAX_LINE_LENGTH, fp);
            printf("Lido: %s\n",line);
            parse = strtok(line,",");
            for(int i = 0; i < n_machines; i++)
            {
            	task_time[i] = atoi(parse);
            	printf("Guardado: %d\n",task_time[i]);
            	parse = strtok(NULL,",");
			} 
			 
            struct node_s *pointer = malloc(sizeof(struct node_s));
            init_node(pointer,n_machines,&task_mach,&task_time);
            add_from_file(list, pointer);
        }
    }
    
    fclose(fp);
    free(line);
    
    printf("Fim da leitura");
}

/*!
 Esta função escreve para um ficheiro a informação armazenada nas estruturas de dados.
*/
int write_file(Jobs *list)
{
    FILE *fp=NULL;
    char s[50];
    printf("Indique a localização do ficheiro: \n");
    scanf("%s",s);
    fp = fopen(s, "w");

    if(fp == NULL) {
        perror("Erro a abrir ficheiro!");
        return 1;
    }
    else
        printf("Ficheiro aberto com sucesso\n");
    
    fprintf(fp,"Processo 1 \n");
    fprintf(fp,"Numero de operações: %d \n", list->n_operations);
    printf("Numero de operações: %d \n", list->n_operations);
    
    Tasks *node = list->head;
    int t = 1;
    while(node != NULL)
    {
        fprintf(fp,"Operação %d \n", t);
        printf("Operação %d \n", t);
        for(int i = 0; i < node->length; i++)
        {
        	fprintf(fp,"Máquina: %d Tempo de Execução: %d \n", node->machines[i], node->times[i]);
        	printf("Máquina: %d Tempo de Execução: %d \n", node->machines[i], node->times[i]);
		}
        t++;    
        node = node->next;
    }
    
    fclose(fp);
    
    return 0;
}

//-----------------------------------------------------------------------------------------------
/*!
 Função de auxilio da main, contém o código de recolha de informação para acrescentar uma nova tarefa
*/
Tasks* aux_choice3()
{
    int size;
    printf("Indique quantas máquinas estão disponiveis para realizar a operação:\n");
    scanf("%d",&size);
    int mach[size];
    int times[size];
    printf("Indique as máquinas por ordem, uma de cada vez\n");
    for(int i = 0; i < size; i++)
    {
        printf("Indique a máquina nº%d:\n",i);
        scanf("%d",&mach[i]);
    }
    printf("Indique os tempos de execução por ordem, um de cada vez\n");
    for(int i = 0; i < size; i++)
    {
        printf("Indique o tempo de exucução na máquina nº%d:\n",i);
        scanf("%d",&times[i]);
    }
    struct node_s *pointer = malloc(sizeof(struct node_s));
    init_node(pointer,size,&mach,&times);
        
    return pointer;
}


/*!
 Função de auxilio da main, contém o código de recolha de informação para a alteração de uma tarefa existente
*/
Tasks* aux_choice5()
{
    int size;
    printf("Indique quantas máquinas estão disponiveis para realizar a operação:\n");
    scanf("%d",&size);
    int mach[size];
    int times[size];
    printf("Indique as máquinas por ordem, uma de cada vez\n");
    for(int i = 0; i < size; i++)
    {
        printf("Indique a máquina nº%d:\n",i);
        scanf("%d",&mach[i]);
    }
    printf("Indique os tempos de execução por ordem, um de cada vez\n");
    for(int i = 0; i < size; i++)
    {
        printf("Indique o tempo de exucução na máquina nº%d:\n",i);
        scanf("%d",&times[i]);
    }
    
    struct node_s *pointer = malloc(sizeof(struct node_s));
    init_node(pointer,size,&mach,&times);
    
    return pointer;
}

/*!
 Função que imprime na consola a informação armazenada na estrutura de dados
*/
void show_info(Jobs *list)
{
	if (list->head == NULL)
	{
		printf("Lista vazia\n");
		return;
	}
	else
	{
		printf("Numero de operacoes: %d \n", list->n_operations);
		Tasks *node = list->head;
    	int t = 1;
    	do
   		{
        	printf("Operacao %d \n", t);
        	for(int i = 0; i < node->length; i++)
        		printf("Maquina: %d Tempo de Execucao: %d \n", node->machines[i], node->times[i]);
        
        	node = node->next;
        	t++;
   		} while(node != NULL);
	}
}

/*!
Função main
*/
int main()
{
    Jobs jobs[1];
    init_list(jobs);
    
    int choice = 10;
    int r = 0;
    int n;
    Tasks *task;
    
    do
    {
        printf("\n");
        printf("\n\t Menu \n\n");
        printf("1. Ler de um ficheiro\n");
        printf("2. Escrever para um ficheiro\n");
        printf("3. Adicionar Operacao\n");
        printf("4. Remover Operacao\n");
        printf("5. Alterar Operacao\n");
        printf("6. Calcular tempo minimo\n");
        printf("7. Calcular tempo maximo\n");
        printf("8. Calcular tempo medio\n");
        printf("9. Mostrar Informacao\n");
        printf("10. Sair\n");
        scanf("%d",&choice);  
     switch (choice)
     {
        case 1: 
            read_file(jobs);
            break;
        case 2: 
            write_file(jobs);
            break;
        case 3:
            printf("Adicionar Operacao\n");
            task = aux_choice3();
            add(jobs,task);
            printf("Operacao adicionada");
            break;
        case 4:
            printf("Remover operacao\n");
            printf("Indique qual o numero da operacao a remover:\n");
            scanf("%d",&n);
            r = remove_by_index(jobs,n-1);
            if (r == 1)
                printf("A operacao indicada nao existe\n");
            else
                printf("Operacao foi removida com sucesso\n");
            break;
        case 5:
            printf("Alterar operacao\n");
            int n;
    		printf("Indique qual o numero da operação a alterar:\n");
    		scanf("%d",&n);
            task = aux_choice5();
            r = substitute(jobs,n-1,task);
            if (r == 1)
                printf("A operacao indicada nao existe\n");
            else
                printf("Operacao foi alterada com sucesso\n");
            break;
        case 6:
            printf("Calcular o tempo minimo:\n");
            minimum(jobs);
            break;
        case 7:
            printf("Calcular o tempo maximo:\n");
            maximum(jobs);
            break;
        case 8:
            printf("Calcular o tempo medio:\n");
            average(jobs);
            break;
        case 9:
        	printf("Mostrar informacao:\n");
        	show_info(jobs);
        	break;
        case 10: 
            printf("Programa Terminado\n"); 
            return 0;
            break;
        default: 
            printf("Operacao Inexistente. Escolha novamente\n");
            break;
     } 
      
    } while (choice != 10);
}
