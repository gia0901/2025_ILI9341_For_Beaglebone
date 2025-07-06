#ifndef _TEST_API_H_
#define _TEST_API_H_


#define DEBUG_PRINT(fmt,...) printf("[%s]: " fmt, __func__, ##__VA_ARGS__)

#define ASSERT_FD_VOID(fd)      do { \
                                    if (fd < 0) { \
                                        DEBUG_PRINT("error[%d][%s]\n", errno, strerror(errno)); \
                                        return; \
                                    } \
                                } while(0)

int get_choice();

void menu();

void test_get_fb_info();

void test_draw_color();

void test_draw_pattern();

void test_draw_test();

void test_animation();



#endif // _TEST_API_H_