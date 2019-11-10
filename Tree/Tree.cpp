#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int elem_t;

const int MAX_PNG_NAME = 20;

struct tree
{
	elem_t data;
	struct tree *left;
	struct tree *right;
	struct tree *parent;
};

tree *TreeConstruct (elem_t value);

tree *CreateNode (tree *parent, elem_t value);
tree *InsertLeft (tree *parent, elem_t value);
tree *InsertRight (tree *parent, elem_t value);
void DeleteNode (tree *node);
void Delete (tree *node);
tree *FindNode (tree *root, elem_t value);

void PrintTree (tree *root, FILE *output);
tree *ReadTree (FILE *input);
FILE *ReadNode (tree *parent, FILE *input);
void WriteLabels (tree *node, FILE *out);
void WriteConnections (tree *node, FILE *out);
void Dump (tree *root, char png_file[MAX_PNG_NAME], const char mode);


int main ()
{
	//tree *root = TreeConstruct (5);
	//tree *node = root;

	FILE *in = fopen ("tree.txt", "r");
	tree *root = ReadTree (in);
	Dump (root, "tree.png", 'l');

	FILE *out = fopen ("tree_1.txt", "w");
	PrintTree (root, out);


	/*node = InsertLeft (node, 10);
	tree *node1 = InsertRight (node->parent, 15);
	InsertLeft (node, 20);
	InsertRight (node, 25);
	node = InsertLeft (node->parent->right, 30);
	InsertLeft (node, 60);
	InsertRight (node, 65);

	node = InsertRight (node1, 0);
	node = InsertRight (node, 1);
	node = InsertRight (node, 2);
	node = InsertRight (node, 3);
	node = InsertRight (node, 4);

	Dump (root, "tree.png", 'l');

	printf("20 = %p\n", FindNode (root, 20));
	printf("5 = %p\n", FindNode (root, 5));
	printf("65 = %p\n", FindNode (root, 65));
	printf("10 = %p\n", FindNode (root, 10));
	printf("30 = %p\n", FindNode (root, 30));
	printf("13 = %p\n", FindNode (root, 13));

	Dump (root, "tree.png", 'l');
	PrintTree (root);
	printf("\n");*/

	/*PrintTree (root);
	printf("\n");
	Dump (root, "tree.png", 'l');
	Delete (node);
	PrintTree (root);
	printf("\n");
	Dump (root, "tree_1.png", 'l');
	Delete (root->right);
	PrintTree (root);
	Dump (root, "tree_2.png", 'l');*/

	return 0;
}


tree *TreeConstruct (elem_t value)
{
	tree *root = (tree *) calloc (1, sizeof (tree));
	root->data = value;
	root->left = NULL;
	root->right = NULL;
	root->parent = NULL;
	return root;
}


tree *CreateNode (tree *parent, elem_t value)
{
	tree *node = (tree *) calloc (1, sizeof (tree));
	node->data = value;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	return node;
}


tree *InsertLeft (tree *parent, elem_t value)
{
	if (parent == NULL) return NULL;
	if (parent->left == NULL)
	{
		parent->left = CreateNode (parent, value);
		return parent->left;
	}
	else return NULL;
}


tree *InsertRight (tree *parent, elem_t value)
{
	if (parent == NULL) return NULL;
	if (parent->right == NULL)
	{
		parent->right = CreateNode (parent, value);
		return parent->right;
	}
	else return NULL;
}


void DeleteNode (tree *node)
{
	if (node != NULL)
	{
		DeleteNode (node->left);
		DeleteNode (node->right);
		free (node);
	}
	return;
}


void Delete (tree *node)
{
	if (node->parent != NULL)
	{
		if (node->parent->left == node) node->parent->left = NULL;
		else node->parent->right = NULL;
	}
	DeleteNode (node);
	return;
}


tree *FindNode (tree *node, elem_t value)
{
	if (node == NULL || node->data == value) return node;
	if (node->left != NULL) return FindNode (node->left, value);
	else if (node->right != NULL) return FindNode (node->right, value);
	else if (node->parent->left == node) return FindNode (node->parent->right, value);
	else if (node->parent->parent != NULL && node->parent->parent->left != NULL && node->parent->parent->right != NULL) return FindNode (node->parent->parent->right, value);
	else return NULL;
}


void PrintTree (tree *node, FILE *output)
{
	assert (node);
	fprintf (output, "{%d", node->data);
	if (node->left == NULL && node->right == NULL)
	{
		fprintf(output, "}");
		return;
	}
	fprintf(output, " ");
	if (node->left != NULL)
	{
		PrintTree (node->left, output);
		fprintf(output, " ");
	}
	else fprintf (output, "X ");
	if (node->right != NULL) PrintTree (node->right, output);
	else fprintf (output, "X");
	fprintf (output, "}");
	return;
}


FILE *ReadNode (tree *parent, FILE *input)
{
	char brace = 0;
	int value = 0;
	tree *node = 0;

	fscanf (input, "%d%c", &value, &brace);
	parent->data = value;
	if (brace == ' ')
	{
		fscanf (input, "%c", &brace);
		switch (brace)
		{
			case '{':
				node = InsertLeft (parent, value);
				node->parent = parent;
				input = ReadNode (node, input);

				fscanf (input, " %c", &brace);
				if (brace == 'X')
				{
					fgetc (input);
					return input;
				}
				if (brace == '{')
				{
					node = InsertRight (parent, value);
					node->parent = parent;
					input = ReadNode (node, input);
					fgetc (input);
				}
				break;

			case 'X':
					fscanf (input, " %c", &brace);
					node = InsertRight (parent, value);
					node->parent = parent;
					input = ReadNode (node, input);
				break;
		}
	}
	if (brace == '}') fgetc (input);
	return input;
}


tree *ReadTree (FILE *input)
{
	tree *root = TreeConstruct (5);
	fgetc (input);
	ReadNode (root, input);
	return root;
}


void WriteLabels (tree *node, FILE *out)
{
	fprintf (out, "%d[label = \" {%d | {left\\n%p | parent\\n%p | adress\\n%p | right\\n%p}} \"]\n", node->data, node->data, node->left, node->parent, node, node->right);
	if (node->left != NULL) WriteLabels (node->left, out);
	if (node->right != NULL) WriteLabels (node->right, out);
	return;
}


void WriteConnections (tree *node, FILE *out)
{
	if (node->left == NULL && node->right == NULL) return;

	if (node->left != NULL)
	{
		fprintf (out, "%d->%d [style=\"bold\", color = \"royalblue\"];\n", node->data, node->left->data);
		WriteConnections (node->left, out);
	}

	if (node->right != NULL)
	{
		fprintf (out, "%d->%d [style=\"bold\", color = \"lawngreen\"];\n", node->data, node->right->data);
		WriteConnections (node->right, out);
	}
	return;
}


void Dump (tree *root, char png_file[MAX_PNG_NAME], const char mode)
{
	tree *node = root;
	FILE *out = fopen ("tree.dot", "w");

	if (mode == 'l')
	{
		fprintf (out, "digraph {\nnode[shape = record];\n");
		WriteLabels (root, out);
		if (node->left != NULL || node->right != NULL) WriteConnections (root, out);
		else fprintf (out, "%d;\n", node->data);
		fprintf(out, "}");
	}

	fclose (out);
	char command[53] = "dot tree.dot -T png -o ";
	strcpy (&command[23], png_file);
	system (command);
	return;
}


TreeOk (tree *root)
