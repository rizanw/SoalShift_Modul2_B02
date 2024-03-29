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
#include <pwd.h>
#include <grp.h>
#include <signal.h>

void crDaemon();

int main() {
  char elenku[100];
  char *uname;
  uname = getlogin();
  sprintf(elenku, "/home/%s/hatiku/elen.ku", uname);

  crDaemon();

  while(1) {
    struct stat kenangan;
  	stat(elenku, &kenangan);
  	struct passwd *euid = getpwuid(kenangan.st_uid);
  	struct group *egid = getgrgid(kenangan.st_gid);

  	if(strcmp(euid->pw_name, "www-data")==0 && strcmp(egid->gr_name, "www-data")==0){
  		remove(elenku);
  	}
  	sleep(3);
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
