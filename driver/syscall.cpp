#include "stdio.h"
extern "C"
{
    int _write(int file, char *data, int len) { (void)file; return len; }
    int _close(int file) { (void)file; return -1; }
    int _getpid(void) { return -1; }
    int _kill(int pid, int sig) { (void)pid; (void)sig; return -1; }
    int _lseek(int file, int ptr, int dir) { (void)file; (void)ptr; (void)dir; return -1; }
    int _read(int file, char *ptr, int len) { (void)file; (void)ptr; (void)len; return -1; }
    void _exit(int status) { (void)status; while (true); }
    void* _sbrk(ptrdiff_t increment) { (void)increment; return (void *)-1; };
    int _isatty(int fd) { if (fd == 1 || fd == 2) return 1; return -1; }
    int _fstat(int fd, struct stat *st) { (void)fd; (void*)st; return -1; }
}