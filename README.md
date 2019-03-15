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

## 4. soal4
Dalam direktori `/home/[user]/Documents/makanan` terdapat file `makan_enak.txt` yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi `makan_enak.txt` karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file `makan_enak.txt` pernah dibuka setidaknya 30 detik yang lalu `(rentang 0 - 30 detik)`.

Jika file itu pernah dibuka, program Anda akan membuat 1 file `makan_sehat#.txt` di direktori `/home/[user]/Documents/makanan` dengan `'#'` berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.
#### Contoh:
File `makan_enak.txt` terakhir dibuka pada detik ke-1  
Pada detik ke-10 terdapat file `makan_sehat1.txt` dan `makan_sehat2.txt`
#### Catatan:
dilarang menggunakan crontab  
Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

## 5. soal5
Kerjakan poin a dan b di bawah:  
a. Buatlah program c untuk mencatat log setiap menit dari file `log` pada `syslog` ke `/home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log`  
	Ket:  
	- Per 30 menit membuat folder `/[dd:MM:yyyy-hh:mm]`  
	- Per menit memasukkan log#.log ke dalam folder tersebut `‘#’` : increment per menit. Mulai dari 1  
b. Buatlah program c untuk menghentikan program di atas.  
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.  
