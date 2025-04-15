#include <sys/types.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/signal.h>

extern _READ_WRITE_RETURN_TYPE  _read(int __fd, const void *__buf, size_t __nbyte) __attribute__((weak));
extern _READ_WRITE_RETURN_TYPE  _write (int __fd, const void *__buf, size_t __nbyte) __attribute__((weak));
extern int                      _close(int file) __attribute__((weak));
extern int                      _fstat(int file, struct stat *st) __attribute__((weak));
extern int                      _isatty(int file) __attribute__((weak));
extern off_t                    _lseek (int __fildes, off_t __offset, int __whence) __attribute__((weak));
extern void                     _exit (int __status)  _ATTRIBUTE ((__noreturn__)) __attribute__((weak));
extern int                      _kill (pid_t, int) __attribute__((weak));
extern pid_t                    _getpid (void) __attribute__((weak));

_READ_WRITE_RETURN_TYPE
_read (int __fd, const void *__buf, size_t __nbyte)
{
    return -1;
}

_READ_WRITE_RETURN_TYPE
_write (int __fd, const void *__buf, size_t __nbyte)
{
    return -1;
}

int
_close(int file)
{
    return -1;
}

int
_fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int
_isatty(int file)
{
    return 1;
}

off_t
_lseek (int __fildes, off_t __offset, int __whence)
{
    return 0;
}

void
_exit (int __status)
{
    while (1)
    {
    }
}

int
_kill (pid_t p, int s)
{
    return -1;
}

pid_t
_getpid (void)
{
    return (pid_t) -1;
}

