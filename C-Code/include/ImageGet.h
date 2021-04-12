//
// Created by kenneth on 14/3/21.
//

#ifndef IMAGETEST_IMAGEGET_H
#define IMAGETEST_IMAGEGET_H
/**
 * Allocates memory for the camera, the file descriptor
 * is the file representation of the camera
 **/
static int xioctl(int fd, int request, void *arg);
/**
 * Prints the characteristics of the camera **/
int print_caps(int fd);
/** Inits the mapping of the camera characteristics needed by the kernel
 * to access it **/
int init_mmap(int fd);
/**
 * Takes a photo using the camera, uses the file descriptor of that camera
 * and saves the  picture to the string that is the path **/
int capture_image(int fd, char *string);
/**
 * Executes all the methods to create an JPEG file named Filename **/
int execute_image(char * filename);

int first_execution;//First time executing
int file_webcam; // file descriptor of the camera

#endif //IMAGETEST_IMAGEGET_H
