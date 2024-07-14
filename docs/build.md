## To build tux4common on Ubuntu 22.04

### Tools : Autoconf, Automake and Autopoint

`sudo apt install autoconf automake autopoint`

### Libraries:

1. GetText: `sudo apt-get install gettext-dev`
2. SDL: `sudo apt-get install libsdl1.2-dev`
3. Libtool: `sudo apt-get install libtool`
4. SDL Image: `sudo apt-get install libsdl-image1.2-dev`
5. SDL Mixer: `sudo apt-get install libsdl-mixer1.2-dev`
6. SDL Pango: `sudo apt-get install libsdl-pango-dev`
7. SDL Net: `sudo apt-get install libsdl-net1.2-dev`
8. RSVG: `sudo apt-get install librsvg2-dev`
9. XML: `sudo apt-get install libxml2-dev`
10. ESpeak: `sudo apt-get install libespeak-dev`

Once you have installed these libraries, you can use the following commands to build tux4common:

1. `mkdir build`
2. `cd build`
3. `autoreconf -if ..`
4. `../configure`
5. `make`
6. `sudo make install`
