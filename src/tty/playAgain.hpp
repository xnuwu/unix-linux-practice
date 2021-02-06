#ifndef PLAY_AGAIN
#define PLAY_AGAIN

#include <cstdio>

#define QUESTION "Do you want play another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP std::putchar('\a')

/**
 * 终端属性配置,0-还原,1-保存
 **/
void ttyMode(int flag);

/**
 * 设置中断非标准、不回显
 **/
void set_cr_noecho_mode();

/**
 * 设置文件非延迟模式
 **/
void set_no_delay_mode();

/**
 * 获取用户中断输入响应是否继续,0-停止,1-继续
 **/
int getResponse();

/**
 * 获取终端输入,直到获取正确的值,并返回.非法字符字节丢弃
 **/
int getOKChar();

/**
 * 中断信号处理
 **/
void ctrlCHandler(int sig);

#endif