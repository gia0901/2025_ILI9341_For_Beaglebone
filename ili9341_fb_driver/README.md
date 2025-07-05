***ILI9341 DEVICE DRIVER***
- This is a SPI driver using framebuffer to display contents on the screen.

***FRAMEBUFFER DATA STRUCTURES***

- struct fb_var_screeninfo
    . Properties of video card: visible resolution, virtual resolution, offset from virtual to visible resolution, bits per pixel

- struct fb_fix_screeninfo
    . fixed properties of video card (by manufacturer or working in a specific mode)

- struct fb_cmap
    . specifies the color map

- struct fb_info
    . the framebuffer itself, contains all above data-structures