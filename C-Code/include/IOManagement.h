//
// Created by kenneth on 14/3/21.
//

#ifndef IMAGETEST_IOMANAGEMENT_H
#define IMAGETEST_IOMANAGEMENT_H
#include <pigpio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>
void * analyse_Gpio(void * gpioValue);
void execute_change(int mode, int gpio);
int * gpio_list;
int list_len;
short active;
void set_gpio_list_predefined(int * list);
void set_gpio_list(int * list, int size);
void clear_list();
void execute_monitoring();
#endif //IMAGETEST_IOMANAGEMENT_H
