#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<string.h>

/*  Proyek Akhir Semester
Judul Proyek: AnterCepat
Nomor grup: 7
Nama Anggota / NPM:
Muhammmad Najih Aflah / 2106653880
Rayhan Akbar Arrizky / 2106632655		*/

//struct untuk Linked List yang menyimpan data paket
struct node{
	char namaPengirim[30];
	char notelpPengirim[13];
	char namaPenerima[30];
	char notelpPenerima[13];
	int resi;
	int berat;
	char jenisPengiriman[10];
	char alamat[200];
	char provinsiTujuan[20];
	char status[40];
	struct node *next;
};

//protoype fungsi-fungsi yang dipanggil di main:
//1.Prototype fungsi yang berguna untuk membaca list paket di dalam file jika ada
void checkFile(char filename[], struct node **head);
//2.Prototype fungsi yang berguna untuk menyimpan list paket ke dalam file
void writeFile(char filename[], struct node *head);
//3. Prototype fungsi yang berguna untuk menampilkan menu sebelum dan sesudah login
void menu(int *status);
//4. Prototype fungsi yang berguna untuk memesan paket
void pesan(struct node **head, struct node **secondHead, struct node **thirdHead); 
//5. Prototype fungsi yang berguna untuk melacak paket
void lacak(struct node *head, struct node *secondHead, struct node *thirdHead); 
//6. Prototype fungsi yang berguna untuk melakukan log in sebagai admin
void logIn(int *status); 
//7. Prototype fungsi yang berguna untuk menampilkan list paket (khusus admin)
void listPaket(struct node **head, struct node **secondHead, struct node **thirdHead);
//8. Prototype fungsi yang berguna untuk melakukan pengiriman paket (khusus admin)
void kirim(struct node **headBefore, struct node **headAfter);
//9. Prototype fungsi yang berguna untuk melakukan konfirmasi kedatangan paket (khusus admin)
void konfirmasi(struct node **headBefore, struct node **headAfter);
//10. Prototype fungsi yang berguna untuk melakukan log out (khusus admin)
void logOut(int *status);
//11. Prototype fungsi yang berguna untuk membuat laporan
void laporan(struct node **head, struct node **secondHead, struct node **thirdHead);

//prototype fungsi-fungsi yang berkaitan dengan Linked List
//1. Prototype fungsi yang berguna untuk menambah elemen Linked List pada awal Linked List
void push(struct node **head, struct node input);
//2. Prototype fungsi yang berguna untuk menambah elemen Linked List pada akhir Linked List
void append(struct node **head, struct node input);
//3. Prototype fungsi yang berguna untuk menambah elemen Linked List pada posisi tertentu dari Linked List
void specialInsert(struct node **head, struct node input, int pos);
//4. Prototype fungsi yang berguna untuk menghitung jumlah elemen linkedList
int count(struct node *head);
//5. Prototype fungsi yang berguna untuk menghapus elemen pada Linked List dan elemen yang dihapus akan dimasukkan ke Linked List lain
void moveNode(struct node **head, struct node *movedNode, int pos);
//6. Prototype fungsi yang berguna untuk mencetak semua elemen yang terdapat pada Linked List
void printList(struct node *head);
//7. Prototype fungsi yang berguna untuk menghitung elemen pada Linked list yang mempunyai atribut spesifik
int samedayCount(struct node *head);
//8. Prototype fungsi yang berguna untuk mencari elemen tertentu pada linked list
void search(struct node *head, struct node *founded, int keyNumber, int *found);

//prototype fungsi-fungsi lainnya
//1. Prototype fungsi yang berguna untuk menampilkan menu sekunder
void subMenu(char info[50]);
//2. Prototype fungsi yang berguna untuk mencetak struk
void struk(struct node input);
//3. Prototype fungsi yang berguna untuk melakukan enkripsi password
void encrypt(char *pass);
//4. Prototype fungsi yang berguna untuk melakukan dekripsi password
void decrypt(char *pass);

int main(){
/*	firstHead: head untuk list paket belum dikirim
	secondHead: head untuk list paket dalam pengiriman
	thirdHead: head untuk list paket terkirim				*/
	struct node *firstHead = NULL, *secondHead = NULL, *thirdHead = NULL;
	int loginStatus = 0, pilihan;
	//memanggil fungsi checkFile() untuk tiga file yang berbeda
	checkFile("List_BelumDikirim.txt", &firstHead);
	checkFile("List_DalamPengiriman.txt", &secondHead);
	checkFile("List_PaketTerkirim.txt", &thirdHead);
	do{ //looping program dengan do-while
		system("cls");
		//memanggil fungsi menu()
		menu(&loginStatus);
		do{//error handling untuk pilihan menu
			scanf("%d", &pilihan);
		}while((loginStatus == 0 && (pilihan < 1 || pilihan > 4)) || (loginStatus == 1 && (pilihan < 1 || pilihan > 8)));
		switch(pilihan){//membuat switch-case untuk pilihan menu
			case 1: pesan(&firstHead, &secondHead, &thirdHead);  //melakukan pemesanan paket
					break;
			case 2: lacak(firstHead, secondHead, thirdHead); //melakukan pelacakan paket
					break;
			case 3: if(loginStatus == 0){ //jika bukan admin
						logIn(&loginStatus); //melakukan log in
						break;
					}else if(loginStatus == 1){ //jika admin
						listPaket(&firstHead, &secondHead, &thirdHead); //melihat list paket
						break;
					}
			case 4: if(loginStatus == 1){ //jika admin
						kirim(&firstHead, &secondHead); //melakukan pengiriman paket
						break;
					}else{ //jika bukan
						break; //exit
					}
			case 5: konfirmasi(&secondHead, &thirdHead); //melakukan konfirmasi kedatangan paket
					break;
			case 6: logOut(&loginStatus); //melakukan log out
					break;
			case 7: laporan(&firstHead, &secondHead, &thirdHead); //membuat laporan
					break;
		}
	}while((loginStatus == 0 && pilihan != 4) || (loginStatus == 1 && pilihan != 8));
	//memanggil fungsi writeFile untuk tiga file yang berbeda
	writeFile("List_BelumDikirim.txt", firstHead);
	writeFile("List_DalamPengiriman.txt", secondHead);
	writeFile("List_PaketTerkirim.txt", thirdHead);
	return 0;
}

