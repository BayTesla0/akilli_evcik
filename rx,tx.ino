void setup() {
  Serial.begin(9600);  // Seri iletişimi başlat
}

void loop() {
  if (Serial.available() > 0) {
    int receivedValue = Serial.parseInt();  // Seri üzerinden gelen değeri oku

    if (receivedValue == 100) {
      Serial.println("000");  // '000' değerini gönder
    }

    delay(1000);  // 1 saniye bekle
  }
}
