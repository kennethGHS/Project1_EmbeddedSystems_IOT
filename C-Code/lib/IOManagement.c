//
// Created by kenneth on 16/3/21.
//

#include "IOManagement.h"

void *analyse_Gpio(void *gpioValue) {
    gpioInitialise();
    int gpio = *(int *) gpioValue;
    gpioSetMode(gpio, PI_INPUT);
    int previous_state = gpioRead(gpio);
    while (active) {
        int actual_state = gpioRead(gpio);
        if (previous_state != actual_state) {
            if (actual_state == PI_HIGH) {
                printf("Change to value %d in pin %d",1,gpio);
                execute_change(1,gpio);
                sleep(0.3);
            } else if (actual_state == PI_LOW) {
                printf("Change to value %d in pin %d",0,gpio);
                execute_change(0,gpio);
                sleep(0.3);

            }
            previous_state = actual_state;
        }
    }
}

/**
 * This function is what happens when we detect a high or a low
 */
void execute_change(int mode, int gpio){
    
}

void set_gpio_list_predefined(int *list) {
    gpio_list = list;
}

void set_gpio_list(int *list, int size) {
    gpio_list = malloc(sizeof(int) * size);
    list_len = size;
    for (int i = 0; i < size; ++i) {
        gpio_list[i] = list[i];
    }
}

void clear_list() {
    if (gpio_list) {
        free(gpio_list);
    } else {
        perror("No list to free");
    }
}

void execute_monitoring() {
    active =1;
    pthread_t id[list_len];
    if (gpioInitialise()<0){
        printf("Error");
    } 
    for (int i = 0; i < list_len; ++i) {
        pthread_create(&(id[i]), NULL, analyse_Gpio, (void *) &(gpio_list[i]));
    }
    char a_word[20];
    printf ("Press any to finish ");
    scanf ("%s", a_word);
    active = 0;
}