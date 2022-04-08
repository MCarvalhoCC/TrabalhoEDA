#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUZZ_SIZE 1024


/*Definição de uma estrutura de dados dinâmica para a representação de um job com um conjunto finito de n
operações;*/

/* Uma lista de Jobs que contém uma lista de operações que contém um array de máquinas e um de tempo de resolução*/


typedef struct opList
{
	int *machines;
	int *times;
	struct node * next;
} opNodes;

typedef struct jobsList {
    struct opList * operations;
    struct node * next;
} jobNodes;

typedef struct plan
{
	int nmachines;
	int noperations;
	int njobs;
	struct jobsList * jobList;
} pPlan;

void remove_by_index(opNodes ** head, int n) {
    int i = 0;
    int retval = -1;
    opNodes * current = *head;
    opNodes * temp_node = NULL;

    if (n == 0) {
        return pop(head);
    }

    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }

    if (current->next == NULL) {
        return -1;
    }

    temp_node = current->next;
    current->next = temp_node->next;
    free(temp_node);
}

void addOperation(opNodes * head, int newMachines[], int newTimes[])
{
	opList * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (opNodes *) malloc(sizeof(opNodes));
    current->next->machines = newMachines;
    current->next->times = newTimes;
    current->next->next = NULL;
}

void addJob(jobNodes * head, opNodes * head)
{
	jobNodes * current = head;
	while (current->next != NULL) {
        current = current->next;
    }
    current->next = (jobNodes *) malloc(sizeof(jobNodes))~;
    current->next->opList = opNodes * head;
    current->next->next = NULL;
}

int initializeStruct()
{
	opNodes * head = NULL;
	head = (opNodes *) malloc (sizeof(opNodes));
	if (head == NULL)
		return 1;

	head->&machines = NULL;
	head->&times = NULL;
	head->next = NULL;
	
	jobNodes * head = NULL;
	head = (jobNodes *) malloc (sizeof(jobNodes));
	if (head == NULL)
		return 1;	
	head->operations = opNodes;
	head->next = NULL;

	pPlan->nmachines = 0;
	pPlan->noperations = 0;
	pPlan->njobs = 0;
	pPlan->jobList = jobNodes;

	return 0;
}


int readFile()
{
	char buff[BUZZ_SIZE];
    FILE *file = fopen("test.txt", "r");
    if (fp)
    {
    	int i = 0;
    	while(fgets(buff, BUZZ_SIZE, file) != NULL)
    	{
    		if(i==0)
    		{
    			pPlan->nmachines = atoi(buff);
    			fgets(buff, BUZZ_SIZE, file);
    			pPlan->njobs = atoi(buff);
    			fgets(buff, BUZZ_SIZE, file);
    			pPlan->noperations = atoi(buff);
    			i++;
    		}
    		else
    		{
    			if(strstr(buff, "Job")!=NULL)
    			{
    				fgets(buff, BUZZ_SIZE, file);
    				int mach[pPlan->nmachines];
    				int time[pPlan->nmachines];
    				int m = 0;
    				int t = 0;
    				while(strstr(buff, "Job")!=NULL)
    				{
    					char *pt;
    					pt = strtok (buff,",");
    					while (pt != NULL) {
        					mach[m] = atoi(pt);
        					m++;
    					}
    					fgets(buff, BUZZ_SIZE, file);
    					pt = strtok (buff,",");
    					while (pt != NULL) {
        					time[t] = atoi(pt);
        					t++;
    					}
    					fgets(buff, BUZZ_SIZE, file);
    				}
    				addOperation(jobNodes->operations,mach,time);
    			}
    		}
    	}
	}
}

int 

int main()
{
	int v = initializeStruct();
	if (v == 1)
		return 1;
	readFile();
}	
