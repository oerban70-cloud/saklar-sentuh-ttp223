# Saklar Sentuh TTP223 — ESP32 + Relay

Saklar lampu jauh dari kasur? Sentuh saja, lampu langsung menyala. Modul
sentuh TTP223 merasakan sentuhan jarimu, ESP32 membalik relay, dan lampu
menyala atau padam — tanpa tombol mekanik, tanpa meraba saklar di gelap.

Video penjelasan (60 detik):
https://youtube.com/shorts/0Mq3UuTUuUk

## Komponen

| Bagian | Keterangan |
|---|---|
| ESP32 DevKit | Otak alat |
| Modul TTP223 | Sensor sentuh kapasitif, output digital (HIGH saat disentuh) |
| Relay 1 channel | Aktif LOW, 10A 250V |
| Lampu 220V | Beban AC lewat kontak NO relay |
| LED hijau | Indikator, menyala saat lampu ON |
| Resistor 220 Ω | Seri dengan LED hijau |
| Adaptor 5V 2A | Catu ESP32 (VIN) + relay (VCC) |
| Stopkontak 220V | Fasa + netral PLN ke relay dan lampu |

## Wiring

| Dari | Ke |
|---|---|
| TTP223 OUT | ESP32 GPIO4 |
| TTP223 VCC | ESP32 3V3 |
| TTP223 GND | GND |
| Relay IN | ESP32 GPIO5 (aktif LOW) |
| Relay VCC | 5V (adaptor) |
| Relay GND | GND |
| Relay COM | Fasa 220V |
| Relay NO | Lampu 220V |
| Lampu 220V | Netral 220V |
| LED hijau | ESP32 GPIO2 (seri 220 Ω ke GND) |
| Adaptor 5V | ESP32 VIN + relay VCC |
| Semua GND | Digabung |

Skema lengkap: [schematic.png](schematic.png)

## Cara kerja

1. TTP223 mengeluarkan logika HIGH saat pad sentuhnya disentuh jari.
2. ESP32 membaca GPIO4 tiap loop; saat muncul transisi LOW ke HIGH,
   status lampu dibalik (toggle).
3. Debounce 50 ms mencegah satu sentuhan dihitung berkali-kali.
4. Lampu menyala → relay diaktifkan dengan menarik GPIO5 ke LOW
   (relay aktif LOW), LED hijau menyala sebagai indikator.
5. Sentuh lagi → relay dilepas (GPIO5 HIGH), lampu padam, LED mati.

## Konfigurasi (di awal sketch)

- `PIN_SENTUH` (4) — output digital TTP223.
- `PIN_RELAY` (5) — IN relay, aktif LOW.
- `PIN_LED` (2) — LED indikator.
- `WAKTU_DEBOUNCE_MS` (50) — jeda anti-goyang sentuhan, dalam milidetik.

## Library yang dibutuhkan

Tidak ada — TTP223 adalah sensor output digital dan relay dikendalikan
langsung dari pin GPIO, jadi sketch ini hanya memakai core ESP32.

## Compile

```sh
arduino-cli compile --fqbn esp32:esp32:esp32 SaklarSentuhTtp223
```

Terverifikasi 23 Nov 2026: sketch 291.585 byte (22% flash),
RAM 23.216 byte (7%), ESP32 core 4.0.0.

## Keamanan 220V

Terminal relay membawa tegangan 220V AC. Pastikan semua kabel bertegangan
mains terisolasi dengan baik dan jangan sentuh terminal relay saat
rangkaian menyala.

## Lisensi

MIT — lihat [LICENSE](LICENSE).
