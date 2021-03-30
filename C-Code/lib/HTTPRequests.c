#include "HTTPRequests.h"
#include <pthread.h>
int update_pins_server(){

    CURL *curl;
    CURLcode res;

    char *post_json =  read_file_to_string() ;

    curl = curl_easy_init();
    if(curl) {

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/api/sensors");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_json);

    /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
        itself */ 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post_json));

    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

    /* always cleanup */ 
    curl_easy_cleanup(curl);
    }
    return 0;
}
static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
 {
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)userp;
    char *ptr =(char *) malloc(  realsize + 8);
    if(ptr == NULL){
        perror("ERROR");
        return 0;  /* out of memory! */
    }
    mem->response = ptr;
    // memcpy(&(mem->response[mem->size]), data, realsize);
    strcpy(mem->response,data);
    mem->size += realsize;
    // mem->response[mem->size] = 0;
    printf("%s \n",mem->response);
    return realsize;
 }


int * get_update_lights(){
    struct memory chunk;
    CURL *curl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/api/lights");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_perform(curl);
   json_object * json_root = json_tokener_parse(chunk.response);
    int * pins = malloc(sizeof(int)*5);
    json_object * temp;
    curl_easy_cleanup(curl);
    perror("Error");
    for(int i = 0;i<5;i++){
        printf("%d\n",i);
        char pin[50];
        perror("Error");
        snprintf(pin, 50, "pin %d", i);
        int value_pin;
        json_object_object_get_ex(json_root,pin,&temp);
        value_pin = json_object_get_int(temp);
        *(pins+i) = value_pin;
    }
    json_object_put(json_root);
    // curl_easy_cleanup(curl);
    perror("Final");
    return pins;
}



int upload_picture(){

    CURL *curl;
    CURLcode res;
    struct stat file_info;
    curl_off_t speed_upload, total_time;
    FILE *fd;
    struct stat st = {0};

    if (stat("../Image", &st) == -1) {
      mkdir("../Image", 0777);
    }

    fd = fopen("../Image/picture.jpeg", "rb"); /* open file to upload */ 
    if(!fd)
        return 1; /* can't continue */ 


    if(fstat(fileno(fd), &file_info) != 0)
        return 1; 

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
                            "http://127.0.0.1:8000/api/picture");

        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        curl_easy_setopt(curl, CURLOPT_READDATA, fd);

        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                            (curl_off_t)file_info.st_size);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        }
        else {
            curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &speed_upload);
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total_time);

            fprintf(stderr, "Speed: %" CURL_FORMAT_CURL_OFF_T " bytes/sec during %"
                    CURL_FORMAT_CURL_OFF_T ".%06ld seconds\n",
                    speed_upload,
                    (total_time / 1000000), (long)(total_time % 1000000));

        }
        curl_easy_cleanup(curl);
    }
    fclose(fd);
    return 0;
 }

 int get_image_take(){
    struct memory chunk;
    CURL *curl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    json_object * json_root = json_tokener_parse(chunk.response);
    json_object * temp ;
    json_object_object_get_ex(json_root,"key",&temp);
    int value = json_object_get_int(temp);
    
    json_object_put(temp);
    json_object_put(json_root);
    if (value == 1)
    {
        execute_image("../Image/picture.jpeg");
        curl_easy_cleanup(curl);
        upload_picture();
    }
    
    return 0;
 }