# esp8266_roaster_dimmer
Create an Artisan controlled coffe roaster out of an Klarstein VitAir hot air fryer!
This project combines Yoku-San's initial project for the "BilloJoe" and LeFish's websocket implementation.
* Project video by Yoku-San: https://www.youtube.com/watch?v=pTog0-5Rx7I
* Kaffe-Netz.de project thread: https://www.kaffee-netz.de/threads/umbau-heissluftfritteuse-zu-artisan-gesteuertem-kaffeeroester-billojoe-v2-komplettes-tutorial.133648/
* YokuSun initial project dropbox: https://www.dropbox.com/sh/431hbvdjr7k1vzb/AACo8r3lY0x2tSdt06G-mDh7a?dl=0
* LeFish project: https://github.com/LeFish1/ESP8266_CoffeeRoaster 

## What's inside?
* [scr](/src): Arduino sketch for Adafruit feather ESP8266.
* [lib](/lib): Library of RDBDimmer modified to work with Adafruit feather ESP8266. All other libraries in sketch can be downloaded via Arduino library manager.
* [artisan](/artisan): TBD

## How does it work?
Build based on BilloJoe project video, but with saxe74 & Richards variation of dimmer wiring (see Kaffe-Netz thread).
Adafruit feather ESP8266 used to open websocket connection with Artisan.
ESP8266 reads max6675 thermocouple values and control heating via rbdimmer.

Currently no WifiManager implementation, if Wifi credentials change sketch needs to be modified and re-uploaded.

## How to start?
Change wifi credentials to our own, adjust static IP setting to your environment.

Look at used libraries in sketch and import them to arudino (either via arduino library manager or via github),
library creators are mentioned in sketch so they should be easy to find.
If RBDDimmer does not work for your ESP8266 board, chances are high you need the modified one included in this repo (see sketch comment for more details).

Built for your board and upload to esp. 
Use Artisan configuration provided in this repo (remember to change websocket IP to IP of esp you configured).

Circuit:
https://asciiflow.com/#/share/eJy9VutugjAUfpWmf9UswmDOf1jmtkzRjI0tC4khWc1Mhi6IRGN8iz3OnsYnWQUv0JZScFnTBCg93%2BX0tLCGU8%2FHsA3x%2FKul6PoomHnzEAej94nv4wDW4ae3Itc2XLtw6cL2tX5Zd%2BGK3CkthdyFeBmSBxeCCm37%2FVO2c4Jddyrg0Bxwa5nZsZoaqUmsgxAAfeNV16%2B0BLM0QRyUCaTeCUaqUxQmykYPacdSua0oRNYtKCYQrbbIM7Ir01JIz3aHKyLr%2FDyn8dwbe7jbcanIIwVnNQfp%2BIb8fhGLMN69cbCYhDkicp2KUbvYCz9wAPJQ0%2FqovZlgF1cqncykHddOc2osZ6XzJlHDITudIarDvs5wR3sQbTeRnDgpjYyFPJA9RNewmAheOs2mlps7CjJuJI5f8vm7nQfDBEWZELOpFq2rdAFEPE28p4JteFCmSBYEnfGy1VSshXnKDD12TDP%2BNpdwB8zcahD34Ut%2Fj2Cgxr0Fepx8PJGt7s%2FGizm%2BMFCPzPoDk9SYpEnZJaT6GzogJCYtjkk06P%2BzLdlaPLbTL00ZJmHLUvQGhlk%2BOUL0Y2d%2B0SqK3rJHViybV7h35DOFgyos0TlHANzAzS%2B7WH6S

                                                        +---------------+          +-+
                               5V GND              +3v3-+VCC  MAX6675   |          |+|
                                | |                     |               |          +-+
                                | |           +---------+SCK           ++----------+ |
                                | |           |         |               |            |
                               ++-++          | +-------+CS             |            |
                           +---+USB+---+      | |       |               |            |
                           |  ESP8266  |      | |  +----+SO            -+------------+
                           | Adafruit  |      | |  |    |               |
                           | Feather   |      | |  | +--+GND            |
    +---------+            |        USB+-5V+  | |  | |  +---------------+
    |         |       +3v3-+3V         |      | |  | v
    | 5V   VCC+-5V+        |           |      | |  |
    | FAN     |         +--+GND     D15+------+ |  |
    |      GND+--+      |  |           |        |  |
    |         |  |      v  |        D13+--------+  |
    +---------+  v         |           |           |
                           |        D12+-----------+    +-----------------+
                           |           |                |     RBDDimmer   |
                           |         D5+----------------+PWM       AC-IN L+----- Thermofuse/ACL-IN
                           |           |                |                 |
                           |         D2+----------------+ZC        AC-IN N+----- COM
                           |           |                |                 |
                           +-----------+             v3-+VC               |
                                                        |           LOAD N+----- COM
                                                     +--+GND              |
                                                     |  |           LOAD L+----- Heater
                                                     v  +-----------------+

      _    _                                  
     | |  | |                                 
     | |__| | __ _ _ __  _ __  _   _          
     |  __  |/ _` | '_ \| '_ \| | | |         
     | |  | | (_| | |_) | |_) | |_| |         
     |_|  |_|\__,_| .__/| .__/ \__, |         
                  | |   | |     __/ |         
      _____       |_|   |_| _  |___/          
     |  __ \               | | (_)            
     | |__) |___   __ _ ___| |_ _ _ __   __ _ 
     |  _  // _ \ / _` / __| __| | '_ \ / _` |
     | | \ \ (_) | (_| \__ \ |_| | | | | (_| |
     |_|  \_\___/ \__,_|___/\__|_|_| |_|\__, |
                                         __/ |
                                        |___/ 
     
