#include <ESP8266HTTPClient.h>

#include <Wire.h>
#include "Adafruit_VL53L0X.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Wifi connection information
//const char* wifiName = "microlab_IoT";
//const char* wifiPassword = "shibboleet";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* wifiName = "VladimirRoutin";
const char* wifiPassword = "jn3qvGncNbn8";

String IFTTT_WEBHOOK = "cK7bnlFm1xR2Xtn1Z5X1CO";
String triggerName_SHEETS = "range_readings";
String triggername_SMS = "food_low";
String IFTTT_URL = "https://maker.ifttt.com/trigger/";
const char* host = "maker.ifttt.com";
String LOG_TO_SHEET;
String SEND_SMS_WEBHOOK;
float Distance;
float percentage;
float ID = 1;
const int httpPort = 80;

int button = 13;
int buttonStatus = 0;

ESP8266WebServer server(80);

Adafruit_VL53L0X ToF = Adafruit_VL53L0X();
WiFiClient client;
HTTPClient http;

float containerSize = 272;
float smsAlert = 218;

void setup() {
  Serial.begin(115200);

  display123();

//  wait until serial port opens for native USB devices
  while (!Serial) {
    delay(1);
  }
  ToFtest();
  display.clearDisplay();
  display.print("Connecting");
  display.display();
  WiFiSetup();
  display.print("Connected");
  display.display();
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  if(digitalRead(button) == 0) {
    VL53L0X_RangingMeasurementData_t measure;
    ToF.rangingTest(&measure, false);

    client.connect(host,httpPort);
    Serial.println("Connected");
    Serial.println(measure.RangeMilliMeter);
    display.print(measure.RangeMilliMeter);
    display.display();
    float Emptypercentage = (measure.RangeMilliMeter/containerSize)*100;
    percentage = 100 - Emptypercentage;
    delay(10000);
    display.clearDisplay();

    if(percentage >= 95) {
      ID++;
    }

    LOG_TO_SHEET = IFTTT_URL + triggerName_SHEETS + "/with/key/" + IFTTT_WEBHOOK + "/?value1=" + measure.RangeMilliMeter + "&value2=" + percentage + "&value3=" + ID;
    Serial.println(LOG_TO_SHEET);
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;     }
    client.print(String("POST ") + LOG_TO_SHEET + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

    if(measure.RangeMilliMeter >= smsAlert) {
    SEND_SMS_WEBHOOK = IFTTT_URL + triggername_SMS + "/with/key/" + IFTTT_WEBHOOK +"/?value1=" + percentage;
    Serial.print(SEND_SMS_WEBHOOK);
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;     }
    client.print(String("POST ") + SEND_SMS_WEBHOOK + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    delay(500);
}
  }
Serial.println("Waiting for button to be pushed");
}

void display123() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.display();
  delay(3000);
  display.clearDisplay();
//  display.display();
}

void ToFtest() {
  //  Checking to see if the range sensor boots properly
  Serial.println("Testing VL53L0X");
  if (!ToF.begin()) {
    Serial.println(F("Failed to boot"));
    while(1);
  }
}

void WiFiSetup() {
  int n = WiFi.scanNetworks();
  Serial.print(n);
  for (int i = 0; i < n; ++i) {
    Serial.println(WiFi.SSID(i));
  }
//  WiFi.mode(WIFI_STA);
  WiFi.persistent(false); 
  WiFi.begin(wifiName,wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Trying to connect to "); Serial.print(wifiName); Serial.print(" on "); Serial.print(WiFi.localIP()); Serial.println(".");
  }
  Serial.print("Connected to "); Serial.println(wifiName); Serial.println(WiFi.localIP());
//  const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    server.begin();
    Serial.println("HTTP Server Started");
}
