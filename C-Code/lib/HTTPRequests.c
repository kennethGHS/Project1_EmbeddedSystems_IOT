#include "HTTPRequests.h"
#include <pthread.h>
int update_pins_server(){
    sem_wait(request_semaphore);

    CURL *curl;
    CURLcode res;

    char *post_json =  read_file_to_string() ;

    curl = curl_easy_init();
    if(curl) {

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/api/sensors");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_json);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post_json));

    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
        free(post_json);
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        sem_post(request_semaphore);
        return -1;
    }
    }
    free(post_json);
    curl_easy_cleanup(curl);
    sem_post(request_semaphore);
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
    strcpy(mem->response,data);
    mem->size += realsize;
    printf("%s \n",mem->response);
    return realsize;
 }


int * get_update_lights(){
    sem_wait(request_semaphore);
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
    for(int i = 0;i<5;i++){
        char pin[50];
        snprintf(pin, 50, "pin %d", i);
        int value_pin;
        json_object_object_get_ex(json_root,pin,&temp);
        value_pin = json_object_get_int(temp);
        *(pins+i) = value_pin;
    }
    json_object_put(json_root);
    sem_post(request_semaphore);
    return pins;
}



int upload_picture(){
    printf("Uploading \n");
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
    sem_wait(request_semaphore);
    struct memory chunk;
    CURL *curl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/api/take_picture");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    json_object * json_root = json_tokener_parse(chunk.response);
    json_object * temp ;
    json_object_object_get_ex(json_root,"photo",&temp);
    int value = json_object_get_int(temp);
    struct stat st = {0};

    if (stat("../Image", &st) == -1) {
      mkdir("../Image", 0777);
    }
    json_object_put(json_root);
    if (value == 1)
    {
        execute_image("../Image/picture.jpeg");
        upload_picture();
    }
    sem_post(request_semaphore);
    return 0;
 }

 void init_semaphore(){
    request_semaphore = malloc(sizeof(sem_t));
    sem_init(request_semaphore, 0, 1);
 }
