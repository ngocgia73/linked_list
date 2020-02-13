#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

// my malloc 
#define GIANN_MALLOC(var_type, var_name, alloc_func, free_func) \
void free_##var_name##_func(var_type *p) { if(*p) {free_func(*p); printf("call free func\n");}} \
var_type var_name __attribute__((cleanup(free_##var_name##_func)))= (alloc_func)

typedef bool (* COMPARE) (void *data, int index);
typedef void (* DISPLAY) (void *data);

typedef struct employee
{
    int id;
    int age;
    char cname[50];
} EMPLOYEE;

typedef struct node
{
    struct node *pprev;
    struct node *pnext;
    EMPLOYEE *pEmployee;
} NODE;

typedef struct list
{
    NODE *phead;
    NODE *ptail;
}LIST;

// init linked list 
void initLinkedList(LIST *list)
{
    if(!list)
        return;
    list->phead = list->ptail = NULL;
}

// add head
void addHead(LIST *list, EMPLOYEE *pEmployee)
{
    NODE *node = NULL;
    if(!list || !pEmployee)
    {
        printf("err input\n");
        return;
    }

    node = (NODE *)malloc(sizeof(NODE));
    if(!node)
    {
        printf("unable to allocate mem for node\n");
        return;
    }
    node->pEmployee = pEmployee;

    if(list->phead == NULL)
    {
        list->phead = list->ptail = node;
        list->phead->pnext = list->ptail->pnext = node;
        list->phead->pprev = list->ptail->pprev = node;
    }
    else
    {
        node->pnext = list->phead;
        list->phead->pprev = node;
    }
    // update phead
    list->phead = node;

    list->phead->pprev = list->ptail;
    list->ptail->pnext = list->phead;
    
}

// add tail
void addTail(LIST *list, EMPLOYEE *pEmployee)
{
    NODE *node = NULL;
    if(!list || !pEmployee)
    {
        printf("err input\n");
        return;
    }

    node = (NODE *)malloc(sizeof(NODE));
    if(!node)
    {
        printf("unable to allocate mem for node\n");
        return;
    }
    node->pEmployee = pEmployee;
    
    if(list->ptail == NULL)
    {
        list->ptail = list->phead = node;
        list->ptail->pprev = list->phead->pprev = node;
        list->ptail->pnext = list->phead->pnext = node;
    }
    else
    {
        list->ptail->pnext = node;
        node->pprev = list->ptail;
    }

    // update ptail
    list->ptail = node;
    list->ptail->pnext = list->phead;
    list->phead->pprev = list->ptail;
}

// dell head
void dellHead(LIST *list)
{
    NODE *ntmp = NULL;
    if(list->phead == NULL)
    {
        printf("list is empty\n");
        return;
    }

    ntmp = list->phead;
    // just have one node
    if(list->phead == list->ptail)
    {
        list->phead = list->ptail = NULL;
    }
    else
    {
        // update head
        list->phead = ntmp->pnext;
        // update head->prev
        list->phead->pprev = list->ptail; 
    }

    if(ntmp)
        free(ntmp);
}

// dell tail
void dellTail(LIST *list)
{
    NODE *ntmp = NULL;

    if(list->ptail == NULL)
    {
        printf("list is empty\n");
        return;
    }

    ntmp = list->ptail;

    // only one node in list
    if(list->ptail == list->phead)
    {
       list->ptail = list->phead = NULL; 
    }
    else
    {
        // update tail
        list->ptail = ntmp->pprev;
        //update ptail->ppnext
        list->ptail->pnext = list->phead;
    }

    if(ntmp)
        free(ntmp);
}

static bool compare_idx(void *data, int idx)
{
    EMPLOYEE *pEmployee = NULL;
    if(!data)
        return false;
    pEmployee = (EMPLOYEE *)data;

    if(pEmployee->id == idx)
        return true;
    return false;
}

NODE *getNode(LIST *list, COMPARE compare, int index)
{
    NODE *ntmp = NULL;
    if(!list || !compare)
        return NULL;
    ntmp = list->phead;
    do
    {
        if(compare((void *)ntmp->pEmployee, index) == true)
        {
            return ntmp;
        }
        ntmp = ntmp->pnext;
    }while(ntmp!=list->phead); 
    return NULL;    
}

// add node after specific index
void addNodeAfterIndex(LIST *list, void *data, int index)
{
    NODE *node_idx = NULL;
    NODE *node = NULL;
    if(!list)
        return;
    node_idx = getNode(list, compare_idx, index);
    if(!node_idx)
        return;
    
    // proceed to add node after node_idx
    if(list->ptail == node_idx) // last node
    {
        addTail(list, (EMPLOYEE *)data);
    }
    else
    {
        // create node which need to add
        node = (NODE *)malloc(sizeof(NODE));
        if(!node)
            return;
        node->pEmployee = (EMPLOYEE *)data;


        node->pnext= node_idx->pnext;
        node->pprev = node_idx;
        
        node->pnext->pprev = node;        
        node_idx->pnext = node;

    }
}

// delete node after index
void deleteNodeIndex(LIST *list, int index)
{
    NODE *node_idx = NULL;
    if(!list)
        return;
    node_idx = getNode(list, compare_idx, index);
    if(!node_idx)
        return;

    if(list->ptail == node_idx) // last one
    {
        dellTail(list);
    }
    else if(list->phead == node_idx)
    {
        dellHead(list);
    }
    else
    {
        NODE *npre_idx = node_idx->pprev;
        NODE *naft_idx = node_idx->pnext;
        npre_idx->pnext = naft_idx;
        naft_idx->pprev = npre_idx;
    }
}

static void displayEmployee(EMPLOYEE *pEmployee)
{
    if(!pEmployee)
        return;
    printf("%d\t%d\t%s\n",pEmployee->id, pEmployee->age, pEmployee->cname);
}

void displayList(LIST *list, DISPLAY display_func)
{
    NODE *ntmp = NULL;
    if(!list || !display_func)
        return;
    ntmp = list->phead;
    if(!ntmp)
    {
        printf("list is empty\n");
        return;
    }
    printf("id\tage\tname\n");
    if(list->phead == list->ptail) // one element
    {
        display_func((void *)ntmp->pEmployee);
    }
    else
    {
        while(ntmp->pnext != list->phead)
        {
            display_func((void *)ntmp->pEmployee);
            // move to next node
            ntmp = ntmp->pnext;
        }
        display_func((void *)ntmp->pEmployee);
    }
}

void freeList(LIST *list)
{
    if(!list)
        return;
    do
    {
        dellHead(list);
    }while(list->phead != NULL);
}

//main
int main(int argc , char *argv[])
{
	LIST list;
	//EMPLOYEE *e1 = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
	GIANN_MALLOC(EMPLOYEE *, e1, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e1)
	{
		strcpy(e1->cname, "name1");
		e1->age = 21;
		e1->id = 1;
	}

	//EMPLOYEE *e2 = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
	GIANN_MALLOC(EMPLOYEE *, e2, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e2)
	{
		strcpy(e2->cname, "name2");
		e2->age = 22;
		e2->id = 2;
	}

	//EMPLOYEE *e3 = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
	GIANN_MALLOC(EMPLOYEE *, e3, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e3)
	{
		strcpy(e3->cname, "name3");
		e3->age = 23;
		e3->id = 3;
	}

	//EMPLOYEE *e4 = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
	GIANN_MALLOC(EMPLOYEE *, e4, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e4)
	{
		strcpy(e4->cname, "name4");
		e4->age = 24;
		e4->id = 4;
	}
	
	GIANN_MALLOC(EMPLOYEE *, e5, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e5)
	{
		strcpy(e5->cname, "name5");
		e5->age = 25;
		e5->id = 5;
	}
	initLinkedList(&list);

	addHead(&list, e1);
    printf("============================================\n");
	displayList(&list, (void(*)(void *))displayEmployee);
	addHead(&list, e2);
    printf("============================================\n");
	displayList(&list, (void(*)(void *))displayEmployee);
	addHead(&list, e3);
    printf("============================================\n");
	displayList(&list, (void(*)(void *))displayEmployee);
	printf("proceed addTail\n");
	addTail(&list, e4);
	displayList(&list, (void(*)(void *))displayEmployee);
	printf("proceed add node e5 after index 3\n");
	addNodeAfterIndex(&list, e5, 3);
	displayList(&list, (void(*)(void *))displayEmployee);
	printf("proceed delete node at index 2\n");
	deleteNodeIndex(&list, 2);
	displayList(&list, (void(*)(void *))displayEmployee);
	freeList(&list);
}
