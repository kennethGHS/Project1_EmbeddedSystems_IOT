#include <unistd.h>
#include <stdio.h>
#include "ManualGPIO.h"
#include "Monitoring.h"
int test_GPIO_input(){
    int array_gpio[] = {23,24};
    initialize_values(array_gpio,2);
    char a_word[100];
    printf ("Press something then enter to finish ");
    scanf ("%s", a_word);
    printf ("finishing");
    destroy_monitoring();
}
int main(int argc, char const *argv[]) {
    int gpio_in[] = {14,15,18,23};
    int gpio_out[] = {2,3,4,17,27};
    initialize_values(gpio_in,4,gpio_out,5);
	return 0;
}
