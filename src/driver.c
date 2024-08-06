#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	rbtree *t = NULL;
	node_t *root;

	printf("1: Create a RB-tree.\n");
	printf("2: Insert to the RB-tree.\n");
	printf("0: Delete the RB-tree and Quit;\n");

	int i, c = 1;

	while (c != 0)
	{
		printf("\nPlease input your choice(1/2/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			t = new_rbtree();
			printf("One empty RB-tree is successfully created.");
			break;
		case 2:
			printf("Input an integer that you want to add to the RB-tree: ");
			scanf("%d", &i);
			rbtree_insert(t, i);
			printf("insert done\n");
			printTree(t, t->root);
			break;
		case 0:
			delete_rbtree(t);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}
	}

	return 0;
}