#include "syscall.h"

.globl trampoline
trampoline:
  movl $SYS_sigret , %eax
  int $64
  ret
