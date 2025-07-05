#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/of.h>
#include <linux/fb.h>

#include "ili9341_defines.h"
#include "ili9341_regs.h"

#define DRIVER_NAME     "ili9341_fb"
#define LCD_WIDTH       240
#define LCD_HEIGHT      320

/* Global variables */
static struct fb_info *ili9341_fb_info;    /* Framebuffer instance for the display */
static u8 *video_buffer;

/* Function prototypes */
static int ili9341_init_display(struct spi_device *spi);
static void ili9341_write_command(struct spi_device *spi, u8 cmd);
static void ili9341_write_data(struct spi_device *spi, const u8 *data, int len);

/* File operations */
static struct fb_ops ili9341_fbops = {
    .owner = THIS_MODULE,
    .fb_fillrect = sys_fillrect,
    .fb_copyarea = sys_copyarea,
    .fb_imageblit = sys_imageblit,
};

/**
 * @brief   Probe function for the driver
 * @note    Initialize the driver's necessary settings:
 * 
 */
static int ili9341_probe(struct spi_device *spi)
{
    int ret;

    /* Allocate the framebuffer */
    ili9341_fb_info = framebuffer_alloc(0, &spi->dev);
    if (!ili9341_fb_info)
        return -ENOMEM;
    
    video_buffer = vzalloc(LCD_WIDTH * LCD_HEIGHT * 2); /* RGB565 */
    if (!video_buffer)
        return -ENOMEM;
    
    ili9341_fb_info->screen_base = (char __iomem *)video_buffer;
    ili9341_fb_info->fbops = &ili9341_fbops;
    ili9341_fb_info->fix = (struct fb_fix_screeninfo) {
        .line_length = LCD_WIDTH * 2,
        .smem_len = LCD_WIDTH * LCD_HEIGHT * 2,
        .visual = FB_VISUAL_TRUECOLOR,
        .type = FB_TYPE_PACKED_PIXELS,
    };
    ili9341_fb_info->var = (struct fb_var_screeninfo) {
        .xres = LCD_WIDTH,
        .yres = LCD_HEIGHT,
        .bits_per_pixel = 16,
    };

    ret = register_framebuffer(ili9341_fb_info);
    if (ret < 0)
        return ret;

    /* Init display */
    ili9341_init_display(spi);

    dev_info(&spi->dev, "ILI9314 framebuffer driver initialized!");

    return 0;
}

/**
 * @brief   Clean the resources when removing the driver
 */
static int ili9341_remove(struct spi_device *spi)
{
    unregister_framebuffer(ili9341_fb_info);
    vfree(video_buffer);
    framebuffer_release(ili9341_info);
    return 0;
}


static struct of_device_id ili9341_dt_ids[] = {
    {.compatible = "gia,ili9341"},
    {},
};

/**
 * @brief   : spi driver instance
 */
static struct spi_driver ili9341_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9341_dt_ids,
    },
    .probe = ili9341_probe,
    .remove = ili9341_remove,
};

module_spi_driver(ili9341_driver);

MODULE_AUTHOR("gianguyen.hcmut@gmail.com");
MODULE_DESCRIPTION("ILI9341 SPI Framebuffer Driver");
MODULE_LICENSE("GPL");
