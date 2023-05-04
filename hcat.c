#define EFNF "Error file not found\n"
#define ERR "Error\n"
#define BUF_SIZE 8192
#define O_RDONLY 00

typedef long int ssize_t;
typedef long int size_t;

inline ssize_t hread(int fd, void *buf, size_t count);
inline int hopen(const char *path, int flags);
unsigned long hstrlen(const char *str);
static _Noreturn void hexit(int code);
static inline ssize_t hwrite(int fd, const void *restrict buf, size_t count);
static inline int cat_impl(char* path);

__attribute__((naked)) void _start() {
    asm("xorl %ebp, %ebp;"
        "movq (%rsp), %rdi;"
        "leaq 8(%rsp), %rsi;"
        "call main;"
        "mov %eax, %edi;"
        "mov $60, %eax;"
        "syscall;"
    );
}

int main(int argc, char **argv) {
    return cat_impl(argv[1]);
}

static inline int cat_impl(char *path) {
    char buf[BUF_SIZE];

    int fd = hopen(path, O_RDONLY);

    if(fd < 0) {
        hwrite(1, EFNF, sizeof EFNF);

        hexit(1);
    }

    int len;

    while(1) {
        len = hread(fd, buf, BUF_SIZE);

        if(len == 0) {
            goto success;
        }

        if(hwrite(1, buf, len) < 0) {
            goto error;
        }
    };

    goto success;

error:
    hwrite(1, ERR, sizeof ERR);

    return 1;

success:
    return 0;
}

static _Noreturn void hexit(int code) {
    asm volatile("mov $60, %%eax;"
        "syscall;"
        :
        : "D" (code)
    );
    __builtin_unreachable();
}

static ssize_t hwrite(int fd, const void *restrict buf, size_t count) {
    int out;
    asm volatile("mov $1, %%eax;"
        "syscall;"
        : "=a"(out)
        : "D" (fd),
          "S" (buf),
          "d" (count)
        : "rcx", "r11", "memory"
    );

    return out;
}

int hopen(const char *restrict path, int flags) {
    int out;
    asm volatile("mov $2, %%eax;"
        "syscall;"
        : "=a"(out)
        : "D" (path),
          "S" (flags)
        : "rcx", "r11", "memory"
    );

    return out;
} 

ssize_t hread(int fd, void *restrict buf, size_t count) {
    int out;
    asm volatile("mov $0, %%eax;"
        "syscall;"
        : "=a"(out)
        : "D" (fd),
          "S" (buf),
          "d" (count)
        : "rcx", "r11", "memory"
    );

    return out;
}

unsigned long hstrlen(const char *str) {
    size_t len = 0;
    for(;*(str + len) != '\0';len++) {}
    return len;
}
