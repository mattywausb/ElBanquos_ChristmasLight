# ElBanquos Christmas Lights - Manual

## Functions
The system contains multiple programs, that can be accessed by using the two buttons:
* Mode
* Step
 
Especially for assembly and installation there are multiple test programs to verify the electrical function and the correct arrangement of the lamps.

The build in LED on the Arduino board is used to give feedback about the mode.

### Show Mode (Default Mode when switched on)
The show mode shows the static picture for a random number of seconds. After the wait time is over, a random new picture is determined and the program switches to transition mode.

Status LED: OFF

Press MODE-> Countdown Mode

Press STEP-> Start transition to next picture in list

### Transition Mode
The Transition Mode modifies the lamps slowly in random order to transition to the next picture. After it is done, the mode switches to Show Mode.

Status LED: ON

Press MODE->  Countdown Mode

Press STEP-> Foreward transition to target picture an switch to show mode. 

### Countdown Mode
The Countdown Mode will display time of an internal clock and switch to fireworks automatically at 00:00. 
To undersatnd the time display, see description below.

Status LED: OFF

Press MODE-> Switch to Fireworks (Will be done automatically when reaching 00:00:00)

Press STEP-> Switch to Set Hour

### Set Hour Mode
The Set Hour Mode is used to set the Hour of the countdown. (Indicated by blue blinking Middle light)

Status LED: Blinking (2 per Second)

Press MODE-> 1 Hour Backward

Press STEP-> 1 Hour Forward

STEP Hold 2 sec and release: Switch to Set Minute Mode


### Set Minute Mode
The Set Hour Mode is used to set the Hour of the countdown. (Indicated by blue blinking Middle light)

Status LED: Blinking (4 per Second)

Press MODE-> 1 Minute Backward

Press STEP-> 1 Minute Forward

STEP Hold 2 sec and release: Start Clock (with 0 Seconds) and switch to Countdown Mode


### Fireworks Mode
The Fireworks Mode plays randomly chosen firework animations

Status LED: ON in show pause, OFF during running pattern
Press MODE-> Switch to Show Mode

Press STEP-> Change to next fireworks pattern

MODE Hold 2 sec and release: Switch to Placement Test

### Placement Test
The placement test shows a rainbow color pattern on the different circles, starting with red on the upper lamp and forwarding clockwise.
The circles are: Inner Pentagon, outer Pentagon, inbetween circle, additonal Lamps

Status LED: OFF

Press MODE-> Switch to Picture Test

Press STEP-> Switch to next circle

### Picture Test
The Picture Test shows a picture until it is stopped.

Status LED: OFF

Press MODE-> Switch to Fade Solo Test

Press STEP-> Switch to next picture

### Fade Solo Test
The Fade Solo Test fades one lamp fading green value from 0 to 255. Should be used while assembling the chain, to test added lamps.

Status LED: OFF

Press MODE-> Switch to Fade Ensemble Test

Press STEP-> Switch to next lamp in the physical chain

### Fade Ensemble Test
The Fade Ensemble Test lights all lamps in cyan and a selected lamp with a fading red. Should be used while assembling the chain, to approve conductivity.

Status LED: OFF

Press MODE-> Switch to Scaling Test

Press STEP-> Switch to next lamp in physical chain

### Scaling Current Test
The Scaling test lights lamps in with fading white, starting with one lamp. Should be used to check, if assemble cables and power supply will provide enough current. When all lamps are running, you have reached the worst case consumption scenario. Fading must still be smooth and steady-

Status LED: OFF

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


