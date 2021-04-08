//
// Created by kenneth on 21/3/21.
//
#include <zconf.h>
#include "Monitoring.h"

void *execute_monitoring(void *gpio) {
    int pin = *(int *) gpio;
    sem_wait(export_semaphore);
    pinMode(pin, 1);
    sem_post(export_semaphore);
    int actual_value = digitalRead(pin);
    int prev_value = actual_value;
    while (thread_executing) {
        actual_value = digitalRead(pin);
        if (actual_value != prev_value) {
            prev_value = actual_value;
            execute_on_change(pin, actual_value);
        }
        sleep(1);
    }
    pthread_exit(0);
}

void initialize_values(int *gpio_in_list, int gpio_in_list_len, int * gpio_out_list, int gpio_out_list_len) {
    thread_executing = 1;
    init_semaphore();
    while (update_pins_server()==-1)
    {
        printf("Error, no server detected, connecting again in 3s \n");
        sleep(3);
        printf("trying again \n");
    }
    gpio_list = malloc(sizeof(int) * gpio_in_list_len);
    for (int i = 0; i < gpio_in_list_len; ++i) {
        gpio_list[i] = gpio_in_list[i];
    }
    gpio_list_len = gpio_in_list_len;
    export_semaphore = malloc(sizeof(sem_t));
    action_semaphore = malloc(sizeof(sem_t));
    unexport_semaphore = malloc(sizeof(sem_t));
    sem_init(export_semaphore, 0, 1);
    sem_init(action_semaphore, 0, 1);
    sem_init(unexport_semaphore, 0, 1);
    pthread_t thread_list[gpio_in_list_len];
    for (int i = 0; i < gpio_in_list_len; ++i) {
        pthread_create(&(thread_list[i]), NULL, execute_monitoring, (void *) &(gpio_list[i]));
    }

    light_list_len = gpio_out_list_len;
    light_list = malloc(sizeof(int)*light_list_len);
    for(int i = 0;i<light_list_len;i++){
        light_list[i]=gpio_out_list[i];
        sem_wait(export_semaphore);
        pinMode(light_list[i],0);
        sem_post(export_semaphore);
    }
    pthread_t light_thread;
    pthread_create(&(light_thread), NULL, execute_light_monitoring,NULL);
    while (thread_executing)
    {
        get_image_take();
        sleep(1);
    }
    
}

void * execute_light_monitoring(){
    while (thread_executing)
    {
        int * gpio_states = get_update_lights();
        for(int i=0; i<light_list_len;i++){
            sem_wait(action_semaphore);
            digitalWrite(light_list[i],gpio_states[i]);
            sem_post(action_semaphore);
        }
        sleep(1);
    }
    
}

void destroy_monitoring() {
    thread_executing = -1;
    for (int i = 0; i < gpio_list_len; ++i) {
        gpio_unexport(gpio_list[i]);
    }
    for(int i = 0; i < light_list_len;i++){
        gpio_unexport(light_list[i]);
    }
}
int get_position_gpio(int gpio){
    for (int i = 0; i < gpio_list_len; i++)
    {
        if (gpio_list[i] == gpio)
        {
            return i;
        }
    }
    return -1;
}

void execute_on_change(int gpio, int value) {
    sem_wait(action_semaphore);
    printf("The pin %d changed to value %d\n", gpio, value);
    gpio = get_position_gpio(gpio);
    if (gpio == -1)
    {
        printf("Error the GPIO doesnt exist in list \n");
        return;
    }
    modify_pin_state(gpio,value);
    update_pins_server();
    sem_post(action_semaphore);
}