#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>

#include "test_api.h"


int main(int argc, char** argv)
{    
    int choice;
    std::string input_text;

    menu();

    while(1)
    {
        choice = get_choice();

        switch (choice) {
        case 1:
            test_get_fb_info();
            break;
        

        case 10:
            printf("Exiting....\n");
            exit(EXIT_SUCCESS);
        default:
            DEBUG_PRINT("Wrong choice. Please try again\n");
            break;
        }
    }

    return 0;
}
