# ElBanquos Christmas Lights - Manual

## Functions
The system contains multiple programs, that can be accessed by using the two buttons:
* Mode
* Step
 
Especially for assembly and installation there are various test programs to verify the electrical function and the correct arrangement of the lamps.


### Comprehensive mode change table:

|Current Mode|Press "Mode"|Press "Step"|Hold "Mode" Long|Hold "Step" Long|
|---|---|---|---|---|
|Show|>Countdown|>Transition||
|Transition|>Countdown|>Show||
|Countdown|>Fireworks|>Set Hour||
|Set Hour|Hour down|Hour up||>Set Minute|
|Set Minute|Minute down|Minute up||>Countdown|
|Fireworks|>Show|Next Effect|>Sensor Calibration||
|Sensor Calibration|>Show|>Tst: Placement|||
|Tst: Placement|>Tst: Palette|Switch Lamp group||
|Tst: Palette|>Tst: Pictures|Switch color of inner lamps||
|Tst: Picture|Previous Picture|Next Picture|>Tst: Fade Solo||
|Tst: Fade Solo|>Tst: Fade Ensemble|Next Lamp||
|Tst: Fade Ensemble|>Tst: Scaling|Next Lamp||
|Tst: Scaling Current|>Show|Add Lamp||

## Components and setup

* LED Light chains (currently 3 chains with 8 lamps each)
* controller board with
   * arduino
   * connectors for the light chains
   * Buttons
   * light sensor
   * light threshold control
   * USB Power cable Plug (Delivers power only, not data of arduino)
* USB Power Supply (5 V with at least 1.5 A)

