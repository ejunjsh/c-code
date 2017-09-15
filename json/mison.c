/*
 * mison.c
 *
 *  Created on: 2016.11.11
 *      Author: loda
 */

#include "mison.h"
#include "util.h"
#include <string.h>

static int parse_json_value(json_context *context, json_value *value);

void *mi_malloc(size_t size) {
	void *mem = malloc(size);
	if (mem == NULL) {
		handle_allocate_error("mallocate error\n");
	}
	return mem;
}

void *mi_calloc(size_t count, size_t size) {
	void *mem = calloc(count, size);
	if (mem == NULL) {
		handle_allocate_error("callocate error\n");
	}
	return mem;
}

void *mi_realloc(void *block, size_t size) {
	void *mem = realloc(block, size);
	if (mem == NULL) {
		handle_allocate_error("reallocate error\n");
	}
	return mem;
}

void init_json_value(json_value *value) {
	if (value != NULL) {
		switch (value->type) {
		case MI_STRING:
			free(value->u.str.s);
			break;
		case MI_ARRAY:
			for (int i = 0; i < value->u.arr.len; i++) {
				json_value *element = get_arr_value(&(value->u.arr),i);
				init_json_value(element);
				free(element);
			}
			free(value->u.arr.value);
			break;
		case MI_OBJECT:
			for (int i = 0; i < value->u.map.cap; i++) {
				member *m = get_member(&(value->u.map),i);
				member *next;
				for (int j=0; m != NULL; m = next,j++) {
					next = m->next;
					
					if (j != 0) {
						free(m->key.s);
						init_json_value(m->value);
						free(m->value);
					}
					free(m);
				}
			}

			free(value->u.map.mb);
			break;
		default:
			break;
		}
		value->type = MI_NULL;
		value->u.num = 0;
		value->u.str.extra = 0;
		value->u.str.len = 0;
	}
}

static int is_whitespace(char ch) {
	return ch == ' ' || ch == '\t' || ch == '\n';
}

static void parse_whitespace(json_context *context) {
	const char *p = context->json_string;
	while (is_whitespace(*p)) {
		p++;
	}

	context->json_string = p;
}

static int parse_true(json_context *context, json_value *value) {
	const char *p = context->json_string;
	if (p[0] == 't' && p[1] == 'r' && p[2] == 'u' && p[3] == 'e') {
		p += 4;
		context->json_string = p;
		value->type = MI_TRUE;
		return PARSE_OK;
	}
	else {
		return PARSE_ERROR;
	}
}

static int parse_false(json_context *context, json_value *value) {
	const char *p = context->json_string;
	if (p[0] == 'f' && p[1] == 'a' && p[2] == 'l' && p[3] == 's'
		&& p[4] == 'e') {
		p += 5;
		context->json_string = p;
		value->type = MI_FALSE;
		return PARSE_OK;
	}
	else {
		return PARSE_ERROR;
	}
}

static int parse_null(json_context *context, json_value *value) {
	const char *p = context->json_string;
	if (p[0] == 'n' && p[1] == 'u' && p[2] == 'l' && p[3] == 'l') {
		p += 4;
		context->json_string = p;
		value->type = MI_NULL;
		return PARSE_OK;
	}
	else {
		return PARSE_ERROR;
	}
}

/**
 * just int
 */
static int parse_number(json_context *context, json_value *value) {
	char *end;
	value->u.num = strtod(context->json_string, &end);

	if (context->json_string == end) {
		return PARSE_ERROR;
	}
	value->type = MI_NUMBER;
	context->json_string = end;
	return PARSE_OK;
}

static int parse_string(json_context *context, json_value *value) {
	assert('"' == *(context->json_string++));
	init_json_value(value);
	string s = init_string();
	for (;;) {
		switch (*context->json_string) {
		case '"':
			(context->json_string)++;
			value->type = MI_STRING;
			value->u.str = s;
			return PARSE_OK;
		case '\0':
			return PARSE_MISS_QUOTES;
		default:
			append(&s, *(context->json_string++));
			break;
		}
	}

	return PARSE_ERROR;
}

static int parse_array(json_context *context, json_value *value) {
	assert('[' == *(context->json_string++));
	init_json_value(value);
	json_array arr = init_array();
	int r;
	char ch;
	for (;;) {
		ch = *context->json_string;
		if (ch == ',') {
			(context->json_string)++;
		}
		else if (ch == ']') {
			(context->json_string)++;
			value->type = MI_ARRAY;
			value->u.arr = arr;
			return PARSE_OK;
		}
		else if (is_whitespace(ch)) {
			parse_whitespace(context);
		}
		else {
			json_value* cp = mi_malloc(sizeof(json_value));
			if ((r = parse_json_value(context, cp)) != PARSE_OK) {
				return r;
			}
			append_arr(&arr, cp);
		}
	}

	return PARSE_ERROR;
}

static int parse_object(json_context *context, json_value *value) {
	assert('{' == *(context->json_string++));
	init_json_value(value);
	char ch;
	int r;
	hashmap map = init_hashmap(DEFAULT_MAP_LEN);
	for (;;) {
		ch = *context->json_string;
		if (ch == ',') {
			(context->json_string)++;
		}
		else if (ch == '}') {
			(context->json_string)++;
			value->type = MI_OBJECT;
			value->u.map = map;
			return PARSE_OK;
		}
		else if (ch == '"') {
			json_value k = init_value();
			if ((r = parse_string(context, &k)) != PARSE_OK) {
				return r;
			}
			parse_whitespace(context);
			assert(':' == *(context->json_string++));
			parse_whitespace(context);

			json_value* v = mi_malloc(sizeof(json_value));
			if ((r = parse_json_value(context, v)) != PARSE_OK) {
				return r;
			}
			put(&map, &(k.u.str), v);
		}
		else if (is_whitespace(ch)) {
			parse_whitespace(context);
		}
		else {
			return PARSE_ERROR;
		}
	}
	return PARSE_OK;
}

static int parse_json_value(json_context *context, json_value *value) {
	int rs;
	switch (*context->json_string) {
	case 'n':
		rs = parse_null(context, value);
		break;
	case 't':
		rs = parse_true(context, value);
		break;
	case 'f':
		rs = parse_false(context, value);
		break;
	case '"':
		rs = parse_string(context, value);
		break;
	case '[':
		rs = parse_array(context, value);
		break;
	case '{':
		rs = parse_object(context, value);
		break;
	case '\0':
		rs = PARSE_END;
		(context->json_string)++;
		break;
	default:
		rs = parse_number(context, value);
		break;
	}
	return rs;
}

int parse(json_value *value, const char *json) {
	json_context context;
	context.json_string = json;
	value->type = MI_NULL;
	parse_whitespace(&context);
	return parse_json_value(&context, value);
}
