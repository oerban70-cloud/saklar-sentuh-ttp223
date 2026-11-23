// SaklarSentuhTtp223.ino
// Saklar sentuh TTP223 + ESP32 + relay: sentuh modul sekali untuk
// menyalakan lampu, sentuh lagi untuk mematikannya (toggle).
// Modul TTP223 memberi logika HIGH saat disentuh (output digital,
// tidak butuh library). Relay modul aktif LOW: pin IN LOW = relay ON.
// LED hijau menyala mengikuti status lampu.
//
// Rangkaian:
//   TTP223 OUT  -> GPIO4   (input digital)
//   TTP223 VCC  -> 3V3
//   TTP223 GND  -> GND
//   Relay IN    -> GPIO5   (aktif LOW)
//   Relay VCC   -> 5V (VIN ESP32 / adaptor 5V)
//   Relay GND   -> GND
//   Relay COM   -> fasa 220V, Relay NO -> lampu, lampu -> netral 220V
//   LED hijau   -> GPIO2 + resistor 220 ohm ke GND
//
// PERINGATAN: terminal relay membawa 220V AC. Pastikan kabel terisolasi
// dan jangan sentuh terminal relay saat rangkaian menyala.

// ---------- konfigurasi pin ----------
const uint8_t PIN_SENTUH = 4;  // output digital TTP223
const uint8_t PIN_RELAY = 5;   // IN relay, aktif LOW
const uint8_t PIN_LED = 2;     // LED indikator (anoda via resistor 220 ohm)

// ---------- konfigurasi waktu ----------
const unsigned long WAKTU_DEBOUNCE_MS = 50;  // jeda anti-goyang sentuhan

// ---------- status ----------
bool lampuMenyala = false;       // status lampu saat ini
bool sentuhanSebelumnya = LOW;   // pembacaan TTP223 di loop sebelumnya
unsigned long waktuSentuhanTerakhir = 0;  // millis() sentuhan terakhir yang sah

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SENTUH, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  // Relay aktif LOW: HIGH berarti relay mati.
  digitalWrite(PIN_RELAY, HIGH);
  digitalWrite(PIN_LED, LOW);

  Serial.println("Saklar sentuh TTP223 siap. Sentuh modul untuk toggle lampu.");
}

void loop() {
  bool sentuhan = digitalRead(PIN_SENTUH);

  // Hanya proses saat sentuhan BARU muncul (transisi LOW -> HIGH),
  // dan hanya bila sudah lewat dari jeda debounce.
  if (sentuhan == HIGH && sentuhanSebelumnya == LOW &&
      millis() - waktuSentuhanTerakhir >= WAKTU_DEBOUNCE_MS) {
    waktuSentuhanTerakhir = millis();
    lampuMenyala = !lampuMenyala;
    terapkanStatusLampu();
  }

  sentuhanSebelumnya = sentuhan;
}

// Menulis status lampu ke relay dan LED indikator.
void terapkanStatusLampu() {
  // Relay aktif LOW: lampu menyala = pin IN LOW.
  digitalWrite(PIN_RELAY, lampuMenyala ? LOW : HIGH);
  digitalWrite(PIN_LED, lampuMenyala ? HIGH : LOW);
  Serial.print("Lampu: ");
  Serial.println(lampuMenyala ? "MENYALA" : "PADAM");
}
