#ifndef EK_CONF_H
#define EK_CONF_H

#ifdef __cplusplus
extern "C"
{
#endif

// 平台/运行环境
// EKCFG_RTOS: 0=裸机（对应 bak EK_USE_RTOS=0）
#define EKCFG_RTOS (0)

// EKCFG_PICOLIBC: 0=不使用 ek_utils 内置 picolibc
// ESP-IDF 已自带 picolibc，此处必须显式设 0（ek_conf_internal.h 默认值为 1）
// 若为 1 会激活 ek_picolibc_port.c，与 ESP-IDF 运行时冲突
#define EKCFG_PICOLIBC (0)

// EKCFG_IO_LWPRTF: 0=不使用 lwprintf（对应 bak EK_IO_NO_LWPRTF=1，逻辑反转）
#define EKCFG_IO_LWPRTF (0)

// 核心服务开关
// EKCFG_EXPORT: 对应 bak EK_EXPORT_ENABLE=0
#define EKCFG_EXPORT (0)

// EKCFG_ASSERT: 必须为 1（bak EK_ASSERT_ENABLE=0）
// 新版 ek_assert.h 无 #else 回退，RINGBUF 编译需要 ek_assert_param 有定义
#define EKCFG_ASSERT (1)

// EKCFG_LOG: 对应 bak EK_LOG_ENABLE=0
#define EKCFG_LOG (0)

// 数据结构开关
// 对应 bak 各 EK_*_ENABLE 宏
#define EKCFG_STR          (0)
#define EKCFG_LIST         (0)
#define EKCFG_VEC          (0)
#define EKCFG_RINGBUF      (1)
#define EKCFG_RINGBUF_SPSC (1)
#define EKCFG_STACK        (0)
#define EKCFG_EVOKE        (0)

// 模块子配置
// EKCFG_HEAP_TLSF: 0=不使用 TLSF（对应 bak EK_HEAP_NO_TLSF=1，逻辑反转）
// 直接用宏将 ek_malloc/ek_free/ek_realloc 映射到 ESP-IDF 系统 malloc/free/realloc
#define EKCFG_HEAP_TLSF (0)

// EKCFG_HEAP_SIZE: 对应 bak EK_HEAP_SIZE=0（TLSF 已禁用，此值无实际作用）
#define EKCFG_HEAP_SIZE (0)

// 内存分配器映射（EKCFG_HEAP_TLSF=0 时生效）
// ek_heap.h 声明 ek_malloc/ek_free/ek_realloc 为函数，此处用宏替换为系统函数
// ek_heap.h 中的 ek_free_safely 宏会自动使用映射后的 free
#ifndef ek_malloc
#    define ek_malloc malloc
#endif
#ifndef ek_free
#    define ek_free free
#endif
#ifndef ek_realloc
#    define ek_realloc realloc
#endif

// 断言子配置（对应 bak EK_ASSERT_* 宏）
// EKCFG_ASSERT_TINY: 1=轻量级断言（失败时 while 死循环）
#define EKCFG_ASSERT_TINY (1)

// EKCFG_ASSERT_LOG: 0=断言时不输出日志（对应 bak EK_ASSERT_WITH_LOG=0）
#define EKCFG_ASSERT_LOG (0)

#ifdef __cplusplus
}
#endif

#endif // EK_CONF_H
