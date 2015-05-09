#ifndef XD_TYPES_H
#define XD_TYPES_H

// assert
#include <assert.h>
// bool
#include <stdbool.h>
// malloc
//#include "zmalloc.h"
// shareptr
#include <memory>

// 一些类型定义

// 数据类型定义
typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

typedef char                int8;
typedef short int           int16;
typedef int                 int32;
typedef long long           int64;

#ifndef NULL
#define NULL 0
#endif

// sock 相关
// poll id
typedef int32 XDPollFD;
// sock id
typedef int32 XDSockFD;
// sock handle
typedef int64 XDHandle;
// session id
typedef int64 XDSessionID;

// thread
typedef uint32 XDTID;

// 只能指针
// server socket ptr
class XDSocketConnection;
typedef std::shared_ptr<XDSocketConnection> XDSocketConnectionPtr;
// session ptr
class XDSession;
typedef std::shared_ptr<XDSession> XDSessionPtr;

// 在cpp 编译器下使用c编译
#ifdef __cplusplus
#define XD_BEGIN_DECLS extern "C" {
#define XD_END_DECLS }
#else
#define XD_BEGIN_DECLS
#define XD_END_DECLS
#endif

// 条件 判断
#define XD_STMT_START do
#define XD_STMT_END while(0)

#define XD_LOGDOMAIN ((char*)(0))
#define XD_EXTENSION  __extension__
#define XD_STRFUNC ((const char*)(__PRETTY_FUNCTION__))

#define XD_BOOLEAN(expr)                        \
    XD_EXTENSION({                              \
    int xd_boolean_var;                         \
    if (expr) {                                 \
        xd_boolean_var = 1;                     \
    } else {                                    \
        xd_boolean_var = 0;                     \
    }                                           \
    xd_boolean_var;                             \
})
#define XD_LIKELY(expr) (__builtin_expect(XD_BOOLEAN(expr),1))
#define XD_UNLIKELY(expr) (__builtin_expect(XD_BOOLEAN(expr),0))

#define XD_RETURN_IF_FAIL(expr)                 \
    XD_STMT_START {                             \
    if XD_LIKELY(expr) {                        \
    } else {                                    \
        XD_LOG_mwarn("assertion %s fail",       \
                                  #expr);       \
        return;                                 \
    }                                           \
    } XD_STMT_END

#define XD_RETURN_VAL_IF_FAIL(expr, val)        \
    XD_STMT_START {                             \
    if XD_LIKELY(expr) {                        \
    } else {                                    \
        XD_LOG_mwarn("assertion %s fail",       \
                                  #expr);       \
        return (val);                           \
    }                                           \
    } XD_STMT_END

// api
#define XD_API
// inline
#define FROCEINLINE __inline__ __attribute__((always_inline))
#define check(exp) assert(exp)
// lock and unlock
#define LOCK(q) while (__sync_lock_test_and_set(&(q)->lock,1)) {}
#define UNLOCK(q) __sync_lock_release(&(q)->lock);

// min
#define MIN(a, b) (a) < (b) ? (a) : (b)

#endif // end xd_types_h
