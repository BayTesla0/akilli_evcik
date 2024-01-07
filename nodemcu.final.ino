#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define WIFI_SSID "Azimbot"
#define WIFI_PASSWORD "JDNKPLQAHKDK1881"
#define BOT_TOKEN "6741248267:AAFP1e9KvdiytTBAiktMkWgKQsPL9d1Ukog"
#define CHAT_ID "1148814370"

#define Zil D4

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

bool buttonPressed = false;

// Her 1 saniyede bir olacak şekilde yeni bir mesaj gelip gelmediğini kontrol edeceğiz.
int botIstekGecikmesi = 1;

unsigned long botSonCalismaZamani;
//led'i bağlayacağımız pin numarası. Nodemcu için farklı tanımlayın


void setup() {



  Serial.begin(9600);
  Serial.println();

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setTrustAnchors(&cert);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);

  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }

  Serial.println(now);
  bot.sendMessage(CHAT_ID, "Bot başlatıldı", "");
}
/*
void buton(){
if(digitalRead(SalonPinBtn)==1){

    digitalWrite(SalonPin, HIGH);


}else if(digitalRead(BanyoPinBtn)==1){
    digitalWrite(BanyoPinBtn, HIGH);

}else if(digitalRead(Yt1PinBtn)==1){
    digitalWrite(Yt1PinBtn, HIGH);

}else if(digitalRead(Yt2PinBtn)==1){
    digitalWrite(Yt2PinBtn, HIGH);

}else if(digitalRead(MutfakPinBtn)==1){
    digitalWrite(MutfakPinBtn
    digitalWrite(SalonPin2, HIGH);

}
}*/
void loop() {

/*
  if(analogDeger>=analogRead(analogPin)){
    if(MutfakPin==LOW){
        digitalWrite(MutfakPin, HIGH);
        

    }else if(MutfakPin==HIGH){
              digitalWrite(MutfakPin, LOW);

    }
  }
void cekup();


  if( digitalRead(D5)==1){
      DegerGonder(0, 0, 0, 0, 0);

  }
*/
int buttonState=digitalRead(D4);

  if (buttonState == HIGH && !buttonPressed) {
    bot.sendMessage(CHAT_ID, "Kapı zili çalınıyor!", "");
    buttonPressed = true; // Buton basıldı
  } else if (buttonState == LOW) {
    buttonPressed = false; // Buton bırakıldı
  }

if (millis() > botSonCalismaZamani + botIstekGecikmesi)  {
    int kacYeniMesaj = bot.getUpdates(bot.last_message_received + 1);

    while(kacYeniMesaj) { //yeni mesaj gelmişse yazdıralım
      Serial.println("mesaj alındı");
      yeniMesajlariYonet(kacYeniMesaj);
      kacYeniMesaj = bot.getUpdates(bot.last_message_received + 1);
    }
    botSonCalismaZamani = millis();
  }
  delay(100);
}

void serialGonder(int veri) {
  Serial.write(veri);
}

void yeniMesajlariYonet(int kacYeniMesaj) {
  Serial.println("Gelen Yeni Mesaj");
  Serial.println(String(kacYeniMesaj));

  for (int i=0; i<kacYeniMesaj; i++) {
    // talebi yapan kullanıcının chatid'si
    String chat_id = String(bot.messages[i].chat_id);
    //eğer farklı bir kullanıcıdan komut gelirse izin vermeyeceğiz
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Yetkisiz Kullanıcı", "");
      continue;
    }
    
    // Alınan komut mesajını yazdıralım
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/baslat") {
      String merhaba = "Merhaba, " + from_name + ".\n";
      merhaba += "Akıllı evciği kontrol etmek için aşşağıdaki komutları kullanın .\n\n";
      merhaba += "/sal_ac Salon ışığı açılır \n";
      merhaba += "/sal_kapat Salonun ışığı kapatılır \n";
      merhaba += "/banyo_ac Banyonun ışığı açılır \n";
      merhaba += "/banyo_kapat Banyonun ışığı kapatılır \n";
      merhaba += "/yt1_ac  1.Yatak Odasının  ışığı açılır \n";
      merhaba += "/yt1_kapat  1.Yatak Odasının  ışığı kapatılır \n";
      merhaba += "/yt2_ac 2.Yatak Odasının  ışığı açılır \n";
      merhaba += "/yt2_kapat 2.Yatak Odasının  ışığı kapatılır \n";
      merhaba += "/mtf_ac Mutfağın ışığı açılır \n";
      merhaba += "/mtf_kapat Mutfağın ışığı kapatılır \n";
       merhaba += "/Priz1_ac priz 1 açılır \n";
     merhaba += "/Priz1_kapat priz 1  kapatılır \n";
 merhaba += "/Priz2_ac priz 2 açılır \n";
     merhaba += "/Priz2_kapat priz 2  kapatılır \n";

      bot.sendMessage(chat_id, merhaba, "");
    }
