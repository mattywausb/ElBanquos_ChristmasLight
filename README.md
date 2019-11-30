# ElBanquos Christmas Lights
An Ardiuno project using Neopixels to create slowly changing christmas light pictures in my window.

![demo_ensemble](https://raw.githubusercontent.com/mattywausb/ElBanquos_ChristmasLight/master/Documentation/demo_pictures/ensemble.png
      "some Impressions")

## Whats the idea ?
I like to amaze people with riddles and small wonders. My favourinte approach is to establish little miracles that can only be noticed by people that observe the world carefully (its not for the 7x24 smartphone viewers). A long term vision was doing this with christmas decoration lights that change their pattern or picture without attracting attention by blinking or fast changing colors.

People should see the picture in the window and next time they come by will see another picture. They may wonder if they remembered it correctly or they might think that the guy behind the window changes it manually every day.  Only if they watch long enough they will see, how the pattern of light evolves slowly from one picture to another. Maybe they get curious enough to wait, what picture will come next and if they have seen them all.

Since there is not programmable christmas light chain in the stores, I have built my own.

The following 
## Features
* Easy maintainable programming of the light patterns
  * Picture is described on a logical lamp arrangemant. Translation to physical arrangement on the window is done at runtime.
  * Colorpalette helps to keep colors conistent an save memory.
  * Pixels can be distributed over multiple chains but appear in a linear adressspace
* Randomized transformation from one picture to the next
  * Random next picture
  * Random transformation order and timing of the pixels
  * Asynchronous and overlapping transformation of the pixels
  * Random hold time for one picture
* Testmodes to check 
  * correct pixel position
  * Pictures
  * Pixelfunction

## Whats in this repository
* Ardunio sketches for testing pixels and the final application
* Inkscape SVG with my light arrangement with 24 lamps 
* Fritzing circuit documentation
* Pictures of different assembly state phases to give you an idea how to build the chains
* [manual] (Documentation/manual_en.md) 

## Hardwarelist
* Arduino Uno board (or anything similar with 5 Digital Outs and 2 KByte SRAM)
* 24 Neopixel 8 mm throughole LED (or more)
* enough 3 core cable that is flexible and as thin as possible (I used old USB mouse wire, white = data, red+green=+5V, black+ground=gound)
* enough prototype PCB to cut the small connection pcb for every lamp
* 2 buttons
* whires 
* small breadboard (I used the Uno starterkit)
* "sugru" glue clay to protect the led mounting PCB's
* powert supply 5V / 1,5 A (for 24 pixels)
* scotchtape

## Softwarelist
* Arduino 
* Neo Pixel Library
* Fritzing (optional)
* Inkscape (optional)

## Tools
* Soldering iron 
* soldering holder
* wire stripper for very small diameters
* wire cutter
* pcb cutting tool (I used a saw)

## Main steps to build
* Assemble test circuit on the breadboard
* Test LED's on the breadboard (up to 8 when using 5V Adrunio board power)
* assemble every chain
  * Cut the small PCB boards (1 for the plug and 1 for every lamp)
  * Cut the mouse cable an remove isolations (1 for every lamp, 1st should be longer to reach from the window to the uno)
  * Create a 3 pin plug at one end of the long whire
  * Add pcb/Neopixel and continuation whire as often as needed (I made 3 chains of 8 pixels)
### Build effort
Initial time is needed to set up the uno and breadboard for testing.
After getting into a stable workflow it took me about 10 Minutes for every lamp (Cut pcb, cut cable, solder lamp, test). So all in all 240 Minutes for 24 Lamps (4 Hours) for assembling the chains. With better soldering technique and tools this might get better.

### Main material costs 43 EUR
* roundabout 25 Euro for 25 Neopixels (I killed one by testing with wrong polarity, thats why i have only 24)
* 8 Euros for the Sugru Glue Clay (1 Package with 3 portions)
* Old usb mouse cables should be available at every flea market
* My dedicated power supply const 10 Eur but mostly you already have some in stock


