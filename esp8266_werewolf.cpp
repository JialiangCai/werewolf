#include <TFT_eSPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

int WIDTH=128;
int HEIGHT=160;
int TEXT_CENTRE_WIDTH=WIDTH/1.6;
int TEXT_CENTRE_HEIGHTE=HEIGHT/2.5;
TFT_eSPI tft = TFT_eSPI(WIDTH,HEIGHT);

// SSID is a wifi name.
String SSID="PlaceHolder";
String PASSWORD="PlaceHolder";

unsigned int listen_Port=9999;
char databuffer[512];
WiFiUDP udp;

String text="Please close your eyes";



void wifi_setup(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  print_text("Connect to "+SSID+"...");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }
  udp.begin(listen_Port);
  print_text("WiFi connected!"); 
}

void tft_sSPI_setup(){
  tft.init();
  tft.setRotation(5);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_LIGHTGREY,TFT_BLACK);
  // tft.setTextColor(TFT_RED,TFT_BLACK);
  tft.setTextDatum(CC_DATUM);
	tft.setTextSize(1);
}

void check_wifi_status(){
  if (WiFi.status() != WL_CONNECTED){
    WiFi.disconnect();
    WiFi.begin(SSID, PASSWORD);
    print_text("Reconnect to "+SSID+"...");

    while (WiFi.status() != WL_CONNECTED){
      delay(1000);
    }
    udp.begin(listen_Port);
    print_text("WiFi connected!");
  }
}

void print_text(String text){
  tft.fillScreen(TFT_BLACK);
  tft.drawString(text,TEXT_CENTRE_WIDTH,TEXT_CENTRE_HEIGHTE);
}



String get_udp(){

// Python boardcast debug code
// import socket
// listen_Port=9999
// sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
// sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
// sock.sendto(message.encode('utf-8'), (broadcast_address, listen_Port))
  String data="";
  int packetSize =udp.parsePacket();
  if (packetSize){
    memset(databuffer, 0, sizeof(databuffer));
    udp.read(databuffer,511);
    udp.flush();
    for(int i=0;i<packetSize;i++){
      data+=databuffer[i];
    }
    print_text(data);
  }
  return data;
}

void setup() {
  Serial.begin(9600);
  tft_sSPI_setup();
  wifi_setup();
  print_text("This is a blood moon day.");
}

void loop() {
  check_wifi_status();
  get_udp();
}