if (text == "/sal_ac") {
    serialGonder(12);
    bot.sendMessage(chat_id, "Salon ışığı açıldı.", "");
      

  }

  if (text == "/sal_kapat") {
        serialGonder(02);
    bot.sendMessage(chat_id, "Salon ışığı kapatıldı.", "");
    
  }

  if (text == "/banyo_ac") {
            serialGonder(13);

    bot.sendMessage(chat_id, "Banyo ışığı açıldı.", "");
   
  }

  if (text == "/banyo_kapat") {          serialGonder(03);

    bot.sendMessage(chat_id, "Banyo ışığı kapatıldı.", "");
    
  }

  if (text == "/yt1_ac") {            serialGonder(14);

    bot.sendMessage(chat_id, "Yatak Odası 1 ışığı açıldı.", "");
  
  }

  if (text == "/yt1_kapat") {        serialGonder(04);

    bot.sendMessage(chat_id, "Yatak Odası 1 ışığı kapatıldı.", "");
    
  }

  if (text == "/yt2_ac") {            serialGonder(15);

    bot.sendMessage(chat_id, "Yatak Odası 2 ışığı açıldı.", "");
 
  }

  if (text == "/yt2_kapat") {        serialGonder(05);

    bot.sendMessage(chat_id, "Yatak Odası 2 ışığı kapatıldı.", "");
 
  }

  if (text == "/mtf_ac") {            serialGonder(16);

    bot.sendMessage(chat_id, "Mutfak ışığı açıldı.", "");
  }

  if (text == "/mtf_kapat") {                  serialGonder(06);


    bot.sendMessage(chat_id, "Mutfak ışığı kapatıldı.", "");
  }

  if (text == "/Priz1_ac") {            serialGonder(17);

    bot.sendMessage(chat_id, "Priz açıldı.", "");
  }

  if (text == "/Priz1_kapat") {                  serialGonder(07);


    bot.sendMessage(chat_id, "Priz kapatıldı.", "");
  }

  if (text == "/Priz2_ac") {            serialGonder(18);

    bot.sendMessage(chat_id, "Priz 2 açıldı.", "");
  }

  if (text == "/Priz2_kapat") {                  serialGonder(1080);


    bot.sendMessage(chat_id, "Priz 2 kapatıldı.", "");
  }


    }
  }
/*
void cekup(){
  int degerD6 = digitalRead(lsb);
  int degerD7 = digitalRead(mid);
  int degerD8 = digitalRead(msb);

  // Okunan değerlere göre odaları kontrol et
  String odalar = String(degerD8) + String(degerD7) + String(degerD6);

  if (odalar == "000") {
    acKapatSalon(HIGH); // Salonu aç
  } else if (odalar == "001") {
    acKapatBanyo(HIGH); // Banyoyu aç
  } else if (odalar == "010") {
    acKapatYatakOdasi1(HIGH); // Yatak Odası 1'i aç
  } else if (odalar == "011") {
    acKapatYatakOdasi2(HIGH); // Yatak Odası 2'yi aç
  } else if (odalar == "100") {
    acKapatSalon(LOW); // Salonu kapat
  } else if (odalar == "101") {
    acKapatBanyo(LOW); // Banyoyu kapat
  } else if (odalar == "110") {
    acKapatYatakOdasi1(LOW); // Yatak Odası 1'i kapat
  } else if (odalar == "111") {
    acKapatYatakOdasi2(LOW); // Yatak Odası 2'yi kapat
  }
}

void acKapatSalon(int durum) {
  digitalWrite(SalonPin, durum);
  // Ekstra işlemler eklenebilir
}

void acKapatBanyo(int durum) {
  digitalWrite(BanyoPin, durum);
  // Ekstra işlemler eklenebilir
}

void acKapatYatakOdasi1(int durum) {
  digitalWrite(Yt1Pin, durum);
  // Ekstra işlemler eklenebilir
}

void acKapatYatakOdasi2(int durum) {
  digitalWrite(Yt2Pin, durum);
  // digitalWrite(Yt2Pin, durum);
  // Ekstra işlemler eklenebilir
}*/
