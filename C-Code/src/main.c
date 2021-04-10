#include <unistd.h>
#include <stdio.h>
// #include "ManualGPIO.h"
#include "Monitoring.h"

int main(int argc, char const *argv[]) {
    int gpio_in[] = {14,18,23,24};
    int gpio_out[] = {2,3,4,17,22};
    initialize_values(gpio_in,4,gpio_out,5);
	return 0;
}
