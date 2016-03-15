#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list_head {
	struct list_head* next;
};
#define LIST_HEAD_INIT(name) { NULL }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)
#define list_entry(ptr, type, member) container_of(ptr, type, member)
#ifndef offsetof
#define offsetof(type, member) ((size_t) &((type *)0)->member) 
#endif

#define container_of(ptr, type, member) ({\
	const typeof(((type*)0)->member)* _mptr = (ptr);\
	(type*)((char*)_mptr - offsetof(type, member));})

#define list_destroy(head, type, member) do{ \
    for(struct list_head *cur=head.next; cur!=NULL;) { \
        struct list_head *next = cur->next;             \
        type *node = list_entry(cur, type, member);     \
        free(node);                                     \
        cur = next;                                     \
    }                                                   \
}while(0)


struct my_node {
	char data[30];
	struct list_head list;
};

int list_add(struct list_head *head, struct list_head *new_head)
{
	if (head == NULL)
		return -1;
	new_head->next = head->next;
	head->next = new_head;
	return 0;
}

int list_deleteLastOne(struct list_head *head){
	if(head == NULL)
		return -1;
	int cnt = 0;
	for(struct list_head *cur = head->next; cur != NULL; cur = cur->next){
		cnt++;
		/****
		debug message
		struct my_node *data = list_entry(cur, struct my_node, list);
		printf("%s\n", data->data);
		***********/
		if(cnt == 24){
			struct list_head *next = cur->next;
			struct my_node *node = list_entry(next, struct my_node, list);
			free(node);
			cur->next = NULL;
		}
	}
}


int main(int argc, const char *argv[]) {
	char input[30];
	int cnt = 24;
	LIST_HEAD(my_list);
	for(size_t tmp = 2; tmp <= argc; tmp++){
		strcpy(input, argv[tmp - 1]);
		if(cnt == 25){
			list_deleteLastOne(&my_list);
			--cnt;
		}
		else ++cnt;			
		struct my_node *node = malloc(sizeof(struct my_node));
		strcpy(node->data, input);
		list_add(&my_list, &(node->list));

		for(struct list_head *cur = my_list.next; cur != NULL; cur = cur->next){
			if(cur->next != NULL){
				for(struct list_head *tmp = cur->next; tmp != NULL; tmp = tmp->next){
					struct my_node *node_1 = list_entry(cur, struct my_node, list);
					struct my_node *node_2 = list_entry(tmp, struct my_node, list);
					if(strcmp(node_1->data, node_2->data) == 0){
						printf("the word %s is repeated\n", node_1->data);
					}
				}
			}
			
		}
	}

		
	//this piece of code is designed for debugging linux_kernel_list.

/*
	struct my_node *node1 = malloc(sizeof(struct my_node));
	strcpy(node1->data, "mua");
	list_add(&my_list, &(node1->list));
	struct my_node *node2 = malloc(sizeof(struct my_node));
	strcpy(node2->data, "mua");
	list_add(&my_list, &(node2->list));
	struct my_node *node3 = malloc(sizeof(struct my_node));
	strcpy(node3->data, "Hsienyu");
	list_add(&my_list, &(node3->list));
	struct my_node *node4 = malloc(sizeof(struct my_node));
	strcpy(node4->data, "love");
	list_add(&my_list, &(node4->list));
	struct my_node *node5 = malloc(sizeof(struct my_node));
	strcpy(node5->data, "I");
	list_add(&my_list, &(node5->list));
	list_deleteLastOne(&my_list);

	for(struct list_head *cur = my_list.next; cur != NULL; cur = cur->next){
		if(cur->next != NULL){
			for(struct list_head *tmp = cur->next; tmp != NULL; tmp = tmp->next){
				struct my_node *node_1 = list_entry(cur, struct my_node, list);
				struct my_node *node_2 = list_entry(tmp, struct my_node, list);
				if(strcmp(node_1->data, node_2->data) == 0){
					printf("the word %s is repeated\n", node_1->data);
				}
			}
		}
			
	}
*/	
	list_destroy(my_list, struct my_node, list);
	return 0;
}

