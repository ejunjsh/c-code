/*
 * mison.h
 *
 *  Created on: 2016.11.10
 *      Author: loda
 */

#ifndef MISON_H_
#define MISON_H_
#include <stdio.h>

#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>

#include <assert.h>
#include <string.h>

#define DEFAULT_STR_LEN 10
#define JSON_VALUE_SIZE (sizeof(json_value))
#define DEFAULT_ARR_LEN 10
#define DEFAULT_MAP_LEN 10

typedef enum {
	MI_NULL, MI_TRUE, MI_FALSE, MI_NUMBER, MI_STRING, MI_ARRAY, MI_OBJECT,MI_UNDEFINE
} json_type;

typedef struct {
	char *s;
	size_t len;
	size_t extra;
} string;

typedef struct json_value json_value;

typedef struct member member;

typedef struct {
	json_value **value;
	size_t len;
	size_t extra;
} json_array;

typedef struct {
	member **mb;
	size_t cap;
	size_t size;
} hashmap;

struct json_value {
	union {
		string str;
		double num;
		json_array arr;
		hashmap map;
	} u;
	json_type type;
};

typedef struct member {
	string key;
	json_value *value;
	member *next;
} member;

enum {
	PARSE_OK, PARSE_ERROR, PARSE_MISS_QUOTES, PARSE_END
};

typedef struct {
	const char *json_string;
} json_context;

int parse(json_value *value, const char *json);

void init_json_value(json_value *value);

void *mi_malloc(size_t size);

void *mi_calloc(size_t count, size_t size);

void *mi_realloc(void *block,size_t size);

#endif /* MISON_H_ */
