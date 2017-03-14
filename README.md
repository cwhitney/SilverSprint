[SilverSprint](http://cwhitney.github.io/SilverSprint/)
=============
A front end for the [OpenSprints](https://www.opensprints.com) hardware kit.  It allows for the racing of 1 to 4 bikers against each other in a goldsprints style race.  The software is free to use and modify and you're encouraged to hold your own races!  See _LICENSE_ for more details.

[SilverSprint](http://cwhitney.github.io/SilverSprint/) is for OSX (10.7 and up) and Windows (7 and up).

Written in [Cinder](https://libcinder.org/), an opensource C++ library for creative coding.  
The version of Cinder at the time of coding was 0.9.0 [[Download](https://github.com/cinder/Cinder/releases/tag/v0.9.0)]

##Using the Application
####Installation
1. Download the [Arduino Desktop IDE](https://www.arduino.cc/en/main/software)
2. Use the IDE to upload this [custom sketch file](https://raw.githubusercontent.com/cwhitney/SilverSprint/master/Arduino/basic_msg/basic_msg.ino) to the Arduino Uno in the OpenSprints Hardware.  If you need to revert it to work with [GoldSprintsFX](http://www.goldsprintsfx.com) upload the [default basic_msg sketch](https://github.com/opensprints/basic_msg/releases/latest).
3. Download the latest [SilverSprint](https://github.com/cwhitney/SilverSprint/releases/latest) for your Operating System.  No additional software is needed.

####Settings
1. Go to the Settings page by clicking the gear icon or pressing Command + 3
2. Set the roller diameter as the distance from the magnet to the center of the roller multiplied by 2.
3. Choose the number of racers competing from 1-4
4. If SilverSprint detects an Arduino connected it will show a checkmark, otherwise it will display an X.

####Roster
1. Go to the Roster page by clicking the list icon or pressing Command + 2
2. Set all active participants names

####Race
1. Click the START button to begin a race
2. At any time, press STOP to stop and reset the race.

##Code Setup
1. Clone Cinder 0.9.0 or download via zipped archive. [[Cinder Github](https://github.com/cinder/Cinder)]
2. SilverSprint wants to be in a directory one above the Cinder root. For example:  


```
- Cinder
   - apps  
     - SilverSprint
   - blocks  
   - docs
   - include
   - lib
   - ...
```
3. Open the project file for your platform. It will be in `SilverSprint/xcode/SilverSprint.xcodeproj` for mac, and `SilverSprint/vs2013` for PC.
4. Build and enjoy. There are no external dependencies aside from Cinder itself.

##Editing / Creating New Graphics
Coming soon.

##Troubleshooting
Requires OXS version 10.7 and up.
PC requires Windows 7 and up
In the event of a crash, a log file will be created in a folder called "logs" in the SilverSprint directory. Please attach this file when asking for support along with any other relevant information.

-
Project website http://cwhitney.github.io/SilverSprint/
* Code by [Charlie Whitney](http://sharkbox.com)
* Design by [Erica Gorochow](http://gorociao.com)
* Hardware [OpenSprints](https://www.opensprints.com)