//fungsi untuk membaca file
void checkFile(char filename[], struct node **head){
	FILE *fptr;
	struct node input;
	char temp[500];
	if((fptr = fopen(filename, "r")) == NULL){//jika file tidak ada, tidak akan membaca apa-apa
	}else{
		while((fscanf(fptr, "%[^\n]", temp))!= EOF){ //melakukan pembacaan sampai End Of File (EOF)
			fgetc(fptr);
			//data dipisahkan dengan ; (titik koma)
			sscanf(temp, " %[^;]%*c %[^;]%*c %[^;]%*c %[^;]%*c %d%*c %d%*c %[^;]%*c %[^;]%*c %[^;]%*c %[^;]",
					input.namaPengirim, input.notelpPengirim, input.namaPenerima, input.notelpPenerima, &input.resi, 
					&input.berat, input.alamat, input.provinsiTujuan, input.jenisPengiriman, input.status);
			
			//menambahkan hasil pembacaan ke dalam linkedList
			if(strcmp(input.jenisPengiriman, "Same day") == 0){ //paket dengan jenis pengiriman same day ditambahkan ke awal linked list
//				printf("masuk sameday");
				push(head, input);
			}else if(strcmp(input.jenisPengiriman, "Next day") == 0){ //paket dengan jenis pengiriman next day ditambahkan diantara same day dan reguler
//				printf("masuk nextday");
				specialInsert(head, input, samedayCount(*head) + 1);
			}else{ //paket dengan jenis pengiriman reguler ditambahkan ke akhir linked list
//				printf("masuk reguler");
				append(head, input);
			}
		}
		fclose(fptr); //menutup file
	}
}

//fungsi untuk menyimpan data ke dalam file agar tidak hilang ketika program diexit
void writeFile(char filename[], struct node *head){
	FILE *fptr;
	struct node *iterator;
	fptr = fopen(filename, "w"); //overwrite data yang lama dengan data yang baru
	if(head != NULL){ //jika linked list mempunyai elemen
		iterator = head;
		while(iterator != NULL){ //loop sampai elemen terakhir linked list
		//data dipisahkan dengan ; (titik koma)
		fprintf(fptr, "%s; %s; %s; %s; %d; %d; %s; %s; %s; %s\n", 
				iterator->namaPengirim, iterator->notelpPengirim, iterator->namaPenerima, iterator->notelpPenerima, iterator->resi, 
				iterator->berat, iterator->alamat, iterator->provinsiTujuan, iterator->jenisPengiriman, iterator->status);
		iterator = iterator->next;
	}
	}
	fclose(fptr); //menutup file

	}

//fungsi untuk menampilkan menu sebelum atau sesudah login
void menu(int *status){
	if(*status == 0){ //menu yang ditampilkan jika belum log in
		printf( "________________________________________________________"
				"\n\n|          AnterCepat: Jasa pengiriman barang           |" 
        		"\n\n--------------------------------------------------------\n"
        		"|			Main Menu			|"
        		"\n--------------------------------------------------------\n\n"
				" 1. |	Pesan jasa kirim paket\n 2. |	Lacak paket yang sedang dikirim\n"
				" 3. |	Log in sebagai admin\n 4. |	keluar\n"
				"\nMasukkan pilihan (1 - 4) : ");
	}else if(*status == 1){ //menu yang ditampilkan jika sudah login
		printf( "________________________________________________________"
				"\n\n|          AnterCepat: Jasa pengiriman barang           |" 
        		"\n\n--------------------------------------------------------\n"
        		"|			Main Menu			|"
        		"\n--------------------------------------------------------\n\n"
				" 1. |	Pesan jasa kirim paket\n 2. |	Lacak paket yang sedang dikirim\n" 
				" 3. |	Lihat antrian paket\n 4. |	Kirim paket\n"
				" 5. |	Konfirmasi kedatangan paket\n 6. |	Log out\n"
				" 7. |	Buat/perbarui laporan\n 8. |	Keluar\nMasukkan pilihan (1 - 8) : ");
	}
		
}

