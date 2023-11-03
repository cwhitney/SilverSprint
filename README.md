SilverSprints - [http://SilverSprints.com](http://silversprints.com)
=============

This repo contains an updated OSX project to support newer OSX ARM based processors as well as general updates.


## Building instructions for OSX
1. Clone the repo into a new folder: `git clone https://github.com/cwhitney/SilverSprint.git`
2. We need to pull in Cinder as a submodule: `git submodule update --init --recursive`
3. We need to build the Cinder library so we can link against it in. Open `libs/Cinder/proj/xcode/cinder.xcodeproj`
   - The default configuration is for debug, and if you run `Product > Build` it should create the library in `Cinder/lib/macosx/Debug/libcinder.a`
   - You can alternately Edit the scheme and change the build configuration to `Release` if you also want to build that, but it's not strictly necessary.
   - If you're having trouble building Cinder, it may be missing some of its dependencies.  You may need to run `git submodule update --init --recursive` to make sure Cinder has all of its submodules.
4. Open the SilverSprints project file in `apps/SilverSprints/xcode/SilverSprint.xcodeproj`
6. Build SilverSprints. There are no external dependencies aside from Cinder itself.
  - If you only built Debug for Cinder, make sure you're also building the Debug configuration for Silversprints
  - The app should build to `apps/SilverSprints/xcode/build`