This will not work with USB power supplies, which provide higher current only after been requested. (most USB power supplies don't care)

Maximum power consumption: LEDS (24x60mA) + Controllerboard (80mA) = 1.5 A 


## Setup
* Position light chains on the window
* connect light chains to controller board
* connect power

Never connect/disconnect light chains, when board has power. After disconnect from power, wait 1 Minute until connectin/disconnecting the light chains. This guarantees full discarge of the controller boards buffer 1000uF capacitor.

After connecting, check out the Testprogramm
   

## Mode descriptions

### Show Mode (Default Mode when switched on)
The show mode shows the static picture for a random number of seconds. After the wait time is over, a random new picture is determined and the program switches to transition mode.


Press MODE-> Countdown Mode

Press STEP-> Start transition to next picture in list

#### Daylight detection
The show mode uses a photocell to detect the ambient light situation. Only during low light, the show mode will choose one of the normal pictures. During "dailight", the show mode will activate one single lamp as an power indicator.

The threshold to determine "dailight" is set by the potentiometer. For adustment see Tst: Light sensor calibration.
When in daylight opreration, the sensor will be checked for a change every 60 seconds.

### Transition Mode
The Transition Mode modifies the lamps slowly in random order to transition to the next picture. After it is done, the mode switches to Show Mode.


Press MODE->  Countdown Mode

Press STEP-> Foreward transition to target picture an switch to show mode. 

### Countdown Mode
The Countdown Mode will display time of an internal clock and switch to fireworks automatically at 00:00. 
To understand the time display, see description "Hot the countdown time is displayed" below or check the table on the userpanel.

Press MODE-> Switch to Fireworks (Will be done automatically when reaching 00:00:00)

Press STEP-> Switch to Set Hour

### Set Hour Mode
The Set Hour Mode is used to set the hours left of the countdown. 

During the Set Hour Mode, the center lamp is **blinking red**

Press MODE-> 1 Hour Backward

Press STEP-> 1 Hour Forward

STEP Hold 2 sec (center lamp turns white) and release: Switch to Set Minute Mode


### Set Minute Mode
The Set Minute Mode is used to set the minutes left of the countdown. 
During the Set Hour Mode, the center lamp is **blinking blue**

Press MODE-> 1 Minute Backward

Press STEP-> 1 Minute Forward

Hold STEP for at least 2 sec (center lamp turns white): The release will start Clock (with 0 Seconds) and switch to Countdown Mode


### Fireworks Mode
The Fireworks Mode plays randomly chosen firework animations

Press MODE-> Switch to Show Mode

Press STEP-> Change to next fireworks pattern

MODE Hold 2 sec and release: Switch to Sensor Calibration

### Sensor Calibration
Sensor calibration activates most of the lamps in white and display the 
deviation between the current light sensor value and the threshold as a red lamp. 
The scale is formed by lamps E,10,K,11,A,1,G,2,8 (See user panel for lamp identification). The middle lamp on the star tip (A),
represents the current sensor value. The red indicated postion shows the current threshold, 
defined by the user setting. Placing the threshold to the left (E,10,K,11) defines the current
sensor value as lowlight. Setting it to the right (1,G,2,B) , defines the current sensor value as daylight.
One lamp stands for one histeresys interval. 
Value changes inside the histeresys interval will not change the current low/day state. 
To adjust the threshold for a spefic light level, wait for the level to come up and place the red dot 
in the center (A).

Press MODE -> Switch to Placement Test

Press STEP -> Switch to Show Mode

This calibration mode is only for easy search of the threshold.  The setting of the threshold is measured independently at every change of a picture, 
so adjustments are accepted, event  without using this mode.

Also be shure to place the sensor in a, it is not affected by the light of the christmas pictures. To help you
when tracing he path of the light, this mode switches most of the lights to white.
Placing some shade over the sensor, might be necessary. If the light of the pictures can reach the sensor,
some pitures might be bright enough to trigger the day mode.

### Placement Test
The placement test shows a rainbow color pattern on the different groups of lamps:

Press MODE-> Switch to Palette Test

Press STEP-> Switch to next group

The groups are:
* Chain identification: Three lamps in the center should be red,green,blue from left to right (if not you might have swapped cables when plugging)
* outer Pentagon/Star tips: Red on 12 o'clock, going clockwise to yellow, green, cyan, blue
* Middle circle: Red on 12 o'clock, going clockwise to yellow, green, cyan, blue (the 2nd lamp of a pair will be brighter)
* Inner Pentagon: Red on 6 o'clock, going clockwise to yellow, green, cyan, blue
* Bottom: Red, Middle yellow, "Head left" green, "Head right" blue

Should this be mixed up you need to check cables or placement of the lamps


### Palette Test
The palette test shows every palette color on a single lamp (Starting with lamp 6), and allows swithching through all palette colors on the inner ring and center lamp.

Press MODE-> Switch to Picture Test

Press STEP-> Switch inner lamps to next palette color

### Picture Test
The Picture Test shows a picture until it is stopped.

Press MODE-> Switch to Fade Solo Test

Press STEP-> Switch to next picture

Hold MODE for 2 Seconds -> Swith to Fade Solo Test

### Fade Solo Test
The Fade Solo Test fades one lamp fading green value from 0 to 255. Should be used while assembling the chain, to test added lamps.

Press MODE-> Switch to Fade Ensemble Test

Press STEP-> Switch to next lamp in the physical chain

### Fade Ensemble Test
The Fade Ensemble Test lights all lamps in cyan and a selected lamp with a fading red. Should be used while assembling the chain, to approve conductivity.

Press MODE-> Switch to Scaling Test

Press STEP-> Switch to next lamp in physical chain

### Scaling Current Test
The scaling current test lights lamps in with fading white, starting with one lamp. 
Should be used to check, if assemble cables and power supply will provide enough current. 
When all lamps are running, you have reached the highest power consumption possible. 
Fading must still be smooth and steady on all lamps an the should be no change in the white color. 
(When white is getting warmer, this indicates unsufficient power)

Press MODE-> Switch to Show Mode

Press STEP-> Add next lamp in physical chain.

# How the countdown time is displayed
The time display is designed to appear very static and add more movement, the nearer the time is to 00:00 resulting in a 1 Minute Countdown at 23:59.
It uses color and positions to symbolize the time as follows:

* Outer star points = Hours
* middle led Circle = Minute 0-58, Seconds of Minute 59
* inner 5 led circle = seconds (only for hour 23)
* center light = seconds (for hour 18 to 22)

In general colors transistion from blue/ green over white to orange/red, so red is always the last color before a change to a next element.
The minute ring displays "unused" minute lamps in yellow and used minute lamps in blue, changing the color of the current lamp accordingly to the upper schema.


The 5 hour lamps will light as follows (going backwards)
* 23:00 - all off
* 20:00 - 3 red 2 off
* 18:00 - all red
* 15:00 - 3 orange 2 red
* 13:00 - all orange
* 12:00	- all yellow
* 11:00 - all green
* 10:00 - all cyan
* 8:00 -  2 blue 3 cyan
* 5:00 - all blue
* 1:00 - 1 blue
* 0:00 - 2 pink lamps on the bottom (but normally here is the fireworks to see)

The minute lamps have some easy to recognize  positions/color as follows:
00 - all yellow
15 - 3rd right white
30 - bottom middle white
45 - 3rd left white

# Mode indication on the built in LED
The built in LED should not be visible in the final casing, but can be helpful during development:

* Show Mode: OFF
* Transition running: ON
* Countdown Mode: OFF
* Set Hour Mode: Blinking 2 per Second
* Set Minute Mode: Blinking 4 per Second
* Fireworks Mode: ON in show pause, OFF during running pattern
* All Tests: OFF

