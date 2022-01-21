# esp8266_roaster_dimmer
Create an Artisan controlled coffe roaster out of an Klarstein VitAir hot air fryer!
This project combines Yoku-San's initial project for the "BilloJoe" and LeFish's websocket implementation.
Project video and link to thread: https://www.youtube.com/watch?v=pTog0-5Rx7I
YokuSun initial project dropbox: https://www.dropbox.com/sh/431hbvdjr7k1vzb/AACo8r3lY0x2tSdt06G-mDh7a?dl=0
LeFish project: https://github.com/LeFish1/ESP8266_CoffeeRoaster 

##What's inside?
scr: Arduino sketch for Adafruit feather ESP8266.
lib: Library of RDBDimmer modified to work with Adafruit feather ESP8266. All other libraries in sketch can be downloaded via Arduino library manager.
artisan: TBD

##How does it work?
Build based on BilloJoe project video, but with Richards variation of dimmer wiring (see Kaffe-Netz thread).
Adafruit feather ESP8266 used to open websocket connection with Artisan.
ESP8266 reads max6675 thermocouple values and control heating via rbdimmer.
Currently no WifiManager implementation, if Wifi credentials change sketch needs to be modified and re-uploaded.

Circuit:
https://asciiflow.com/#/share/eJy1VttOwjAYfpWmt%2BAFm5vI3Q4iRjnE4TRmCVlCjSQOTJkLhPAWPo5P45NYGIxtPTKwabL18H%2F%2F95%2FaruA0jBBsQTT%2FbGqmOcKzcB4jPBpPoghhWIcf4ZJ8W3AVwEUAW9fmZT2AS%2FKnNTXyF6NFTAYBBBXa7%2FfPsZ0hHARTgQ7DB7c9tzhX0xM9lfUdB4Cu9WKaV0aKWVAApPD7Xdw1wYyiBQwVUjd5zn3eXiXPViSiai2QKxDFWmSz41VWW0J68mwmiaLlp1m63XvjDTb1lpPMVDCi2c%2FLX6hXi5iENQ7f8Nck5pDgWipGbaMwfkcY8FDz%2FEqVmWLLM7XszLTZ1jBdN%2FwarbPSaZOyYSg7nCC6Ty8XdCc7EGOzkZw3OY6UCTyQHUTb6lESLHe6DYPruxLkthE5dsrzq50FQwklBRG3ocviqpwACYsTayQpwz0zTTEhyh4%2FNpvkXKgRb%2Bow18azCHT6QzV8l5sb4j547gLwaLvu9mUAgOWoeuK8dt%2F1FO1UjWmpvzo06Pkjp3wjiTIxa4fnTBHroS%2B50XmNwdpyT4%2B3UGHWqfdaxmY4Ax1yvSBcUcW%2FFC%2B%2FJQFcw%2FUfNdx1ow%3D%3D)

                                                    +---------------+          +-+
                           5V GND              +3v3-+VCC  MAX6675   |          | |
                            | |                     |               |          +-+
                            | |           +---------+SCK           ++----------+ |
                            | |           |         |               |            |
                           ++-++          | +-------+CS             |            |
                       +---+USB+---+      | |       |               |            |
                       |  ESP8266  |      | |  +----+SO            -+------------+
                       | Adafruit  |      | |  |    |               |
                       | Feather   |      | |  | +--+GND            |
+---------+            |        BAT+-5V+  | |  | |  +---------------+
|         |       +3v3-+3V         |      | |  | v
| 5V   VCC+-5V+        |           |      | |  |
| FAN     |         +--+GND     D15+------+ |  |
|      GND+--+      |  |           |        |  |
|         |  |      v  |        D13+--------+  |
+---------+  v         |           |           |
                       |        D12+-----------+    +-----------------+
                       |           |                |                 |      From HOT
                       |         D5+----------------+PWM  RBDDimmer  AC -------------
                       |           |                |                IN
                       |         D2+----------------+ZC               |
                       |           |                |                 |
                       +-----------+           +3v3-+VC              LO
                                                    |                AD -------------
                                                 +--+GND              |      To Heater
                                                 |  +-----------------+
                                                 v



 _    _                           _____                 _   _             
| |  | |                         |  __ \               | | (_)            
| |__| | __ _ _ __  _ __  _   _  | |__) |___   __ _ ___| |_ _ _ __   __ _ 
|  __  |/ _` | '_ \| '_ \| | | | |  _  // _ \ / _` / __| __| | '_ \ / _` |
| |  | | (_| | |_) | |_) | |_| | | | \ \ (_) | (_| \__ \ |_| | | | | (_| |
|_|  |_|\__,_| .__/| .__/ \__, | |_|  \_\___/ \__,_|___/\__|_|_| |_|\__, |
             | |   | |     __/ |                                     __/ |
            |_|   |_|    |___/                                     |___/ 
 
