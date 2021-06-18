/* Firmansyah Wahyu @2021
 * Latest Version : https://github.com/bywahjoe/ESP8266-Monitoring-Gas-Suhu-Api.git
   HTTPS Connection
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <LiquidCrystal_I2C.h>
#include "pinku.h"
#include "DHT.h"

#define URLWEB "monitor.bywahjoe.com"
#define PATH "/post.php"
#define sha "CE 6A 43 36 69 39 8D 5D 10 F6 2E EA D4 09 3A 79 2F DA 4E 4E"
//Valid Cert Wed, 15 Sep 2021 03:09:07 GMT
#define httpsPort 443

String APIKEY = "sendIP4";

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(pin_suhu, type_suhu);

//Status Sensor
int mygas = 0;
int myapi = 0;
int myrelay = 0;
float mysuhu;

//Milis Setting
const long interval = 12000;
unsigned long before = 0;
unsigned long now = 0;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  dht.begin();

  pinMode(pin_relay, OUTPUT);
  pinMode(pin_api, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  String statusWifi = "Connected";
  int retry = 1;
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("Connecting WiFi");
    lcd.setCursor(1, 1);
    lcd.print(retry);

    if (retry > 100) {
      statusWifi = "Error";
      break;
    }
    else {
      retry++;
    }
    delay(500);
  }
}

void loop() {

  mygas = getGas();
  myapi = !digitalRead(pin_api);
  mysuhu = getSuhu();

  if (mygas == 1 || myapi == 1) {
    myrelay = 1;
    relayON();
  }
  else {
    myrelay = 0;
    relayOFF();
  }

  tampil();

  now = millis();
  if (now - before >= interval) {
    kirimWeb();
    now=millis();
    before = now;
  }

}
int getGas() {
  int val = analogRead(A0);
  int result = 0;
  Serial.println(val);

  if (val >= setGas) result = 1;
  return result;
}
void relayON() {
  digitalWrite(pin_relay, HIGH);
}
void relayOFF() {
  digitalWrite(pin_relay, LOW);
}
String warning(bool val) {
  String notice;

  if (val) notice = "WARNING!!";
  else notice = "STABIL";
  return notice;
}
String statusRelay(bool val) {
  String notice;

  if (val)notice = "ON";
  else notice = "OFF";
  return notice;
}
float getSuhu() {
  float val = dht.readTemperature();
  return val;
}
void tampil() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas :");
  lcd.print(warning(mygas));
  lcd.setCursor(0, 1);
  lcd.print("Api :");
  lcd.print(warning(myapi));
  delay(800);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu  :");
  lcd.print(mysuhu);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Relay :");
  lcd.print(statusRelay(myrelay));
  delay(800);
}
void kirimWeb() {
  String d1 = String(mygas);
  String d2 = String(myapi);
  String d3 = String(mysuhu);
  String d4 = String(myrelay);
  //  String d1 = String(random(0,2));
  //  String d2 = String(random(0,2));
  //  String d3 = String(random(0,2));
  //  String d4 = String(random(29,35));

  //4D 54 07 D8 EA 7D 9A 47 DC 08 7C 46 A8 54 0F AB B8 D9 0B A75
  WiFiClientSecure httpsClient;
  httpsClient.setInsecure();

  //  Serial.printf("Using fingerprint '%s'\n", sha);
  //  httpsClient.setFingerprint(sha);
  //  httpsClient.setTimeout(15000);

  Serial.print("HTTPS Connecting: "); Serial.println(URLWEB);
  //    int r=0; //retry counter
  //    while((!httpsClient.connect(URLWEB, 443)) && (r < 30)){
  //        delay(100);
  //        Serial.print(".");
  //        r++;
  //    }
  //    if(r==30) {
  //      Serial.println("Connection failed");
  //    }
  //    else {
  //      Serial.println("Connected to web");
  //    }
    if (!httpsClient.connect(URLWEB, httpsPort)) {
      Serial.println("connection failed");
    } else {
      Serial.println("connection OK");
    }

  Serial.print("requesting URL: ");
  Serial.print(URLWEB); Serial.println(PATH);
  String dataku = "apiKEY=" + APIKEY + "&d1=" + d1 + "&d2=" + d2 + "&d3=" + d3 + "&d4=" + d4;
  //TEST
  //String dataku="apiKEY=sendIP4&MYID=AEST&MYIP=192.168.43.245";
  //String dataku="apiKEY=sendIP4&d1=0&d2=0&d3=0&d4=0";

  //Format----
  /*
    POST /post.php HTTP/1.1
    Host: monitor.bywahjoe.com
    Content-Type: application/x-www-form-urlencoded
    Content-Length: 34

    apiKEY=sendIP4&d1=0&d2=0&d3=0&d4=0
  */

  httpsClient.print(String("POST ") + PATH + " HTTP/1.1\r\n" +
                    "Host: " + URLWEB + "\r\n" +
                    "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
                    "Content-Length: " + String(dataku.length()) + "\r\n\r\n" +
                    dataku + "\r\n" +
                    "Connection: close\r\n\r\n");
  Serial.println(dataku);
}
