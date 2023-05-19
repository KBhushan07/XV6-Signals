#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

void handler(int sig) {
    printf(1, "Signal %d received\n", sig);
}

void sigint_handler(int signum){
  printf(1, "SIGINT received\n");
}

void sigusr_handler(int signum){
  printf(1, "SIGUSR1 received\n");
}

void sigint_handler1(int signum){
  int a=2;
  printf(1, "SIGINT received  : %d\n" , getpid()+a);
}

void handle_sigsegv(int signum){
  printf(1,"Segmentation Fault\n");
  exit();
}

void checksigprocmask(){
    int pid = getpid();

    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    signal(SIGUSR1, handler);

    sigprocmask(SIG_BLOCK , SIGINT);
    sigprocmask(SIG_BLOCK , SIGTERM);
    sigprocmask(SIG_BLOCK , SIGUSR1);

    sigprocmask(SIG_UNBLOCK , SIGINT);
    sigprocmask(SIG_UNBLOCK , SIGTERM);
    sigprocmask(SIG_UNBLOCK , SIGUSR1);

    printf(1,"\nsignals should get executed\n");

    sigprocmask(SIG_BLOCK , SIGINT);
    sigprocmask(SIG_BLOCK , SIGTERM);
    sigprocmask(SIG_BLOCK , SIGUSR1);

    sigkill(pid,SIGINT);
    sigkill(pid,SIGTERM);
    sigkill(pid,SIGUSR1);

    sigprocmask(SIG_UNBLOCK , SIGINT);
    sigprocmask(SIG_UNBLOCK , SIGTERM);
    sigprocmask(SIG_UNBLOCK , SIGUSR1);

    printf(1,"\nsigprocmask executed succesfully\n");
}


void checksigstopsigcont(){
  signal(SIGSTOP, handler);
  signal(SIGCONT, handler);

  int pid = fork();

  if (pid == 0) {
    printf(1, "Child process (PID %d) starting...\n", getpid());
    printf(1, "Child process (PID %d) going to sleep for 5 seconds...\n", getpid());
    sleep(5);
    printf(1, "Child process (PID %d) woke up from sleep\n", getpid());
    exit();
  }
  else {
    printf(1, "Parent process (PID %d) starting...\n", getpid());
    printf(1, "Parent process (PID %d) sending SIGSTOP to child process (PID %d)...\n", getpid(), pid);
    sigkill(pid, SIGSTOP);
    printf(1, "Parent process (PID %d) sent SIGSTOP to child process (PID %d)\n", getpid(), pid);

    printf(1, "Parent process (PID %d) sending SIGCONT to child process (PID %d)...\n", getpid(), pid);
    sigkill(pid, SIGCONT);
    printf(1, "Parent process (PID %d) sent SIGCONT to child process (PID %d)\n", getpid(), pid);

    wait();
    printf(1, "Parent process (PID %d) exiting...\n", getpid());
  }

  printf(1 , "sigstop and sigcont -- ok\n");
}

void checkuserdefinedhandlers(){
  signal(SIGINT, sigint_handler);
  signal(SIGUSR1, sigusr_handler);
  printf(1, "Sending SIGUSR1 to process %d\n", getpid());
  sigkill(getpid(), SIGUSR1);
  printf(1, "Sending SIGINT to process %d\n", getpid());
  sigkill(getpid(), SIGINT);

  printf(1 , "user defined handlers -- ok\n");
}

void checksyscallinhandler(){
  signal(SIGINT, sigint_handler1);
  printf(1, "Sending SIGINT to process %d\n", getpid());
  sigkill(getpid(), SIGINT);
  signal(SIGTERM, sigint_handler);
  printf(1, "Sending SIGTERM to process %d\n", getpid());

  printf(1 , "system call in user defined handlers -- ok\n");
}

void check_sigign_dfl(){
  signal(SIGINT, SIG_IGN);
  printf(1, "Sending SIGINT to process %d\n", getpid());
  sigkill(getpid(), SIGINT);
  signal(SIGTERM, sigint_handler);
  printf(1, "Sending SIGTERM to process %d\n", getpid());
  signal(SIGTERM, SIG_DFL);
  sigkill(getpid(), SIGTERM);
  printf(1, "Process exiting...\n");

  printf(1 , "SIGIGN and SIGDFL -- ok\n");
}

void checksigpause(){
  int pid = fork();
  signal(SIGINT, sigint_handler1);

  if (pid == 0){
    printf(1, "Child process is running...\n");
    while(1){
      sigpause();
    }
  }
  else if (pid > 0){
    printf(1, "Pausing child process...\n");
    sigkill(pid, SIGINT);
    printf(1, "Resuming child process...\n");
    sigkill(pid, SIGKILL);
    printf(1, "Killing child process...\n");
    wait();
  }
  else{
    printf(1, "Failed to fork a child process!\n");
    exit();
  }

  printf(1 , "sigpause -- ok\n");
}

void checksigsegv(){
  int pid = fork();

  if(pid == 0){
    int *p = (int*)0xffffffff;
    *p = 1;
  }
  else{
    wait();
  }

  printf(1 , "sigsegv -- ok\n");
}

void checksigill(){
  int pid = fork();

  if(pid == 0){
    asm volatile("ud2");
  }
  else{
    wait();
  }
  printf(1 , "sigill -- ok\n");
}

int main() {
    checkuserdefinedhandlers();
    checksigprocmask();
    checksigpause();
    checksigsegv();
    checksigill();
    check_sigign_dfl();
    exit();
}
