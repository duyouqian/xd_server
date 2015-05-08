#include "log.h"


#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __unix__
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#endif /* __unix__ */

#ifdef __APPLE__
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#endif /* __APPLE__ */

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

/************************************* log item queue *************************************/

typedef struct log_queue_item LQ_ITEM;
struct log_queue_item {
    int level;
    time_t tm;
    char *locate;
    char *context;
#ifdef __unix__
    pthread_t thread;
#endif /* __unix */
#ifdef __APPLE__
    int thread;
#endif /* __APPLE__ */
#ifdef _WIN32
    DWORD thread;
#endif /* _WIN32 */
    size_t sz;
    LQ_ITEM *next;
};

typedef struct log_queue LQ;
struct log_queue {
    LQ_ITEM *head;
    LQ_ITEM *tail;
#ifdef __unix__
    pthread_mutex_t lock;
#endif /* __unix__ */
#ifdef __APPLE__
    pthread_mutex_t lock;
#endif /* __APPLE__ */
#ifdef _WIN32
    CRITICAL_SECTION crit;
#endif /* _WIN32 */
};

static LQ slq;

static void lq_init(LQ *lq) {
#ifdef __unix__
    pthread_mutex_init(&lq->lock, NULL);
#endif /* __unix__ */
#ifdef __APPLE__
    pthread_mutex_init(&lq->lock, NULL);
#endif /* __APPLE__ */
#ifdef _WIN32
    InitializeCriticalSection(&lq->crit);
#endif /* _WIN32 */
    lq->head = NULL;
    lq->tail = NULL;
}

static void lq_push(LQ *lq, LQ_ITEM *item) {
    item->next = NULL;
    
#ifdef __unix__
    pthread_mutex_lock(&lq->lock);
#endif /* __unix__ */
#ifdef __APPLE__
    pthread_mutex_lock(&lq->lock);
#endif /* __APPLE__ */
#ifdef _WIN32
    EnterCriticalSection(&lq->crit);
#endif /* _WIN32 */
    
    
    if (NULL == lq->tail)
        lq->head = item;
    else
        lq->tail->next = item;
    lq->tail = item;
    
#ifdef __unix__
    pthread_mutex_unlock(&lq->lock);
#endif /* __unix__ */
#ifdef __APPLE__
    pthread_mutex_unlock(&lq->lock);
#endif /* __APPLE__ */
#ifdef _WIN32
    LeaveCriticalSection(&lq->crit);
#endif /* _WIN32 */
}

static LQ_ITEM *lq_pop(LQ *lq) {
    LQ_ITEM *item;
    
#ifdef __unix__
    pthread_mutex_lock(&lq->lock);
#endif /* __unix__ */
#ifdef __APPLE__
    pthread_mutex_lock(&lq->lock);
#endif /* __APPLE */
#ifdef _WIN32
    EnterCriticalSection(&lq->crit);
#endif /* _WIN32 */
    
    item = lq->head;
    if (NULL != item) {
        lq->head = item->next;
        if (NULL == lq->head)
            lq->tail = NULL;
    }
    
#ifdef __unix__
    pthread_mutex_unlock(&lq->lock);
#endif /* __unix__ */
#ifdef __APPLE__
    pthread_mutex_unlock(&lq->lock);
#endif /* __APPLE__ */
#ifdef _WIN32
    LeaveCriticalSection(&lq->crit);
#endif /* _WIN32 */
    
    return item;
}

/************************************* log impl *************************************/

log_t glog;

static int create_file(FILE **f, const char* path, time_t t)
{
    if (NULL != *f) {
        fclose(*f);
        *f = NULL;
    }
    
    char buf[1024];
    struct tm *ptm = localtime(&t);
    char timestamp[128];
    strftime(timestamp, 128, "_%Y%m%d.log", ptm);
#ifdef __unix__
    snprintf(buf, 1024, "%s%s", path, timestamp);
#endif /* __unix__ */
#ifdef __APPLE__
    snprintf(buf, 1024, "%s%s", path, timestamp);
#endif /* __APPLE__ */
#ifdef _WIN32
    sprintf_s(buf, 1024, "%s%s",path, timestamp);
#endif /* _WIN32 */
    if (NULL == (*f = fopen(buf, "at+"))) {
        fprintf(stderr, "fopen path:%s failed!\n", buf);
        return -1;
    }
    
    return 0;
}

static const char* const_basename(const char* filepath) {
    const char* base = strrchr(filepath, '/');
#ifdef _WIN32  // Look for either path separator in Windows
    if (!base)
        base = strrchr(filepath, '\\');
#endif
    return base ? (base+1) : filepath;
}

