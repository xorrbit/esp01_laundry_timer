/*
Prototype photo: https://goo.gl/photos/NjTwntU1QywMau2y6

This requires the ESP8266 board support files to be installed
as per http://www.arduinesp.com/getting-started. This also makes
use of the TM1637 library from https://github.com/avishorp/TM1637

This simple sketch uses an ESP8266 ESP-01 module as a countdown
reminder timer. I frequently will throw a load of laundry in the
washer and even when I set a timer to remind me to put the clothes
in the dryer, I will dismiss the timer and then forget about it.
This will keep sending notification emails until I turn it off.

On powerup it will start counting down from 35 minutes and display
this countdown on a 4 digit 7-segment display. When the countdown
reaches 0, it will connect up to your wifi network and send an
email notification message. It will then restart the counter at 30
seconds and keep sending new email notifications every 30 seconds
until you power it down.

This uses an authenticated smtp server account from smtp2go.com. You
have to sign up for a free account and then base64 encode the username
and password below for it to connect and send email.
*/

#include <ESP8266WiFi.h>
#include <TM1637Display.h>

// 35 minutes initial timer delay
#define TIMER_START (35*60)
// 30 seconds between notifications
#define TIMER_RESTART 30
#define WIFI_SSID "ssid"
#define WIFI_PASS "pass"

#define EMAIL_FROM "from@blah.com"
#define EMAIL_TO "to@blah.com"
#define EMAIL_SUBJECT "Laundry"
#define EMAIL_MESSAGE "Put your clothes in the dryer"
#define SMTP_SERVER "smtpcorp.com"
#define SMTP_PORT 2525
#define SMTP_USER_BASE64 "blah"
#define SMTP_PASS_BASE64 "blah"

// TM1637 pins
const int CLK = 2;
const int DIO = 0;

int timer, minutes, seconds, digits;
int countdown_start = TIMER_START;

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  display.setBrightness(0x00);  //set the diplay to min brightness 0-7
  Serial.println("Counting down...");
}

void recv()
{
  byte thisByte;
  while(client.available())
  {
    thisByte = client.read();    
    Serial.write(thisByte);
  }
}

void send_sms()
{
   client.connect(SMTP_SERVER, SMTP_PORT);
   client.println("EHLO 0.0.0.0");
   recv();
   client.println("auth login");
   recv();
   client.println(SMTP_USER_BASE64);
   recv();
   client.println(SMTP_PASS_BASE64);
   recv();
   client.print("MAIL From: <");
   client.print(EMAIL_FROM);
   client.println(">");
   recv();
   client.print("RCPT To: <");
   client.print(EMAIL_TO);
   client.println(">");
   recv();
   client.println("DATA");
   recv();
   client.print("To: You <");
   client.print(EMAIL_TO);
   client.println(">");
   recv();
   client.print("From: Me <");
   client.print(EMAIL_FROM);
   client.println(">");
   recv();
   client.print("Subject: ");
   client.print(EMAIL_SUBJECT);
   client.println("\r\n");
   recv();
   client.println(EMAIL_MESSAGE);
   recv();
   client.println(".");
   recv();
   client.println("QUIT");
   recv();
   client.stop();
}

void loop()
{
  for(timer = countdown_start; timer > 0; timer--)
  {
    minutes = timer / 60;
    seconds = timer % 60;
    digits = (minutes * 100) + seconds;
    display.showNumberDec(digits);
    delay(1000);
  }
  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    Serial.println("Connected!");
  }
  
  Serial.println("Sending email...");
  send_sms();
  countdown_start = TIMER_RESTART;
}
