/* ABI sketch only. The current GenOS kernel cannot load user-mode ELF yet. */
typedef long genos_syscall_result_t;
extern genos_syscall_result_t genos_sys_write(int fd, const char *buffer, unsigned long length);
int genos_app_main(void) {
    static const char message[] = "Hello from a future native GenOS app\n";
    return genos_sys_write(1, message, sizeof(message) - 1) < 0 ? 1 : 0;
}
