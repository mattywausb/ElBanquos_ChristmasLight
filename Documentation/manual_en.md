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
The Countdown Mode can be set to coutndown to Midnight

Status LED: OFF

Press MODE-> Switch to Fireworks (Will be done automatically when reaching 00:00:00)

Press STEP-> Switch to Set Hour

### Set Hour Mode
The Set Hour Mode is used to set the Hour of the countdown. (Indicated by blue blinking Middle light)

Status LED: OFF

Press MODE-> 1 Hour Backward

Press STEP-> 1 Hour Forward

STEP Hold 2 sec and release: Switch to Set Minute Mode


### Set Minute Mode
The Set Hour Mode is used to set the Hour of the countdown. (Indicated by blue blinking Middle light)

Status LED: OFF

Press MODE-> 1 Minute Backward

Press STEP-> 1 Minute Forward

STEP Hold 2 sec and release: Start Clock (with 0 Seconds) and switch to Countdown Mode


### Fireworks Mode
The Fireworks Mode plays randomly chosen Firework Animations

Status LED: ON in show pause, OFF during show

Press MODE-> Switch to placement Test

Press STEP-> Switch to Show Mode

STEP Hold 2 sec and release: Switch to Placement Test

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