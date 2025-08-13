#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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


node	*parse_expr(char *s);
node    *parse_number_or_group(char **s);
node    *parse_addition(char **s);
node    *parse_multiplication(char **s);