/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 by David A. Mellis
 
 */

#define BTN_MODE (66)
#define BTN_SEEK (25)
#define BTN_PRESET (230)
#define BTN_VUP (121)
#define BTN_VDOWN (164)
#define BTN_TOD (12)
#define btn_gis (5)
#define btn_gis_low (5)
#define HUM_ADDRESS (100)
#define HUM_ENABLE_VALUE (120)
#define HUM_DISABLE_VALUE (121)
#define HUM_PIN (5)
#define HUM_LED (4)
#define NET_EN (6)
#define speaker (8) 
#define TOD_PIN (3)
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <MsTimer2.h>
//#include <avr/delay.h> 

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,17,177 };
//byte server[] = { 77,88,21,3 }; // Google
//byte server[] = { 192,168,17,102 };
byte server[] = { 192,168,17,9 };
byte gw[] = { 192,168,17,9 };
//byte server[] = { 192,168,17,9 };
//byte gw[] = { 192,168,17,9 };
char cmd[] = {'a', 'a', 'a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a','a', 'a'};
boolean mpc_playing = false;
int iCnt = 0;
int hu_mode = false;
boolean network_enabled = false;
boolean play_store = false;
volatile byte ReconectTimer=0;
// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
Client client(server, 20123);

void SendCmd(int Cmd)
{
  //
}

void ReconectTimerCallback()
{
  ReconectTimer++;
  if (ReconectTimer==10) ReconectTimer=0; 
}

void setup() {
  // start the Ethernet connection:
  // start the serial library:
  pinMode(NET_EN, OUTPUT);
  pinMode(HUM_LED, OUTPUT);
  pinMode(HUM_PIN, OUTPUT);
  pinMode(TOD_PIN, OUTPUT);
  pinMode(speaker, OUTPUT);
  Ethernet.begin(mac, ip, gw);
  MsTimer2::set(1000, ReconectTimerCallback);
  MsTimer2::start();
  //ddigitalWrite(HUM_PIN,, HIGH);
  //Serial.begin(9600);
  //PORTC |= B00100000;
  /*if (analogRead(5) < 50) 
  {
    byte server[] = { 192,168,17,102 };
    byte gw[] = { 192,168,17,100 };
    beep(10, 250);
    delay(10);
    beep(10, 250);
    delay(10);
    beep(10, 250);
    delay(10);
  }  
  
  Ethernet.begin(mac, ip, gw);
  Client client(server, 20123);*/
  // give the Ethernet shield a second to initialize:
  /*delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect()) {
    Serial.println("connected");
    // Make a HTTP request:
    //client.println("GET /search?q=arduino HTTP/1.0");
    client.println("gettime");
    client.println();
    client.stop();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }*/
  PORTC |= B00100001;
  
  byte tmp = EEPROM.read(HUM_ADDRESS);
  if (tmp==HUM_ENABLE_VALUE) hu_mode = true; else hu_mode=false; 
}

void beep(byte dur, int frq) 
{ 
  dur=(1000/frq)*dur; 
  for(byte i=0; i<dur; i++) 
  { 
   digitalWrite(speaker, HIGH);  
   //_delay_us(frq);
   delayMicroseconds(frq);  
   digitalWrite(speaker, LOW); 
   delayMicroseconds(frq); 
  }  
} 

void CarButtons()
{
  int btn = analogRead(0);
  if (((btn + btn_gis) > BTN_SEEK) && ((btn - btn_gis) < BTN_SEEK)) 
  {
    delay(30);
    btn = analogRead(0);
    if (((btn + btn_gis) > BTN_SEEK) && ((btn - btn_gis) < BTN_SEEK)) 
    {
      //Serial.println("mode");
      //Client client(server, 20123);client.stop();
      if (client.connect()) {
      if (!mpc_playing)
      {
        client.println("mpc play");
        mpc_playing = true;
      }
      else
      {
        client.println("mpc pause");
        mpc_playing = false;
      }
      client.println();
      //delay(500);
        /*while (client.available()>0) {
          char c = client.read();
          Serial.print(c);
        }*/

      client.stop();
      delay(200);
      }
    }    
  }
  
  //=======

  btn = analogRead(0);
  if (((btn + btn_gis) > BTN_VUP) && ((btn - btn_gis) < BTN_VUP)) 
  {
    delay(30);
    btn = analogRead(0);
    if (((btn + btn_gis) > BTN_VUP) && ((btn - btn_gis) < BTN_VUP)) 
    {
      //Serial.println("mode");
      //client.stop();
      if (client.connect()) {
        client.println("mpc next");
        mpc_playing = true;
        client.println();
        /*delay(500);
        while (client.available()>0) {
          char c = client.read();
          Serial.print(c);
        }*/

      client.stop();
      delay(200);
      }
    }    
  }
  
  //========================
  btn = analogRead(0);
  if (((btn + btn_gis) > BTN_VDOWN) && ((btn - btn_gis) < BTN_VDOWN)) 
  {
    delay(30);
    btn = analogRead(0);
    if (((btn + btn_gis) > BTN_VDOWN) && ((btn - btn_gis) < BTN_VDOWN)) 
    {
      //Serial.println("mode");
      //client.stop();
      if (client.connect()) {
        client.println("mpc prev");
        mpc_playing = true;
        client.println();
        /*delay(500);
        while (client.available()>0) {
          char c = client.read();
          Serial.print(c);
        }*/

      client.stop();
      delay(200);
      }
    }    
  }
  
}

