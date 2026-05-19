#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// Node untuk antrian cuti
struct NodeQ {
    int nip;
    NodeQ* next;
};  
// Queue untuk antrian cuti
struct Queue {
    NodeQ* depan;
    NodeQ* belakang;
};
// Fungsi untuk menambahkan karyawan ke antrian
void enqueue(Queue& q, int nip) {
    NodeQ* baru  = new NodeQ; 
    baru->nip    = nip;
    baru->next   = NULL;

    if (q.depan == NULL) { //jika antrian kosong
        q.depan    = baru;
        q.belakang = baru;
    } else { //jika tidak
        q.belakang->next = baru; 
        q.belakang       = baru;
    }
}
// Fungsi untuk menghapus karyawan dari antrian
int dequeue(Queue& q) {
    if (q.depan == NULL) {
        return -1; //antrian kosong
    }

    NodeQ* temp  = q.depan; //simpan node depan buat hapus
    int nip      = q.depan->nip; 
    q.depan      = q.depan->next; //geser
    if (q.depan == NULL) { //kalo antrian habis setelah dequeue
        q.belakang = NULL;
    }
    delete temp; //hapus node yang disimpan
    return nip;
}   

struct Aksi { //untuk menyimpan riwayat aksi cuti
    int nip;
    string jenis;   // saya buat "ajukan" untuk yang di antrian atau "selesai" untuk yang selesai cuti
    bool statusLama; //status cuti sebelum aksi (true = cuti, false = tidak cuti)
};

struct NodeS { //node untuk stack riwayat aksi
    Aksi data;
    NodeS* next; 
};
NodeS* top = NULL; //top stack

void push(Aksi a) { //tambah aksi ke stack
    NodeS* baru  = new NodeS;   
    baru->data   = a;
    baru->next   = top;
    top        = baru;
}

Aksi pop() { //ambil aksi dari stack
    NodeS* temp  = top;
    Aksi a       = top->data;
    top        = top->next;
    delete temp;
    return a;
}

struct Node { //node untuk data karyawan
    int nip;
    string nama;
    string divisi;
    bool statusCuti; //true = sedang cuti, false = tidak cuti
    Queue antrian;   //antrian untuk pengajuan cuti berikutnya
    Node* kiri;
    Node* kanan;
};

Node* buatNode(int nip, string nama, string divisi) {
    Node* baru = new Node;
    baru->nip = nip; 
    baru->nama = nama;
    baru->divisi = divisi;
    baru->statusCuti = false;
    baru->antrian.depan = NULL; //inisialisasi antrian kosong nyambung ke struct Queue
    baru->antrian.belakang = NULL;
    baru->kiri = NULL;
    baru->kanan = NULL;
    return baru;
}
 
//tambsh karyawan
Node* insert(Node* root, int nip, string nama, string divisi) {
    if (root == NULL) {
        return buatNode(nip, nama, divisi);
    }
    if (nip < root->nip) {
        root->kiri = insert(root->kiri, nip, nama, divisi);
    } else if (nip > root->nip) {
        root->kanan = insert(root->kanan, nip, nama, divisi);
    }
    return root;
}

//cari karyawan berdasarkan NIP
Node* search(Node* root, int nip) {
    if (root == NULL || root->nip == nip) {
        return root;
    }
    if (nip < root->nip) {
        return search(root->kiri, nip);
    }
    return search(root->kanan, nip);
}

