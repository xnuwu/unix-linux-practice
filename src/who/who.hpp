#ifndef WHO
#define WHO

#include <utmp.h>

#define SHOWHOST
#define UTNUM 16
#define NULLUT ((struct utmp*) NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmp_buffer[UTNUM * UTSIZE];
static int utmp_fd;
static int utmp_total_buffer_num;
static int utmp_current_index;

//显示单条utmp记录数据
void showInfo(const utmp& utmpR);

//格式化显示时间
void showTime(long timeval);

//打开utmp文件
int utmpOpen(const char* utmpPath);

//获取下一条utmp数据并自动批量加载
struct utmp* utmpNext();

//批量加载utmp数据，返回加载的成功数量
int utmpLoad();

//关闭utmp文件并清理缓存
void utmpClose();

#endif