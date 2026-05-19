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