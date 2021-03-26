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

void initialize_values(int *list, int list_len) {
    thread_executing = 1;
    gpio_list = malloc(sizeof(int) * list_len);
    for (int i = 0; i < list_len; ++i) {
        gpio_list[i] = list[i];
    }
    gpio_list_len = list_len;
    export_semaphore = malloc(sizeof(sem_t));
    action_semaphore = malloc(sizeof(sem_t));
    unexport_semaphore = malloc(sizeof(sem_t));
    sem_init(export_semaphore, 0, 1);
    sem_init(action_semaphore, 0, 1);
    sem_init(unexport_semaphore, 0, 1);
    pthread_t thread_list[list_len];
    for (int i = 0; i < list_len; ++i) {
        pthread_create(&(thread_list[i]), NULL, execute_monitoring, (void *) &(gpio_list[i]));
    }
}

void destroy_monitoring() {
    thread_executing = -1;
    for (int i = 0; i < gpio_list_len; ++i) {
        gpio_unexport(gpio_list[i]);
    }
}

void execute_on_change(int gpio, int value) {
    sem_wait(action_semaphore);
    printf("The pin %d changed to value %d\n", gpio, value);
    sem_post(action_semaphore);
}