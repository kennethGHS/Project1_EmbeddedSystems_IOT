#include "JsonAdmin.h"
void modify_entry( json_object * jobj, char * key,char * value){
    create_simple_file();
    char * value_for_json = malloc(15);
    strcpy(value_for_json,value);
    char * key_for_json = malloc(15);
    strcpy(key_for_json,key);
    json_object * new_json_obj=  json_object_new_string(value_for_json);
    json_object_object_del(jobj,key);
    json_object_object_add(jobj,key_for_json,new_json_obj);
    return;
}
json_object * read_json(){
    create_simple_file();
    return json_object_from_file("../JsonFile/PinStates.json");
}
void write_json( json_object * jobj){
    create_simple_file();
    json_object_to_file("../JsonFile/PinStates.json",jobj);

}
int validate_files_dir(){
    FILE * file;
    if (file = fopen("../JsonFile/PinStates.json","r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
int create_simple_file(){
    FILE * file;
    if(validate_files_dir()){
        perror("Files already exist");
        return -1;
    }
    else{
        printf("Validating file \n");
        char * document = "{\
   \"pin 1\":\"0\",\
   \"pin 2\":\"0\",\
   \"pin 3\":\"0\",\
   \"pin 4\":\"0\",\
   \"pin 5\":\"0\",\
   \"pin 6\":\"0\",\
   \"pin 7\":\"0\",\
   \"pin 8\":\"0\"}";
   file = fopen("../JsonFile/PinStates.json","w");
   if(!file){
       perror("Error with file");
       return -1;
   }
   printf("Writting file\n");
   if(strlen(document) !=fwrite(document,1,strlen(document),file) ){
       fclose(file);
       perror("Error while writting file");
       return -1;
   }
   else
   {
        printf("Finished writting file \n");
       fclose(file);
   }
    }

}

char * read_file_to_string(){
    create_simple_file();
    json_object * object = json_object_from_file("../JsonFile/PinStates.json");
    char * string = malloc(strlen(json_object_to_json_string(object)));
    strcpy(string,json_object_to_json_string(object));
    json_object_put(object);
    return string;
}