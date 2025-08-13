#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


//added
#include <stdlib.h>
#include <ctype.h>

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

int	peek(FILE *stream);
void	unexpected(FILE *stream);
int	accept(FILE *stream, char c);
int	expect(FILE *stream, char c);
void	free_json(json j);
void	serialize(json j);
int parser(json *dst, FILE *stream);
int parse_int(json *dst, FILE *stream);
int parse_string(json *dst, FILE *stream);
int parse_map(json *dst, FILE *stream);
int argo(json *dst, FILE *stream);