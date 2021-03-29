//
// Created by kenneth on 14/3/21.
//

#ifndef IMAGETEST_IMAGEGET_H
#define IMAGETEST_IMAGEGET_H
static int xioctl(int fd, int request, void *arg);
int print_caps(int fd);
int init_mmap(int fd);
int capture_image(int fd, char *string);
int execute_image(char * filename);
#endif //IMAGETEST_IMAGEGET_H
