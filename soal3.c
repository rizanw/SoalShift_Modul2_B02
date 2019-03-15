#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
	pid_t pid1,pid2,pid3,pid4;
	int pipefd[4];
	int status;
	
	pipe(pipefd);
	
	pid1 = fork();
	
	
	if (pid1 == 0) {
		char *argv[3] = {"unzip", "/home/sherly/modul2/campur2.zip", NULL};
		execv("/usr/bin/unzip", argv);
	}
	
	
	while ((wait(&status))>0);
	DIR *d;
    	struct dirent *dir;
    	d = opendir("/home/sherly/modul2/campur2");
    	if (d)
    	{
	        while ((dir = readdir(d)) != NULL)
        	{
		    int len = strlen(dir->d_name);
		    if(dir->d_name[len-4] =='.' && dir->d_name[len-3] =='t' && dir->d_name[len-2] =='x'&& dir->d_name[len-1] =='t')
	            printf("%s\n", dir->d_name);
			
	        }
		
closedir(d);
	}
pid2 = fork();
if (pid2 == 0){ execlp("touch", "touch", "daftar.txt", NULL);}
pid3 = fork();
if(pid3 == 0)
    {
        // anak 2
        char *argv[3] = {"ls", "campur2", NULL};
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        execv("/bin/ls", argv);
    }
    while((wait(&status)) > 0);
    pid4 = fork();
    if(pid4 == 0)
    {
        // anak 3
        char *argv[3] = {"grep", ".txt$", NULL};
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);

       freopen("daftar.txt", "w", stdout);
        execv("/bin/grep", argv);
}
close(pipefd[0]);
close(pipefd[1]);
while ((wait(&status))>0);
return 0;
}

	
	



		

	
	
