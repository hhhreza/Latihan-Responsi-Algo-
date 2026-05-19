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

void push(Stack& s, Aksi a) {
    NodeS* baru  = new NodeS;   
    baru->data   = a;
    baru->next   = s.top;
    s.top        = baru;
}

Aksi pop(Stack& s) {
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
    cout << "NIP    : " << root->nip << endl;
    cout << "Nama   : " << root->nama << endl;
    cout << "Divisi : " << root->divisi << endl;
    cout << "Status : " << (root->statusCuti ? "Cuti" : "Tidak Cuti") << endl;
    cout << "--------------------------" << endl;
    inorder(root->kanan);
}

