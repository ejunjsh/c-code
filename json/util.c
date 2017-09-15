/*
 * util.c
 *
 *  Created on: 2016.11.11
 *      Author: loda
 */

#include "util.h"

member *get_member(hashmap *m, int i) {
	return *(m->mb + i);
}

void set_member(hashmap *m, int i, member *member) {
	*(m->mb + i) = member;
}

void print_string_desc(string *s) {
	if (s == NULL) {
		printf("null");
	} else if (s->len == 0) {
		printf("empty string\n");
	} else {
		printf("string len:%I64u,extra len:%I64u\n", s->len, s->extra);
	}
}

void print_string(string *s) {
	if (s == NULL) {
		printf("null");
	} else if (s->len == 0) {
		printf("");
	} else {
		for (int i = 0; i < s->len; i++) {
			putchar(*(s->s + i));
		}
	}
}

void print_value(json_value *element) {
	if (element == NULL) {
		printf("null");
		return;
	}
	switch (element->type) {
	case MI_NULL:
		printf("null");
		break;
	case MI_TRUE:
		printf("true");
		break;
	case MI_FALSE:
		printf("false");
		break;
	case MI_NUMBER:
		printf("%f", element->u.num);
		break;
	case MI_STRING:
		print_string(&(element->u.str));
		break;
	case MI_ARRAY:
		print_array(&(element->u.arr));
		break;
	case MI_OBJECT:
		print_object(&(element->u.map));
		break;
	default:
		fprintf(stdout, "unknown json value type [%d]\n", element->type);
		exit(-1);
		break;
	}

}

void print_object_desc(hashmap *map) {
	if (map == NULL) {
		printf("null");
	} else if (map->size == 0) {
		printf("empty map\n");
	} else {
		printf("object size:%I64u,cap:%I64u\n", map->size, map->cap);
	}
}

void print_object(hashmap *map) {
	if (map == NULL) {
		printf("null");
		return;
	}
	int size = map->size;
	printf("{");
	for (int i = 0; i < map->cap; i++) {
		member *m = get_member(map, i);
		if (m != NULL) {
			member *next = m->next;
			while (next != NULL) {
				print_string(&(next->key));
				printf(":");
				print_value(next->value);
				next = next->next;
				if (--size > 0) {
					printf(",");
				}
			}
		}
	}
	printf("}");
}

void print_array_desc(json_array *array) {
	assert(array!=NULL);
	if (array->len == 0) {
		printf("empty array\n");
	} else {
		printf("array len:%I64u,extra len:%I64u\n", array->len, array->extra);
	}
}

void set_arr_value(json_array *arr, int i, json_value *value) {
	*(arr->value + i) = value;
}

json_value *get_arr_value(json_array *arr, int i) {
	return *(arr->value + i);
}

void print_array(json_array *array) {
	assert(array!=NULL);
	if (array->len == 0) {
		printf("[]");
	} else {
		printf("[");
		for (int i = 0; i < array->len; i++) {
			json_value *element = get_arr_value(array, i);
			print_value(element);

			if (i < array->len - 1) {
				printf(",");
			}
		}
		printf("]");
	}
}

void handle_allocate_error(const char *msg) {
	fprintf(stdout, msg);
	exit(1);
}

void append(string *s, char c) {
	if (s->s == NULL) {
		s->s = mi_malloc(DEFAULT_STR_LEN);
		s->extra = DEFAULT_STR_LEN;
		s->len = 0;
	}

	if (s->extra == 0) {
		size_t new_len = s->len << 1;
		s->s = mi_realloc(s->s, new_len);
		s->extra = new_len - s->len;
	}
	s->s[s->len++] = c;
	s->extra--;
}

void append_arr(json_array *arr, json_value *value) {
	if (arr->value == NULL) {
		arr->value = mi_malloc(DEFAULT_ARR_LEN * JSON_VALUE_SIZE);
		arr->extra = DEFAULT_ARR_LEN;
		arr->len = 0;
	}

	if (arr->extra == 0) {
		size_t new_len = arr->len << 1;
		arr->value = mi_realloc(arr->value,
			new_len * JSON_VALUE_SIZE);

		arr->extra = new_len - arr->len;
	}
	set_arr_value(arr, arr->len++, value);
	arr->extra--;
}

hashmap init_hashmap(size_t cap) {
	hashmap m;
	m.size = 0;
	m.cap = cap;
	m.mb = mi_calloc(cap, sizeof(member*));
	return m;
}

static unsigned hash(hashmap *map, string *s) {
	unsigned h = 0;
	for (int i = 0; i < s->len; i++) {
		h = 31 * h + s->s[i];
	}
	return h % (map->cap);
}

static char *to_char_array(string *s) {
	if (s->len == 0) {
		return NULL;
	}
	char *ch = mi_malloc(s->len + 1);
	ch[s->len] = '\0';
	for (int i = 0; i < s->len; i++) {
		ch[i] = s->s[i];
	}
	return ch;
}

int stringcmp(string *first, string *second) {
	char *f = to_char_array(first);
	char *s = to_char_array(second);
	free(f);
	free(s);
	return strcmp(f, s);
}

member *init_member() {
	member *mb = mi_malloc(sizeof(member));
	mb->next = NULL;
	mb->value = NULL;
	return mb;
}

static void resize(hashmap *map) {
	printf("resize\n");
	hashmap new_map = init_hashmap(map->cap << 1);
	for (int i = 0; i < map->cap; i++) {
		member *m = get_member(map, i);

		if (m != NULL) {
			member *next = m->next;

			while (next != NULL) {
				put(&new_map, &(next->key), next->value);
				next = next->next;
			}
		}
	}

	free(map->mb);
	map->cap = new_map.cap;
	map->size = new_map.size;
	map->mb = new_map.mb;
}

void put(hashmap *map, string *s, json_value *value) {
	if (map == NULL) {
		hashmap mp = init_hashmap(DEFAULT_MAP_LEN);
		map = &mp;
	}

	if (map->size == map->cap) {
		resize(map);
	}
	unsigned h = hash(map, s);

	member *mb = get_member(map, h);
	if (mb == NULL) {
		mb = init_member();
		set_member(map, h, mb);
	}

	member *next = mb->next;
	while (next != NULL) {
		if (!stringcmp(&(next->key), s)) {
			next->value = value;
		}
		next = next->next;
	}

	member *insert = init_member();
	insert->key = *s;
	insert->value = value;
	insert->next = mb->next;
	mb->next = insert;
	map->size++;
}

json_value *get(hashmap *map, string *s) {
	if (map == NULL) {
		hashmap mp = init_hashmap(DEFAULT_MAP_LEN);
		map = &mp;
		return NULL;
	}
	unsigned h = hash(map, s);
	member *mb = get_member(map, h);
	if (mb == NULL) {
		mb = init_member();
		set_member(map, h, mb);
		return NULL;
	}

	member *next = mb->next;
	while (next != NULL) {
		if (!stringcmp(&(next->key), s)) {
			return next->value;
		}
		next = next->next;
	}

	return NULL;
}

string init_string(void) {
	string s = {.s=NULL,.extra=0,.len=0};
	return s;
}

json_value init_value(void) {
	json_value v = {.type=MI_UNDEFINE};
	return v;
}

json_array init_array(void) {
	json_array arr = {.value=NULL,.extra=0,.len=0};
	return arr;
}