//fungsi untuk memesan jasa pengiriman
void pesan(struct node **head, struct node **secondHead, struct node **thirdHead){
	struct node input;
	int pulau, provinsi, kota, pengiriman, harga, bayar, i;
	char *jalan, *rt, *rw;
	//menghitung jumlah paket keseluruhan dan digunakan untuk menentukan nomor resi
	i = count(*head) + count(*secondHead) + count(*thirdHead);
	do{ //looping program dengan do-while (apabila user ingin mengedit form)
		system("cls");
		//line 210-370 merupakan input data paket
		printf( "________________________________________________________"
				"\n\n|         FORM PEMESANAN JASA PENGIRIMAN BARANG         |" 
        		"\n--------------------------------------------------------\n\n"
				"Masukkan nama pengirim: ");
				scanf(" %[^\n]30s", input.namaPengirim);
				printf("Masukkan nomor telepon pengirim: "); scanf(" %s", input.notelpPengirim);
				printf("Masukkan nama penerima: "); scanf(" %30[^\n]s", input.namaPenerima);
				printf("Masukkan nomor telepon penerima: "); scanf(" %s", input.notelpPenerima);
 		printf("Pilih daerah tujuan paket\n"
				" 1. |	Jawa, Bali, & Nusa Tenggara\n 2. |	Sumatera & Bangka Belitung\n" 
				" 3. |	Kalimantan\n 4. |	Sulawesi\n"
				" 5. |	Maluku & Papua\n\nMasukkan pilihan (1 - 5) : ");
		do{
			scanf("%d", &pulau);
		}while(pulau < 1 || pulau > 5);
		//if-else if untuk menentukan provinsi tujuan paket
		if(pulau == 1){
			printf("\nPilih Provinsi tujuan paket\n"
					" 1. |	DKI Jakarta\n 2. |	Jawa Barat\n" 
					" 3. |	Jawa Tengah\n 4. |	DI Yogyakarta\n"
					" 5. |	Jawa Timur\n 6. |	Banten\n"
					" 7. |	Bali\n 8. |	Nusa Tenggara Barat\n"
					" 9. |	Nusa Tenggara Timur\n\nMasukkan pilihan (1 - 9) : ");
			do{
				scanf("%d", &provinsi);
			}while(provinsi < 1 || provinsi > 9);
			switch(provinsi){
			case 1: strcpy(input.provinsiTujuan,"DKI Jakarta");
					break;
			case 2: strcpy(input.provinsiTujuan,"Jawa Barat");
					break;
			case 3: strcpy(input.provinsiTujuan,"Jawa Tengah");
					break;
			case 4: strcpy(input.provinsiTujuan,"DI Yogyakarta");
					break;
			case 5: strcpy(input.provinsiTujuan,"Jawa Timur");
					break;
			case 6: strcpy(input.provinsiTujuan,"Banten");
					break;
			case 7: strcpy(input.provinsiTujuan,"Bali");
					break;
			case 8: strcpy(input.provinsiTujuan,"Nusa Tenggara Barat");
					break;
			case 9: strcpy(input.provinsiTujuan,"Nusa Tenggara Timur");
					break;
			}
		}else if(pulau == 2){
			printf("\nPilih Provinsi tujuan paket\n"
					" 1. |	Aceh\n 2. |	Sumatera Utara\n" 
					" 3. |	Sumatera Barat\n 4. |	Riau dan Kep. Riau\n"
					" 5. |	Jambi\n 6. |	Sumatera Selatan\n"
					" 7. |	Bengkulu\n 8. |	Lampung\n"
					" 9. |	Kepulauan Bangka Belitung\n\nMasukkan pilihan (1 - 9) : ");
			do{
				scanf("%d", &provinsi);
			}while(provinsi < 1 || provinsi > 9);
			switch(provinsi){
			case 1: strcpy(input.provinsiTujuan,"Aceh");
					break;
			case 2: strcpy(input.provinsiTujuan,"Sumatera Utara");
					break;
			case 3: strcpy(input.provinsiTujuan,"Sumatera Barat");
					break;
			case 4: strcpy(input.provinsiTujuan,"Riau dan kep. Riau");
					break;
			case 5: strcpy(input.provinsiTujuan,"Jambi");
					break;
			case 6: strcpy(input.provinsiTujuan,"Sumatera Selatan");
					break;
			case 7: strcpy(input.provinsiTujuan,"Bengkulu");
					break;
			case 8: strcpy(input.provinsiTujuan,"Lampung");
					break;
			case 9: strcpy(input.provinsiTujuan,"Bangka Belitung");
					break;
			}
		}else if(pulau == 3){
			printf("\nPilih Provinsi tujuan paket\n"
					" 1. |	Kalimantan Utara\n 2. |	Kalimantan Timur\n" 
					" 3. |	Kalimantan Selatan\n 4. |	Kalimantan Barat\n"
					" 5. |	Kalimantan Tengah\n\nMasukkan pilihan (1 - 5) : ");
			do{
				scanf("%d", &provinsi);
			}while(provinsi < 1 || provinsi > 5);
			switch(provinsi){
			case 1: strcpy(input.provinsiTujuan,"Kalimantan Utara");
					break;
			case 2: strcpy(input.provinsiTujuan,"Kalimantan Timur");
					break;
			case 3: strcpy(input.provinsiTujuan,"Kalimantan Selatan");
					break;
			case 4: strcpy(input.provinsiTujuan,"Kalimantan Barat");
					break;
			case 5: strcpy(input.provinsiTujuan,"Kalimantan Tengah");
					break;
			}
		}else if(pulau == 4){
			printf("\nPilih Provinsi tujuan paket\n"
					" 1. |	Sulawesi Utara\n 2. |	Sulawesi Tengah\n" 
					" 3. |	Sulawesi Selatan\n 4. |	Sulawesi Barat\n"
					" 5. |	Gorontalo\n 6. |	Sulawesi Tenggara\n"
					"\nMasukkan pilihan (1 - 6) : ");
			do{
				scanf("%d", &provinsi);
			}while(provinsi < 1 || provinsi > 6);	
			switch(provinsi){
			case 1: strcpy(input.provinsiTujuan,"Sulawesi Utara");
					break;
			case 2: strcpy(input.provinsiTujuan,"Sulawesi Tengah");
					break;
			case 3: strcpy(input.provinsiTujuan,"Sulawesi Selatan");
					break;
			case 4: strcpy(input.provinsiTujuan,"Sulawesi Barat");
					break;
			case 5: strcpy(input.provinsiTujuan,"Gorontalo");
					break;
			case 6: strcpy(input.provinsiTujuan,"Sulawesi Tenggara");
					break;
			}
			}else if(pulau == 5){
			printf("\nPilih Provinsi tujuan paket\n"
					" 1. |	Maluku\n 2. |	Maluku Utara\n" 
					" 3. |	Papua\n 4. |	Papua Barat\n"
					"\nMasukkan pilihan (1 - 4) : ");
			do{
				scanf("%d", &provinsi);
			}while(provinsi < 1 || provinsi > 4);
			switch(provinsi){
			case 1: strcpy(input.provinsiTujuan,"Maluku");
					break;
			case 2: strcpy(input.provinsiTujuan,"Maluku Utara");
					break;
			case 3: strcpy(input.provinsiTujuan,"Papua");
					break;
			case 4: strcpy(input.provinsiTujuan,"Papua Barat");
					break;
			}
		}
		input.resi = pulau * 100000 + provinsi * 10000; //bagian dari menentukan nomor resi
		printf("\nMasukkan alamat lengkap (sampai kota/kabupaten):"); scanf(" %[^\n]200s", input.alamat);
		printf("Timbang dan masukkan berat paket (kg):"); scanf("%d", &input.berat);
		printf("\nPilih opsi pengiriman\n"
				" 1. |	Same day (Tidak tersedia untuk luar daerah Jawa, Bali, & Nusa Tenggara)\n"
				" 2. |	Next day (Tidak tersedia untuk daerah Maluku & Papua)\n 3. |	Reguler\n");
		do{
			printf("\nMasukkan pilihan (1 - 3) : ");
			scanf("%d", &pengiriman);
		}while((pulau == 5 && pengiriman != 3) || ((pulau == 2 || pulau == 3 || pulau == 4) && pengiriman == 1) || pengiriman < 1 || pengiriman > 4);
		switch(pengiriman){
			case 1: strcpy(input.jenisPengiriman,"Same day");
					break;
			case 2: strcpy(input.jenisPengiriman,"Next day");
					break;
			case 3: strcpy(input.jenisPengiriman,"Reguler");
					break;
		}
		input.resi += pengiriman * 1000 + (i+1); //bagian dari menentukan nomor resi
		harga = 24000 / pengiriman + 1000 * input.berat + 10000 * pulau; //menentukan biaya yang perlu dibayar oleh pemesan
		printf("\nPesanan berhasil dicatat!\n Total yang harus dibayar: %d\nSilahkan bayar ke petugas, struk akan keluar setelah pembayaran\n", harga);
		do{
		printf("(tekan 1 apabila sudah membayar, 0 apabila ingin mengedit form pemesanan): "); scanf("%d", &bayar);
		}while(bayar < 0 || bayar > 1);
		if(bayar == 1){
			strcpy(input.status, "Belum dikirim");
			struk(input); //mencetak struk
			
			//memasukkan input pemesanan paket ke dalam linked list berdasarkan jenis pengirimannya
			if(strcmp(input.jenisPengiriman, "Same day") == 0){ //paket dengan jenis pengiriman same day ditambahkan ke awal linked list
//				printf("masuk sameday");
				push(head, input);
			}else if(strcmp(input.jenisPengiriman, "Next day") == 0){ //paket dengan jenis pengiriman next day ditambahkan diantara same day dan reguler
//				printf("masuk nextday");
				specialInsert(head, input, samedayCount(*head) + 1);
			}else{ //paket dengan jenis pengiriman reguler ditambahkan ke akhir linked list
//				printf("masuk reguler");
				append(head, input);
			}
			subMenu("");	
		}
	}while(bayar == 0); //jika belum bayar, paket tidak akan dimasukkan ke dalam list untuk dikirim
}

