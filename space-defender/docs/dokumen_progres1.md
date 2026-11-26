# Dokumen Progres 1 - Proyek Akhir Pengenalan Pemrograman

**Nama Game:** Space Defender
**Kelompok:** ...
**Kelas:** ...
**Tanggal:** 25 November 2026

## 1. Anggota Kelompok
| No | Nama | NPM | Peran |
|---|---|---|---|
| 1 | ... | ... | Logika game (loop, tabrakan, skor, level, base) |
| 2 | ... | ... | Antarmuka ncurses (menu, HUD, efek, layar game over) |
| 3 | ... | ... | Musuh, boss, power-up, file handling, dokumentasi |

## 2. Deskripsi Game
**Space Defender** adalah game berbasis terminal yang dikendalikan dengan keyboard
menggunakan library **ncurses** (bahasa C). Pemain mengendalikan pesawat untuk
mempertahankan **base** dari serangan armada alien. Musuh yang lolos akan merusak
base; permainan berakhir jika base hancur atau seluruh nyawa pesawat habis.

## 3. Fitur
- Menu utama, pilihan tingkat kesulitan (Easy/Normal/Hard), petunjuk, papan skor
- 4 jenis musuh: basic, fast (zigzag), tank (3 hit), shooter (menembak balik)
- Boss setiap 5 level dengan health bar dan fase kedua
- Power-up: nyawa, perisai, tembakan tiga arah, bom, perbaikan base
- Sistem combo (pengali skor hingga x4)
- Statistik akhir: rank, akurasi, combo terbaik, waktu
- Papan skor Top 5 dengan nama pemain (disimpan di file)

## 4. Penerapan Materi Praktikum
| Materi | Penerapan |
|---|---|
| Variabel dan tipe data | Posisi, skor, nyawa, timer, HP base |
| Percabangan dan perulangan | Game loop, pengecekan tabrakan, pola serangan boss |
| Array | Peluru, musuh, power-up, bintang, ledakan |
| Fungsi | Pemisahan logika menjadi modul-modul kecil |
| Struct dan enum | Player, Enemy, Boss, Bullet, PowerUp, Game, Difficulty |
| Pointer | Parameter `Game *g`, `Enemy *e`, `Player *p` |
| Alokasi memori dinamis | `calloc` dan `free` untuk struktur Game |
| File handling | Penyimpanan papan skor `highscores.txt` |
| Random | Kemunculan musuh, drop power-up, gerak zigzag |
| Program multi-file | Folder `include/` dan `src/`, serta `Makefile` |

## 5. Struktur Program
(Salin struktur folder dari README.md)

## 6. Progres Saat Ini
- [x] Struktur proyek, Makefile, unit test
- [x] Gerakan pemain, tembakan, musuh, tabrakan, skor
- [x] Base, level, boss, power-up, bom
- [x] Menu, kesulitan, papan skor, layar game over
- [ ] Penyempurnaan tampilan dan balancing
- [ ] Pengujian akhir dan dokumentasi

## 7. Rencana Ke Depan
| Tahap | Rencana |
|---|---|
| Progres 2 (29 Nov) | Demo game yang dapat dimainkan |
| Presentasi Akhir (4 Des) | Game final, dokumentasi, dan demo |

## 8. Tautan Repositori GitHub
https://github.com/...

## 9. Screenshot
(Tempel screenshot menu dan gameplay)
