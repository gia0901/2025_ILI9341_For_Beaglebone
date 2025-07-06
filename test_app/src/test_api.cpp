#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>   // framebuffer
#include <errno.h>

#include "test_api.h"


void test_get_fb_info()
{
    int fb = open("/dev/fb0", O_RDWR);
    ASSERT_FD_VOID(fb);

    fb_var_screeninfo vinfo;
    fb_fix_screeninfo finfo;

    ioctl(fb, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);

    DEBUG_PRINT("Resolution: %dx%d\n", vinfo.xres, vinfo.yres);
    DEBUG_PRINT("Bits per pixel: %d\n", vinfo.bits_per_pixel);
    DEBUG_PRINT("Line length: %d\n", finfo.line_length);
}

int get_choice()
{
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

void menu()
{
    printf("--------------------------------------------\n");
    printf("                TEST APP                    \n");
    printf("--------------------------------------------\n");
    printf("1.  Get Framebuffer Info                    \n");

    printf("10. Exit                                    \n");
    printf("--------------------------------------------\n");
}
