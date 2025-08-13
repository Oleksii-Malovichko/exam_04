#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// #include "vbc.h"

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

node *parse_expr(char *s)
{
	node *res = parse_addition(&s);
	if (!res)
		return NULL;
	if (*s)
	{
		destroy_tree(res);
		unexpected(*s);
		return NULL;
	}
	return res;
}

node *parse_addition(char **s)
{
	node *left;
	node *right;
	node tmp;

	left = parse_multiplication(s);
	if (!left)
		return NULL;
	while (**s == '+')
	{
		(*s)++;
		right = parse_multiplication(s);
		if (!right)
		{
			destroy_tree(left);
			return NULL;
		}
		tmp.type = ADD;
		tmp.l = left;
		tmp.r = right;
		left = new_node(tmp);
	}
	return left;
}

node *parse_multiplication(char **s)
{
	node *left;
	node *right;
	node tmp;

	left = parse_number_or_group(s);
	if (!left)
		return NULL;
	while (**s == '*')
	{
		(*s)++;
		right = parse_number_or_group(s);
		if (!right)
		{
			destroy_tree(left);
			return NULL;
		}
		tmp.type = MULTI;
		tmp.l = left;
		tmp.r = right;
		left = new_node(tmp);
	}
	return left;
}

node *parse_number_or_group(char **s)
{
	node *res;
	node tmp;

	res = NULL;
	if (**s == '(')
	{
		(*s)++;
		res = parse_addition(s);
		if (!res)
			return NULL;
		if (!expect(s, ')'))
		{
			destroy_tree(res);
			return NULL;
		}
		// (*s)++;
		return res;
	}
	if (isdigit(**s))
	{
		tmp.type = VAL;
		tmp.val = **s - '0';
		res = new_node(tmp);
		(*s)++;
		return res;
	}
	unexpected(**s);
	return NULL;
}

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
