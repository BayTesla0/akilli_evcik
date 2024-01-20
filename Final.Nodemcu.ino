#include <Wire.h>
#include <TimeLib.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define WIFI_SSID "Axxxxxxot"
#define WIFI_PASSWORD "JxxxxxxxxDK1881"
#define BOT_TOKEN "xxxxxxxxxxxxxx:xxxxxxxxxxxxxxxxxxxxx"
#define CHAT_ID "11488xxxx0"

// d0, d1, d2, d3 pinlerini çıkış olarak tanımlıyoruz
#define d0 D0
#define d1 D1
#define d2 D2
#define d3 D3

// d4 pinini giriş olarak tanımlıyoruz
#define d4 D4
#define relayPin D5

int targetHourStart = 19; // Açılma saati (19:00)
int targetHourEnd = 23;   
/*
#define SDA_PIN D2       //Declare SCL Pin on NodeMCU 
#define SCL_PIN D1         //Declare SDA Pin on NodeMCU
#define ADDRESS 0x05

#define Zil D4
*/



X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

bool buttonPressed = false;

int botIstekGecikmesi = 1;

unsigned long botSonCalismaZamani;


void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      //receive byte as a character
  }
}

void setup() {


 Serial.begin(9600);  

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setTrustAnchors(&cert);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

 

  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);

  while (now < 24 * 3600) {
    delay(100);
    now = time(nullptr);
  }

  bot.sendMessage(CHAT_ID, "Akıllı Evcik başlatıldı", "");
}

void saat(){
  int currentHour = hour();  // Geçerli saat bilgisini al

  // Belirtilen saat aralığında
  if (currentHour >= targetHourStart && currentHour <= targetHourEnd) {
    digitalWrite(relayPin, HIGH);  // Röleyi aç
  } else {
    digitalWrite(relayPin, LOW);   // Röleyi kapat
  }

  delay(1000);  // 1 saniye bekle
}


void loop() {
 


saat();

int buttonState=digitalRead(D6);

  if (buttonState == HIGH && !buttonPressed) {
    bot.sendMessage(CHAT_ID, "Kapı zili çalınıyor!", "");
    buttonPressed = true; // Buton basıldı
  } else if (buttonState == LOW) {
    buttonPressed = false; // Buton bırakıldı
  }

if (millis() > botSonCalismaZamani + botIstekGecikmesi)  {
    int kacYeniMesaj = bot.getUpdates(bot.last_message_received + 1);

    while(kacYeniMesaj) { //yeni mesaj gelmişse yazdıralım
      yeniMesajlariYonet(kacYeniMesaj);
      kacYeniMesaj = bot.getUpdates(bot.last_message_received + 1);
    }
    botSonCalismaZamani = millis();
  }
  delay(100);
}

void serialGonder(bool d0_value, bool d1_value, bool d2_value, bool d3_value) {
  // d0, d1, d2, d3 pinlerine gönderilen değerleri yazıyoruz
  digitalWrite(d0, d0_value);
  digitalWrite(d1, d1_value);
  digitalWrite(d2, d2_value);
  digitalWrite(d3, d3_value);

  // d4 inputunda 1 değeri gelene kadar bekliyoruz
  while (digitalRead(d4) != HIGH) {
    delay(100); // 100 ms gecikme
  }
  // d0, d1, d2, d3 değerlerini 0 yapıyoruz
  digitalWrite(d0, LOW);
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
}
void yeniMesajlariYonet(int kacYeniMesaj) {


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
    serialGonder(0,0,0,1);
    bot.sendMessage(chat_id, "Salon ışığı açıldı.", "");
      

  }

  if (text == "/sal_kapat") {
        serialGonder(1,0,0,1);
    bot.sendMessage(chat_id, "Salon ışığı kapatıldı.", "");
    
  }

  if (text == "/banyo_ac") {
            serialGonder(0,0,1,0);

    bot.sendMessage(chat_id, "Banyo ışığı açıldı.", "");
   
  }

  if (text == "/banyo_kapat") {          serialGonder(1,0,1,0);

    bot.sendMessage(chat_id, "Banyo ışığı kapatıldı.", "");
    
  }

  if (text == "/yt1_ac") {   
             serialGonder(0,0,1,1);

    bot.sendMessage(chat_id, "Yatak Odası 1 ışığı açıldı.", "");
  
  }

  if (text == "/yt1_kapat") {        serialGonder(1,0,1,1);

    bot.sendMessage(chat_id, "Yatak Odası 1 ışığı kapatıldı.", "");
    
  }

  if (text == "/yt2_ac") {            serialGonder(0,1,0,0 );

    bot.sendMessage(chat_id, "Yatak Odası 2 ışığı açıldı.", "");
 
  }

  if (text == "/yt2_kapat") {        serialGonder(1,1,0,0);

    bot.sendMessage(chat_id, "Yatak Odası 2 ışığı kapatıldı.", "");
 
  }

  if (text == "/mtf_ac") {            serialGonder(0,1,0,1);

    bot.sendMessage(chat_id, "Mutfak ışığı açıldı.", "");
  }

  if (text == "/mtf_kapat") {                  serialGonder(1,1,0,1);


    bot.sendMessage(chat_id, "Mutfak ışığı kapatıldı.", "");
  }

  if (text == "/Priz1_ac") {            serialGonder(0,1,1,0);

    bot.sendMessage(chat_id, "Priz açıldı.", "");
  }

  if (text == "/Priz1_kapat") {                  serialGonder(1,1,1,0);


    bot.sendMessage(chat_id, "Priz kapatıldı.", "");
  }

  if (text == "/Priz2_ac") {            serialGonder(0,1,1,1);

    bot.sendMessage(chat_id, "Priz 2 açıldı.", "");
  }

  if (text == "/Priz2_kapat") {                  serialGonder(1,1,1,1);


    bot.sendMessage(chat_id, "Priz 2 kapatıldı.", "");
  }if (text.startsWith("/saat")) {
      // "/saat" komutunu kontrol et
      text.remove(0, 6); // "/saat" kısmını kaldır

      // Saat aralığını kontrol et ve ayarla
      if (parseHourRange(text)) {
        bot.sendMessage(chat_id, "Saat aralığı güncellendi: " + String(targetHourStart) + " - " + String(targetHourEnd));
      } else {
        bot.sendMessage(chat_id, "Geçersiz saat formatı!");
      }
    }


    }
  }


bool parseHourRange(String input) {
  int separatorIndex = input.indexOf('-');

  if (separatorIndex != -1) {
    // Geçerli bir saat aralığı bulundu
    String startHourStr = input.substring(0, separatorIndex);
    String endHourStr = input.substring(separatorIndex + 1);

    int startHour = startHourStr.toInt();
    int endHour = endHourStr.toInt();

    // Saat aralığı kontrolü yap
    if (startHour >= 0 && startHour <= 24 && endHour >= 0 && endHour <= 24 && startHour <= endHour) {
      targetHourStart = startHour;
      targetHourEnd = endHour;
      return true;
    }
  }

  return false;
}
