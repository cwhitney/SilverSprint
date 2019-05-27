SilverSprints - [http://SilverSprints.com](http://silversprints.com)
=============
A front end for the [OpenSprints](https://www.opensprints.com) hardware kit.  It allows for the racing of 1 to 4 bikers against each other in a goldsprints style race.  The software is free to use and modify and you're encouraged to hold your own races!  See _LICENSE_ for more details.

[SilverSprints](http://silversprints.com/) is for OS X (10.7 and up) and Windows (7 and up).

Written in [Cinder](https://libcinder.org/), an opensource C++ library for creative coding.

## Using the Application

#### Requirements
* Requires OS X version 10.7 and up.
* PC requires Windows 7 and up.

#### Installation
1. Download the latest [SilverSprints](https://github.com/cwhitney/SilverSprint/releases/latest) for your operating system.
2. In the downlaoded zip file, there is an `Arduino` folder with software you'll need to load onto the OpenSprints hardware.  Detailed instructions on how to do this can be found here: [http://cwhitney.github.io/SilverSprint/installation.html](http://cwhitney.github.io/SilverSprint/installation.html).
 
#### App Settings
1. Go to the Settings page by clicking the gear icon or pressing Command + 3.
2. Set the roller diameter as the distance from the magnet to the center of the roller multiplied by 2.
3. Choose the number of racers competing from 1-4.
4. If SilverSprints detects an Arduino connected it will show a checkmark, otherwise it will display an X.

#### Roster
1. Go to the Roster page by clicking the list icon or pressing Command + 2.
2. Set all active participants' names.

#### Race
1. Click the START button to begin a race.
2. At any time, press STOP to stop and reset the race.

## Editing / Creating New Graphics
If you just want to swap graphics, simply replace the graphics in the assets folder. No need to recompile.

## Troubleshooting
In the event of a crash, a log file will be created in a folder called "logs" in the SilverSprints directory.
Please attach this file when asking for support along with any other relevant information.

## Code Setup
1. Clone the repo with all of its submodules with: `git clone https://github.com/cwhitney/SilverSprint.git --depth 1 --recursive`
2. Build Cinder for your platform [OS X](https://libcinder.org/docs/guides/mac-setup/index.html) - [Windows](https://libcinder.org/docs/guides/windows-setup/index.html).
If you're having trouble building Cinder, it may be missing some of its dependencies.  You may need to run `git submodule update --init --recursive` to make sure Cinder has all of its submodules.
3. The structure of the repo is like so:  

```
Root	
  - apps  
    - Arduino
    - SilverSprints
  - libs
    - Cinder
```
4. Update Arduino if necessary. (See above.)
5. Open the project file for your platform. It will be in `apps/SilverSprints/xcode/SilverSprint.xcodeproj` for OS X, and `apps/SilverSprints/vs2015` for PC.
6. Build SilverSprints. There are no external dependencies aside from Cinder itself.

## Credits

* Project website [http://silversprints.com](http://silversprints.com)
* Code by [Charlie Whitney](http://sharkbox.com)
* Design by [Erica Gorochow](http://gorociao.com)
* Hardware [OpenSprints](https://www.opensprints.com)
