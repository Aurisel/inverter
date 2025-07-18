#include "stdio.h"
#include "sys/stat.h"
#include "stdlib.h"
#include "errno.h"
#include "stdio.h"
#include "signal.h"
#include "time.h"
#include "sys/time.h"
#include "sys/times.h"

extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

extern "C"
{
    


    char *__env[1] = { 0 };
    char **environ = __env;

    void _init(void) {}

    int _getpid(void)
    {
        return 1;
    }

    int _kill(int pid, int sig)
    {
        (void)pid;
        (void)sig;
        errno = EINVAL;
        return -1;
    }

    void _exit (int status)
    {
        _kill(status, -1);
        while (1) {}
    }

    __attribute__((weak)) int _read(int file, char *ptr, int len)
    {
        (void)file;
        int DataIdx;

        for (DataIdx = 0; DataIdx < len; DataIdx++)
        {
            *ptr++ = __io_getchar();
        }

        return len;
    }

    __attribute__((weak)) int _write(int file, char *ptr, int len)
    {
        (void)file;
        int DataIdx;

        for (DataIdx = 0; DataIdx < len; DataIdx++)
        {
            __io_putchar(*ptr++);
        }
        return len;
    }

    int _close(int file)
    {
        (void)file;
        return -1;
    }


    int _fstat(int file, struct stat *st)
    {
        (void)file;
        st->st_mode = S_IFCHR;
        return 0;
    }

    int _isatty(int file)
    {
        (void)file;
        return 1;
    }

    int _lseek(int file, int ptr, int dir)
    {
        (void)file;
        (void)ptr;
        (void)dir;
        return 0;
    }

    int _open(char *path, int flags, ...)
    {
        (void)path;
        (void)flags;
        return -1;
    }

    int _wait(int *status)
    {
        (void)status;
        errno = ECHILD;
        return -1;
    }

    int _unlink(char *name)
    {
        (void)name;
        errno = ENOENT;
        return -1;
    }

    int _times(struct tms *buf)
    {
        (void)buf;
        return -1;
    }

    int _stat(char *file, struct stat *st)
    {
        (void)file;
        st->st_mode = S_IFCHR;
        return 0;
    }

    int _link(char *old, char * _new)
    {
        (void)old;
        (void)_new;
        errno = EMLINK;
        return -1;
    }

    int _fork(void)
    {
        errno = EAGAIN;
        return -1;
    }

    int _execve(char *name, char **argv, char **env)
    {
        (void)name;
        (void)argv;
        (void)env;
        errno = ENOMEM;
        return -1;
    }

    static uint8_t *__sbrk_heap_end = NULL;

    void *_sbrk(ptrdiff_t incr)
    {
        extern uint8_t _end;
        extern uint8_t _estack;
        extern uint32_t _Min_Stack_Size;
        const uint32_t stack_limit = (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
        const uint8_t *max_heap = (uint8_t *)stack_limit;
        uint8_t *prev_heap_end;

        if (NULL == __sbrk_heap_end)
        {
            __sbrk_heap_end = &_end;
        }
        if (__sbrk_heap_end + incr > max_heap)
        {
            errno = ENOMEM;
            return (void *)-1;
        }
        prev_heap_end = __sbrk_heap_end;
        __sbrk_heap_end += incr;

        return (void *)prev_heap_end;
    }
}