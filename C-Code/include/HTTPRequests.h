#include <curl/curl.h>
#include <semaphore.h>
#include <json-c/json.h>
#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "ImageGet.h"
#include "JsonAdmin.h"
sem_t * request_semaphore;
int update_pins_server();
int * get_update_lights();
static size_t cb(void *data, size_t size, size_t nmemb, void *userp);
 struct memory {
   char *response;
   size_t size;
 };
 void init_semaphore();
 int upload_picture();
 int get_image_take();