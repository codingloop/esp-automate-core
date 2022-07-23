#include<EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
#include<SPI.h>
#include<Ticker.h>

#include "index.h"

/*Functions*/
void zero_cross_detect();
void homepage();
void switching();
void fan_speed();
void timer0();
void fan_up();
void senddata();
void changedata();
void changemod();
void senderrormsg();
void resetfn();
void esprst();
void auto_fan();
void auto_light();
void light_auto();
void writechange(String,String,byte,byte);
void crashrecovery();
/*end*/
/*Global variables*/
bool flag=true;
bool tmr0=true;
Ticker crash;
Ticker f_sp;
int crash_c=0;
ESP8266WebServer server(80);
IPAddress ip(192,168,0,23);
IPAddress gateway(192,168,0,23);
IPAddress subnet(255,255,255,0);
//GPIO and InterruptPins
volatile byte dl1=0;
volatile byte dl2=0.5;
volatile bool light_f=false;
volatile bool fan_f=false;
const byte a0=17;   //analog pin
const byte d0=16;   //no interrupt
const byte d1=5;    //LDR interrupt
const byte d2=4;    //light1
const byte d3=0;
const byte d4=2;    //bultin LED
const byte d5=14;
const byte d6=12;
const byte d7=13;   //zer0-cross detect
const byte d8=15;   //ssr output
/*end*/
/*VALUES*/
  //Serial.println(getattribs(0,32));
  //Serial.println(getattribs(32,64));
  //Serial.println(getattribs(65,81));
/*end*/

void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);
  String ss=getattribs(0,32);
  String ps=getattribs(32,64);
  const char* ssid=ss.c_str();
  const char* pass=ps.c_str();
  Serial.println(ss);
  Serial.println(ps);
  byte ota=EEPROM.read(100);
  if(ota==49){
    WiFi.mode(WIFI_STA);
  }
  //WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid,pass);
  //WiFi.begin(ssid);
  Serial.println("Connecting");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.println(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
  if(ota==49)
  {
    Serial.println("ota");
    flag=false;
    ArduinoOTA.setPassword((const char *)getattribs(65,81).c_str());
      ArduinoOTA.onStart([]() {
      Serial.println("Start");
      EEPROM.write(100,48);
      EEPROM.commit();
      });
      ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
      resetfn();
      });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      });
      ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if   (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
      EEPROM.write(100,48);
      EEPROM.commit();
      resetfn();
      });
      ArduinoOTA.begin();
      Serial.println("ota bigin");
  }
  else 
  {
    flag=true;
    pinMode(d2,OUTPUT);
    pinMode(d4,OUTPUT);
    pinMode(d7,INPUT_PULLUP);
    pinMode(a0,INPUT_PULLUP);
    pinMode(d1,INPUT_PULLUP);
    pinMode(d8,OUTPUT);
    attachInterrupt(digitalPinToInterrupt(d7),zero_cross_detect,RISING);
    timer0_isr_init();
    server.on("/",homepage);
    server.on("/switching",switching);
    server.on("/speed",fan_speed);
    server.on("/getdata",senddata);
    server.on("/datachange",changedata);
    server.on("/modchange",changemod);
    server.on("/esprst",esprst);
    server.on("/fan_mode",auto_fan);
    server.on("/li_mode",auto_light);
    server.begin();
  }
  crash.attach(1,crashrecovery);
  Serial.println(ota);
}

void loop() {
  if(flag)
  {
    server.handleClient();
  }
  else
  {
    ArduinoOTA.handle();
  }
  crash_c=0;
}

String getattribs(byte i1,byte i2)
{
  int l=i2;
  Serial.println(l);
  String c="                                ";
  int count=0;
  byte re;
  for(int i=i1;i<l;i++)
  {
    if((re=EEPROM.read(i))==0)
    {
      break;
    }
    c[count]=(char)re;
    count++;
  }
  c.trim();
  return c;
}

void homepage()
{
  String s=MAIN_page;
  server.send(200,"text/html",s);
}

void zero_cross_detect()
{
  timer0_write(ESP.getCycleCount()+(dl1*80000L));//80MHz=1sec
  //if(dl1<=7&&dl1>=1)
    timer0_attachInterrupt(timer0);
  //else
    //digitalWrite(d8,LOW);
}

