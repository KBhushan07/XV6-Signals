#define NSIGS 20


#define SIGABRT 2
#define SIGALRM 3
#define SIGCHLD 4
#define SIGCONT 5
#define SIGFPE  6
#define SIGINT  7
#define SIGKILL 8
#define SIGQUIT 9
#define SIGSEGV 10
#define SIGSTOP 11
#define SIGTERM 12
#define SIGUSR1 13
#define SIGILL  14
#define SIGVTALRM 15

typedef void (*sighandler_t)(int);

#define SIG_DFL ((void (*)(int))0)
#define SIG_IGN ((void (*)(int))1)

#define SIG_BLOCK 100
#define SIG_UNBLOCK 101
#define SIG_SETMASK 102

