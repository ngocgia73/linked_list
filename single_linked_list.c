#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct _employee
{
	char name[32];
	int age;
	int index;
} EMPLOYEE;

// define NODE
typedef struct _node
{
	void *data;
	struct _node *next;
} NODE;

// define LIST
typedef struct _list
{
	NODE *head;
	NODE *tail;
	NODE *current;
} LIST;

// my malloc 
#define GIANN_MALLOC(var_type, var_name, alloc_func, free_func) \
void free_##var_name##_func(var_type *p) { if(*p) {free_func(*p); printf("call free func\n");}} \
var_type var_name __attribute__((cleanup(free_##var_name##_func)))= (alloc_func)

typedef bool (* COMPARE)(EMPLOYEE *, int);
typedef void (* DISPLAY)(EMPLOYEE *);
typedef void (* FREE)(NODE *);

bool compare(EMPLOYEE *e1, int index)
{
	if(e1)
	{
		if(e1->index == index)
			return true;
		else
			return false;
	}
	else
		return false;
}

// display employee
void displayEmployee(EMPLOYEE *e)
{
	if(e)
	{
		printf("%d\t%s\t%d\n",e->index, e->name,e->age);
	}
	else
		printf("error input\n");
}

// free node
void freeNode(NODE *node)
{
	if(!node)
	{
		printf("input error\n");
		return;
	}
	else
	{
		node->data = NULL;
		node->next = NULL;
		free(node);
		node = NULL;
	}
}

// initialize  linked list
void initializeList(LIST *list)
{
	list->head=NULL;
	list->tail=NULL;
	list->current=NULL;
}

// add head 
int addHead(LIST *list, void *data)
{
	NODE *node = (NODE *)malloc(sizeof(NODE));
	if(node && data && list)
	{
		node->data = data;
		if(list->head == NULL)
		{
			list->tail = node;
			node->next = NULL;
		}
		else
		{
			// list not empty
			node->next = list->head;
		}
		list->head = node;
	}
	else
	{
		printf("allocate mem for NODE failed\n");
		return -1;
	}
	return 0;
}
// add tail
int addTail(LIST *list, void *data)
{
	NODE *node = (NODE *)malloc(sizeof(NODE));
	if(node && list && data)
	{
		node->data = data;
		node->next = NULL;
		if(list->head == NULL)
		{
			list->head = node;
		}
		else
		{
			// list not empty
			list->tail->next = node;
		}
		list->tail = node;
	}
	else
	{
		printf("error\n");
		return -1;
	}
	return 0;
}
// delete head
int deleteHead(LIST *list)
{
	NODE *tmp = NULL;
	if(list)
	{
		if(list->head == NULL)
		{
			printf("list is empty\n");
			return -2;
		}
		else
		{
			// one element in list
			if(list->head->next == NULL)
			{
				free(list->head);
				list->head = list->tail = NULL;
			}
			else
			{
				tmp = list->head;
				list->head=list->head->next;
				free(tmp);
				tmp = NULL;
			}
		}
	}
	else
	{
		printf("error input\n");
		return -1;
	}
	return 0;
}
// delete tail
int deleteTail(LIST *list)
{
	NODE *tmp = NULL;
	if(list)
	{
		if(list->head == NULL)
		{
			printf("list is empty\n");
			return -2;
		}
		else
		{
			// just have only one element
			if(list->head->next == NULL)
			{
				free(list->tail);
				list->tail = list->head = NULL;
			}
			else
			{
				tmp = list->head;
				while(tmp != NULL)
				{
					if(tmp->next->next == NULL)
					{
						free(list->tail);
						list->tail=tmp;
					}
					tmp = tmp->next;
				}
			}
		}
	}
	else
	{
		printf("error input\n");
		return -1;
	}
	return 0;
}	

// get node

NODE * getNode(LIST *list, COMPARE compare, int index)
{
	NODE *tmp = NULL;
	if(!list || !compare)
	{
		printf("error input\n");
		return NULL;
	}
	tmp = list->head;
	while(tmp!= NULL)
	{
		if(compare(tmp->data, index))
			return tmp;
		else
			tmp = tmp->next;
	}
	return NULL;
}
// add after specific position

int addNodeAfterIndex(LIST *list, void *data, int index)
{
	NODE *tmp = NULL;
	NODE *tmp2 = NULL;
	if(!list)
	{
		printf("input error\n");
		return -1;
	}
	tmp = getNode(list,(bool (*)(void *, int))compare,index);
	if(!tmp)
	{
		printf("get node failed\n");
		return -2;
	}
	else
	{
		// if node is last element in list
		if(tmp->next == NULL)
		{
			addTail(list, data);
		}
		else
		{
			tmp2 = (NODE *)malloc(sizeof(NODE));
			if(tmp2)
			{
				tmp2->data = data;
				tmp2->next = tmp->next;
				tmp->next = tmp2;
			}
			else
			{
				printf(" malloc fail \n");
				return -3;
			}
		}
	}
	return 0;
}
// delete after specific position
int deleteNodeAfterIndex(LIST *list, int index)
{
	NODE *tmp = NULL;
	NODE *tmp2 = NULL;
	int ret = -1;
	if(!list)
	{
		printf("error input\n");
		return -1;
	}
	tmp = getNode(list, (bool(*)(void *, int)) compare, index);
	if(tmp)
	{
		if(tmp->next == NULL) // last node
		{
			ret = deleteTail(list);
		}
		else if(tmp == list->head) // first node
		{
			ret = deleteHead(list);
		}
		else
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
			tmp2= NULL;
			ret = 0;
		}
	}
	else
	{
		printf("unable to get node\n");
		return -2;
	}
	return ret;
}
// display list 
void displayList(LIST *list, DISPLAY display)
{
	NODE *current = NULL;
	if(!list || !display)
	{
		printf("error input\n");
		return;
	}
	else
	{
		current = list->head;
		printf("index\tname\tage\n");
		while(current != NULL)
		{
			display(current->data);
			current = current->next;
		}
	}
}
// free list 

void freeList(LIST *list, FREE freeNode)
{
	NODE *current = NULL;
	NODE *tmp = NULL;
	if(!list)
	{
		printf("input error\n");
		return;
	}
	else
	{
		current = list->head;
		if(!current)
		{
			printf("list is empty\n");
			return;
		}
		while(current != NULL)
		{
			tmp = current->next;

			freeNode(current);
			current = tmp;
		}
	}
}
//main
int main(int argc , char *argv[])
{
	LIST list;
	//EMPLOYEE *e1 = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
	GIANN_MALLOC(EMPLOYEE *, e1, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e1)
	{
		strcpy(e1->name, "name1");
		e1->age = 21;
		e1->index = 1;
	}

	//EMPLOYEE *e2 = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
	GIANN_MALLOC(EMPLOYEE *, e2, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e2)
	{
		strcpy(e2->name, "name2");
		e2->age = 22;
		e2->index = 2;
	}

	//EMPLOYEE *e3 = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
	GIANN_MALLOC(EMPLOYEE *, e3, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e3)
	{
		strcpy(e3->name, "name3");
		e3->age = 23;
		e3->index = 3;
	}

	//EMPLOYEE *e4 = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
	GIANN_MALLOC(EMPLOYEE *, e4, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e4)
	{
		strcpy(e4->name, "name4");
		e4->age = 24;
		e4->index = 4;
	}
	
	GIANN_MALLOC(EMPLOYEE *, e5, (EMPLOYEE*)malloc(sizeof(EMPLOYEE)),free);
	if(e5)
	{
		strcpy(e5->name, "name5");
		e5->age = 25;
		e5->index = 5;
	}
	initializeList(&list);

	addHead(&list, e1);
	addHead(&list, e2);
	addHead(&list, e3);
	displayList(&list, displayEmployee);
	printf("proceed addTail\n");
	addTail(&list, e4);
	displayList(&list, displayEmployee);
	printf("proceed add node after index 4\n");
	addNodeAfterIndex(&list, e5, 4);
	displayList(&list, displayEmployee);
	printf("proceed delete node after index 2\n");
	deleteNodeAfterIndex(&list, 10);
	displayList(&list, displayEmployee);
	freeList(&list, freeNode);
}