//tampil semua karyawan
void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->kiri);

    string status;
    if (root->statusCuti == true) {
        status = "Cuti";
    } else {
        status = "Tidak Cuti";
    }
    cout << left;
    cout << "| " << setw(8)  << root->nip
         << "| " << setw(20) << root->nama
         << "| " << setw(15) << root->divisi
         << "| " << setw(12) << status << "|" << endl;

    inorder(root->kanan);
}
//cari node dengan nilai kecil (untuk delete)
Node* cariMin(Node* root) {
    while (root->kiri != NULL) {
        root = root->kiri;
    }
    return root;
}
Node* hapus(Node* root, int nip) {
    if (root == NULL) return root;
    if (nip < root->nip) {
        root->kiri = hapus(root->kiri, nip);
    } else if (nip > root->nip) {
        root->kanan = hapus(root->kanan, nip);
    } else {
        if (root->kiri == NULL) {
            Node* temp = root->kanan;
            delete root;
            return temp;
        } else if (root->kanan == NULL) {
            Node* temp = root->kiri;
            delete root;
            return temp;
        }
        Node* temp = cariMin(root->kanan);
        root->nip = temp->nip;
        root->nama = temp->nama;
        root->divisi = temp->divisi;
        root->statusCuti = temp->statusCuti;
        root->antrian = temp->antrian; // Salin antrian juga
        root->kanan = hapus(root->kanan, temp->nip);
    }
    return root;
}               

