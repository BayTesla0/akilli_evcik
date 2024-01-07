int relePinleri[] = {2, 3, 4, 5, 6, 7, 8, 9, 10}; // Röle pinleri
int butonPinleri[] = {11, 12, 13, 14, 15, 16, 17, 18}; // Buton pinleri
int rxPin = 0; // RX pin
int txPin = 1; // TX pin
int analogDegerler[3];
int mevcut[2]=(0,0);

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
  analogDegerler[0] =  map(analogRead(A0), 0, 1023, 1, 100);
  analogDegerler[1] =  map(analogRead(A1), 0, 1023, 1, 100);
  analogDegerler[2] = map(analogRead(A2), 0, 1023, 1, 100);
}

void loop() {


  if(analogDegerler[0]<=map(analogRead(A0), 0, 1023, 1, 100)+10){

    for(int i=0;i>100;i++){


       if(analogDegerler[1]<=map(analogRead(A1), 0, 1023, 1, 100)+10){
       mevcut[0]++;
       digitalWrite(relePinleri[2], HIGH);

 }else if(analogDegerler[2]<=map(analogRead(A2), 0, 1023, 1, 100)+10){
  mevcut[1]++;
       digitalWrite(relePinleri[3], HIGH);

    }
    }
   

  }else if(analogDegerler[1]<=map(analogRead(A1), 0, 1023, 1, 100)+10){
    for(int i=0;i>100;i++){
if(analogDegerler[0]<=map(analogRead(A0), 0, 1023, 1, 100)+10){
  mevcut[1]--;
  if(mevcut[1]==0){         digitalWrite(relePinleri[1], LOW);
}

}
}
  }else if(analogDegerler[2]<=map(analogRead(A2), 0, 1023, 1, 100)+10){
    for(int i=0;i>100;i++){
if(analogDegerler[0]<=map(analogRead(A0), 0, 1023, 1, 100)+10){
    mevcut[2]--;

    if(mevcut[2]==0){         digitalWrite(relePinleri[2], LOW);
}

}
}
  }
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