static int is_same_day(time_t t1, time_t t2)
{
    struct tm *ptm = localtime(&t1);
    int year1 = ptm->tm_year;
    int mon1 = ptm->tm_mon;
    int day1 = ptm->tm_mday;
    ptm = localtime(&t2);
    int year2 = ptm->tm_year;
    int mon2 = ptm->tm_mon;
    int day2 = ptm->tm_mday;
    if (year1 == year2 && mon1 == mon2 && day1 == day2)
        return 1;
    return 0;
}

#ifdef __unix__

static pthread_t thread;

static void exec_log_item(log_t *l, LQ_ITEM *item)
{
    time_t curr_time = time(NULL);
    if (0 == is_same_day(l->last_touch, curr_time)) {
        if (0 == create_file(&l->f, l->pathname, curr_time)) {
            l->last_touch = curr_time;
        }
    }
    
    const char *clr = "\033[0m";
    
    /* color clear */
    const char *lv_color = NULL;
    if (item->level == LOG_LEVEL_FATAL)
        lv_color = "\033[35m";
    else if (item->level == LOG_LEVEL_ERROR)
        lv_color = "\033[31m";
    else if (item->level == LOG_LEVEL_WARN)
        lv_color = "\033[36m";
    else if (item->level == LOG_LEVEL_INFO)
        lv_color = "\033[33m";
    else if (item->level == LOG_LEVEL_DEBUG)
        lv_color = "\033[34m";
    
    /* time */
    char timestamp[32];
    struct tm *ptm = localtime(&(item->tm));
    strftime(timestamp, 32, "%H:%M:%S ", ptm);
    
    /* level text */
    const char *lv_text = NULL;
    if (item->level == LOG_LEVEL_FATAL)
        lv_text = "[fatal] ";
    else if (item->level == LOG_LEVEL_ERROR)
        lv_text = "[error] ";
    else if (item->level == LOG_LEVEL_WARN)
        lv_text = "[warn ] ";
    else if (item->level == LOG_LEVEL_INFO)
        lv_text = "[info ] ";
    else if (item->level == LOG_LEVEL_DEBUG)
        lv_text = "[debug] ";
    
    /* thread */
    char thread_info[32];
    snprintf(thread_info, 32, "[tid=%lu] ", item->thread);
    
    if (l->ctrl_stdout) {
        if (l->ctrl_locate) {
            char buf[1024];
            snprintf(buf, 1024, "%s%s%s%s%s%s%s%s\n",
                     clr, lv_color, timestamp, lv_text, thread_info, const_basename(item->locate), item->context, clr);
            printf("%s", buf);
        } else {
            char buf[1024];
            snprintf(buf, 1024, "%s%s%s%s%s%s%s\n",
                     clr, lv_color, timestamp, lv_text, thread_info, item->context, clr);
            printf("%s", buf);
        }
    }
    
    if (l->ctrl_locate) {
        char buf[1024];
        snprintf(buf, 1024, "%s%s%s%s%s\n",
                 timestamp, lv_text, thread_info, const_basename(item->locate), item->context);
        fwrite(buf, strlen(buf), 1, l->f);
        fflush(l->f);
    } else {
        char buf[1024];
        snprintf(buf, 1024, "%s%s%s%s\n",
                 timestamp, lv_text, thread_info, item->context);
        fwrite(buf, strlen(buf), 1, l->f);
        fflush(l->f);
    }
    
    /* free */
    if (item->locate)
        free(item->locate);
    if (item->context)
        free(item->context);
    free(item);
}

static void *log_thread_func(void *arg)
{
    log_t* l = (log_t *)arg;
    
    while (l)
    {
        LQ_ITEM *item = lq_pop(&slq);
        if (NULL == item) {
            if (l->run)
                sleep(1);
            else
                break;
        } else {
            exec_log_item(l, item);
        }
    }
    
    return NULL;
}

#endif /* __unix__ */

#ifdef __APPLE__


static pthread_t thread;