//fungsi untuk mencetak struk dalam program dan dalam file
void struk(struct node input){
	FILE *fptr;
	fptr = fopen("Struk.txt", "w");
	//mencetak struk di program
	printf( " __________________________________________"
			"\n|    AnterCepat: Jasa pengiriman barang    |" 
        	"\n ------------------------------------------\n"
        	"|              STRUK PEMBAYARAN            |"
        	"\n ------------------------------------------\n"
			"  %-20s: %s\n  %-20s: %s\n  %-20s: %s\n  %-20s: %s\n"
			"  %-20s: %d\n  %-20s: %d Kg\n  %-20s: %s, %s\n  %-20s: %s\n"
			"\n\n (Struk juga dicetak dalam bentuk file)\n", "NAMA PENGIRIM", input.namaPengirim, 
			"NO. TELPON PENGIRIM",input.notelpPengirim, "NAMA PENERIMA", input.namaPenerima, 
			"NO. TELPON PENERIMA", input.notelpPenerima, "NOMOR RESI", input.resi, "BERAT PAKET", 
			input.berat, "ALAMAT", input.alamat, input.provinsiTujuan, "OPSI PENGIRIMAN", input.jenisPengiriman);
			
	//mencetak struk di file
	fprintf(fptr, " __________________________________"
				"\n|    AnterCepat: Jasa pengiriman barang    |" 
        		"\n -------------------------------------------------------\n"
        		"|                 STRUK PEMBAYARAN                |\n"
        		" -------------------------------------------------------\n"
        		"  %-25s: %s\n  %-21s: %s\n  %-24s: %s\n  %-20s: %s\n"
				"  %-28s: %d\n  %-29s: %d Kg\n  %-32s: %s, %s\n  %-25s: %s\n", "NAMA PENGIRIM", input.namaPengirim, 
				"NO. TELPON PENGIRIM",input.notelpPengirim, "NAMA PENERIMA", input.namaPenerima, 
				"NO. TELPON PENERIMA", input.notelpPenerima, "NOMOR RESI", input.resi, "BERAT PAKET", 
				input.berat, "ALAMAT", input.alamat, input.provinsiTujuan, "OPSI PENGIRIMAN", input.jenisPengiriman);
	fclose(fptr); //menutup file
}

