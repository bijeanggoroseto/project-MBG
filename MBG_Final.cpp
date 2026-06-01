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
             << "| " << setw(18) << supplier
             << "| " << setw(13) << jumlah
             << setw(1) << "|"
             << '\n';
    }
};

struct BahanMasak{
    int jumlah;
    string nama;
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
        return test;
    }
}

void enterToContinue(){
    cout << "\ntekan enter untuk lanjut.";
    string line;
    getline(cin, line);
}

template <typename T>
int searchNama(vector<T> arr, string target, int left, int right){
    if(left > right){return -1;}
    int mid = (left + right)/2;
    if(arr[mid].nama == target){return  mid;}
    else if(target < arr[mid].nama){return searchNama(arr, target, left, mid - 1);}
    else{return searchNama(arr, target, mid + 1, right);}
}

void simpanFileBahan(vector<Bahan>& arr){
    int size = arr.size();
    ofstream FILE("Bahan.txt");
    if(!FILE.is_open()){
        cout << "file bahan gagal dibuka" << endl;
        return;
    }
    for(int i = 0; i < size; i++){
        FILE << arr[i].nama << "\n";
        FILE << arr[i].supplier << "\n";
        FILE << arr[i].jumlah << "\n";
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
            FILE << arr[i].bahanMasak[j].nama << "\n";
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

void initFile(vector<Bahan>& bahan, vector<Masakan>& makanan){
    Bahan temp1;
    string strtemp;
    Masakan temp2;
    BahanMasak temp3;
    
    ifstream FILE("Bahan.txt");
    if(!FILE.is_open()){cout << "gagal membuka file bahan" << endl; return;}

    while(getline(FILE, temp1.nama)){
        getline(FILE, temp1.supplier);
        FILE >> temp1.jumlah;
        FILE.ignore();
        bahan.push_back(temp1);
    }
    FILE.close();
    FILE.open("Makanan.txt");
    if(!FILE.is_open()){cout << "gagal membuka file makanan" << endl; return;}
    while(getline(FILE, temp2.nama)){
        while(true){
            getline(FILE, strtemp);
            if(strtemp == "###"){break;}
            temp3.nama = strtemp;
            FILE >> temp3.jumlah;
            FILE.ignore();
            temp2.bahanMasak.push_back(temp3);
        }
        while(true){
            getline(FILE, strtemp);
            if(strtemp == "###"){break;}
            temp2.langkahMasak.push_back(strtemp);
        }
        makanan.push_back(temp2);
        temp2.bahanMasak.clear();
        temp2.langkahMasak.clear();
    }
    FILE.close();
}

void tampilkanBahan(vector<Bahan>& arr){
    int size = arr.size();
    if(size == 0){cout << "\nlist bahan masih kosong." << endl;cin.ignore(); enterToContinue(); return;}
    cout << string(56, '-') << endl;
    cout << left
         << setw(20) << "| nama" 
         << setw(20) << "| supplier"
         << setw(15) << "| jumlah"
         << setw(1) << "|"
         << '\n';
    cout << string(56, '-') << endl;
    for(int i = 0; i < size; i++){
        arr[i].print();
    }
    cout << string(56, '-') << endl;
    cout << "Semua data telah ditampilkan";
    cin.ignore(); enterToContinue();
}

void tampilkanMasakan(vector<Masakan>& arr){
    int size = arr.size();
    if(size == 0){cout << "\nlist bahan masih kosong." << endl; cin.ignore(); enterToContinue(); return;}
    cout<<"\n=================================\n";
    for(int i = 0; i < size; i++){
        cout<< i+1;
        cout<<". "<< arr[i].nama;
        cout<<"\n================================="<<endl;
    }
    cout << "Semua data telah ditampilkan"<< endl;
    cin.ignore(); enterToContinue();
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

void sortingBahanJumlahAsc(vector<Bahan> arr){
    int size = arr.size();
    for(int i = 1; i < size; i++){
        Bahan temp = arr[i];
        int j = i-1;
        while(j >= 0 && arr[j].jumlah > temp.jumlah){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;  
    }
    tampilkanBahan(arr);
}

void sortingBahanJumlahDesc(vector<Bahan> arr){
    int size = arr.size();
    for(int i = 1; i < size; i++){
        Bahan temp = arr[i];
        int j = i-1;
        while(j >= 0 && arr[j].jumlah < temp.jumlah){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;  
    }
    tampilkanBahan(arr);
}

bool regis(vector<User>& users){
    cout << "============= REGISTRASI =============" << endl;
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
    cout << "=============== LOGIN ===============" << endl;
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
        cout << "=================== MBG =================" << endl;
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

void inputBahan(vector<Bahan>& arr){
    int quantity, size = arr.size();
    quantity = inputInt("berapa bahan yang ingin diinput: ");
    quantity += size;
    for(int i = size; i < quantity; i++){
        Bahan temp;
        cin.ignore();
        cout << "bahan ke-" << i+1 << endl;
        cout << "masukan nama bahan: "; 
        getline(cin, temp.nama);
        cout << "masukan nama supplier: "; 
        getline(cin, temp.supplier);
        temp.jumlah = inputInt("masukan jumlah bahan: ");
        insertSortedNama(arr, temp);
    }
    cout<< "\nSemua data selesai diinput YEAYYYYY";
    cin.ignore(); enterToContinue();
}

void inputMasakan(vector<Masakan>& arr){
    int quantity, size = arr.size();
    quantity = inputInt("berapa makanan yang ingin diinput: ");
    quantity += size;
    for(int i = size; i < quantity; i++){
        Masakan temp;
        cin.ignore();
        cout << "makanan ke-" << i+1 << endl;
        cout << "masukan nama makanan: "; 
        getline(cin, temp.nama);
        cout << "masukan bahan-bahan (ketik '###' pada baris baru untuk mengakhiri): " << endl; 
        while(true){
            BahanMasak tempBahan;
            getline(cin, tempBahan.nama);
            if(tempBahan.nama == "###"){break;}
            tempBahan.jumlah = inputInt("berapa jumlah bahan yang akan dimasak: ");
            cin.ignore();
            temp.bahanMasak.push_back(tempBahan);
        }
        cout << "masukan langkah-langkah (ketik '###' pada baris baru untuk mengakhiri): " << endl; 
        while(true){
            string strtemp;
            getline(cin, strtemp);
            if(strtemp == "###"){break;}
            temp.langkahMasak.push_back(strtemp);
        }
        insertSortedNama(arr, temp);
    }
    cout<< "\nSemua data selesai diinput YEAYYYY"<< endl;
    enterToContinue();
}

int main(){
    vector<Bahan> bahan;
    vector<Masakan> masakan;
    char mainMenu, subMenu;
    if(!loginOrRegis()){return 0;}
    cout<< "\nSELAMAT DATANG ADMIN SPPG UPNVY\n"<< endl;

    initFile(bahan, masakan);
    do{
        cout <<"========== MENU UTAMA SPPG UPNVY ==========" << endl;
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
                cout<< "\n========== MENU BAHAN ==========" <<endl;
                cout<< "1. Input bahan makanan" <<endl;
                cout<< "2. Tambah jumlah bahan" <<endl;
                cout<< "3. Hapus bahan" <<endl;
                cout<< "4. Tampilkan bahan makanan" <<endl;
                cout<< "5. Cari bahan makanan" <<endl;
                cout<< "6. keluar" <<endl;
                cout<< "pilih menu (1-6): "; cin >> subMenu;

                if(cin.peek() != '\n'){
                    cout << "pilihan tidak ada di menu.\n"; 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    enterToContinue();
                    continue;
                }

                switch(subMenu){
                    case '1':{
                        inputBahan(bahan);
                        break;
                    }
                    case '2':{
                        string nama;
                        int jumlah, size = bahan.size();
                        cin.ignore();
                        cout <<"bahan yang ingin ditambah: "; getline(cin, nama);
                        int id = searchNama(bahan, nama, 0, size - 1);
                        if(id == -1){cout << "Bahan tidak ada.\n"; break;}

                        jumlah = inputInt("jumlah yang ingin ditambah: ");
                        bahan[id].jumlah += jumlah;
                        cout << "berhasil menambah jumlah bahan" << endl;
                        cin.ignore(); enterToContinue();
                        break;
                    }
                    case '3':{
                        string target;
                        cout << "masukan nama bahan: ";
                        cin.ignore();
                        getline(cin, target);
                        int id = searchNama(bahan, target, 0, bahan.size() - 1);
                        if(id == -1){cout << "bahan tidak ditemukan" << endl; enterToContinue(); break;}
                        bahan.erase(bahan.begin() + id);
                        cout << "bahan berhasil dihapus" << endl;
                        enterToContinue();
                        break;
                    }
                    case '4':{
                        int menuTampilan;
                        cout<<"\n========== MENU TAMPILAN ==========";
                        cout<<"\n1. berdasarkan abjad";
                        cout<<"\n2. berdasarkan jumlah stok terkecil";
                        cout<<"\n3. berdasarkan jumlah stok terbanyak";
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
                    case '5':{
                        string target;
                        cout << "masukan nama bahan: ";
                        cin.ignore();
                        getline(cin, target);
                        int id = searchNama(bahan, target, 0, bahan.size() - 1);
                        if(id == -1){cout << "bahan tidak ditemukan" << endl; break;}
                        cout << "--------------------------------" << endl;
                        cout << "nama: " << bahan[id].nama << endl;
                        cout << "supplier: " << bahan[id].supplier << endl;
                        cout << "jumlah: " << bahan[id].jumlah << endl;
                        cout << "--------------------------------" << endl;
                        enterToContinue();
                        break;
                    }
                    case '6':{
                        simpanFileBahan(bahan);
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
                cout<< "========== MENU MAKANAN ==========" <<endl;
                cout<< "1. Input menu makanan" <<endl;
                cout<< "2. Hapus menu makanan" <<endl;
                cout<< "3. Tampilkan menu makanan" <<endl;
                cout<< "4. Cari  menu makanan" <<endl;
                cout<< "5. Masak makanan" <<endl;
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
                        inputMasakan(masakan);
                        break;
                    }
                    case '2':{
                        string target;
                        cout << "masukan nama makanan: ";
                        cin.ignore();
                        getline(cin, target);
                        int id = searchNama(masakan, target, 0, bahan.size() - 1);
                        if(id == -1){cout << "makanan tidak ditemukan" << endl; break;}
                        masakan.erase(masakan.begin() + id);
                        cout << "makanan berhasil dihapus" << endl;
                        enterToContinue();
                        break;
                    }
                    case '3':{
                        tampilkanMasakan(masakan);
                        break;
                    }
                    case '4':{
                        string target;
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
                            cout << i + 1 << ". " << masakan[id].bahanMasak[i].nama << " x" << masakan[id].bahanMasak[i].jumlah << endl;
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
                        cout << "masukan nama makanan: ";
                        cin.ignore();
                        getline(cin, target);
                        int idMasakan = searchNama(masakan, target, 0, masakan.size() - 1);
                        if(idMasakan == -1){cout << "makanan tidak ditemukan" << endl; enterToContinue(); break;}

                        int jumlahMasak;
                        jumlahMasak = inputInt("berapa jumlah yang ingin dimasak: ");

                        vector<int> idBahan;
                        bool bahanCukup = true;
                        for(BahanMasak s : masakan[idMasakan].bahanMasak){
                            int tempId = searchNama(bahan, s.nama, 0, bahan.size() - 1);
                            if(tempId == -1 || bahan[tempId].jumlah < s.jumlah * jumlahMasak){
                                cout << "Bahan tidak cukup.\n";
                                bahanCukup = false;
                                break;
                            }
                            else{idBahan.push_back(tempId);}
                        }
                        if(bahanCukup){
                            int size = idBahan.size();
                            for(int i = 0; i < size; i++){
                                bahan[idBahan[i]].jumlah -= masakan[idMasakan].bahanMasak[i].jumlah * jumlahMasak;
                            }
                            cout << "selesai memasak" << endl;
                        }
                        cin.ignore(); enterToContinue();
                        break;
                    }
                    case '6':{
                        simpanFileMasakan(masakan);
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
        }
        else{
            cout << "pilihan tidak ada di menu." << endl;
            cin.ignore();
            enterToContinue();
        }

    }while(mainMenu != '3');
}