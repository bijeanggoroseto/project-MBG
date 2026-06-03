#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
using namespace std;

struct  User{
    string nama, password;
};

struct Bahan{
    int jumlah;
    string nama, supplier;

    void print(){
        cout << left
             << "| " << setw(18) << nama 
             << "| " << setw(25) << supplier
             << "| " << setw(10) << jumlah
             << setw(1) << "|"
             << '\n';
    }
};

struct BahanMasak{
    int jumlah;
    Bahan* bahan;
};

struct Masakan{
    string nama;
    vector<BahanMasak> bahanMasak;
    vector<string> langkahMasak;
};

int inputInt(string text){
    int test;
    while (true) {
        cout << text;
        cin >> test;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }
        if (cin.peek() != '\n') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Input.\n";
            continue;
        }
        else{break;}
    }
    cin.ignore();
    return test;
}

char inputYesOrNo(string text){
    char test;
    while (true) {
        cout << text;
        cin >> test;
        tolower(test);
        if (cin.peek() != '\n' || (test != 'n' && test != 'y')) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Input.\n";
            continue;
        }
        else{break;}
    }
    cin.ignore();
    return test;
}

void enterToContinue(){
    cout << "\ntekan enter untuk lanjut.";
    string line;
    getline(cin, line);
}

int searchNamaBahan(vector<Bahan*>& bahan, string& target, int left, int right){
    if(left > right){return -1;}
    int mid = (left + right)/2;
    if(bahan[mid]->nama == target){return  mid;}
    else if(target < bahan[mid]->nama){return searchNamaBahan(bahan, target, left, mid - 1);}
    else{return searchNamaBahan(bahan, target, mid + 1, right);}
}

template <typename T>
int searchNama(vector<T>& arr, string& target, int left, int right){
    if(left > right){return -1;}
    int mid = (left + right)/2;
    if(arr[mid].nama == target){return  mid;}
    else if(target < arr[mid].nama){return searchNama(arr, target, left, mid - 1);}
    else{return searchNama(arr, target, mid + 1, right);}
}

void insertSortedNamaBahan(vector<Bahan*>& bahan, Bahan* target){
    bahan.push_back(target);
    int last = bahan.size() - 1;
    Bahan* temp = bahan[last];
    int j = last - 1;
    while(j >= 0 && bahan[j]->nama > temp->nama){
        bahan[j + 1] = bahan[j];
        j--;
    }
    bahan[j + 1] = temp;  
}

template <typename T>
void insertSortedNama(vector<T>& arr, T target){
    arr.push_back(target);
    int last = arr.size() - 1;
    T temp = arr[last];
    int j = last - 1;
    while(j >= 0 && arr[j].nama > temp.nama){
        arr[j + 1] = arr[j];
        j--;
    }
    arr[j + 1] = temp;  
}

void simpanFileBahan(vector<Bahan*>& bahan){
    int size = bahan.size();
    ofstream FILE("Bahan.txt");
    if(!FILE.is_open()){
        cout << "file bahan gagal dibuka" << endl;
        return;
    }
    for(int i = 0; i < size; i++){
        FILE << bahan[i]->nama << "\n";
        FILE << bahan[i]->supplier << "\n";
        FILE << bahan[i]->jumlah << "\n";
    }
    FILE.close();
}

void simpanFileMasakan(vector<Masakan>& arr){
    int size = arr.size(), subsize;
    ofstream FILE("Makanan.txt");
    if(!FILE.is_open()){
        cout << "file makanan gagal dibuka" << endl;
        return;
    }
    for(int i = 0; i < size; i++){
        FILE << arr[i].nama << "\n";
        subsize = arr[i].bahanMasak.size();
        for(int j = 0; j < subsize; j++){
            FILE << arr[i].bahanMasak[j].bahan->nama << "\n";
            FILE << arr[i].bahanMasak[j].jumlah << "\n";
        }
        FILE << "###\n";
        subsize = arr[i].langkahMasak.size();
        for(int j = 0; j < subsize; j++){
            FILE << arr[i].langkahMasak[j] << "\n";
        }
        FILE << "###\n";
    }
    FILE.close();
}