//fungsi untuk melacak file
void lacak(struct node *head, struct node *secondHead, struct node *thirdHead){
	system("cls");
	int keyNumber, found, subPilihan;
	do{
		found = 0;
		printf("Masukkan nomor resi yang valid: "); scanf("%d", &keyNumber); //meminta nomor resi untuk dilacak
		struct node paket;
		search(head, &paket, keyNumber, &found); //memanggil fungsi search untuk melacak paket di list paket belum dikirim
		if(found == 0){ //jika belum ditemukan, memanggil fungsi search untuk melacak paket di list paket dalam pengiriman
			search(secondHead, &paket, keyNumber, &found); 
		}
		if(found == 0){ //jika belum ditemukan, memanggil fungsi search untuk melacak paket di list paket sudah terkirim
			search(thirdHead, &paket, keyNumber, &found);
		}
		if(found == 1){ //jika ditemukan
		//menampilkan paket yang dicari
		printf("\nNama Pengirim: %s\nNo. Telpon Pengirim: %s\nNama Penerima: %s\nNo. Telp. Penerima: %s\n"
				"Nomor Resi : %d\nBerat paket: %d Kg\nOpsi pengiriman: %s\nAlamat: %s, %s\nStatus: %s\n", 
				 paket.namaPengirim, paket.notelpPengirim, paket.namaPenerima, paket.notelpPenerima, paket.resi, 
				 paket.berat, paket.jenisPengiriman, paket.alamat, paket.provinsiTujuan, paket.status);
		}else{
			//jika paket tidak ditemukan
			printf("Nomor resi tidak valid!");
		}
		printf("\n 1. |	Lacak paket lainnya\n 2. |	Kembali ke menu\n\nMasuk");
			do{
				printf("\nMasukkan pilihan (1 / 2) : ");
				scanf("%d", &subPilihan);
			}while(subPilihan < 1 || subPilihan > 2);
	}while(subPilihan == 1);
}

//fungsi untuk melakukan log in sebagai admin
void logIn(int *status){
	system("cls");
	FILE *fptr;
	int pilihan;
	do{ //looping program dengan do-while
		printf("\nPilihan:\n 1. |	Masuk ke akun yang sudah ada\n 2. |	Buat akun baru\n 3. |	Kembali ke menu utama\n\nMasukkan pilihan (1 - 3) : ");
		do{
			scanf("%d", &pilihan);
		}while(pilihan < 1 || pilihan > 3);
		if(pilihan == 1){
			if((fptr = fopen("daftarAkun.txt", "r")) == NULL){ //jika file daftar akun tidak ditemukan
				printf("Belum ada akun! silahkan buat akun terlebih dahulu.\n");
				fclose(fptr); //menutup file kembali
				subMenu("Pilihan");
			}else{
				fseek(fptr, 0, SEEK_END);
   				if(ftell(fptr) == 0){ //jika file ditemukan tetapi tidak ada isinya
   					printf("Belum ada akun! silahkan buat akun terlebih dahulu.\n");
   					fclose(fptr); //menutup file kembali
					subMenu("Pilihan");
				}else{ //jika file ditemukan dan ada isinya
					fseek(fptr, 0, SEEK_SET);
					char username[50][10], pass[50][15], userKey[10], passKey[15], ch; 
					int i = 0, j;
					while(!feof(fptr)){ //membaca akun di dalam file sampai End Of File
						fscanf(fptr, "%10s %15s", username[i], pass[i]);
						i++;
					}
					//meminta admin memasukkan username dan passwordnya
					printf("Masukkan username: "); scanf(" %10s", userKey);
					printf("Masukkan password: "); 
					//masking password sehingga hanya terlihat '*'
					for(i = 0;i < 15;i++){
 						ch = getch();
 						if(ch == 13){
 							break;
						}
  						passKey[i] = ch;
  						ch = '*' ;
  						printf("%c",ch);
 					}
					passKey[i] = '\0';
					for(j = 0; j < i; j++){
						decrypt(pass[j]); //melakukan decrypt password
						//mencocokkan username dan password yang dimasukkan admin dengan data yang ada
						if((strcmp(username[j], userKey) == 0) && (strcmp(pass[j], passKey) == 0)){ 
							subMenu("\nLogin Berhasil!");
							*status = 1; //mengganti status login menjadi sudah login
							pilihan = 3;
						}
					}
					if(*status == 0){ //jika username dan password salah
						subMenu("\nUsername atau password salah!");
					}
					fclose(fptr); //menutup file
				}
			}
		}else if(pilihan == 2){
			fptr = fopen("daftarAkun.txt", "a");
			char username[10], pass[15], confirmPass[15], ch;
			int i;
			//meminta username dan password
			printf("\nPEMBUATAN AKUN\nMasukkan username akun (maks. 10 karakter tanpa spasi): \n"); 
			scanf(" %10s", username);
			printf("Masukkan password baru: "); 
			//masking password sehingga hanya terlihat '*'
			for(i = 0;i < 15;i++){
 				ch = getch();
 				if(ch == 13){
 					break;
				}
  				pass[i] = ch;
  				ch = '*' ;
  				printf("%c",ch);
 			}
			pass[i] = '\0';
			do{ //konfirmasi password baru supaya ingat
			printf("\nUlangi password baru: "); 
			//masking password sehingga hanya terlihat '*'
			for(i = 0;i < 15;i++){
 				ch = getch();
 				if(ch == 13){
 				break;
				}
  				confirmPass[i] = ch;
  				ch = '*' ;
  				printf("%c",ch);
 			}
			pass[i] = '\0';
			if(strcmp(pass, confirmPass) != 0){
				printf("\nSalah! ulangi password baru!");
			}
			}while(strcmp(pass, confirmPass) != 0);
			encrypt(pass); //enkripsi password sebelum dimasukkan ke file
			fprintf(fptr, "%s %s\n", username, pass); //data akun dimasukkan ke file
			fclose(fptr); //menutup file
			subMenu("\nAkun berhasil disimpan!");
		}
	}while(pilihan != 3);
	system("cls");
}

