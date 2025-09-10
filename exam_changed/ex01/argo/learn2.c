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
    size_t cap = 16;
    size_t len = 0;
    char *buffer = malloc(cap);
    if (!buffer)
        return -1;

    if (!expect(stream, '"')) {
        free(buffer);
        return -1;
    }

    while (1) {
        int c = getc(stream);
        if (c == EOF) {
            free(buffer);
            unexpected(stream);
            return -1;
        }
        if (c == '"')
            break;
        if (c == '\\') {
            c = getc(stream);
            if (c == EOF) {
                free(buffer);
                unexpected(stream);
                return -1;
            }
            if (c != '\\' && c != '"') {
                free(buffer);
                unexpected(stream);
                return -1;
            }
        }
        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(buffer, cap);
            if (!tmp) {
                free(buffer);
                return -1;
            }
            buffer = tmp;
        }
        buffer[len++] = (char)c;
    }

    buffer[len] = '\0';
    dst->type = STRING;
    dst->string = buffer;
    return 1;
}

void free_items(pair *items, size_t size)
{
	if (!items)
		return ;
	for (size_t i = 0; i < size; i++)
	{
		free(items[i].key);
		free_json(items[i].value);
	}
	free(items);
}

int parse_map(json *dst, FILE *stream)
{
	pair *items;
	json key;
	size_t size;

	if (!expect(stream, '{'))
		return -1;
	size = 0;
	items = NULL;
	while (!accept(stream, '}'))
	{
		pair *tmp = realloc(items, sizeof(pair) * (size + 1));
		if (!tmp)
		{
			if (items)
				free_items(items, size);
			return -1;
		}
		items = tmp;
		if (parse_string(&key, stream) == -1)
		{
			free_items(items, size);
			return -1;
		}
		if (!expect(stream, ':'))
		{
			free_items(items, size);
			free(key.string);
			return -1;
		}
		if (parser(&items[size].value, stream) == -1)
		{
			free_items(items, size);
			free(key.string);
			return -1;
		}
		items[size].key = key.string;
		size++;
		if (!accept(stream, ',') && peek(stream) != '}')
		{
			unexpected(stream);
			free_items(items, size);
			return -1;
		}
	}
	dst->type = MAP;
	dst->map.size = size;
	dst->map.data = items;
	return 1;
}

int argo(json *dst, FILE *stream)
{
	return parser(dst, stream);
}