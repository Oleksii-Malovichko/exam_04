#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "argo.h"

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;



node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

int parser(json *dst, FILE *stream)
{
	int c;

	c = peek(stream);
	if (c == '"')
		return parse_string(dst, stream);
	else if (isdigit(c) || c == '-')
		return parse_int(dst, stream);
	else if (c == '{')
		return parse_map(dst, stream);
	unexpected(stream);
	return -1;
}

int parse_int(json *dst, FILE *stream)
{
	int n;

	if (fscanf(stream, "%d", &n) == 1)
	{
		dst->type = INTEGER;
		dst->integer = n;
		return 1;
	}
	unexpected(stream);
	return -1;
}

int parse_string(json *dst, FILE *stream)
{
	char buffer[4096];
	char c;
	int i;

	if (!expect(stream, '"'))
		return -1;
	
}


void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n"); //+++++++++++++++
}

int accept(char **s, char c)
{
    if (**s == c)   //+++++++++++++++
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}


node	*parse_expr(char *s);
node    *parse_number_or_group(char **s);
node    *parse_addition(char **s);
node    *parse_multiplication(char **s);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++




int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
    return (0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