void simpanFileUser(vector<User>& arr){
    int size = arr.size();
    ofstream FILE("User.txt");
    if(!FILE.is_open()){
        cout << "file user gagal dibuka" << endl;
        return;
    }

    for(int i = 0; i < size; i++){
        FILE << arr[i].nama << "\n";
        FILE << arr[i].password << "\n";
    }
    
    FILE.close();
}

void initFile(vector<Bahan*>& bahan, vector<Masakan>& masakan){
    Bahan tempBahan;
    Masakan tempMasakan;
    
    ifstream FILE("Bahan.txt");
    if(!FILE.is_open()){cout << "gagal membuka file bahan" << endl; return;}

    while(getline(FILE, tempBahan.nama)){
        getline(FILE, tempBahan.supplier);
        FILE >> tempBahan.jumlah;
        FILE.ignore();
        Bahan* p = new Bahan(tempBahan);
        insertSortedNamaBahan(bahan, p);
    }
    FILE.close();

    bool valid = true;
    FILE.open("Masakan.txt");
    if(!FILE.is_open()){cout << "gagal membuka file makanan" << endl; return;}
    while(getline(FILE, tempMasakan.nama)){
        string tempString;
        while(true){
            BahanMasak tempBahanMasak;
            getline(FILE, tempString);
            if(tempString == "###"){break;}
            int index = searchNamaBahan(bahan, tempString, 0, bahan.size() - 1);
            if(index == -1){valid = false; break;}
            tempBahanMasak.bahan = bahan[index];
            FILE >> tempBahanMasak.jumlah; 
            FILE.ignore();
            tempMasakan.bahanMasak.push_back(tempBahanMasak);
        }

        if(!valid){
            cout << "masakan tidak valid karena data bahan masakan tidak ada di data bahan." << endl;
            break;
        }

        while(true){
            getline(FILE, tempString);
            if(tempString == "###"){break;}
            tempMasakan.langkahMasak.push_back(tempString);
        }
        insertSortedNama(masakan, tempMasakan);
        tempMasakan.bahanMasak.clear();
        tempMasakan.langkahMasak.clear();
    }
    FILE.close();
}

void tampilkanBahan(vector<Bahan*>& bahan){
    int size = bahan.size();
    if(size == 0){cout << "\nlist bahan masih kosong." << endl;cin.ignore(); enterToContinue(); return;}
    cout << string(60, '-') << endl;
    cout << left
         << setw(20) << "| nama" 
         << setw(27) << "| supplier"
         << setw(12) << "| jumlah"
         << setw(1) << "|"
         << '\n';
    cout << string(60, '-') << endl;
    for(int i = 0; i < size; i++){
        bahan[i]->print();
    }
    cout << string(60, '-') << endl;
    cout << "Semua data telah ditampilkan";
    cin.ignore(); enterToContinue();
}

void tampilkanMasakan(vector<Masakan>& masakan){
    int size = masakan.size();
    if(size == 0){cout << "\nlist bahan masih kosong." << endl; cin.ignore(); enterToContinue(); return;}
    cout << string(42, '-') << endl;
    for(int i = 0; i < size; i++){
        cout << "| " << i+1;
        cout <<". " << left << setw(35) << masakan[i].nama << "|" << endl;
        cout << string(42, '-') << endl;;
    }
    cout << "Semua data telah ditampilkan"<< endl;
    cin.ignore(); enterToContinue();
}

void sortingBahanJumlahAsc(vector<Bahan*> bahan){
    int size = bahan.size();
    for(int i = 1; i < size; i++){
        Bahan* temp = bahan[i];
        int j = i-1;
        while(j >= 0 && bahan[j]->jumlah > temp->jumlah){
            bahan[j + 1] = bahan[j];
            j--;
        }
        bahan[j + 1] = temp;  
    }
    tampilkanBahan(bahan);
}

void sortingBahanJumlahDesc(vector<Bahan*> bahan){
    int size = bahan.size();
    for(int i = 1; i < size; i++){
        Bahan* temp = bahan[i];
        int j = i-1;
        while(j >= 0 && bahan[j]->jumlah < temp->jumlah){
            bahan[j + 1] = bahan[j];
            j--;
        }
        bahan[j + 1] = temp;  
    }
    tampilkanBahan(bahan);
}

