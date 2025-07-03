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


/**
 * @brief   Probe function for the driver
 * @note    Initialize the driver's necessary settings:
 * 
 */
static int ili9341_probe(struct spi_device *spi)
{

    return 0;
}

static int ili9341_remove(struct spi_device *spi)
{

    return 0;
}

static struct of_device_id ili9341_dt_ids[] = {
    {.compatible = "ilitek,ili9341"},
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
