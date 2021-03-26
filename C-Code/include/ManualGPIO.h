//
// Created by kenneth on 17/3/21.
//

#ifndef IMAGETEST_MANUALGPIO_H
#define IMAGETEST_MANUALGPIO_H
int stop;
void pinMode(int pin, short mode);
void digitalWrite(int pin,short value);
int digitalRead(int pin);
void blink(short pin, double frequency,double duration);
int gpio_unexport(int pin);
#endif //IMAGETEST_MANUALGPIO_H