void timer0()
{
  if(tmr0)
  {
    tmr0=false;
    digitalWrite(d8,HIGH);
    timer0_write(ESP.getCycleCount()+(dl2*80000L));
  }
  else
  {
    tmr0=true;
    digitalWrite(d8,LOW);
    timer0_detachInterrupt();
  }
}

void fan_speed()
{
  String q1=server.arg("dl1");
  dl1=q1.toInt();
  server.send(200,"text","suc");
}

void switching()
{
  String q1=server.arg("c1");
  String q2=server.arg("c2");
  byte p=q1.toInt();
  byte st=q2.toInt();
  if(st==0)
    digitalWrite(p,LOW);
  else if(st==1)
    digitalWrite(p,HIGH);
  server.send(200,"text/html",MAIN_page);
}

void senddata()
{
  String q1=server.arg("pin");
  byte p=q1.toInt();
  String val;
  if(p==17)
  {
    val=analogRead(p);
  }
  else
  {
  val=String(digitalRead(p));
  }
  server.send(200,"text",val);
}

void auto_fan()
{
  String q1=server.arg("m");
  byte p=q1.toInt();
  if(p==1)
  {
    fan_f=true;
    f_sp.attach(5,fan_up);
  }
  else
  {
    fan_f=false;
    f_sp.detach();
  }
  server.send(200,"text","success");
}

void fan_up()
{
  int tmp=(analogRead(a0)*300)/1024;
  Serial.println(tmp);
  if(tmp>=32)
    dl1=1;
  else if(tmp>=29)
    dl1=3;
  else if(tmp>=26)
    dl1=4;
  else if(tmp>=23)
    dl1=6;
  else 
    dl1=0;
}

void auto_light()
{
  String q1=server.arg("l");
  byte p=q1.toInt();
  Serial.println("light");
  if(p==1)
  {
    light_f=true;
    attachInterrupt(digitalPinToInterrupt(d1),light_auto,CHANGE);
    server.send(200,"text","on");
  }
  else
  {
    light_f=false;
    detachInterrupt(digitalPinToInterrupt(d1));
    server.send(200,"text","off");
  }
}

void light_auto()
{
  byte i=digitalRead(d1);
  Serial.println("auto");
  Serial.println(i);
  digitalWrite(d2,i);
}

void changedata()
{
  String q1=server.arg("ov");
  String q2=server.arg("nv");
  String q3=server.arg("ak");
  String q4=server.arg("ty");
  if(q3=="ssid1234"&&q2.length()<=32&&q2.length()>=1&&q4=="s")
  {
    writechange(q1,q2,0,32);
  }
  else if(q3=="pass1234"&&q2.length()<=32&&q2.length()>=8&&q4=="p")
  {
    writechange(q1,q2,32,64);
  }
  else if(q3=="ota1234"&&q2.length()<=15&&q2.length()>=4&&q4=="o")
  {
    writechange(q1,q2,65,81);
  }
  else 
  {
    senderrormsg();
  }
}

void writechange(String ov,String nv,byte st,byte en)
{
  String ch=getattribs(st,st+ov.length());
  int i;
  if(ch==ov)
  {
    for(i=st;i<en;i++)
      {
        EEPROM.write(i,0);
      }
    byte cn=0;
    byte ll=nv.length()+st;
    for(i=st;i<ll;i++)
    {
      EEPROM.write(i,nv[cn]);
      cn++;
    }
    EEPROM.commit();
    server.send(200,"text","success");
  }
  else
  {
    senderrormsg();
  }
}

void senderrormsg()
{
  server.send(200,"text","Invalid data provided");
}

void changemod()
{
  String q1=server.arg("ak");
  if(q1=="modchange")
  {
    EEPROM.write(100,'1');
    EEPROM.commit();
    server.send(200,"text","success");
    resetfn();
  }
  else
  {
    senderrormsg();
  }
}

void esprst()
{
  String q1=server.arg("rst");
  if(q1=="resetesp8266")
  {
    server.send(200,"text","success");
    resetfn();
  }
}

void resetfn()
{
  ESP.reset();
}

void crashrecovery()
{
  crash_c++;
  if(crash_c>10)
    resetfn();
}