//fungsi untuk menampilkan list paket
void listPaket(struct node **head, struct node **secondHead, struct node **thirdHead){	
	int pilihan, subPilihan;
	do{
		system("cls");
		printf("Pilih list paket yang ingin dilihat:\n"
			   " 1. |	List paket yang belum dikirim\n"
 			   " 2. |	List paket yang sedang dalam pengiriman\n"
			   " 3. |	List paket yang sudah tiba di lokasi\n");
		do{
			printf("\nMasukkan pilihan (1 - 3) : ");
			scanf("%d", &pilihan);
		}while(pilihan < 1 || pilihan > 3);
		switch(pilihan){ //switch-case untuk pilihan list yang ingin dilihat
			case 1: printf("\nLIST PAKET YANG BELUM DIKIRIM (MASIH DI GERAI)\n");
					printList(*head); //print list paket yang belum dikirim
					break;
			case 2: printf("\nLIST PAKET YANG SEDANG DALAM PENGIRIMAN\n");
					printList(*secondHead); //print list paket yang sedang dalam pengiriman
					break;
			case 3: printf("\nLIST PAKET YANG SUDAH TIBA DI LOKASI\n");
					printList(*thirdHead); //print list paket yang sudah terkirim
					break;
		}
		printf("\n 1. |	Lihat list lainnya\n 2. |	Kembali ke menu\n\nMasuk");
		do{
			printf("\nMasukkan pilihan (1 / 2) : ");
			scanf("%d", &subPilihan);
		}while(subPilihan < 1 || subPilihan > 2);
	}while(subPilihan == 1);
}

//fungsi untuk melakukan pengiriman paket
void kirim(struct node **headBefore, struct node **headAfter){
	struct node move;
	int pilihan;
	do{
		system("cls");
		printf("\nLIST PAKET YANG BELUM DIKIRIM (MASIH DI GERAI)\n");
		printList(*headBefore);
		if(count(*headBefore) > 0){
			do{
				printf("\npilih paket-paket yang akan dikirim, prioritaskan same day\n(0 jika ingin kembali ke menu): ");
				scanf("%d", &pilihan);
			}while(pilihan < 0);
			if(pilihan > 0){
				//memanggil fungsi moveNode untuk memindahkan elemen dari linked list pertama ke linked list berikutnya
				moveNode(headBefore, &move, pilihan); 
				strcpy(move.status, "Paket sedang dalam pengiriman"); //mengganti status paket menjadi "paket sedang dalam pengiriman"
				//memasukkan input pemesanan paket ke dalam linked list berdasarkan jenis pengirimannya
				if(strcmp(move.jenisPengiriman, "Same day") == 0){ //paket dengan jenis pengiriman same day ditambahkan ke awal linked list
					push(headAfter, move);
				}else if(strcmp(move.jenisPengiriman, "Next day") == 0){ //paket dengan jenis pengiriman next day ditambahkan diantara same day dan reguler
					specialInsert(headAfter, move, samedayCount(*headAfter) + 1);
				}else{ //paket dengan jenis pengiriman reguler ditambahkan ke akhir linked list
					append(headAfter, move);
				}
			}
		}else{
			pilihan = 0;
			subMenu("");
		}
	}while(pilihan != 0);
}

