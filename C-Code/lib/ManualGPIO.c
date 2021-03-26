//
// Created by kenneth on 17/3/21.
//

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "ManualGPIO.h"

void pinMode(int pin, short mode) {
    char pin_string[100];
    snprintf(pin_string, 10, "%d", pin);
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        close(fd);
        perror("Unable to open /sys/class/gpio/export");
        exit(1);
    }

    if (write(fd, pin_string, strlen(pin_string)) != strlen(pin_string)) {
        close(fd);
        perror("Error writing to /sys/class/gpio/export");
        exit(1);
    }
    close(fd);

    snprintf(pin_string, 100, "/sys/class/gpio/gpio%d/direction", pin);

    fd = open(pin_string, O_WRONLY);
    if (fd == -1) {
        perror("Unable to write in the pin");
        close(fd);
        exit(1);
    }
    if (mode == 1) {
        if (write(fd, "in", 2) != 2) {
            close(fd);
            perror("Error while writing in the pin");
            exit(1);
        }
    } else if (mode == 0) {
        if (write(fd, "out", 3) != 3) {
            close(fd);
            perror("Error while writing in the pin");
            exit(1);
        }
    } else {
        perror("Unable to identify mode, only accepted modes are 1 or 0");
        close(fd);
        exit(1);
    }
    close(fd);
}

void digitalWrite(int pin, short value) {
    char pin_string[100];
    int fd;
    snprintf(pin_string, 100, "/sys/class/gpio/gpio%d/value", pin);

    fd = open(pin_string, O_WRONLY);
    if (fd == -1) {
        close(fd);
        perror("Error while opening the pin value file, try enabling it first");
        exit(1);
    }
    if (value == 0) {
        if (write(fd, "0", 1) != 1) {
            close(fd);
            perror("Error while writing the pin value");
            exit(1);
        }
    } else if (value == 1) {
        if (write(fd, "1", 1) != 1) {
            close(fd);
            perror("Error while writing the pin value");
            exit(1);
        }
    }
    close(fd);

}

int digitalRead(int pin) {
    char pin_string[100];
    char result[3];
    int fd;
    snprintf(pin_string, 100, "/sys/class/gpio/gpio%d/value", pin);

    fd = open(pin_string, O_RDONLY);
    if (fd == -1) {
        close(fd);
        perror("Error while opening the pin value file, try enabling it first");
        exit(1);
    }
    if (read(fd, result, 3) <0) {
        close(fd);
        perror("Error while reading the pin value");
        exit(1);
    }
    close(fd);
    return atoi(result);
}

void blink(short pin, double frequency, double duration) {
    double time = 0;
    double period = 1;

    if (frequency == 0) {

        perror("Frequency cant be 0");
        exit(1);
    }
    period = 1 / frequency;

    while (time < duration) {
        digitalWrite(pin, 1);
        sleep(period / 2);
        digitalWrite(pin, 0);
        sleep(period / 2);
        time += period;
    }

}

int gpio_unexport(int pin)
{
    char buffer[64];
    int len;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open unexport for writing!\n");

        return -1;
    }

    len = snprintf(buffer, sizeof(buffer), "%d", pin);

    if (write(fd, buffer, len) < 0) {
        perror("Failed to unexport gpio!");
        return -1;
    }
    printf("Unexported pin %d \n",pin);
    close(fd);
    return 0;
}

