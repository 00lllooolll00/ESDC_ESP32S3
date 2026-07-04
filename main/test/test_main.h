#ifndef TEST_MAIN_H
#define TEST_MAIN_H

// 统一测试入口：在 main.c 的 ek_export_init() 之后调用
// 注释 main.c 里的 test_run_all() 即可关闭所有测试
void test_run_all(void);

#endif // TEST_MAIN_H
