#include <iostream>
#include <string>
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
    string jenis;   // "ajukan" atau "selesai"
    bool statusLama; //status cuti sebelum aksi (true = cuti, false = tidak cuti)
};

struct NodeS { //node untuk stack riwayat aksi
    Aksi data;
    NodeS* next; 
};

struct Stack { //stack untuk riwayat aksi
    NodeS* top;
};

void push(Stack& s, Aksi a) { //tambah aksi ke stack
    NodeS* baru  = new NodeS;   
    baru->data   = a;
    baru->next   = s.top;
    s.top        = baru;
}

Aksi pop(Stack& s) { //ambil aksi dari stack
    NodeS* temp  = s.top;
    Aksi a       = s.top->data;
    s.top        = s.top->next;
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
    baru->antrian.depan = NULL;
    baru->antrian.belakang = NULL;
    baru->kiri = NULL;
    baru->kanan = NULL;
    return baru;
}

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

Node* search(Node* root, int nip) {
    if (root == NULL || root->nip == nip) {
        return root;
    }
    if (nip < root->nip) {
        return search(root->kiri, nip);
    }
    return search(root->kanan, nip);
}

void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->kiri);
    cout << "\nNIP    : " << root->nip << endl;
    cout << "Nama   : " << root->nama << endl;
    cout << "Divisi : " << root->divisi << endl;
    cout << "Status : " << (root->statusCuti ? "Cuti" : "Tidak Cuti") << endl;
    cout << "--------------------------" << endl;
    inorder(root->kanan);
}
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
    Stack riwayat; //stack untuk menyimpan riwayat aksi cuti
    riwayat.top = NULL;
    int pilihan;

    do {
        cout << "\n=============================" << endl;
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

        if (pilihan == 1) {
            int nip, jumlah; 
            string nama, divisi;
            cout << "Berapa karyawan yang ingin ditambahkan? "; 
	        cin >> jumlah;
	        cin.ignore();

	for (int i = 0; i < jumlah; i++) {
        cout << "\nKaryawan ke-" << (i + 1) << ":\n";
            cout << "NIP    : "; cin >> nip;
            if (search(root, nip) != NULL) {
                cout << "[!] NIP sudah terdaftar! Karyawan tidak dapat ditambahkan.\n";
                continue;
            }
            cout << "Nama   : "; cin.ignore(); getline(cin, nama);
            cout << "Divisi : "; getline(cin, divisi);
            root = insert(root, nip, nama, divisi);
            cout << "Data karyawan berhasil ditambahkan" << endl;
        }
    } 

        else if (pilihan == 2) {
            if (root == NULL) {
                cout << "Belum ada data karyawan" << endl;
            } else {
                inorder(root);
            }
        }

        else if (pilihan == 3) {
            int nip; 
            cout << "NIP karyawan: "; 
            cin >> nip;
            if (nip < 0) {
                cout << "NIP tidak valid!" << endl;
                continue;
            }
            Node* k = search(root, nip);
            if (k == NULL) {
                cout << "Karyawan tidak ditemukan!" << endl;
            } else if (k->statusCuti == false) {
                Aksi a = {nip, "ajukan", false};
                push(riwayat, a);
                k->statusCuti = true;
                cout << "Pengajuan cuti berhasil" << endl;
            } else {
                int antrian;
                cout << "Karyawan sedang cuti, Masukkan NIP karyawan antrian: ";
                cin >> antrian;
                if (search(root, antrian) == NULL) {
                    cout << "Karyawan antrian tidak ditemukan!" << endl;
                } else {
                    enqueue(k->antrian, antrian);
                    cout << "Pengajuan cuti untuk NIP " << antrian << " masuk ke dalam antrian" << endl;
                } 
            }
        }
        else if (pilihan == 4) {
            int nip; 
            cout << "NIP karyawan: "; 
            cin >> nip;
            Node* k = search(root, nip);
            if (k == NULL) {
                cout << "Karyawan tidak ditemukan!" << endl;
            } else if (!k->statusCuti) {
                cout << "Karyawan tidak sedang cuti!" << endl;
            } else {
                Aksi a = {nip, "selesai", true};
                push(riwayat, a);
                k->statusCuti = false;
                cout << "Cuti berhasil diselesaikan" << endl;  
                if (k->antrian.depan != NULL) {
                    int nextNip = dequeue(k->antrian);
                    Node* nextK = search(root, nextNip);
                    if (nextK != NULL) {
                        Aksi a2 = {nextNip, "ajukan", false};
                        push(riwayat, a2);
                        nextK->statusCuti = true;
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
            if (riwayat.top == NULL) {
                cout << "Tidak ada aksi untuk di-undo!" << endl;
            } else {
                Aksi a = pop(riwayat);
                Node* k = search(root, a.nip);
                if (k != NULL) {
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
                NodeQ* temp = k->antrian.depan;
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





