/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____
   Justine MEHOUNOU
*/

#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include "LiquidCrystal_I2C.h"
#include "SoftwareSerial.h"
#include <Servo.h>
#define yellowLED 8
#define redLED 6
#define greenLED 7
// if you require to change the pin number, Edit the pin with your arduino pin.
// Emulate softserial on pins A9/A8 if not present
//#ifndef HAVE_HWSERIAL1
#define SERVO_PIN 3
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address from 0x27 to 0x3f if i2c address is 0x3f
SoftwareSerial softserial(A9, A8); // A9 to ESP_TX, A8 to ESP_RX by default
Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
int row = 0;
int temperatureReceive;
char ssid[] = "iPhone de Asma";            // your network SSID (name)
char pass[] = "asma3007.";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received
int readingState = 0;
int currentState = 0;
unsigned long startTime; // Variable to store the start time
unsigned int localPort = 2390;  // local port to listen on

char packetBuffer[255];          // buffer to hold incoming packet
char ReplyBuffer[] = "HI ASMA!";      // a string to send back

WiFiEspUDP Udp;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);   // initialize serial for debugging
  pinMode(redLED, OUTPUT);  // initialize digital pin Red LED as an output.
  pinMode(greenLED, OUTPUT);  // initialize digital pin Red LED as an output.
  pinMode(yellowLED, OUTPUT);  // initialize digital pin Red LED as an output.
  pinMode(SERVO_PIN, OUTPUT);
  softserial.begin(115200);
  softserial.write("AT+CIOBAUD=9600\r\n");
  softserial.write("AT+RST\r\n");
  softserial.begin(9600);    // initialize serial for ESP module
  WiFi.init(&softserial);    // initialize ESP module

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);

  Serial.print("Listening on port ");
  Serial.println(localPort);
}

void loop() {

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();

  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    temperatureReceive = atoi(packetBuffer);
    currentState = temperatureReceive;
    int res = 0;
    if (readingState != currentState) {
      if (temperatureReceive >= 20 && temperatureReceive <= 24) {
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, LOW);
        turnServoandLed(20000, greenLED, temperatureReceive);
      }
      else if (temperatureReceive >= 25 && temperatureReceive <= 29) {
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED, LOW);
        turnServoandLed(30000, yellowLED, temperatureReceive);
      }
      else if (temperatureReceive >= 30 && temperatureReceive <= 39) {
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
        turnServoandLed(40000, redLED, temperatureReceive);
      }
      else {
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED, LOW);
        printTemperatureLCD(temperatureReceive);
      }
      //strcpy(readingState, currentState);
      readingState = currentState;
    }
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printTemperatureLCD(int temp) {
  lcd.clear();
  lcd.print("Time: ");
  // Assuming DHT.temperature is a float or double
  lcd.setCursor(0, 1); // Move to the second line of the LCD
  lcd.print("Temp: ");
  lcd.print(temp); // Print the temperature value
  lcd.print("C"); // Print units if needed
  delay(500);
}
void turnServoandLed(int time, int led, int temp) {
  myservo.attach(SERVO_PIN);
  delay(300);
  startTime = millis();
  int angle = 0;
  int i, res;
  if (time = 20000) {
    j = 20;
  }
  else if (time = 30000) {
    j = 30;
  }
  else if (time = 40000) {
    j = 40;
  }
  printTemperatureLCD(temp);
  while (millis() - startTime < time) { // Run for 20 seconds (20,000 milliseconds)
    myservo.write(angle);
    angle += 50;
    res++;
    digitalWrite(led, HIGH);
    lcd.setCursor(7, 0); // Move to of the LCD
    delay(1000); // Delay to allow the servo to move smoothly
    lcd.print(j);
    i = j;
    j = i - 1 ? i > 0 : 0;
  }
  digitalWrite(led, LOW);
  delay(300);
  myservo.detach(); // save current of servo
  digitalWrite(SERVO_PIN, LOW);
}
