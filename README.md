# SoalShift_Modul2_B02

### Daftar Isi :
- [1. Soal1](#1-soal1)
- [2. Soal2](#2-soal2)
- [3. Soal3](#3-soal3)
- [4. Soal4](#4-soal4)
- [5. Soal5](#5-soal5)

## 1. soal1
Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi '“[namafile]_grey.png”'. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori `/home/[user]/modul2/gambar`.
##### Catatan : Tidak boleh menggunakan crontab.
#### Jawaban :
#### Penjelasan :
1. Menyimpan direktori lalu disimpan pada file dalam bentuk struct
	```c
	...
	struct dirent *de;
	...
	```
2. Membuka direktori file lalu mengecek isi didalamnya
	```c
	...
	DIR *dr = opendir(".");
	...
	```
3. Mengecek .png lalu di `rename`
	```c
	...
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
	...
	```
4. Men-copy file yang telah di `rename` ke dalam file `modul2/gambar`
	```c
	...
	char *argv[4] = {"cp", nama, "/home/sherly/modul2/gambar", NULL};
		execv("/bin/cp", argv);
	...
	```
5. Memberikan daemon

## 2. soal2
Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
##### Catatan: Tidak boleh menggunakan crontab
### Jawaban:
> Check : [Full SourceCode](../soal2.c)
### Penjelasan :
1. karena file yang dimaksud tidak terdapat pada paket `Soal Shift Modul 2`, maka buat file yang dimaksud, dengan cara :
```sh
$ mkdir ~/hatiku
$ touch ~/hatiku/elen.ku
```
dan lalu buat `owner` dan `group` menjadi `www-data`. Dengan cara :
```sh
$ sudo chown www-data:www-data ~/hatiku/elen.ku
```
dan beri permission menjadi readable, writeable, dan executable. Dengan cara :
```sh
$ sudo chmod 777 ~/hatiku/elen.ku
```
2. Buat file `.c`, inisiasi variable untuk menampung alamat file elen.ku.
```c
...
int main(){
	char elenku[100];
	char *uname;
	uname = getlogin();
	sprintf(elenku, "/home/%s/hatiku/elen.ku", uname);
	...
}
```
untuk membuat `username` dalam folder `home` lebih dinamik, maka dapat memanfaatkan fungsi `getlogin()`. Dan untuk menginisiai alamat elen.ku dapat menggunakan `sprintf` lalu ditampung di variable `elenku` yang sudah dibuat.
3. Karena program yang diminta harus berjalan setiap detik, maka dapat menggunakan `while(1)` untuk melakukan `looping` dengan bantuan fungsi `sleep(3)` untuk men-delay program selama tiga detik.
```c
...
int main(){
	...
	while(1){
		...
		sleep(3);
	}
}
```
4. Untuk memeriksa status file dapat menggunakan `stat` dalam [libray](http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html) `sys/stat.h`
```c
...
int main(){
	...
	while(1){
		...
		struct stat kenangan;
		stat(elenku, &kenangan);
		...
	}
	...
}
```
dengan library ini kita dapat mendapatkan `user id` serta `group id` dari file `elenku`.
5. Selanjutnya kita perlu memeriksa apakah file tersebut benar dimiliki (`owner`) dan dalam `group`nya `www-data` dengan bantuan [library](http://pubs.opengroup.org/onlinepubs/009696699/basedefs/pwd.h.html) `pwd.h` dan [libray](http://pubs.opengroup.org/onlinepubs/007904975/basedefs/grp.h.html) `grp.h`.
```c
...
int main(){
	...
	while(1){
		...
		struct passwd *euid = getpwid(kenangan.st_uid);
		struct passwd *egid = getgrid(kenangan.st_gid);
		...
	}
	...
}
```
6. Lalu dapat diperiksa dengan `if statement ` apakah nama own dan group file elen.ku sama dengan `www-data`.
```c
...
int main(){
	...
	while(1){
		...
		struct passwd *euid = getpwid(kenangan.st_uid);
		struct passwd *egid = getgrid(kenangan.st_gid);
		if(strcmp(euid->pw_name, "www-data")==0 && strcmp(egid->gr_name, "www-data")==0){
  		remove(elenku);
		}
		...
	}
	...
}
```
7. Maka keseluruan program yang kita buat ialah :
```c
...
int main() {
	char elenku[100];
	char *uname;
	uname = getlogin();
	sprintf(elenku, "/home/%s/modul2/hatiku/elen.ku", uname);

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
}
```
Jika `onwer` dan `group` file elen.ku sama dengan `www-data` maka file tersebut dapat langsung saja di hapus.
8. Untuk menjalankan program dalam `background` maka dapat didapat ditambahkan fungsi daemon.  
	```c
		...
		void crDaemon();
		int main() {
		...
		crDaemon();

		while(1) {
		 ...
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
	```

## 3. soal3
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”.
Buatlah program C yang dapat:  
	i)  mengekstrak file zip tersebut.  
	ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt.
#### Catatan:
- Gunakan fork dan exec.
- Gunakan minimal 3 proses yang diakhiri dengan exec.
- Gunakan pipe
- Pastikan file daftar.txt dapat diakses dari text editor
#### Jawaban:
#### Penjelasan:
1. Mendeklarasikan dengan cara :
	```c
	...
	int main() {
	pid_t pid1,pid2,pid3,pid4;
	int pipefd[4];
	int status;
	
	pipe(pipefd);
	
	pid1 = fork();
	...
	```	
2. Proses untuk me-unzip file `Campur2` dengan menggunakan command `unzip`
	```c
	...
	char *argv[3] = {"unzip", "/home/sherly/modul2/campur2.zip", NULL};
		execv("/usr/bin/unzip", argv);
	...
	```	
3. Dengan menggunakan bantuan `dir` me-list file mana saja yang mempunyai format .txt
	```c
	...
	while ((wait(&status))>0);
	DIR *d;
    	struct dirent *dir;
    	d = opendir("/home/sherly/modul2/campur2");
    	if (d)
    	{
	        while ((dir = readdir(d)) != NULL)
        	{
		    int len = strlen(dir->d_name);
		    if(dir->d_name[len-4] =='.' && dir->d_name[len-3] =='t' && dir->d_name[len-2] =='x'&& dir->d_name[len-1] 				=='t')
	            printf("%s\n", dir->d_name);
			
	        }
	closedir(d);
	...
	```
4. Membuat file baru bernama `daftar.txt` dengan bantuan `touch`
	```c
	...
	execlp("touch", "touch", "daftar.txt", NULL);
	...
	```
5. Me-list file .txt dan dimasukkan ke child ke empat untuk memasukkan ke `daftar.txt` menggunakan erintah `freopen`
	```c
	...
	if(pid3 == 0)
    	{
        char *argv[3] = {"ls", "campur2", NULL};
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        execv("/bin/ls", argv);
    	}
    	while((wait(&status)) > 0);
    	pid4 = fork();
    	if(pid4 == 0)
    	{
        char *argv[3] = {"grep", ".txt$", NULL};
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);

       freopen("daftar.txt", "w", stdout);
        execv("/bin/grep", argv);
	}
	...
	```
	
## 4. soal4
Dalam direktori `/home/[user]/Documents/makanan` terdapat file `makan_enak.txt` yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi `makan_enak.txt` karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file `makan_enak.txt` pernah dibuka setidaknya 30 detik yang lalu `(rentang 0 - 30 detik)`.

Jika file itu pernah dibuka, program Anda akan membuat 1 file `makan_sehat#.txt` di direktori `/home/[user]/Documents/makanan` dengan `'#'` berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.
###### Contoh:
File `makan_enak.txt` terakhir dibuka pada detik ke-1  
Pada detik ke-10 terdapat file `makan_sehat1.txt` dan `makan_sehat2.txt`
###### Catatan:
dilarang menggunakan crontab  
Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst
#### Jawaban :
> [Full SourceCode](../master/soal4.c)
#### Penjelasan :
1. Lakukan inisiasi direktori, dengan cara :
	```c
	...
	int main(){
		char dir[55];
		char *uname;
		uname = getlogin();
		sprintf(dir, "/home/%s/Documents/makanan", uname);
		...
	}
	```
	Buat variable untuk menampung alamat direktori. Manfaatkan fungsi `getlogin()` untuk mendapatkan `username`. Inisiai alamat ke dalam variable `dir` menggunakan fungsi `sprintf`.
2. Inisiai file `makan_enak.txt` dengan bantuan `dir` yang sudah dibuat.
	```c
	...
	int main(){
		...
		char file[100];
		sprintf(file, "%s/makan_enak.txt", dir);
	}
	```  

3. Mulai buat program utamanya ke dalam `while(1)` dan dengan fungsi `sleep(5)`, karena program akan terus berjalan dan hanya berjeda dalam 5 detik.
	```c
	...
	int main(){
		...
		while(1){
			sleep(5);
			...
		}
	}
	```
4. Periksa kapan terakhir kali file `makan_enak.txt` diakses, Manfaatkan fungsi `stat()` pada [library](pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html) `sys/stat.h` dan [library](http://pubs.opengroup.org/onlinepubs/7908799/xsh/time.h.html) `time.h`
	```c
	...
	int main(){
		...
		while(1){
			sleep(5);
			struct stat filestat;
			stat(file, &filestat);

			time_t fileAccessed = filestat.st_atime;
			...
		}
	}
	```
	setelah status file `makan_enak.txt` diambil, cukup ambil kapan file tersebut dengan `st_atime` lalu simpan dalam variable fileAccessed dengan tipe data `time_t`.
5. Mulai periska jika perbandingan selisih waktu terakhir kali diakses dengan waktu saat ini kurang dari sama dengan 30detik, maka buat file `makan_sehat#.txt` dengan  `#` berisi bilangan bulat.
	```c
	...
	int main(){
		...
		int ct = 1;
		while(1){
			...
			if(difftime(time(NULL), fileAccessed) <= 30){
				char newfile[100];
				sprintf(newfile, "%s/makan_sehat%d.txt", dir, ct);
				...
			}
		}
	}
	```
	dengan memanfaatkan variable `dir` yang pernah dibuat, buat alamat file baru dengan bantuan fungsi `sprintf` dan tambahkan variable `ct` yang akan digunakan untuk penomoran file baru yang akan dibuat.
	```c
	...
	int main(){
		pid_t = child;
		int status;
		...
		while(1){
			...
			if(difftime(time(NULL), fileAccessed) <= 30){
				...
				child = fork();
				if(child == 0){
					char *argv[3] = {"touch", newfile, NULL};
					execv("/usr/bin/touch", argv);
					} while ((wait(&status)) > 0); kill(child, SIGKILL);
					ct++;
				}
			}
		}
	}
	```
	karena saat menjalankan fungsi `execv` program akan berhenti/keluar dari `while(1)` maka dapat dibuatkan proses baru dengan `fork()` setelah itu jalankan `touch` untuk membuat file baru. Jangan lupa untuk menutup proses dengan men-kill pid yang baru dibuat dengan fungsi `kill`, namun manfaatkan fungsi `wait()` terlebih dahulu untuk memastikan proses selesai dijalankan sebelum ditutup. setelah itu jangan lupa meng-increament `ct`.  
6. Program utama selesai, buat dan tambahkan fungsi untuk membuat `daemon`.
	```c
	...
	void crDaemon();

	int main() {
		...
		crDaemon();

		while(1) {
		 ...
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
	```  

## 5. soal5
Kerjakan poin a dan b di bawah:  
a. Buatlah program c untuk mencatat log setiap menit dari file `log` pada `syslog` ke `/home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log`  
	Ket:  
	- Per 30 menit membuat folder `/[dd:MM:yyyy-hh:mm]`  
	- Per menit memasukkan log#.log ke dalam folder tersebut `‘#’` : increment per menit. Mulai dari 1  
b. Buatlah program c untuk menghentikan program di atas.  
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.
#### Jawaban :
> Check : [Full SourceCode](../master/soal5a.c)
> Check : [Full SourceCode](../master/soal5b.c)
#### Penjelasan :
##### a. Soal5a
1. Inisiasi direktori `log`, manfaatkan fungsi `getlogin()` untuk mendapatkan username user secara dinamik.
	```c
	...
	int main(){
		char dir[100];
		char *uname;
		uname = getlogin();
		sprintf(dir, "/home/%s/log", uname);
		...

	}
	```
2. Mulai buat program utama, karena program akan melakukan `looping` dengan jeda waktu permenit, maka :
	```c
	...
	int main(){
		...
		while(1){
			...
			sleep(60);
		}

	}
	```
	dan untuk membuat folder berdasarkan tanggal dan waktu dengan format `[dd:MM:yyyy-hh:mm]`, dapat dengan memanfaatkan [library](http://pubs.opengroup.org/onlinepubs/7908799/xsh/time.h.html) `time.h` untuk mengambil waktu saat ini, namun dengan syarat jika pada menit pertama, ketiga puluh dan keliapatannya dan lalu disimpan kedalam variable `dtime`.
	```c
	...
	int main(){
		int minute = 0;
		char dtime[20];
		...
		while(1){
			if(minute%30 == 0){
				time_t now = time(NULL);
				struct tm *t = localtime(&now);
				strftime(dtime, sizeof(dtime)-1, "%Y:%m:%d-%H:%M", t);
			}
			...
		}
	}
	```
3. Buat direktori berdasarkan waktu dengan cara di atas menggunakan `execv`.
	```c
	...
	int main(){
		pid_t child;
		int status;
		...
		while(1){
			...
			child = fork();
			if(child == 0){
				char path[200];
				sprintf(path, "%s/%s", dir, dtime);
				char *arg[4] = {"mkdir", "-p" ,path, NULL};
				execv("/bin/mkdir", arg);
			}
			while ((wait(&status)) > 0);
			kill(child, SIGKILL);
		}

	}
	```
	Karena menggunakan `execv` maka diperlukan untuk membuat proses baru agar program tidak keluar dari `while(1)` dan atau `terminate`. setelah itu tunggu hingga proses selesai, lalu `kill()`.
4. Lakukan increment pada variable `minute` sebagai penanda sudah berapa menit program dijalankan.
	...
	int main(){
		int minute = 0;
		char dtime[20];
		...
		while(1){
			...
			minute+=1;
			...
		}
	}
	```
5. Lakukan backup `syslog` menggunakan program `cp` dengan `execv`.
	```c
	...
	int main(){
		...
		while(1){
			...
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
			...
		}

	}
	```
	Gunakan `fork()` sekali lagi untuk menjalankan `execv` dan boleh menggunakan variable yang sama spt `mkdir` karena sebelumnya sudah di`kill()`.  
6. Karena program berjalan seperti menggunakan `crontab` maka program akan dijalankan dalam `background` menggunakan `daemon process`.
	```c
	...
	void crDaemon();

	int main() {
		...
		crDaemon();

		while(1) {
		 ...
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
	```  



##### b. soal5b
1. Buat program menggunakan `execv` untuk menjalankan program `killall` dan jangan lupa memasukkan nama program yang sesuai dengan no 5.a
```c
...
int main() {
	char namap[10] = {"soal5a"};

	char *argv[3] = {"killall", namap, NULL};
	execv("/usr/bin/killall", argv);

	return 0;
}
```
