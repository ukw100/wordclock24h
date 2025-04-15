#define __end__ _end // fm: see *.ld
typedef char* caddr_t;
caddr_t _sbrk(int incr);
caddr_t _sbrk(int incr) {
  extern char __end__;      /* Defined by the linker */
  extern char __HeapLimit;  /* Defined by the linker */
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0) { heap_end = &__end__; }

  prev_heap_end = heap_end;

  if (heap_end + incr > &__HeapLimit) {
    return (caddr_t) -1; // signal error
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}
