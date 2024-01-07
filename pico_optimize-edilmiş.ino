const int relePinleri[] = {2, 3, 4, 5, 6, 7, 8, 9, 10}; // Röle pinleri
const int butonPinleri[] = {11, 12, 13, 14, 15, 16, 17, 18}; // Buton pinleri
const int rxPin = 0; // RX pin
const int txPin = 1; // TX pin
int analogDegerler[3];
int mevcut[2] = {0, 0};

void setup() {
  Serial.begin(9600);

  // Röle pinleri çıkış olarak ayarlanır
  for (int i = 0; i < sizeof(relePinleri) / sizeof(relePinleri[0]); i++) {
    pinMode(relePinleri[i], OUTPUT);
  }

  // Buton pinleri giriş olarak ayarlanır
  for (int i = 0; i < sizeof(butonPinleri) / sizeof(butonPinleri[0]); i++) {
    pinMode(butonPinleri[i], INPUT);
  }

  // Analog pinlerden değerleri oku ve kaydet
  for (int i = 0; i < 3; i++) {
    analogDegerler[i] = map(analogRead(A0 + i), 0, 1023, 1, 100);
  }
}

void kontrolEtVeIslemYap(int analogPinIndex, int relayIndex) {
  int okunanDeger = analogRead(A0 + analogPinIndex);
  if (analogDegerler[analogPinIndex] <= map(okunanDeger, 0, 1023, 1, 100) + 10) {
    for (int i = 0; i < 100; i++) {
      mevcut[relayIndex]++;
      digitalWrite(relePinleri[relayIndex], HIGH);
    }
  } else {
    for (int i = 0; i < 100; i++) {
      mevcut[relayIndex]--;
      if (mevcut[relayIndex] == 0) {
        digitalWrite(relePinleri[relayIndex], LOW);
      }
    }
  }
}

void loop() {
  kontrolEtVeIslemYap(0, 2);
  kontrolEtVeIslemYap(1, 3);
  kontrolEtVeIslemYap(2, 4);

  // Seri iletişimden gelen veriyi kontrol et
  if (Serial.available() >= 2) {
    int rxData = Serial.read();
    int txData = Serial.read();

    // Gelen veriye göre röleleri kontrol et
    if (rxData == 12) {
      digitalWrite(relePinleri[rxPin - 2], txData);
    }
  }

  // Buton durumlarını kontrol et
  for (int i = 0; i < sizeof(butonPinleri) / sizeof(butonPinleri[0]); i++) {
    int butonDurumu = digitalRead(butonPinleri[i]);

    // Buton durumu değişirse, ilgili röle pini değiştirilir
    if (butonDurumu == HIGH) {
      digitalWrite(relePinleri[i], HIGH);
    } else {
      digitalWrite(relePinleri[i], LOW);
    }
  }
}
