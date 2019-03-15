#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int main() {
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

  while(1) {
    char* file[105];
struct dirent *de;
DIR *dr = opendir(".");
char* temp;
if (dr == NULL) {
	return 0;
}
int i=0;
while ((de = readdir(dr)) != NULL){
	char nama[105];
	memset (nama, '\0', sizeof(nama));
	file[i] = (de->d_name);
	int len = strlen(file[i]);
	char a = file[i][len -1];
	char b = file[i][len -2];
	char c = file[i][len -3];
	char d = file[i][len -4];
	for(int j =0; j<len - 4; j++){
		nama[j] = file[i][j];
	}
	strcat(nama, "_grey.png");
	i++;
	
	if(a=='g' && b=='n' && c=='p' && d=='.'){
		int nilai = rename(de->d_name, nama);
	if(!nilai) {
			printf("%s\n", "Sukses");
		}
		else{
			perror("Error");
		}
		printf("%s\n", nama);
		char *argv[4] = {"cp", nama, "/home/sherly/modul2/gambar", NULL};
		execv("/bin/cp", argv);
	
}
}
    sleep(30);
  }
  
  exit(EXIT_SUCCESS);
}