bool regis(vector<User>& users){

    cout << string(50, '=') << endl;
    cout << "|" << string(20, ' ') << "Register" << string(20, ' ') << "|" << endl;
    cout << string(50, '=') << endl;

    User temp;
    cin.ignore();
    cout << "masukan username: ";
    getline(cin, temp.nama);
    if(searchNama(users, temp.nama, 0, users.size() - 1) != -1){
        cout << "username sudah ada. pilih username yang lain." << endl;
        enterToContinue();
        return false;
    }
    cout << "masukan password: ";
    getline(cin, temp.password);
    insertSortedNama(users, temp);
    cout << "Berhasil Registrasi.";
    enterToContinue();

    simpanFileUser(users);
    return true;
}

bool login(vector<User>& users){
    string temp;
    cin.ignore();

    cout << string(50, '=') << endl;
    cout << "|" << string(21, ' ') << "Login" << string(22, ' ') << "|" << endl;
    cout << string(50, '=') << endl;

    for(int i = 2;  i >= 0; i--){
        cout<< "masukan username: ";
        getline(cin, temp);
        int userId = searchNama(users, temp, 0, users.size() - 1);
        if(userId == -1){
            cout << "username tidak ada. kesempatan sisa: " << i << endl;
            continue;
        }
        cout<< "masukan password: ";
        getline(cin, temp);
        if(temp == users[userId].password){
            cout << "Login berhasil.";
            enterToContinue();
            return true;
        }
        cout << "password salah. Kesempatan sisa: " << i << endl;;
    }
    cout << "kesempatan habis.";
    enterToContinue();
    return false;
}

bool loginOrRegis(){
    vector<User> users;
    User tempUser;

    ifstream userFILE("User.txt");
    if(!userFILE.is_open()){cout << "gagal membuka file user"; return false;}
    while(getline(userFILE, tempUser.nama)){
        getline(userFILE, tempUser.password);
        users.push_back(tempUser);
    }
    userFILE.close();
    
    char menu;
    do{
        cout << string(50, '=') << endl;
        cout << "|" << string(17, ' ') << "MBG MANGEMENT" << string(18, ' ') << "|" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "pilih menu: ";
        cin >> menu;

        if(cin.peek() != '\n'){
            cout << "pilihan tidak ada di menu.\n"; 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            enterToContinue();
            continue;
        }

        if(menu == '1'){
            if(login(users)){break;}
        }
        else if(menu == '2'){
            if(regis(users)){break;}
        }
        else if(menu == '3'){
            return false;
            break;
        }
        else{
            cout << "pilihan tidak ada di menu." << endl; 
            cin.ignore();
            enterToContinue();
        }
    }while(menu != 3);
    return true;
}

void inputBahan(vector<Bahan*>& bahan){
    int quantity, size = bahan.size();
    quantity = inputInt("berapa bahan yang ingin diinput: ");
    quantity += size;
    for(int i = size; i < quantity; i++){
        Bahan* temp;
        cin.ignore();
        cout << "bahan ke-" << i+1 << endl;
        cout << "masukan nama bahan: "; 
        getline(cin, temp->nama);
        cout << "masukan nama supplier: "; 
        getline(cin, temp->supplier);
        temp->jumlah = inputInt("masukan jumlah bahan: ");
        insertSortedNamaBahan(bahan, temp);
    }
    cout<< "\nSemua data selesai diinput YEAYYYYY";
    cin.ignore(); enterToContinue();
}

void inputMasakan(vector<Masakan>& masakan, vector<Bahan*>& bahan){
    int quantity, size = masakan.size();
    quantity = inputInt("berapa makanan yang ingin diinput: ");
    quantity += size;

    for(int i = size; i < quantity; i++){
        Masakan temp;
        cin.ignore();
        cout << "masakan ke-" << i+1 << endl;
        cout << "masukan nama masakan: "; 
        getline(cin, temp.nama);

        bool bahanMasakanValid;
        cout << "masukan bahan-bahan (ketik '###' pada baris baru untuk mengakhiri): " << endl; 
        while(true){
            BahanMasak tempBahan;
            string tempNama;
            getline(cin, tempNama);
            if(tempNama == "###"){break;}
            int index = searchNamaBahan(bahan, tempNama, 0, bahan.size() - 1);
            if(index == -1){
                cout << "bahan tidak ada di data. Pastikan bahan sudah ada di data bahan." << endl;
                bahanMasakanValid = false;
                break;
            }
            else{
                tempBahan.bahan = bahan[index];
            }
            tempBahan.jumlah = inputInt("berapa jumlah bahan yang akan dimasak: ");
            temp.bahanMasak.push_back(tempBahan);
        }
        if(!bahanMasakanValid){continue;}

        cout << "masukan langkah-langkah (ketik '###' pada baris baru untuk mengakhiri): " << endl; 
        while(true){
            string tempLangkah;
            getline(cin, tempLangkah);
            if(tempLangkah == "###"){break;}
            temp.langkahMasak.push_back(tempLangkah);
        }
        insertSortedNama(masakan, temp);
    }
    cout<< "\nSemua data selesai diinput YEAYYYY"<< endl;
    enterToContinue();
}

