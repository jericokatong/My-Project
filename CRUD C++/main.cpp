#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

int ID;

struct Mahasiswa{
	int pk;
	char NIM[20];
	char nama[40];
	char jurusan[40];
};

void checkDataBase(fstream &data);
void checkID(fstream &primaryKey);
int getsize(fstream &data);
void writeData(fstream &data , Mahasiswa &inputMahasiswa , int posisi);
Mahasiswa readData(fstream &data, int posisi);
void addMahasiswa(fstream &data, fstream &primaryKey);
void DisplayMahasiswa(fstream &data);
void UpdateMahasiswa(fstream &data);
void DeleteMahasiswa(fstream &data);

int main()
{
	fstream data,primaryKey;
	int choice;
	char lanjut;

	checkDataBase(data);
	checkID(primaryKey);

	system("cls");
	do{
		cout << "================================" << endl;
		cout << "1. Add Mahasiswa" << endl;
		cout << "2. Display Mahasiswa" << endl;
		cout << "3. Update Mahasiswa" << endl;
		cout << "4. Delete Mahasiswa" << endl;
		cout << "5. Selesai" << endl;
		cout << "================================" << endl;
		cout << "Pilih No : ";
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(),'\n');

		switch (choice){
			case 1 : 
				addMahasiswa(data,primaryKey);
				break;

			case 2 : 
				DisplayMahasiswa(data);
				break;

			case 3 :
				DisplayMahasiswa(data);
				UpdateMahasiswa(data);
				DisplayMahasiswa(data);
				break;

			case 4 :
				DisplayMahasiswa(data);
				DeleteMahasiswa(data);
				DisplayMahasiswa(data);
				break;

			case 5 : 
				goto end;
				break;
		}

		cout << "Lanjut (y/n ) : ";
		cin >> lanjut;

		if (lanjut == 'y' || lanjut == 'Y'){
			system("cls");
		}
		else {
			end:
			break;
		}


	} while (choice != 5);

	cout << "PROGRAM BERAKHIR" << endl;
	cout << "Terima Kasih" << endl;
	
	return 0;
}

void checkDataBase(fstream &data){
	data.open("data.bin", ios::binary | ios::out | ios::in);

	if (!data.is_open()){
		data.close();
		data.open("data.bin",ios::trunc | ios::binary | ios::out | ios::binary);
	}	
}

void checkID(fstream &primaryKey){
	primaryKey.open("PK.txt",ios::out | ios::in);
	if (!primaryKey.is_open()){
		primaryKey.close();
		primaryKey.open("PK.txt",ios::trunc | ios::in | ios::out);

	}
}

int getsize(fstream &data){

	int start , end;
	data.seekg(0,ios::beg);
	start = data.tellg();
	data.seekg(0,ios::end);
	end = data.tellg();

	return (end-start)/sizeof(Mahasiswa);
}

void writeData(fstream &data , Mahasiswa &inputMahasiswa , int posisi){
	data.seekp((posisi-1)*sizeof(Mahasiswa),ios::beg);
	data.write(reinterpret_cast<char*>(&inputMahasiswa),sizeof(Mahasiswa));

}

Mahasiswa readData(fstream &data, int posisi){
	Mahasiswa BacaMahasiswa;
	data.seekg((posisi-1)*sizeof(Mahasiswa),ios::beg);
	data.read(reinterpret_cast<char*>(&BacaMahasiswa),sizeof(Mahasiswa));

	return BacaMahasiswa;
}

void addMahasiswa(fstream &data, fstream &primaryKey){
	Mahasiswa inputMahasiswa;
	int size = getsize(data);

	if (size == 0){
		inputMahasiswa.pk = 0;
		ID = 0;
		primaryKey << inputMahasiswa.pk;
	}
	else {
		primaryKey >> ID;
		ID++;
		inputMahasiswa.pk = ID;
		primaryKey.close();
		primaryKey.open("PK.txt",ios::out);
		primaryKey << ID;
	}

	cout << "Masukan NIM : ";
	cin.getline(inputMahasiswa.NIM,20);
	cout << "Masukan Nama : ";
	cin.getline(inputMahasiswa.nama,40);
	cout << "Masukan Jurusan : ";
	cin.getline(inputMahasiswa.jurusan,40);

	writeData(data,inputMahasiswa,size+1);
}

void DisplayMahasiswa(fstream &data){
	Mahasiswa displayData;
	int size = getsize(data);

	cout << "No\tPk\tNIM\tNama\tjurusan" << endl;
	for (int i=1;i<=size;i++){
		displayData = readData(data,i);
		cout << i << "\t";
		cout << displayData.pk << "\t";
		cout << displayData.NIM << "\t";
		cout << displayData.nama << "\t";
		cout << displayData.jurusan << endl;
	}
}

void UpdateMahasiswa(fstream &data){
	int nomor;
	char pilihan;
	Mahasiswa Update;
	cout << "Update Nomor : ";
	cin >> nomor;

	Update = readData(data,nomor);
	cout << "No      : " << nomor << endl;
	cout << "Pk      : " << Update.pk << endl;
	cout << "NIM     : " << Update.NIM << endl;
	cout << "Nama    : " << Update.nama << endl;
	cout << "Jurusan : " << Update.jurusan << endl;

	cout << "Do you want to Update (y/n) ? ";
	cin >> pilihan;
	cin.ignore(numeric_limits<streamsize>::max(),'\n');

	if (pilihan == 'y' || pilihan == 'Y'){
		cout << "Masukan NIM : ";
		cin.getline(Update.NIM,20);
		cout << "Masukan Nama : ";
		cin.getline(Update.nama,40);
		cout << "Masukan Jurusan : ";
		cin.getline(Update.jurusan,40);

		writeData(data,Update,nomor);
	}
	else {
		cout << "-------------------" << endl;
		cout << "Data isn't updated" << endl;
		cout << "-------------------" << endl;
	}
}

void DeleteMahasiswa(fstream &data){
	int nomor;
	char pilihan;
	int size = getsize(data);
	int offset;
	Mahasiswa hapusMhs;
	Mahasiswa fillMhs;
	fstream temp;
	cout << "Delete No : ";
	cin >> nomor;

	hapusMhs = readData(data,nomor);
	cout << "No      : " << nomor << endl;
	cout << "Pk      : " << hapusMhs.pk << endl;
	cout << "NIM     : " << hapusMhs.NIM << endl;
	cout << "Nama    : " << hapusMhs.nama << endl;
	cout << "Jurusan : " << hapusMhs.jurusan << endl;

	cout << "Do you want to Delete (y/n) ? ";
	cin >> pilihan;
	cin.ignore(numeric_limits<streamsize>::max(),'\n');

	if (pilihan == 'y' || pilihan == 'Y'){
		temp.open("temp.bin", ios::trunc | ios::out | ios::in | ios::binary);
		offset = 0;

		for (int i=1 ; i<=size ; i++){
			fillMhs = readData(data,i);
			if (hapusMhs.pk != fillMhs.pk){
				writeData(temp,fillMhs,i-offset);
			}
			else {
				offset++;
			}
		}

		size = getsize(temp);
		data.close();
		data.open("data.bin", ios::trunc | ios::out | ios::in | ios::binary);
		data.close();
		data.open("data.bin",  ios::out | ios::in | ios::binary);

		for (int i=1 ; i<=size ; i++){
			fillMhs = readData(temp,i);
			writeData(data,fillMhs,i);
		}

	}
	else {
		cout << "Data isn't Deleted" << endl;
	}
}