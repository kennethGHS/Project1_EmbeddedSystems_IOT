// #include "Monitoring.h"
#include "HTTPRequests.h"
int main(int argc, char const *argv[]) {
    init_semaphore();
    get_image_take();
    // get_update_lights();
    // upload_picture();
    // int gpio_in[] = {14,15,18,23};
    // int gpio_out[] = {2,3,4,17,27};
    // initialize_values(gpio_in,4,gpio_out,5);
    // get_update_lights();
    // struct stat st = {0};

    // if (stat("../Image", &st) == -1) {
    //   mkdir("../Image", 0777);
    // }
    // execute_image("../Image/picture.jpeg");
    // upload_picture();
    // update_pins_server();
}