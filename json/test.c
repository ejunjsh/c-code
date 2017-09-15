/*
 * test.c
 *
 *  Created on: 2016.11.11
 *      Author: loda
 */

#include "util.h"
#define ARR_LEN 4

void test_array() {
	json_array arr = init_array();
	json_value v1;
	v1.type = MI_NUMBER;
	v1.u.num = 1;

	char *strArr[ARR_LEN] = { "hello", "jack", "tom", "ok" };

	//	for (int i = 0; i < 100; i++) {
	//		v1.u.num=i;
	//		append_arr(&arr, v1);
	//	}

	//	int arr_len = 4;
	append_arr(&arr, &v1);
	json_value v2;
	v2.type = MI_STRING;
	for (int i = 0; i < ARR_LEN; i++) {
		v2.u.str.s = strArr[i];
		v2.u.str.len = strlen(strArr[i]);
		append_arr(&arr, &v2);
	}
	append_arr(&arr, &v1);
	//	append_arr(&arr, v1);
	//	append_arr(&arr, v2);
	//	append_arr(&arr, v3);
	//	set_arr_value(&arr,0,v2);

	print_array_desc(&arr);
	print_array(&arr);
	//	print_string(&(((&arr)->value + 0)->u.str));
	//	print_string(&(((&arr)->value + 1)->u.str));
}

void test_append_string() {
	string s = init_string();
	for (int i = 0; i < 50; i++) {
		append(&s, i + 64);
	}
	print_string(&s);
}

void test_cmp_string() {
	string s = { "hello", 3, 0 };
	string s1 = { "alkkk", 3, 0 };
	//	print_string(&s);
	//	print_string(&s1);
	int rs = stringcmp(&s, &s1);
	printf("%d", rs);
}

void test_hashmap() {
	hashmap map = init_hashmap(DEFAULT_MAP_LEN);

	string s = { .s = "name",.len = 4 };
	json_value v1 = { .type = MI_STRING };
	string v = { .s = "tom",.len = 4 };
	v1.u.str = v;
	put(&map, &s, &v1);
	print_object_desc(&map);
	print_object(&map);

	string s2 = { .s = "sub_button",.len = 10 };
	json_value v2 = { .type = MI_NUMBER };
	v2.u.num = 23;
	put(&map, &s2, &v2);
	print_object_desc(&map);
	print_object(&map);

	/*string s3 = { .s = "sex",.len = 3 };
	json_value v3 = { .type = MI_STRING };
	string vv = { .s = "male",.len = 4 };
	v3.u.str = vv;
	put(&map, &s3, &v3);
	print_object_desc(&map);
	print_object(&map);

	string get_key = { .s = "sex",.len = 3 };
	json_value *rs = get(&map, &get_key);
	print_value(rs);*/
	//	hashmap map = init_hashmap();
	//	for (int i = 0; i < map.cap; i++) {
	//		member *m = *(map.mb + i);
	//		if (m == NULL) {
	//			printf("null\n");
	//		}
	//	}
}

void test_alloc() {
	json_value v;
	v.u.num = 1;
	json_value *vs = mi_malloc(JSON_VALUE_SIZE * 2);
	vs[0] = v;
	vs[1] = v;
	//printf("%f %f %f %f\n", vs[0].u.num, vs[1].u.num, vs[2].u.num, vs[3].u.num);
	vs = mi_realloc(vs, JSON_VALUE_SIZE * 4);

	printf("%f %f %f %f\n", vs[0].u.num, vs[1].u.num, vs[2].u.num, vs[3].u.num);
}

void test_json_array() {
	json_value v;
	int rs = parse(&v,
		"[1,\"haha\",\"abcaa\",2,[1,5,\"kitty\"],\"tom\",\"brad\"]");
	printf("type is %d\n", v.type);

	if (v.type == MI_NUMBER) {
		printf("number is %f\n", v.u.num);
	}
	if (v.type == MI_STRING) {
		print_string(&(v.u.str));
	}

	if (v.type == MI_ARRAY) {
		print_array_desc(&(v.u.arr));
		print_array(&(v.u.arr));
	}

	printf("\nresult is %d\n", rs);

	init_json_value(&v);

}

void test_print_value() {
	json_value v = init_value();
	//	v.type=MI_NUMBER;
	//	v.u.num=3;
	v.type = MI_STRING;
	string s = { "jack", 4, 0 };
	v.u.str = s;
	print_value(&v);
}

void test_json_object() {
	json_value v = init_value();
	int rs =
		parse(&v,
			"{\"name\":\"jack\",\"age\":23,\"sex\":\"male\",\"hobby\":[\"football\",\"singing\"],\"cellphone\":\"12345678\"}");
	printf("type is %d\n", v.type);

	print_object_desc(&(v.u.map));
	print_value(&v);

	printf("\nresult is %d\n", rs);

	init_json_value(&v);
}

void test_array_alloc_free() {
	json_value* v1 = mi_malloc(10);
	json_value* v2 = mi_malloc(10);
	json_array arr = init_array();
	append_arr(&arr, v1);
	//free(v1);
	append_arr(&arr, v2);
	//free(v2);
	json_value *j1 = get_arr_value(&arr, 0);
	free(j1);
	json_value *j2 = get_arr_value(&arr, 1);
	free(j2);
	free(arr.value);
}

void test_pointer() {
	char *ch = mi_malloc(10);

	

	//char c = *ch;

	//printf("%p  ---%p %p\n", &c, ch, ch);
	//free(&c);//error
	//free(ch);//right
}

void parse_file() {
	FILE *f=fopen("china.txt","r");

	if (f == NULL) {
		printf("no such file\n");
		exit(1);
	}

	char buf[4096];
	size_t len = fread(buf, 1, 4095, f);
	buf[len] = '\0';

	printf("---origin--------\n%s\n---------after parse-------\n", buf);

	json_value v = init_value();
	int rs = parse(&v, buf);

	//printf("%s", buf);
	print_value(&v);

	printf("=============free begin================\n");
	init_json_value(&v);

	printf("\n===============================\n");

	fclose(f);
}

int main(int argc, char **argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	test_array();
	//	test_alloc();
		//test_json_array();
	//	test_cmp_string();
		//test_hashmap();
	//	test_print_value();
		//test_json_object();
	//	test_array_alloc_free();
	//test_pointer();
	parse_file();
	return 1;
}