//fungsi untuk melakukan konfirmasi kedatangan paket
void konfirmasi(struct node **headBefore, struct node **headAfter){
	struct node move;
	int pilihan;
	do{
		system("cls");
		//pada fungsi ini, list paket yang dicetak adalah paket yang sedang dikirim
		printf("\nLIST PAKET YANG SEDANG DALAM PENGIRIMAN\n");
		printList(*headBefore);
		if(count(*headBefore) > 0){
			do{
				printf("\nKonfirmasi paket yang sudah terkirim (0 jika ingin kembali ke menu): ");
				scanf("%d", &pilihan);
			}while(pilihan < 0);
			if(pilihan > 0){
				//proses pemindahan elemen linked list mirip dengan fungsi kiri()
				moveNode(headBefore, &move, pilihan);
				strcpy(move.status, "Paket terkirim"); //mengganti status paket menjadi "paket terkirim"
				//memasukkan input pemesanan paket ke dalam linked list berdasarkan jenis pengirimannya
				if(strcmp(move.jenisPengiriman, "Same day") == 0){
					push(headAfter, move);
				}else if(strcmp(move.jenisPengiriman, "Next day") == 0){
					specialInsert(headAfter, move, samedayCount(*headAfter) + 1);
				}else{
					append(headAfter, move);
				}
			}
		}else{
			pilihan = 0;
			subMenu("");
		}
	}while(pilihan != 0);
}

//fungsi untuk melakukan log out
void logOut(int *status){
	system("cls");
	int pilihan;
	printf("Yakin ingin log out?\n 1. |	Ya\n 2. |	Tidak, kembali ke menu utama\n\nMasukkan pilihan (1 - 2) :");
	do{
		scanf("%d", &pilihan);
	}while(pilihan < 1 || pilihan > 2);
	if(pilihan == 1){
		subMenu("Log out berhasil!");
		*status = 0; //mengganti status menjadi belum login
	}else{
		system("cls");
	}
}

//fungsi untuk membuat laporan
void laporan(struct node **head, struct node **secondHead, struct node **thirdHead){
	system("cls");
	FILE *fptr;
	fptr = fopen("LaporanTransaksi.txt", "w");
	fprintf(fptr, "Jumlah pesanan saat ini (belum dikirim + dalam pengiriman): %d\nJumlah paket masih di gerai : %d\nJumlah paket dalam perjalanan: %d\nJumlah paket sudah terkirim: %d", 
	(count(*head) + count(*secondHead)), count(*head), count(*secondHead), count(*thirdHead));
	fclose(fptr); //menutup file
}

//fungsi subMenu yang berguna untuk menampilkan menu sekunder
void subMenu(char info[50]){ 
	int subPilihan;
	printf("%s\n 1. |	Kembali\n\nMasukkan pilihan: ", info);
	do{
		scanf("%d", &subPilihan); //meminta input dari user untuk memilih menu
	}while(subPilihan != 1);
	system("cls");
}

//fungsi untuk menambah elemen linked list di awal linked list
void push(struct node **head, struct node input){
	struct node *new = (struct node*) malloc(sizeof(struct node));
	//memasukkan data paket
	strcpy(new->namaPengirim, input.namaPengirim);
	strcpy(new->notelpPengirim, input.notelpPengirim);
	strcpy(new->namaPenerima, input.namaPenerima);
	strcpy(new->notelpPenerima, input.notelpPenerima);
	strcpy(new->jenisPengiriman, input.jenisPengiriman);
	strcpy(new->alamat, input.alamat);
	strcpy(new->provinsiTujuan, input.provinsiTujuan);
	strcpy(new->status, input.status);
	new->resi = input.resi;
	new->berat = input.berat;
	//mengganti head menjadi elemen baru
	new->next = (*head);
	(*head) = new;
}

//fungsi untuk menambah elemen linked list di akhir linked list
void append(struct node **head, struct node input){
	struct node *new = (struct node*) malloc(sizeof(struct node));
	struct node *iterator = *head;
	//memasukkan data paket
	strcpy(new->namaPengirim, input.namaPengirim);
	strcpy(new->notelpPengirim, input.notelpPengirim);
	strcpy(new->namaPenerima, input.namaPenerima);
	strcpy(new->notelpPenerima, input.notelpPenerima);
	strcpy(new->jenisPengiriman, input.jenisPengiriman);
	strcpy(new->alamat, input.alamat);
	strcpy(new->provinsiTujuan, input.provinsiTujuan);
	strcpy(new->status, input.status);
	new->resi = input.resi;
	new->berat = input.berat;
	new->next = NULL;
	//jika linked list tidak memiliki elemen, elemen baru akan menjadi head
	if(*head == NULL){
		*head = new;
		return;
	}
	//menyusuri linked list sampai akhir
	while(iterator->next != NULL){
		iterator = iterator->next;
	}
	//menambahkan elemen baru di akhir
	iterator->next = new;
	return;	
}

//fungsi untuk menambah elemen linked list pada posisi tertentu
void specialInsert(struct node **head, struct node input, int pos){
	struct node *new = (struct node*) malloc(sizeof(struct node));
	struct node *iterator = (*head);
	int i = 1;
 	if(pos == 1){ //jika posisi yang diinginkan adalah posisi pertama
		strcpy(new->namaPengirim, input.namaPengirim);
		strcpy(new->notelpPengirim, input.notelpPengirim);
		strcpy(new->namaPenerima, input.namaPenerima);
		strcpy(new->notelpPenerima, input.notelpPenerima);
		strcpy(new->jenisPengiriman, input.jenisPengiriman);
		strcpy(new->alamat, input.alamat);
		strcpy(new->provinsiTujuan, input.provinsiTujuan);
		strcpy(new->status, input.status);
		new->resi = input.resi;
		new->berat = input.berat;
		new->next = (*head);
		(*head) = new;
		return;
	}
	//menyusuri linked list sampai posisi yang ditentukan
	while(iterator->next != NULL && i < pos - 1){
		iterator = iterator->next;
		i++;
	}
	//memasukkan data paket
	strcpy(new->namaPengirim, input.namaPengirim);
	strcpy(new->notelpPengirim, input.notelpPengirim);
	strcpy(new->namaPenerima, input.namaPenerima);
	strcpy(new->notelpPenerima, input.notelpPenerima);
	strcpy(new->jenisPengiriman, input.jenisPengiriman);
	strcpy(new->alamat, input.alamat);
	strcpy(new->provinsiTujuan, input.provinsiTujuan);
	strcpy(new->status, input.status);
	new->resi = input.resi;
	new->berat = input.berat;
	//menambahkan elemen
	new->next = iterator->next;
	iterator->next = new;
}

