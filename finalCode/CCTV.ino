#include <SPI.h>
#include <WiFiUdp.h>
#include <WiFi.h>
const char* ssid = "smallTV";
const char* password = "superTollesPasswort100";
int messwert;
enum CCTVStates {
     DoorOpen,
     DoorClose,
};
CCTVStates cState = DoorClose;

// Lokaler UDP-Port
unsigned int localPort = 8888;

// Empfänger IP - Adresse
IPAddress RecipientIP(10, 42, 0, 1);

// Empfänger UDP-Port
unsigned int RecipientPort = 55056;

// Buffer für Sende- und Empfangsnachrichten
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
char ReplyBufferO[] = "Door Opened";
char ReplyBufferC[] = "Door Closed";

// UDP-Instanz --> Senden und Empfangen
WiFiUDP Udp;

void setup() {
Serial.begin(115200);
WiFi.mode(WIFI_STA);
// Wifi Beginn mit gesetzten Parametern
WiFi.begin(ssid, password);
//Verbindungsaufbau läuft [...]
while (WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.print(".");
       delay(500);}
 
// Verbindung aufgebaut
Serial.println("");
Serial.print("Pico-W ist mit WiFi verbunden: ");
Serial.println(WiFi.SSID());
 
// Zugewiesene IP-Adresse ausgeben
Serial.print("Zugewiesene IP-Addresse: ");
Serial.println(WiFi.localIP());

// Start UDP
Udp.begin(localPort);
}

void loop()
{ 
  if(digitalRead(0) == 0){
    cState = DoorClose;
  }
   else {
    cState = DoorOpen;   
   }

switch (cState){
  
  case DoorClose:
       digitalWrite(LED_BUILTIN, HIGH);
       sendUDP(ReplyBufferC);
       delay(5000);
       break;
  case DoorOpen:
       digitalWrite(LED_BUILTIN, LOW);
       sendUDP(ReplyBufferO);
       delay(5000);
       break;
}
}
// UDP-Pakete senden
void sendUDP(char ReplyBufferSend[])
{
    Udp.beginPacket(RecipientIP, RecipientPort);
    Udp.write(ReplyBufferSend);
    Udp.endPacket();
    delay(10);
}
