# esp01_laundry_timer
Laundry timer for the ESP8266 ESP-01: http://www.esp8266.com/wiki/doku.php?id=esp8266-module-family#esp-01

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
and password in the source for it to connect and send email.