//fungsi untuk menghitung jumlah elemen keseluruhan pada linked list
int count(struct node *head){
	if(head == NULL){ //jika linked list tidak memiliki elemen, fungsi langsung return 0
		return 0;
	}
	struct node *iterator = head;
	int count = 1;
	//menyusuri linked list untuk menentukan jumlah elemennya
	while(iterator->next != NULL){
		iterator = iterator->next;
		count++;
	}
	return count;
}

//Fungsi untuk menghapus elemen pada Linked List dan elemen yang dihapus akan dimasukkan ke Linked List lain
void moveNode(struct node **head, struct node *movedNode, int pos){
	struct node *iterator = (*head);
	int i = 1;
	if(pos == 1){ //jika posisi yang diinginkan adalah posisi pertama
		*movedNode = *iterator;
		*head = iterator->next;
		iterator->next = NULL;
		free(iterator);
		return;
	}
	//menyusuri linked list sampai posisi yang diinginkan
	while(iterator->next != NULL && i < pos -1){
		iterator = iterator->next;
		i++;
	}
	if(i + 1 < pos || iterator->next == NULL){
		printf("Paket tidak ditemukan!\n");
		return;
	}
	*movedNode = *iterator; //menyimpan elemen yang akan dipindahkan
	struct node *delete = iterator->next;
    iterator->next = iterator->next->next;     
    delete->next = NULL;
    free(delete); //menghapus elemen yang dipindahkan
}

//Fungsi untuk menghitung elemen pada Linked list yang mempunyai atribut spesifik yaitu yang memiliki jenis pengiriman same day
int samedayCount(struct node *head){
	if(head == NULL){
		return 0;
	}
	struct node *iterator = head;
	int count = 0;
	//menyusuri linked list sampai akhir
	while(iterator != NULL){
		//penambahan count hanya dilakukan jika jenis pengiriman adalaha same day
		if(strcmp(iterator->jenisPengiriman, "Same day") == 0){
//			printf("same day count: %d\n", count +1);
			count++;	
		}
		iterator = iterator->next;
	}
	return count;
}

//Fungsi untuk mencetak semua elemen yang terdapat pada Linked List
void printList(struct node *head){
	struct node *Iptr;
	int count = 1;
	if(head != NULL){ //jika linked list mempunyai elemen
		Iptr = head;
		//menyusuri linked list sampai akhir sambil mencetak setiap elemennya
		while(Iptr != NULL){
			printf("\n%d.%-18s: %s\n%-20s: %s\n%-20s: %s\n%-20s: %s\n"
					"%-20s: %d\n%-20s: %d Kg\n%-20s: %s, %s\n%-20s: %s\n%-20s: %s\n", 
					count, "Nama Pengirim", Iptr->namaPengirim, "No. Telpon Pengirim", Iptr->notelpPengirim, 
					"Nama Penerima", Iptr->namaPenerima, "No. Telp. Penerima", Iptr->notelpPenerima,
					"Nomor Resi", Iptr->resi, "Berat paket", Iptr->berat, "Alamat", Iptr->alamat, 
					Iptr->provinsiTujuan, "Opsi pengiriman", Iptr->jenisPengiriman, "Status", Iptr->status);
			Iptr = Iptr->next;
			count++;
		}
		return;
	}else{ //jika tidak
		printf("Antrian kosong!\n");
		return;
	}
}

//Fungsi untuk mencari elemen tertentu pada linked list
void search(struct node *head, struct node *founded, int keyNumber, int *found){
	if(head == NULL){ //jika linked list tidak memiliki elemen, fungsi akan langsung return
		*found = 0;
		return;
	}
	struct node *iterator = head;
	if(iterator->resi == keyNumber){ 
			*found = 1;
			*founded = *iterator;
			return;
		}
	//menyusuri linked list sampai elemen ditemukan atau sampai akhir jika tidak ditemukan
	while(iterator->next != NULL){
		iterator = iterator->next;
		if(iterator->resi == keyNumber){
			*found = 1;
			*founded = *iterator;
			return;
		}
	}
	*found = 0;
}

//Fungsi untuk melakukan enkripsi password
void encrypt(char *pass){
	int n = strlen(pass), i;
	//melakukan enkripsi secara paralel
	#pragma omp parallel for schedule(static)
	for(i = 0; i < n; i++){
		pass[i] = pass[i] + n;
	}
}

//Fungsi untuk melakukan dekripsi password
void decrypt(char *pass){
	int n = strlen(pass), i;
	//melakukan dekripsi secara paralel
	#pragma omp parallel for schedule(static)
	for(i = 0; i < n; i++){
		pass[i] = pass[i] - n;
	}
}
