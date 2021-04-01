//
// Created by kenneth on 17/3/21.
//

#ifndef IMAGETEST_MANUALGPIO_H
#define IMAGETEST_MANUALGPIO_H
int stop;
/**
 * Sets the mode of the GPIO and initializes it 
 * @param pin the pin to be set
 * @param mode 0 for output and 1 for input
 **/
void pinMode(int pin, short mode);
/**
 * Writes the value, either 0 or one to the pin
 * @param pin pint to be written to
 *@param value value to be written
 **/
void digitalWrite(int pin,int value);
/**
 *Reads the pin value
 * @param pin the pin to be read 
 **/
int digitalRead(int pin);
/**
 * Turns off and on a pin with a given frequency and duration
 * @param pin The pin that is going to be tuirn on
 * @param frequency the frequency of the blink
 * @param duration The duration of the blink
 **/
void blink(short pin, double frequency,double duration);
/**
 * Unexports given pin
 * @param pin pin to unexport
 **/
int gpio_unexport(int pin);
#endif //IMAGETEST_MANUALGPIO_H
