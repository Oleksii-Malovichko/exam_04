#include "vbc.h"

// 4
node *parse_number_or_group(char **s)
{
	node *res;
	node tmp;

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

// 2
node    *parse_addition(char **s)
{
    node    *left;
    node    *right;
    node    tmp;

    left = parse_multiplication(s);
    if (!left)
        return (NULL);
    while (**s == '+')
    {
        (*s)++;
        right = parse_multiplication(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        tmp.type = ADD;
        tmp.l = left;
        tmp.r = right;
        left = new_node(tmp);
    }
    return (left);
}

// 3
node    *parse_multiplication(char **s)
{
    node    *left;
    node    *right;
    node    tmp;

    left = parse_number_or_group(s);
    if (!left)
        return (NULL);
    while (**s == '*')
    {
        (*s)++;
        right = parse_number_or_group(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        tmp.type = MULTI;
        tmp.l = left;
        tmp.r = right;
        left = new_node(tmp);
    }
    return (left);
}

// 1
node *parse_expr(char *s)
{
    node *ret = parse_addition(&s);
    if (!ret)
        return NULL;
    if (*s) // если остались символы после разбора — ошибка
    {
        destroy_tree(ret);
        unexpected(*s);
        return NULL;
    }
    return ret;
}