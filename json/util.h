/*
 * util.h
 *	2016.11.10
 *      Author: loda
 */

#ifndef UTIL_H_
#define UTIL_H_
#include "mison.h"

void print_string_desc(string *s);

void print_string(string *s);

void print_array_desc(json_array *array);

void print_array(json_array *array);

void print_object_desc(hashmap *map);

void print_object(hashmap *map);

void print_value(json_value *value);

void handle_allocate_error(const char *msg);

void append(string *s, char c);

void append_arr(json_array *arr, json_value *value);

string init_string(void);

json_array init_array(void);

json_value init_value(void);

member *init_member();

int stringcmp(string *first, string *second);

void put(hashmap *map, string *s, json_value *value);

json_value *get(hashmap *map, string *s);

member *get_member(hashmap *m, int i);

void set_member(hashmap *m, int i, member *member);

hashmap init_hashmap(size_t cap);

json_value *get_arr_value(json_array *arr, int i);

void set_arr_value(json_array *arr, int i, json_value *value);

#endif /* UTIL_H_ */