static void exec_log_item(log_t *l, LQ_ITEM *item)
{
    time_t curr_time = time(NULL);
    if (0 == is_same_day(l->last_touch, curr_time)) {
        if (0 == create_file(&l->f, l->pathname, curr_time)) {
            l->last_touch = curr_time;
        }
    }
    
    const char *clr = "\033[0m";
    
    /* color clear */
    const char *lv_color = NULL;
    if (item->level == LOG_LEVEL_FATAL)
        lv_color = "\033[35m";
    else if (item->level == LOG_LEVEL_ERROR)
        lv_color = "\033[31m";
    else if (item->level == LOG_LEVEL_WARN)
        lv_color = "\033[36m";
    else if (item->level == LOG_LEVEL_INFO)
        lv_color = "\033[33m";
    else if (item->level == LOG_LEVEL_DEBUG)
        lv_color = "\033[34m";
    
    /* time */
    char timestamp[32];
    struct tm *ptm = localtime(&(item->tm));
    strftime(timestamp, 32, "%H:%M:%S ", ptm);
    
    /* level text */
    const char *lv_text = NULL;
    if (item->level == LOG_LEVEL_FATAL)
        lv_text = "[fatal] ";
    else if (item->level == LOG_LEVEL_ERROR)
        lv_text = "[error] ";
    else if (item->level == LOG_LEVEL_WARN)
        lv_text = "[warn ] ";
    else if (item->level == LOG_LEVEL_INFO)
        lv_text = "[info ] ";
    else if (item->level == LOG_LEVEL_DEBUG)
        lv_text = "[debug] ";
    
    /* thread */
    char thread_info[32];
    snprintf(thread_info, 32, "[tid=%d] ", item->thread);
    
    if (l->ctrl_stdout) {
        if (l->ctrl_locate) {
            char buf[1024];
            snprintf(buf, 1024, "%s%s%s%s%s%s%s%s\n",
                     clr, lv_color, timestamp, lv_text, thread_info, const_basename(item->locate), item->context, clr);
            printf("%s", buf);
        } else {
            char buf[1024];
            snprintf(buf, 1024, "%s%s%s%s%s%s%s\n",
                     clr, lv_color, timestamp, lv_text, thread_info, item->context, clr);
            printf("%s", buf);
        }
    }
    
    if (l->ctrl_locate) {
        char buf[1024];
        snprintf(buf, 1024, "%s%s%s%s%s\n",
                 timestamp, lv_text, thread_info, const_basename(item->locate), item->context);
        fwrite(buf, strlen(buf), 1, l->f);
        fflush(l->f);
    } else {
        char buf[1024];
        snprintf(buf, 1024, "%s%s%s%s\n",
                 timestamp, lv_text, thread_info, item->context);
        fwrite(buf, strlen(buf), 1, l->f);
        fflush(l->f);
    }
    
    /* free */
    if (item->locate)
        free(item->locate);
    if (item->context)
        free(item->context);
    free(item);
}

static void *log_thread_func(void *arg)
{
    log_t* l = (log_t *)arg;
    
    while (l)
    {
        LQ_ITEM *item = lq_pop(&slq);
        if (NULL == item) {
            if (l->run)
                sleep(1);
            else
                break;
        } else {
            exec_log_item(l, item);
        }
    }
    
    return NULL;
}

#endif /* __APPLE__ */

#ifdef _WIN32

static HANDLE thread;

static void exec_log_item(log_t *l, LQ_ITEM *item)
{
    time_t curr_time = time(NULL);
    if (0 == is_same_day(l->last_touch, curr_time)) {
        if (0 == create_file(&l->f, l->pathname, curr_time)) {
            l->last_touch = curr_time;
        }
    }
    
    /* color clear */
    if (item->level == LOG_LEVEL_FATAL)
        SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else if (item->level == LOG_LEVEL_ERROR)
        SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_RED | FOREGROUND_INTENSITY);
    else if (item->level == LOG_LEVEL_WARN)
        SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else if (item->level == LOG_LEVEL_INFO)
        SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else if (item->level == LOG_LEVEL_DEBUG)
        SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else
        SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),
                                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    
    /* time */
    char timestamp[32];
    struct tm *ptm = localtime(&(item->tm));
    strftime(timestamp, 32, "%H:%M:%S ", ptm);
    
    /* level text */
    const char *lv_text = NULL;
    if (item->level == LOG_LEVEL_FATAL)
        lv_text = "[fatal] ";
    else if (item->level == LOG_LEVEL_ERROR)
        lv_text = "[error] ";
    else if (item->level == LOG_LEVEL_WARN)
        lv_text = "[warn ] ";
    else if (item->level == LOG_LEVEL_INFO)
        lv_text = "[info ] ";
    else if (item->level == LOG_LEVEL_DEBUG)
        lv_text = "[debug] ";
    
    /* thread */
    char thread_info[32];
    sprintf_s(thread_info, 32, "[tid=%lu] ", item->thread);
    
    if (l->ctrl_locate) {
        char buf[1024];
        sprintf_s(buf, 1024, "%s%s%s%s%s\n",
                  timestamp, lv_text, thread_info, const_basename(item->locate), item->context);
        printf(buf);
        fwrite(buf, strlen(buf), 1, l->f);
        fflush(l->f);
    } else {
        char buf[1024];
        sprintf_s(buf, 1024, "%s%s%s%s\n",
                  timestamp, lv_text, thread_info, item->context);
        printf(buf);
        fwrite(buf, strlen(buf), 1, l->f);
        fflush(l->f);
    }
    SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    
    /* free */
    if (item->locate)
        free(item->locate);
    if (item->context)
        free(item->context);
    free(item);
}

