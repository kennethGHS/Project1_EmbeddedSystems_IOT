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
/** Post request that updates the pin status in the server **/
int update_pins_server();
/** Get request that retreives the supposed values of the lights
 * @return An array of all the lights values in order **/
int * get_update_lights();
/** Function used by get functions to receive the otput of the server
 * the userp is actually the struct memory, this function writes the output
 * of the server into the attribute response **/
static size_t cb(void *data, size_t size, size_t nmemb, void *userp);
 struct memory {
   char *response;
   size_t size;
 };
 /** Inits the semaphore necessary for the execution **/
 void init_semaphore();
 /** PUT request that uploads a photo to the server **/
 int upload_picture();
 /** GET request that receives a response of the server indicating if 
  * you need to take a photo **/
 int get_image_take();