int main() {
    Node* root = NULL;
    int pilihan;

    do {
        cout << "=============================" << endl;
        cout << "   SISTEM MANAJEMEN CUTI    " << endl;
        cout << "=============================" << endl;
        cout << "1. Tambah Karyawan" << endl;
        cout << "2. Tampil Karyawan" << endl;
        cout << "3. Ajukan Cuti" << endl;
        cout << "4. Selesai Cuti" << endl;
        cout << "5. Hapus Karyawan" << endl;
        cout << "6. Undo" << endl;
        cout << "7. Tampil Antrian" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) { //tambah karyawan
            int nip, jumlah; 
            string nama, divisi;
            cout << "Berapa karyawan yang ingin ditambahkan? "; 
	        cin >> jumlah;

            for (int i = 0; i < jumlah; i++) { //loop untuk tambah beberapa karyawan
                cout << "\nKaryawan ke-" << (i + 1) << ":\n";
                cout << "NIP    : "; cin >> nip;
            if (search(root, nip) != NULL) { //cek duplikat NIP
                cout << "NIP sudah terdaftar! Karyawan tidak dapat ditambahkan.\n";
                continue;
            } else if (nip < 0) { 
                cout << "NIP tidak valid! Karyawan tidak dapat ditambahkan.\n";
                continue;
            }
                cout << "Nama   : "; cin.ignore(); getline(cin, nama);
                cout << "Divisi : "; getline(cin, divisi);
                root = insert(root, nip, nama, divisi);
                cout << "Data karyawan berhasil ditambahkan" << endl;
        }
    } 

        else if (pilihan == 2) { //tampil karyawan
            if (root == NULL) {
                cout << "Belum ada data karyawan" << endl;
            } else {
                    system("cls");
                    cout << "==== Daftar Karyawan ====" << endl;
                    cout << left;
                    cout << "| " << setw(8)  << "NIP"
                    << "| " << setw(20) << "Nama"
                    << "| " << setw(15) << "Divisi"
                    << "| " << setw(12) << "Status" << "|" << endl;
                    cout << string(62, '-') << endl;

                inorder(root);
            }
        }

        else if (pilihan == 3) { //ajukan cuti
            int nip; 
            cout << "NIP karyawan: "; 
            cin >> nip;
            if (nip < 0) {
                cout << "NIP tidak valid!" << endl;
                continue;
            }
            Node* k = search(root, nip); //untuk cari
            if (k == NULL) {
                cout << "Karyawan tidak ditemukan!" << endl;
            } else if (k->statusCuti == false) { //jika tidak sedang cuti
                Aksi a = {nip, "ajukan", false};
                push(a); //simpan aksi sebelum perubahan
                k->statusCuti = true;
                cout << "Pengajuan cuti berhasil" << endl;
            } else { //jika sudah sedang cuti, masukin karyawan lain ke antrian
                int antrian;
                cout << "Karyawan sedang cuti, \nMasukkan NIP karyawan lain ke antrian: ";
                cin >> antrian;
                if (search(root, antrian) == NULL) {
                    cout << "Karyawan antrian tidak ditemukan!" << endl;
                } else {
                    enqueue(k->antrian, antrian); //masukin ke antrian cuti
                    cout << "Pengajuan cuti untuk NIP " << antrian << " berhasil dikirim ke antrian" << endl;
                } 
            }
        }
        else if (pilihan == 4) { //selesai cuti 
            int nip; 
            cout << "NIP karyawan: "; 
            cin >> nip;
            Node* k = search(root, nip);

            if (k == NULL) {
                cout << "Karyawan tidak ditemukan!" << endl;
            } else if (!k->statusCuti) {
                cout << "Karyawan tidak sedang cuti!" << endl;
            } else {
                Aksi a = {nip, "selesai", true}; //simpan aksi sebelum perubahan (status lama = true, karena sedang cuti)
                push(a); //simpan aksi ke stack untuk menu undo
                k->statusCuti = false; //ubah status cuti
                cout << "Cuti berhasil diselesaikan" << endl;  

                if (k->antrian.depan != NULL) { //cek apakah ada antrian cuti
                    int nextNip = dequeue(k->antrian); //ambil NIP berikutnya dari antrian
                    Node* nextK = search(root, nextNip); //cari node karyawan berikutnya 
                    if (nextK != NULL) { //pastikan karyawan berikutnya masih ada
                        Aksi a2 = {nextNip, "ajukan", false}; //simpan aksi untuk pengajuan cuti berikutnya (status lama = false, karena belum cuti)
                        push(a2); //simpan aksi ke stack untuk menu undo
                        nextK->statusCuti = true; //ubah status cuti karyawan berikutnya
                        cout << "Pengajuan cuti untuk NIP " << nextNip << " berhasil diproses dari antrian" << endl;
                    }
                }
            }   
        }
        else if (pilihan == 5) {
            int nip; 
            cout << "NIP karyawan: "; 
            cin >> nip;
            if (search(root, nip) == NULL) {
                cout << "Karyawan tidak ditemukan!" << endl;
            } else {
                root = hapus(root, nip);
                cout << "Data karyawan berhasil dihapus" << endl;
            }
        }

        else if (pilihan == 6) {
            if (top == NULL) {
                cout << "Tidak ada aksi untuk di-undo!" << endl;
            } else {
                Aksi a = pop(); //ambil aksi terakhir
                Node* k = search(root, a.nip); //cari karyawan yang sudah terkait aksi
                if (k != NULL) { //pastikan karyawan masih ada sebelum undo
                    if (a.jenis == "ajukan") { 
                        k->statusCuti = a.statusLama;
                        cout << "Undo pengajuan cuti untuk NIP " << a.nip << " berhasil" << endl;
                    } else if (a.jenis == "selesai") {
                        k->statusCuti = a.statusLama;
                        cout << "Undo penyelesaian cuti untuk NIP " << a.nip << " berhasil" << endl;
                    }
                }
            }
        }

        else if (pilihan == 7) {
            int nip; 
            cout << "NIP karyawan: "; 
            cin >> nip;
            Node* k = search(root, nip);
            if (k == NULL) {
                cout << "Karyawan tidak ditemukan!" << endl;
            } else if (k->antrian.depan == NULL) { 
                cout << "Antrian cuti kosong untuk NIP " << nip << endl;
            } else { 
                cout << "Antrian cuti untuk NIP " << nip << ": ";
                
                NodeQ* temp = k->antrian.depan; //variabel sementara untuk cari antrian cuti
                while (temp != NULL) { 
                    cout << temp->nip << " ";
                    temp = temp->next;
                }
                cout << endl;
            }
        }

        else if (pilihan == 0) {
            cout << "Keluar dari program..." << endl;
            return 0;
        }

        else {
            cout << "Pilihan tidak valid!" << endl;
        }   
        
        cout << "\n Tekan Enter untuk lanjut...";
        cin.ignore(); 
        cin.get();
        system("cls");

    } while (pilihan != 0);
}   





