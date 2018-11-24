# ElBanquos Christmas Lights
An Ardiuno project using Neopixels to create slowly changing christmal light pictures in my window.

## Whats the idea ?
I like to amaze people with riddles and small wonders and wanted. I do this by providing some little miracles that can only be noticed by people that observe the world around them (its not for the 7x24 smartphone viewers). A long term vision was doing this with  christmas decoration lights that change their pattern or picture without attracting attention by blinking or fast changing colors.

People should see the picture in the window and next time they come by will see another picture. They may wonder if they remembered it correctly or they might think that the guy behind the window changes it manually every day.  Only if they watch long enough they will see, how the pattern of light evolves slowly from one picture to another. Maybe they get curious enough to wait, what picture will come next, an if they have seen them all.

Since you can only buy christmas ligh chains with a steady lighting or with color chasing patterns that will give you a nervous breakdown I started to build my own.

## Features
* Easy maintainable table to describe the light patterns
  * Picture is described by using a "logical" lamp arrangemant. Translation to physical arrangement on the window is done in the software.
  * Colorpalette
  * Pixels can be distributed over multiple chains but appear in a linear adressspace
* Randomized transformation from one picture to the next
  * Random next picture
  * Random transformation order and timing of the pixels
  * Asynchronous an overlapping transformation of the pixels
  * Random hold time for one picture
* Testmodes to check 
  * correct pixel position
  * Pictures
  * Pixelfunction

## Whats in this repository
* Ardunio sketches for testing and final application
* Inkscape SVG with my light pattern with 24 lamps 
* Fritzing circuit documentation
* Pictures of different assembly state phases to give you an idea how to build it

## Hardwarelist
* Arduino Uno board (or anything similar with 5 Digital Outs and 2 KByte SRAM)
* 24 Neopixel 8mm throughole LED (or more)
* enough 3 core cable that is flexible and as thin as possible (I used old USB mouse wires)
* enough prototype PCB to cut the small connection pcb
* 2 buttons
* whires 
* small breadboard (I used the Uno Starterkit)
* powert supply 5v / 1,5 A (for 24 depends on amount of lamps)

## Softwarelist
* Arduino 
* Neo Pixel Library
* Fritzing (optional)
* Inkscape (optional)

## Main steps to build
* Assemble test circuit on the breadboard
* Test LED's on the breadboard (up to 8 when using only Adrunio 5V power)
* Cut the small PCB boards
* Cut the mouse cable an remove isolations
* assemble chain
  * plug that connects to the breadboard
  * 