void HUControl(boolean Enabled)
{
  if (Enabled)
      {
        digitalWrite(HUM_PIN, HIGH);
        digitalWrite(HUM_LED, HIGH);
        EEPROM.write(HUM_ADDRESS, HUM_ENABLE_VALUE);
        //Serial.println("hum enabled");
        if ((mpc_playing) && (play_store))
        {
          if (client.connect())
          {
            //Serial.println("/usr/bin/launch2.py");
            client.println("mpc play");
            client.println(); 
            client.stop();
            play_store = false;
          }  
        }
      } 
      else
      { 
        digitalWrite(HUM_PIN, LOW);
        EEPROM.write(HUM_ADDRESS, HUM_DISABLE_VALUE);
        digitalWrite(HUM_LED, LOW);
        //Serial.println("hum disabled"); 
        play_store = true;
        if (client.connect())
        {
          client.println("mpc pause");
          client.println(); 
          client.stop();
        }

      }
  
}

void SendTOD(boolean IsLong)
{
  int sendDelay = 0;
  if (IsLong) {sendDelay=1500;beep(30, 250);delay(30);beep(30, 250);} else {sendDelay=160;beep(30, 250);}
  digitalWrite(TOD_PIN, HIGH);
  delay(sendDelay);
  digitalWrite(TOD_PIN, LOW);
  delay(300);
}

void HUMode()
{
  int btn = analogRead(0);
  if (((btn + btn_gis_low) > BTN_TOD) && ((btn - btn_gis_low) < BTN_TOD)) 
  {
    delay(30);
    btn = analogRead(0);
    if (((btn + btn_gis_low) > BTN_TOD) && ((btn - btn_gis_low) < BTN_TOD))
    {
      delay(500);
      btn = analogRead(0);
      if (((btn + btn_gis_low) > BTN_TOD) && ((btn - btn_gis_low) < BTN_TOD))
      {
        SendTOD(true);
      }
      else
      {
        SendTOD(false);
      }
    }
  }  
  //========================================================
  btn = analogRead(0);
  if (((btn + btn_gis) > BTN_MODE) && ((btn - btn_gis) < BTN_MODE)) 
  {
    delay(30);
    btn = analogRead(0);
    if (((btn + btn_gis) > BTN_MODE) && ((btn - btn_gis) < BTN_MODE)) 
    {
      hu_mode = !hu_mode;
      if (hu_mode) {beep(30,250);} else {beep(25,250);delay(30);beep(25,250);}
      HUControl(hu_mode);
      delay(250);
    }
  }
}

void SendShellCmd()
{
  int btn = analogRead(0);
  if (((btn + btn_gis) > BTN_PRESET) && ((btn - btn_gis) < BTN_PRESET)) 
  {
    delay(30);
    btn = analogRead(0);
    if (((btn + btn_gis) > BTN_PRESET) && ((btn - btn_gis) < BTN_PRESET)) 
    {
      delay(500);
      btn = analogRead(0);
      if (((btn + btn_gis) > BTN_PRESET) && ((btn - btn_gis) < BTN_PRESET)) 
      {
        if (client.connect())
        {
          beep(30,250);
          delay(30);
          beep(30,250);
          //Serial.println("/usr/bin/launch2.py");
          client.println("/usr/bin/launch2.py");
          client.println(); 
          client.stop();
        }
      }
      else
      {
        if (client.connect())
        {
          beep(30,250);
          //beep(10,500); 
          //Serial.println("/usr/bin/launch1.py");
          client.println("/usr/bin/launch1.py");
          client.println(); 
          client.stop();
        }        
      }
    }
    delay(250);
  }  
}

void loop()
{
  if (network_enabled)
  {
    if (hu_mode)
    {
      CarButtons();
    }
    SendShellCmd();       
  }
  else
  {
    if (ReconectTimer==9)
    {
      digitalWrite(NET_EN, HIGH);
      if (client.connect())
      {
      //PlayLone();
        network_enabled = true;
        if (hu_mode)
        {
          HUControl(true);
        }
        digitalWrite(NET_EN, HIGH);
        beep(30,250);
        client.stop();
        MsTimer2::stop();
      }
      else
      {
        network_enabled = false;
        digitalWrite(NET_EN, LOW);
      }
    }  
  }
  HUMode();
  /*if (analogRead(5) < 50)
  {
    //network_enabled = true;
        digitalWrite(HUM_PIN, HIGH);
  }
  else
  {
    digitalWrite(HUM_PIN, LOW);
  }*/
  // if there are incoming bytes available 
  // from the server, read them and print them:
  /*if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }*/
  //Serial.println(analogRead(0));
  //delay(200);
  // if the server's disconnected, stop the client:
  /*if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    //for(;;)
      //;   
  }*/
  
   /*while (Serial.available()>0)
   {
     //for (int i=0; i<8; i++)
     {
       iCnt++;
       cmd[iCnt] = Serial.read();
     }  
     Serial.println(cmd);
     iCnt=0;
     SendCmd(0);
   }*/

}

