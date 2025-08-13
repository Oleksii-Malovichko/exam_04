#include "argo.h"

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
	int i;
	char c;

	if (!expect(stream, '"'))
		return -1;
	i = 0;
	while (1)
	{
		c = getc(stream);
		if (c == EOF)
		{
			unexpected(stream);
			return -1;
		}
		if (c == '"')
			break;
		if (c == '\\')
		{
			c = getc(stream);
			if (c == EOF)
			{
				unexpected(stream);
				return -1;
			}
		}
		buffer[i++] = c;
	}
	buffer[i] = '\0';
	dst->type = STRING;
	dst->string = strdup(buffer);
	return 1;
}

int parse_map(json *dst, FILE *stream)
{
	pair *items;
	json key;
	size_t size;

	if (!expect(stream, '{'))
		return -1;
	items = NULL;
	size = 0;
	while (!accept(stream, '}'))
	{
		items = realloc(items, sizeof(pair) * (size + 1));
		if (parse_string(&key, stream) == -1)
		{
			free(items);
			return -1;
		}
		if (!expect(stream, ':'))
		{
			free(items);
			free(key.string);
			return -1;
		}
		if (parser(&items[size].value, stream) == -1)
		{
			free(items);
			free(key.string);
			return -1;
		}
		items[size].key = key.string;
		size++;
		if (!accept(stream, ',') && peek(stream) != '}')
		{
			unexpected(stream);
			free(items);
			return -1;
		}
	}
	dst->type = MAP;
	dst->map.size = size;
	dst->map.data = items;
	return 1;
}

int argo(json *key, FILE *stream)
{
	return parser(key, stream);
}