static DWORD WINAPI log_thread_func(LPVOID arg)
{
    log_t* l = (log_t *)arg;
    
    while (l)
    {
        LQ_ITEM *item = lq_pop(&slq);
        if (NULL == item) {
            if (l->run) {
                Sleep(1);
            }
            else
                break;
        } else {
            exec_log_item(l, item);
        }
    }
    
    return 0;
}

#endif /* _WIN32 */

int log_open(log_t *l, const char *path, int lv, unsigned int ctrl)
{
    if (strlen(path) >= 512) {
        fprintf(stderr, "path too long!\n");
        return -1;
    }
    
    lq_init(&slq);
    
    strncpy(l->pathname, path, 512);
    l->f = NULL;
    l->level = lv;
    l->ctrl_stdout = ctrl & LOG_CTRL_STDOUT ? 1 : 0;
    l->ctrl_locate = ctrl & LOG_CTRL_LOCATE ? 1 : 0;
    time_t t = time(NULL);
    l->last_touch = t;
    if (0 != create_file(&l->f, l->pathname, t)) {
        l->run = 0;
        return -1;
    }
    
    l->run = 1;
    
    /* create thread for writing asynchronously */
#ifdef __unix__
    int ret = pthread_create(&thread, NULL, log_thread_func, l);
    if (0 != ret) {
        fprintf(stderr, "can't create log thread:%s!\n", strerror(ret));
        return -1;
    }
#endif /* __unix__ */
#ifdef __APPLE__
    int ret = pthread_create(&thread, NULL, log_thread_func, l);
    if (0 != ret) {
        fprintf(stderr, "can't create log thread:%s!\n", strerror(ret));
        return -1;
    }
#endif /* __APPLE__ */
#ifdef _WIN32
    DWORD thread_id;
    thread = ::CreateThread(NULL, NULL, log_thread_func, l, 0, &thread_id);
#endif /* _WIN32 */
    
    return 0;
}

void log_close(log_t *l)
{
    l->run = 0;
#ifdef __unix__
    pthread_join(thread, NULL);
#endif /* __unix__ */
#ifdef __APPLE__
    pthread_join(thread, NULL);
#endif /* __APPLE__ */
#ifdef _WIN32
    WaitForSingleObject(thread, INFINITE);
#endif /* _WIN32 */
}

void log_write(log_t *l, int lv, const char *file, int line, const char *func, const char *format, ...)
{
    if (0 == l->run)
        return;
    
    if (lv < l->level)
        return;
    
    LQ_ITEM *item = (LQ_ITEM *)malloc(sizeof(LQ_ITEM));
    if (NULL == item) {
        fprintf(stderr, "LQ_ITEM alloc failed!\n");
        return;
    }
    
    item->level = lv;
    item->tm = time(NULL);
    if (l->ctrl_locate) {
        item->locate = (char *)malloc(256);
        if (NULL == item->locate) {
            fprintf(stderr, "item->locate alloc failed!\n");
            free(item);
            return;
        } else {
#ifdef __unix__
            snprintf(item->locate, 256, "%s:%d:%s ", file, line, func);
#endif /* __unix__ */
#ifdef __APPLE__
            snprintf(item->locate, 256, "%s:%d:%s ", file, line, func);
#endif /* __APPLE__ */
#ifdef _WIN32
            sprintf_s(item->locate, 256, "%s:%d:%s ", file, line, func);
#endif /* _WIN32 */
        }
    } else {
        item->locate = NULL;
    }
#ifdef __unix__
    item->thread = syscall(SYS_gettid);
#endif /* __unix__ */
#if defined(__APPLE__)
    item->thread = syscall(SYS_gettid);
#endif /* __APPLE__ */
#ifdef _WIN32
    item->thread = GetCurrentThreadId();
#endif /* _WIN32 */
    item->context = (char *)malloc(1024);
    if (NULL == item->context) {
        fprintf(stderr, "item->context alloc failed!\n");
        return;
    } else {
        va_list va;
        va_start(va, format);
        vsnprintf(item->context, 1024, format, va);
        va_end(va);
    }
    
    lq_push(&slq, item);
}

