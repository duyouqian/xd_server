#ifndef XD_TIMER_H
#define XD_TIMER_H

#include "types.h"
#include <string>
#include <time.h>
#include <sys/time.h>

namespace XDTimerUtil {
// 获取本地时间
struct tm* getLocalTime(const time_t* timer, struct tm* result);
// 获取当前时间戳 s(秒)
uint64 now();
// 获取当前时间戳 s(秒)
uint64 getCurTimestampByS();
// 获取当前时间戳 ms(毫秒)
uint64 getCurTimestampByMS();
// 获取当前时间戳 us(微妙)
uint64 getCurTimestampByUS();
// 获取时间
int32 getTimeOfDay(struct ::timeval *tv, struct timezone *tz);
// 判断是否是同一天(秒)
bool isSameDayByTimestampWithS(uint64 fromSecond, uint64 toSecond);
// 安全休眠 s(秒)
void safeSleepByS(uint32 second);
// 安全休眠 ms(毫秒)
void safeSleepByMS(uint32 millisecond);
// 获取绝对时间
int32 getAbsTimespec(struct ::timespec *ts, uint32 millisecond);
// 时间差
void subtratTimeval(const struct timeval *from, struct timeval *sub, struct timeval *rs);
// 格式化时间
std::string getFormatTime(const char *fmt = NULL);
std::string getFormatTime(uint64 timestamp, const char *fmt = NULL);
std::string getFormatTime(const struct ::timeval *tv,const char *fmt = NULL);
}

#endif // end xd_timer_h
