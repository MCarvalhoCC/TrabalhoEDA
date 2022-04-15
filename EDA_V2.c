#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node_s
{
	int *machines;
	int *times;
	int length;
	struct node_s *next;
} Tasks;

typedef struct list_s {
    int n_operations;
    struct node_s * head;
} Jobs;


void init_list(Jobs *list) {
        list->n_operations = 0;
        list->head = NULL;
}


void add(Jobs *list, Tasks *new_node) 
{ 
    Tasks *node = list->head;
    while(node->next != NULL)
        node = node->next;       
    
    node->next = new_node;
    new_node->next = NULL;
    ++list->n_operations;
}

Tasks *pop(Jobs *list) 
{
  Tasks *head = list->head;
  if (head) 
  {
    list->head = head->next;
    head->next = NULL;
  }
  --list->n_operations;
  return head;
}

int remove_by_index(Jobs *list, int n)
{
    if (n==0)
        pop(list);
        
    Tasks *p_node = list->head;
    for (int i = 0; i<n-1; i++)
    {
        p_node = p_node->next;
        if(p_node == NULL)
            return 1;
    }
    p_node->next = p_node->next->next;
    --list->n_operations;
    
    return 0;
}

int substitute(Jobs *list, int n, Tasks *new_node)
{
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
        Tasks *n_node = p_node->next->next;
        p_node->next->next = NULL;
        p_node->next = new_node;
        new_node->next = n_node;
    }
    return 0;
}

void minimum(Jobs *list)
{
    Tasks* node = list->head;
    int minimums[list->n_operations];
    int total_time = 0;
    int i = 0;
    printf("Operações: \n");
    while(node->next != NULL)
    {
        int min_time = find_minimum(node->times,node->length);
        total_time += min_time;
        printf("Operação %d: Maquina %d com duraçao %d",i,node->machines[min_time],node->times[min_time]);
        i++;
        node = node->next;
    }
    
    printf("Tempo minimo: %d \n",total_time);
}

void maximum(Jobs *list)
{
    Tasks* node = list->head;
    int maximums[node->length];
    int total_time = 0;
    int i=0;
    while(node->next != NULL)
    {
        int max_time = find_maximum(node->times,node->length);
        maximums[i] = max_time;
        total_time += max_time;
        printf("Operação %d: Maquina %d com duraçao %d",i,node->machines[max_time],node->times[max_time]);
        i++;
        node = node->next;
    }
    
    printf("Tempo maximo: %d \n",total_time);
}

//Funções Auxiliares
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


int main()
{
    Jobs jobs[1];
    init_list(jobs);
}