#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define WIFI_SSID "xxx"
#define WIFI_PASSWORD "xxx"
#define BOT_TOKEN "xxx"
#define CHAT_ID "xxx"    //xxx yerine karşısındaki şeyin yazıolması gerek ir
#define BUTTON_PIN D4
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

bool buttonPressed = false;

void setup() {
  Serial.begin(115200);
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
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH && !buttonPressed) {
    bot.sendMessage(CHAT_ID, "Kapı zili çalınıyor!", "");
    buttonPressed = true; // Buton basıldı
  } else if (buttonState == LOW) {
    buttonPressed = false; // Buton bırakıldı
  }

  delay(100);
}
