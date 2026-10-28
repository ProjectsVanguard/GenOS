/* Host-testable calculator core; not yet a kernel-loadable application. */
int genos_calc(int a, int b, char op, int *out) {
    if (!out) return -1;
    switch (op) {
        case '+': *out = a + b; return 0;
        case '-': *out = a - b; return 0;
        case '*': *out = a * b; return 0;
        case '/': if (!b) return -1; *out = a / b; return 0;
        default: return -1;
    }
}
