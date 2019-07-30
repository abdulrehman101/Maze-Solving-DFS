#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int debugMode = 0;

typedef struct mazeStruct
{
	//char array[32][32];  /* allows for a maze of size 30x30 plus outer walls */
	char **array;
	int xsize, ysize;
	int xstart, ystart;
	int xend, yend;
} maze;
void allocateSpace(maze*M)
{
	int i;
	M->array = (char**)malloc(sizeof(char*) * (M->xsize + 2));
	for (i = 0; i < (M->xsize + 2); i++)
	{
		M->array[i] = (char*)malloc(sizeof(char) * (M->ysize + 2));
	}
}
void deallocateSpace(maze*M)
{
	int i;
	for (i = 0; i < (M->xsize + 2); i++)
	{
		free(M->array[i]);
	}
	free(M->array);
}
typedef struct nodeStruct
{
	int xPos, yPos;
	int xadd, yadd;
	struct nodeStruct* next;
} node;
node* initializeStack()
{
	return NULL;
}
void push(node** hd, maze *M, int x, int y, int xadd, int yadd)
{
	node* temp;
	node* tempA;
	temp = (node *)malloc(sizeof(node));
	temp->xPos = x;
	temp->yPos = y;
	temp->xadd = xadd;
	temp->yadd = yadd;
	temp->next = NULL;
	if (debugMode == 1)
	{
		printf("pushing (%d,%d) on the stack\n", x, y);
	}
	if (*hd == NULL)
	{
		*hd = temp;
	}
	else
	{
		tempA = *hd;
		*hd = temp;
		temp->next = tempA;
	}
	// mark the position visited
	M->array[x][y] = 'V';
}
void pop(node** hd, int* x, int* y)
{
	if (*hd != NULL)
	{
		if (debugMode == 1)
		{
			printf("popping (%d,%d) of the stack\n",(*hd)->xPos, (*hd)->yPos);
		}
		node* temp;
		*x = *x + (-1 * ((*hd)->xadd));
		*y = *y + (-1 * ((*hd)->yadd));
		temp = *hd;
		*hd = (*hd)->next;
		free(temp);
	}
}
node* top(node* hd)
{
	node* temp;
	temp = hd;
	return temp;
}
int isEmpty(node* hd)
{
	int accept;
	accept = 1;
	if (hd != NULL)
	{
		accept = 0;
	}
	return accept;
}
int notZero(int one, int two)
{
	int accept;
	accept = 1;
	if ((one <= 0) || (two <= 0))
	{
		accept = 0;
	}
	return accept;
}
int validStartEnd(int one, int two, int xsize, int ysize)
{
	int accept;
	accept = 1;
	// values should not be zero
	if (!notZero(one,two))
	{
		accept = 0;
	}
	// checking for xbound
	if ((one > xsize) || (two > ysize))
	{
		accept = 0;
	}
	return accept;
}
int isMoveValid(maze* M, node* N, int x, int y)
{
	int accept;
	accept = 1;
	
	if ((M->array[x][y] == 'V') || (M->array[x][y] == '*'))
	{
		accept = 0;
	}

	return accept;
}
int endFound(maze m1, int x, int y)
{
	int accept;
	accept = 0;
	if ((m1.xend == x) && (m1.yend == y))
	{
		accept = 1;
	}
	return accept;
}
void printRecursive(node* head)
{
	if (head->next == NULL)
	{
		printf("%d %d\n", head->xPos, head->yPos);
	}
	else
	{
		printRecursive(head->next);
		printf("%d %d\n", head->xPos, head->yPos);
	}
}
void resetStack(node* head)
{
	node* temp;
	if (head == NULL)
	{
		return;
	}
	else
	{
		while (head != NULL)
		{
			temp = head;
			head = head->next;
			free(temp);
		}
	}
}
int main(int argc, char **argv)
{
	maze m1;
	node* stack;
	node* popReturn;

	//initializing stack, basically storing null 
	stack = initializeStack();

	int xpos, ypos;
	int xIndexAdd, yIndexAdd;
	int i, j,fileNumber, validInputFound;
	validInputFound = 0;
	// Debugging mode 
	for ( i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "-d"))
		{
			debugMode = 1;
			printf("Debug mode is Active\n");
		}
	}
	FILE *src;

	/* verify the proper number of command line arguments were given */
	if (argc == 1) {
		printf("Usage: %s <input file name>\n", argv[0]);
		exit(-1);
	}
	if (argc == 2)
	{
		if (debugMode)
		{
			printf("Usage: %s <input file name>\n", argv[0]);
			exit(-1);
		}
		else
		{
			fileNumber = 1;
		}
	}
	if (argc > 2)
	{
		for ( i = 1; i < argc; i++)
		{
			if (strcmp(argv[i],"-d") != 0)
			{
				printf("First file name found: %s\n", argv[i]);
				fileNumber = i;
				break;
			}
		}
	}

	/* Try to open the input file. */
	if ((src = fopen(argv[fileNumber], "r")) == NULL)
	{
		printf("Can't open input file: %s\n", argv[fileNumber]);
		exit(-1);
	}

	// read in the size of the maze untill valid
	while (fscanf(src, "%d %d", &m1.xsize, &m1.ysize) != EOF)
	{
		if ((m1.xsize != 0) && (m1.ysize != 0))
		{
			validInputFound += 1;
			break;
		}
	}
	if (validInputFound != 1)
	{
		printf("valid size not found\nExiting program\n");
		// closing the file
		fclose(src);
		exit(-1);
	}
	// read in start coordinates untill Valid
	while (fscanf(src, "%d %d", &m1.xstart, &m1.ystart) != EOF)
	{
		if (validStartEnd(m1.xstart, m1.ystart, m1.xsize, m1.ysize))
		{
			validInputFound += 1;
			break;
		}
	}
	if (validInputFound != 2)
	{
		printf("valid start position not found\nExiting program\n");
		// closing the file
		fclose(src);
		exit(-1);
	}
	// read in end coordinates untill Valid
	while (fscanf(src, "%d %d", &m1.xend, &m1.yend) != EOF)
	{
		if ((m1.xstart != m1.xend) || (m1.ystart != m1.yend))
		{
			if (validStartEnd(m1.xend, m1.yend, m1.xsize, m1.ysize))
			{
				validInputFound += 1;
				break;
			}
		}
	}
	if (validInputFound != 3)
	{
		printf("valid end position not found\nExiting program\n");
		// closing the file
		fclose(src);
		exit(-1);
	}

	/* print them out to verify the input */
	printf("size: %d, %d\n", m1.xsize, m1.ysize);
	printf("start: %d, %d\n", m1.xstart, m1.ystart);
	printf("end: %d, %d\n", m1.xend, m1.yend);

	//allocate space for 2-D array
	allocateSpace(&m1);

	/* initialize the maze to empty */
	for (i = 0; i < m1.xsize + 2; i++)
	{
		for (j = 0; j < m1.ysize + 2; j++)
		{
			m1.array[i][j] = '.';
		}
	}

	/* mark the borders of the maze with *'s */
	for (i = 0; i < m1.xsize + 2; i++)
	{
		m1.array[i][0] = '*';
		m1.array[i][m1.ysize + 1] = '*';
	}
	for (i = 0; i < m1.ysize + 2; i++)
	{
		m1.array[0][i] = '*';
		m1.array[m1.xsize + 1][i] = '*';
	}

	/* mark the blocked positions in the maze with *'s */
	while (fscanf(src, "%d %d", &xpos, &ypos) != EOF)
	{
		if ((xpos <= m1.xsize)&&(ypos <= m1.ysize))
		{
			m1.array[xpos][ypos] = '*';
		}
	}

	// closing the file
	fclose(src);

	/* mark the starting and ending positions in the maze */
	m1.array[m1.xstart][m1.ystart] = 's';
	m1.array[m1.xend][m1.yend] = 'e';

	/* print out the initial maze */
	for (i = 0; i < m1.xsize + 2; i++)
	{
		for (j = 0; j < m1.ysize + 2; j++)
			printf("%c", m1.array[i][j]);
		printf("\n");
	}
	xpos = m1.xstart;
	ypos = m1.ystart;
	xIndexAdd = 0;
	yIndexAdd = 0;
	push(&stack,&m1,xpos,ypos,xIndexAdd,yIndexAdd);
	while (!endFound(m1,xpos,ypos) && !isEmpty(stack))
	{
		// checking for down
		if ((m1.array[xpos + 1][ypos] != '*') && (m1.array[xpos + 1][ypos] != 'V'))
		{
			xIndexAdd = 1;
			yIndexAdd = 0;
			xpos = xpos + 1;
			push(&stack, &m1, xpos, ypos,xIndexAdd,yIndexAdd);
			if (endFound(m1,xpos,ypos))
			{
				break;
			}
			continue;
		}
		// checking for left
		if ((m1.array[xpos][ypos - 1] != '*') && (m1.array[xpos][ypos - 1] != 'V'))
		{
			xIndexAdd = 0;
			yIndexAdd = -1;
			ypos = ypos - 1;
			push(&stack, &m1, xpos, ypos, xIndexAdd, yIndexAdd);
			if (endFound(m1,xpos, ypos))
			{
				break;
			}
			continue;
		}
		// checking for up
		if ((m1.array[xpos - 1][ypos] != '*') && (m1.array[xpos - 1][ypos] != 'V'))
		{
			xIndexAdd = -1;
			yIndexAdd = 0;
			xpos = xpos - 1;
			push(&stack, &m1, xpos, ypos, xIndexAdd, yIndexAdd);
			if (endFound(m1,xpos, ypos))
			{
				break;
			}
			continue;
		}
		// checking for right
		if ((m1.array[xpos][ypos + 1] != '*') && (m1.array[xpos][ypos + 1] != 'V'))
		{
			xIndexAdd = 0;
			yIndexAdd = 1;
			ypos = ypos + 1;
			push(&stack, &m1, xpos, ypos, xIndexAdd, yIndexAdd);
			if (endFound(m1,xpos, ypos))
			{
				break;
			}
			continue;
		}
		if (!isEmpty(stack))
		{
			pop(&stack,&xpos,&ypos);
			continue;
		}
	}
	//reversing the stack
	if (isEmpty(stack))
	{
		printf("No Solution\n");
		resetStack(stack);
		deallocateSpace(&m1);
		exit(-1);
	}
	else
	{
		printf("printing stack with recursion\n");
		printRecursive(stack);
	}
	resetStack(stack);
	deallocateSpace(&m1);
}
