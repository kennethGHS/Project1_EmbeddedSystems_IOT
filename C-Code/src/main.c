#include <unistd.h>
#include <stdio.h>
#include "ManualGPIO.h"
#include "Monitoring.h"

int main(int argc, char const *argv[]) {
    int gpio_in[] = {14,15,18,23};
    int gpio_out[] = {2,3,4,17,27};
    initialize_values(gpio_in,4,gpio_out,5);
	return 0;
}
