#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>
void modify_entry( json_object * jobj, char * key,char * value);
json_object * read_json();
void write_json( json_object * jobj);
int validate_files_dir();
int create_simple_file();
char * read_file_to_string();
int modify_pin_state(int pin, int state);