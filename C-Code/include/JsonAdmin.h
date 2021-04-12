#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <json-c/json.h>
/** modifies a json entry of a given key with a given value
 * @param jobj the json object to be modified
 * @param key the key of the value to be modified
 * @param value value to be given to the key
 **/
void modify_entry( json_object * jobj, char * key,int value);
/** Reads the json file containing the pin values
 * @return returns the jsonb object of the file
 **/
json_object * read_json();
/** writes the Json of the object given to the pin files 
 *  @param jobj the object to be written
 **/
void write_json( json_object * jobj);
/** Validates that the directory exists **/
int validate_files_dir();
/** creates the simple JSON file of the pin states **/
int create_simple_file();
/** Reads the Json file to a string **/
char * read_file_to_string();
/** Modifies the json file and saves it, rewritting the
 * previous file **/
int modify_pin_state(int pin, int state);