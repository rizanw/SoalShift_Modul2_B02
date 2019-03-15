#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <signal.h>

void crDaemon();

int main() {
  pid_t child;
  int status;
  int minute = 0;
  char dtime[20];
  char dir[100];
  char *uname;
  uname = getlogin();
  sprintf(dir, "/home/%s/log", uname);

  crDaemon();

  while(1) {
    if(minute%30 == 0){
      time_t now = time(NULL);
      struct tm *t = localtime(&now);
      strftime(dtime, sizeof(dtime)-1, "%Y:%m:%d-%H:%M", t);
    }

    child = fork();
    if(child == 0){
      char cmd[200];
      sprintf(cmd, "%s/%s", dir, dtime);
      char *arg[4] = {"mkdir", "-p" ,cmd, NULL};
      execv("/bin/mkdir", arg);
    }
    while ((wait(&status)) > 0);
    kill(child, SIGKILL);

    minute+=1;

    child = fork();
    if(child == 0){
      char sr[] = "/var/log/syslog";
      char tg[200];
      sprintf(tg, "%s/%s/log%d.log", dir, dtime, minute);
      char *argv[4] = {"cp", sr, tg, NULL};
      execv("/bin/cp", argv);
    }
    while ((wait(&status)) > 0);
    kill(child, SIGKILL);
    sleep(60);
  }

  exit(EXIT_SUCCESS);
}

void crDaemon(){
    pid_t pid, sid;

    pid = fork();

    if (pid < 0) {
      exit(EXIT_FAILURE);
    }

    if (pid > 0) {
      exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if (sid < 0) {
      exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
      exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