int main(){
    vector<Bahan*> bahan;
    vector<Masakan> masakan;
    char mainMenu, subMenu;

    if(!loginOrRegis()){return 0;}

    cout<< "\nSELAMAT DATANG ADMIN SPPG UPNVY\n"<< endl;

    initFile(bahan, masakan);
    do{
        cout << string(50, '=') << endl;
        cout << "|" << string(19, ' ') << "Menu Utama" << string(19, ' ') << "|" << endl;
        cout << string(50, '=') << endl;
        
        cout <<"1. Atur Bahan Makanan"<<endl;
        cout <<"2. Atur Menu Makanan"<<endl;
        cout <<"3. KELUAR!"<<endl;
        cout<<"pilih menu(1-3): "; cin >> mainMenu;

        if(cin.peek() != '\n'){
            cout << "pilihan tidak ada di menu.\n"; 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            enterToContinue();
            continue;
        }

        if(mainMenu == '1'){
            do{
                cout << string(50, '=') << endl;
                cout << "|" << string(19, ' ') << "Atur Bahan" << string(19, ' ') << "|" << endl;
                cout << string(50, '=') << endl;

                cout<< "1. Input bahan masakan" <<endl;
                cout<< "2. Tambah jumlah bahan" <<endl;
                cout<< "3. Edit bahan" << endl;
                cout<< "4. Hapus bahan" <<endl;
                cout<< "5. Tampilkan bahan masakan" <<endl;
                cout<< "6. Cari bahan masakan" <<endl;
                cout<< "7. keluar" <<endl;
                cout<< "pilih menu (1-6): "; cin >> subMenu;

                if(cin.peek() != '\n'){
                    cout << "pilihan tidak ada di menu.\n"; 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    enterToContinue();
                    continue;
                }

                switch(subMenu){
                    case '1':{
                        cout << string(50, '=') << endl;
                        cout << "|" << string(18, ' ') << "Input Bahan" << string(18, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        inputBahan(bahan);
                        break;
                    }
                    case '2':{
                        cout << string(50, '=') << endl;
                        cout << "|" << string(16, ' ') << "Tambah Stok Bahan" << string(15, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        string nama;
                        int jumlah, size = bahan.size();
                        cin.ignore();
                        cout <<"bahan yang ingin ditambah: "; getline(cin, nama);
                        int id = searchNamaBahan(bahan, nama, 0, size - 1);
                        if(id == -1){cout << "Bahan tidak ada.\n"; break;}

                        jumlah = inputInt("jumlah yang ingin ditambah: ");
                        bahan[id]->jumlah += jumlah;
                        cout << "berhasil menambah jumlah bahan" << endl;
                        cin.ignore(); enterToContinue();
                        break;
                    }
                    case '3':{
                        cout << string(50, '=') << endl;
                        cout << "|" << string(19, ' ') << "Edit Bahan" << string(19, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        string target;
                        cout << "masukan nama bahan: ";
                        cin.ignore();
                        getline(cin, target);

                        int id = searchNamaBahan(bahan, target, 0, bahan.size() - 1);
                        if(id == -1){cout << "bahan tidak ditemukan" << endl; enterToContinue(); break;}

                        cout << "nama baru: ";
                        getline(cin, bahan[id]->nama);
                        cout << "supplier baru: ";
                        getline(cin, bahan[id]->supplier);
                        cout << "Bahan berhasil diedit" << endl;
                        enterToContinue();
                        break;
                    }
                    case '4':{
                        cout << string(50, '=') << endl;
                        cout << "|" << string(19, ' ') << "Hapus Bahan" << string(18, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        string target;
                        cout << "masukan nama bahan: ";
                        cin.ignore();
                        getline(cin, target);

                        int id = searchNamaBahan(bahan, target, 0, bahan.size() - 1);
                        if(id == -1){cout << "bahan tidak ditemukan" << endl; enterToContinue(); break;}

                        vector<string> namaMasakan;
                        for(int i = 0; i < masakan.size(); i++){
                            for(BahanMasak bm : masakan[i].bahanMasak){
                                if(bm.bahan->nama == target){
                                    namaMasakan.push_back(masakan[i].nama);
                                }
                            }
                        }

                        if(!namaMasakan.empty()){
                            cout << "Bahan digunakan pada menu masakan lain. Jika menghapus bahan ini maka semua masakan yang menggunakannya juga akan dihapus.\n";
                            if(inputYesOrNo("Yakin ingin menghapus? (y/n): ") == 'n'){
                                break;
                            }       
                        }

                        for(string nama : namaMasakan){
                            int idMasakan = searchNama(masakan, nama, 0, masakan.size() - 1);
                            masakan.erase(masakan.begin() + idMasakan);
                        }

                        delete bahan[id];
                        bahan.erase(bahan.begin() + id);

                        cout << "Bahan berhasil dihapus." << endl;
                        enterToContinue();
                        break;
                    }
                    case '5':{
                        int menuTampilan;

                        cout << string(50, '=') << endl;
                        cout << "|" << string(16, ' ') << "Pilihan Tampilan" << string(16, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        cout<<"1. Berdasarkan abjad";
                        cout<<"\n2. Berdasarkan jumlah stok terkecil";
                        cout<<"\n3. Berdasarkan jumlah stok terbanyak";
                        cout<<"\npilih menu (1-3): "; cin >> menuTampilan;

                        if(cin.peek() != '\n'){
                            cout << "pilihan tidak ada di menu.\n"; 
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                            enterToContinue();
                            continue;
                        }

                        switch(menuTampilan){
                            case 1:{
                                tampilkanBahan(bahan);
                                break;
                            }
                            case 2:{
                                sortingBahanJumlahAsc(bahan);
                                break;
                            }
                            case 3:{
                                sortingBahanJumlahDesc(bahan);
                                break;
                            }
                            default:{
                                cout<<"pilihan tidak ada di menu";
                                cin.ignore();
                                enterToContinue(); 
                                break;}
                        }
                        break;
                    }
                    case '6':{
                        cout << string(50, '=') << endl;
                        cout << "|" << string(18, ' ') << "Detail Bahan" << string(18, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        string target;
                        cout << "masukan nama bahan: ";
                        cin.ignore();
                        getline(cin, target);
                        int id = searchNamaBahan(bahan, target, 0, bahan.size() - 1);
                        if(id == -1){cout << "bahan tidak ditemukan" << endl; enterToContinue; break;}

                        cout << "--------------------------------" << endl;
                        cout << "nama: " << bahan[id]->nama << endl;
                        cout << "supplier: " << bahan[id]->supplier << endl;
                        cout << "jumlah: " << bahan[id]->jumlah << endl;
                        cout << "--------------------------------" << endl;
                        enterToContinue();
                        break;
                    }
                    case '7':{
                        break;
                    }
                    default:{
                        cout << "pilihan tidak ada di menu, masukin nomor yang bener!!" << endl;
                        cin.ignore();
                        enterToContinue();
                        break;
                    }
                }
            }while(subMenu != '6');
        }
        else if(mainMenu == '2'){
            do{
                cout << string(50, '=') << endl;
                cout << "|" << string(18, ' ') << "Atur Masakan" << string(18, ' ') << "|" << endl;
                cout << string(50, '=') << endl;

                cout<< "1. Input menu masakan" <<endl;
                cout<< "2. Hapus menu masakan" <<endl;
                cout<< "3. Tampilkan menu masakan" <<endl;
                cout<< "4. Cari  menu masakan" <<endl;
                cout<< "5. Masak masakan" <<endl;
                cout<< "6. keluar" <<endl;
                cout<< "pilih menu (1-6): "; cin>> subMenu;

                if(cin.peek() != '\n'){
                    cout << "pilihan tidak ada di menu.\n"; 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    enterToContinue();
                    continue;
                }

                switch(subMenu){
                    case '1':{
                        cout << string(50, '=') << endl;
                        cout << "|" << string(18, ' ') << "Input Masakan" << string(17, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        inputMasakan(masakan, bahan);
                        break;
                    }
                    case '2':{
                        string target;
                        cout << string(50, '=') << endl;
                        cout << "|" << string(18, ' ') << "Hapus Masakan" << string(17, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        cout << "masukan nama makanan: ";
                        cin.ignore();
                        getline(cin, target);
                        int id = searchNama(masakan, target, 0, masakan.size() - 1);
                        if(id == -1){cout << "makanan tidak ditemukan" << endl; break;}
                        masakan.erase(masakan.begin() + id);
                        cout << "makanan berhasil dihapus" << endl;
                        enterToContinue();
                        break;
                    }
                    case '3':{
                        cout << string(50, '=') << endl;
                        cout << "|" << string(17, ' ') << "Daftar Masakan" << string(17, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        tampilkanMasakan(masakan);
                        break;
                    }
                    case '4':{
                        string target;
                        cout << string(50, '=') << endl;
                        cout << "|" << string(18, ' ') << "Cari Masakan" << string(18, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        cout << "masukan nama makanan: ";
                        cin.ignore();
                        getline(cin, target);
                        int id = searchNama(masakan, target, 0, bahan.size() - 1);
                        if(id == -1){cout << "makanan tidak ditemukan" << endl; enterToContinue(); break;}

                        cout << "--------------------------------" << endl;
                        cout << "nama: " << masakan[id].nama << endl;
                        int subsize = masakan[id].bahanMasak.size();
                        cout << "\nbahan-bahan: " << endl;
                        for(int i = 0; i < subsize; i++){
                            cout << i + 1 << ". " << masakan[id].bahanMasak[i].bahan->nama << " x" << masakan[id].bahanMasak[i].jumlah << endl;
                        }
                        subsize = masakan[id].langkahMasak.size();
                        cout << "\nlangkah memasak: " << endl;
                        for(int i = 0; i < subsize; i++){
                            cout << i + 1 << ". " << masakan[id].langkahMasak[i] << endl;
                        }
                        cout << "--------------------------------" << endl;
                        enterToContinue();

                        break;
                    }
                    case '5':{
                        string target;
                        cout << string(50, '=') << endl;
                        cout << "|" << string(18, ' ') << "Masak - Masak" << string(17, ' ') << "|" << endl;
                        cout << string(50, '=') << endl;

                        cout << "masukan nama makanan: ";
                        cin.ignore();
                        getline(cin, target);
                        int idMasakan = searchNama(masakan, target, 0, masakan.size() - 1);
                        if(idMasakan == -1){cout << "makanan tidak ditemukan" << endl; enterToContinue(); break;}

                        int jumlahMasak;
                        jumlahMasak = inputInt("berapa jumlah yang ingin dimasak: ");

                        vector<int> idBahan;
                        bool bahanCukup = true;
                        for(BahanMasak bm : masakan[idMasakan].bahanMasak){
                            if(bm.bahan->jumlah < bm.jumlah * jumlahMasak){
                                cout << "bahan tidak cukup untuk memasak." << endl;
                                bahanCukup = false; 
                                break;
                            }
                        }
                        if(bahanCukup){
                            for(BahanMasak bm : masakan[idMasakan].bahanMasak){
                                bm.bahan->jumlah -= bm.jumlah * jumlahMasak;
                            }
                            cout << "selesai memasak." << endl;
                        }
                        enterToContinue();
                        break;
                    }
                    case '6':{
                        break;
                    }
                    default:{
                        cout << "pilihan tidak ada di menu, gausah main-main ini orang :(" << endl;
                        cin.ignore();
                        enterToContinue();
                        break;
                    }
                }
            }while(subMenu != '6');

        }
        else if(mainMenu == '3'){
            cout << "bye bye admin SPPG Awokawokawok"<<endl;
            simpanFileBahan(bahan);
            simpanFileMasakan(masakan);
        }
        else{
            cout << "pilihan tidak ada di menu." << endl;
            cin.ignore();
            enterToContinue();
        }

    }while(mainMenu != '3');
}