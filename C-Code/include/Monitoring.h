//
// Created by kenneth on 21/3/21.
//
#include <semaphore.h>
#include "ManualGPIO.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "JsonAdmin.h"
#include "HTTPRequests.h"
#ifndef IMAGETEST_MONITORING_H
#define IMAGETEST_MONITORING_H
//This semaphore is to prevent several pins to initialize at the same time
sem_t * export_semaphore;
//This is in case the action is resources sensitive
sem_t * action_semaphore;
//in case that the unexporting results in a conflict,
sem_t * unexport_semaphore;
//The list of gpios that are going to be monitored
int * gpio_list;
//The len of the lists
int gpio_list_len;
//The list of pins related to 
int * light_list;
//The len of the list
int light_list_len;
//Used to stop the thread cycle that reads the gpio
int thread_executing;
/**
 * Makes an http request to update the pins
 **/
void * execute_light_monitoring();
/**
 * Function called by thread to execute
 * @param gpio the gpio to be analyzed
 * @return nothing
 */
void * execute_monitoring(void * gpio);
/**
 * Initializes the values to be analyzed, also starts the semaphores and the exporting of the gpio pins
 *  starts the threads, and returns nothing
 * @param list  the list to be analyzed
 * @param list_len the len of the list
 */
void initialize_values(int * list,int list_len, int * gpio_out_list, int gpio_out_list_len);
/**
 * Unexports all the pins
 */
void destroy_monitoring();
/**
 * The function to be executed when a change is detected
 * @param gpio the gpio that changed
 * @param value the actual value of the GPIO
 */
void execute_on_change(int gpio, int value);
/**
 * Gets the position of a gpio number
 **/
int get_position_gpio(int gpio);

#endif //IMAGETEST_MONITORING_H
