#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define WIFI_SSID "Azimbot"
#define WIFI_PASSWORD "JDNKPLQAHKDK1881"
#define BOT_TOKEN "6741248267:AAFP1e9KvdiytTBAiktMkWgKQsPL9d1Ukog"
#define CHAT_ID "1148814370"
#define SalonPin D0
#define BanyoPin D1
#define Yt1Pin D2
#define Yt2Pin D3
#define MutfakPin D4

#define Zil D5 //zil



X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

bool buttonPressed = false;

// Her 1 saniyede bir olacak şekilde yeni bir mesaj gelip gelmediğini kontrol edeceğiz.
int botIstekGecikmesi = 1;
unsigned long botSonCalismaZamani;

//led'i bağlayacağımız pin numarası. Nodemcu için farklı tanımlayın
bool ledDurum = LOW;//ilk açılışta kapalı olmasını sağlamak için.
const int analogPin = A0; // Kullanılan analog pin

void setup() {

  pinMode(SalonPin, OUTPUT);
  pinMode(BanyoPin, OUTPUT);
  pinMode(Yt1Pin, OUTPUT);
  pinMode(Yt2Pin, OUTPUT);
  pinMode(MutfakPin, OUTPUT);

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

void loop() {


  int buttonState = digitalRead(Zil);


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
      
      merhaba += "/durum ile LED'in şu anki durumunu göster \n";
      bot.sendMessage(chat_id, merhaba, "");
    }
if (text == "/sal_ac") {
    bot.sendMessage(chat_id, "Salon ışığı açıldı.", "");
    digitalWrite(SalonPin, HIGH);
  }

  if (text == "/sal_kapat") {
    bot.sendMessage(chat_id, "Salon ışığı kapatıldı.", "");
    digitalWrite(SalonPin, LOW);
  }

  if (text == "/banyo_ac") {
    bot.sendMessage(chat_id, "Banyo ışığı açıldı.", "");
    digitalWrite(BanyoPin, HIGH);
  }

  if (text == "/banyo_kapat") {
    bot.sendMessage(chat_id, "Banyo ışığı kapatıldı.", "");
    digitalWrite(BanyoPin, LOW);
  }

  if (text == "/yt1_ac") {
    bot.sendMessage(chat_id, "Yatak Odası 1 ışığı açıldı.", "");
    digitalWrite(Yt1Pin, HIGH);
  }

  if (text == "/yt1_kapat") {
    bot.sendMessage(chat_id, "Yatak Odası 1 ışığı kapatıldı.", "");
    digitalWrite(Yt1Pin, LOW);
  }

  if (text == "/yt2_ac") {
    bot.sendMessage(chat_id, "Yatak Odası 2 ışığı açıldı.", "");
    digitalWrite(Yt2Pin, HIGH);
  }

  if (text == "/yt2_kapat") {
    bot.sendMessage(chat_id, "Yatak Odası 2 ışığı kapatıldı.", "");
    digitalWrite(Yt2Pin, LOW);
  }

  if (text == "/mtf_ac") {
    bot.sendMessage(chat_id, "Mutfak ışığı açıldı.", "");
    digitalWrite(MutfakPin, HIGH);
  }

  if (text == "/mtf_kapat") {
    bot.sendMessage(chat_id, "Mutfak ışığı kapatıldı.", "");
    digitalWrite(MutfakPin, LOW);
  }

    if (text == "/durum") {
    String durumMesaji = "Işıkların Durumu:\n";
    durumMesaji += "Salon: " + String(digitalRead(SalonPin) ? "Açık" : "Kapalı") + "\n";
    durumMesaji += "Banyo: " + String(digitalRead(BanyoPin) ? "Açık" : "Kapalı") + "\n";
    durumMesaji += "Yatak Odası 1: " + String(digitalRead(Yt1Pin) ? "Açık" : "Kapalı") + "\n";
    durumMesaji += "Yatak Odası 2: " + String(digitalRead(Yt2Pin) ? "Açık" : "Kapalı") + "\n";
    durumMesaji += "Mutfak: " + String(digitalRead(MutfakPin) ? "Açık" : "Kapalı") + "\n";

    bot.sendMessage(chat_id, durumMesaji, "");
  }
    }
  }


