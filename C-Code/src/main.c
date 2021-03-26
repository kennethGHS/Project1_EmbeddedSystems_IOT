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
    test_GPIO_input();
//	if (argc != 3){
//		printf("Usage: %s <output pin number> <input pin number>\n");
//		return 1;
//	}
//
//	int output = atoi(argv[1]);
//	int input = atoi(argv[2]);
//	int read = 0;
//
//	printf("Testing output on GPIO %d...\n", output);
//	pinMode(output, 0);
//	digitalWrite(output, 1);
//	sleep(1);
//	digitalWrite(output, 0);
//	sleep(1);
//	blink(output, 0.25, 10);
//	gpio_unexport(output);
//
//	printf("Testing input on GPIO %d...\n", input);
//	pinMode(input, 1);
//	for(int i = 0; i < 5; i++){
//		read = digitalRead(input);
//		printf("Value read on pin %d is %d\n", input, read);
//		sleep(1);
//	}
//	gpio_unexport(input);
	return 0;
}
