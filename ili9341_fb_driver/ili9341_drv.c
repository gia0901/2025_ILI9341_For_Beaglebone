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



static int ili9341_init_display(struct spi_device *spi)
{
    u8 buf[2];

    // 1. Software reset (0x01)
    buf[0] = 0x01;  // Cmd: reset
    spi_write(spi, buf, 1);
    mdelay(150);

    // 2. Sleep out (0x11)
    buf[0] = 0x11;
    spi_write(spi, buf, 1);

    // 3. Display ON (0x29)
    buf[0] = 0x29;
    spi_write(spi, buf, 1);

    // 4. Configure resolution & color format
    buf[0] = 0x3A;  // Cmd: Set pixel format
    buf[1] = 0x55;  // Data; RGB565
    spi_write(spi, buf, 2);

    // 5. Configure Address window
    buf[0] = 0x2A;  // Cmd: Set column address
    buf[1] = 0x00;
    spi_write(spi, buf, 2); // Set start column
    buf[1] = 0xEF;
    spi_write(spi, buf, 2); // Set end column

    buf[0] = 0x2B;
    buf[1] = 0x00;
    spi_write(spi, buf, 2); // Set start row
    buf[1] = 0xFF;
    spi_write(spi, buf, 2); // Set end row

    // 6. Others

    return 0;
}


/**
 * @brief   Probe function for the driver
 * @note    Initialize the driver's necessary settings:
 * 
 */
static int ili9341_probe(struct spi_device *spi)
{
    int ret;

    /* Allocate the framebuffer dynamically */
    ili9341_fb_info = framebuffer_alloc(0, &spi->dev);
    if (!ili9341_fb_info)
        return -ENOMEM;
    
    /* Allocate video buffer */
    video_buffer = vzalloc(LCD_WIDTH * LCD_HEIGHT * 2); /* RGB565 = 16 bits = 2 bytes */
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
    framebuffer_release(ili9341_fb_info);
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
