#include <stdio.h>
#include <stdlib.h>

typedef struct linkedList{
	int data;
	struct linkedList *next;
} linkedList;

void append(linkedList *root, int data){
	linkedList *newNode = malloc(sizeof(linkedList));
	newNode->data = data;
	newNode->next = NULL;
	root->next = newNode;

}

typedef struct vectorInt{
	int *array;
	int length;
} vectorInt;

void appendInt(vectorInt *vector, int value){
	// creates a new empty element to the vector
	vector->array = realloc(vector->array, (vector->length + 1) * sizeof(int));
	// sets empty element to value
	vector->array[vector->length] = value;

	vector->length++;
}

void popInt(vectorInt *vector){
	vector->length--;
	vector->array = realloc(vector->array, vector->length*sizeof(int));
}

void printVector(vectorInt vector){
	for(int i = 0;i<vector.length;i++){
		printf("%d, ",vector.array[i]);
	}
	printf("\n");
}

int main()
{
	int x = 0;
    	linkedList list;
	append(&list, 10);
	x = list.next->data;

	printf("%d\n",x);

	vectorInt vector;
	vector.length = 0;

	appendInt(&vector, 142);
	appendInt(&vector, 23);
	appendInt(&vector, 142);
	popInt(&vector);

	printVector(vector);
}
