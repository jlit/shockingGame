# R3 - A Shocking Game
A reaction, roulette, and recall game with consequences

<img width="1170" height="850" alt="image" src="https://github.com/user-attachments/assets/7adf0fb2-a625-4445-8a46-2b409839e039" />

# Description
This is an improvement over the ["Lightning Reaction Reloaded - Shocking Game"](https://www.amazon.com/dp/B0006B2Q50?ref=ppx_yo2ov_dt_b_fed_asin_title) available for $40 online.  In that game, players press the button on their controller when the game's random delay indicates.  The last player to respond gets an electric shock.  The game supports 2-4 players.

This improved version supports 2-5 players and adds two new game modes: "Roulette" amd "Recall".  Roulette is a verison of Russian Roulette and Recall similar to the old [Simon Game](https://en.wikipedia.org/wiki/Simon_(game)) from the 1970s.  In this mode, players must repeat an ever longer sequence.  This first player to mess up the sequence is shocked.

# Features
- Reaction Time game mode
- Russian Roulette game mode
- Recall Sequence game mode
- Up to 5 players
- Variable intensity adjustment knob to control shock
- Digital intensity display
- 30 multi-colored LEDs
- Sound

# Play
Insert 8 AA batteries into the compartment on the bottom of the game.  Begin by turning the power switch to On.  A series of LED annimations should appear and the Intensity digital display should show a number from 1.50 to 6.90.  

__IMPORTANT: If the display shows more than 7 (volts) do not play the game.__

Turn the intensity knob to the desired level.  The digital display shows the voltage.  Under 3 voltages is barely detectable.  3 volts is "normal".  Use above 3 volts at your own risk.  6.9 volts is the maximum and hurts.

## Reaction Mode
Turn the game mode knob to Reaction and press the Number of Players button until the desired number (2-5) of player LEDs illuminate.  An LED animation (green sequence of lights for each active player) will illuminate for each player.  Each player should pick up a controller near illuminated LEDs and grasp it so that their hand touches both aluminum strips.  Turn the intensity knob to the desired level.  See above.

Press the Start button (big white button in center) to start a round of play.  A sound and animation will play, followed by a random length of quiet time (2-10 seconds) and then all the LEDs will turn white and a beep will sound.  Each player should press the button on the their controller as soon as they sense the lights and sound.  The LEDs will illuminate to indicate the order the buttons were pressed.  The player with the fastest reaction time will show 1 blue LED, the seconds fastest 2 blue LEDs, etc. until the last player.  That player will show 4 blinking red LEDs and will have received a 100 ms shock.

The game has cheat detection.  Any player who has their button depressed when the game switches from delay to all white LEDs will be deemed a cheater.  All 4 of their LEDs will blink red and they will receive a 1 second shock for cheating.

After 5 seconds, the game will again display LED animation and wait for the next press of the Start button for another round.

## Russian Roulette Mode
Turn the game mode knob to Roulette and select the desired number of players (2-5).  An LED animation will illuminate (each player's 4 LEDs blue, one at a time, rotating around) for each player.  Each player should pick up a controller near illuminated LEDs and grasp it so that their hand touches both aluminum strips.

Press the Start button (big white button in center) to start a round of play.  In this mode, each player's LEDs illuminate white and the game waits for an active player to press their controller button.  When they do, they will receive a shock and their LEDs with blink red if they are the "target" player, otehrwise their LEDs will turn green.

After 5 seconds, the game will again display the LED animation and wait for the next button press with a new random target player.

## Recall Mode
Turn the game mode knob to Recall and select the desired number of players (2-5).  An LED animation will illuminate (random colored twinkles) for each player.  Each player should pick up a controller near illuminated LEDs and grasp it so that their hand touches both aluminum strips.  Turn the intensity knob to the desired level.  See above.

Press the Start button (big white button in center) to start a round of play.  A single player's LEDs will illuminate in a color specific to that player, and a short sound will play.  That player has 5 seconds to press their button once.  If they do not press their button within 5 seconds their LEDs will flash red and they will receive a shock.  If a different player presses their button they will receive the same treatment.  Either way, the game is over at this point.  

If the proper player presses their button (and only that player), the game progresses.  The same player's sound and LED will again illuminate, followed by a second player's LED and sound (or maybe the first player's twice -- it is random).  The players again have 5 seconds between each item in the sequence to press their button.  If the sequence is not repeated correctly, the offending player is shocked, their LEDS are turned red and the game is over.  If the sequence is mimicked correctly, the game continues with 3 items in the sequence, etc. until a player makes a mistake.  Note: the time between items in the sequence gets slightly shorter with each round.

Five seconds after a mistake, the game will again display LED animation and wait for the next press of the Start button for another round.

# Build
The game uses an Arduino micro-controller to handle the logic.  The shock circuits are harvested from toy shock pens and use a flyback transformer and oscillator operating at between 2.5-6 volts.  The controllers house a momentary button and two polished aluminum electrodes along the sides.  The base houses the control knobs, voltage display, LEDs, and center start button.  Most of the parts are 3D printed in PLA.

# Parts List
- [Arduino R4 Minima](https://www.amazon.com/Arduino-UNO-Minima-ABX00080-Connector/dp/B0C78K4CD4/ref=sr_1_3)
- [Battery Holder (8 AA)](https://www.amazon.com/dp/B07WP1CYYW)
- [Mini ON and Off Rocker Switch 12V 20A T85 2 Pin SPST](https://www.amazon.com/dp/B07S2QJKTX)
- [Momentary Tactile Push Button, 12 x 12 mm x 4.3 mm](https://www.amazon.com/TWTADE-Momentary-Tactile-Button-Switch/dp/B07CG6HVY9)
- [1 Pole 3 Position Rotary Switch (for game mode)](https://www.amazon.com/dp/B07JLVV478e)
- [1K Linear Rotary Potentiometer (for intensity)](https://www.amazon.com/dp/B082FCFSHX)
- [LM2596 DC-DC Buck Converter Step Down Module Power Supply](https://www.amazon.com/Zixtec-LM2596-Converter-Module-1-25V-30V/dp/B07VVXF7YX)
- [3 Wire 0.36" DC 0~30V Digital Voltmeter Digital Red LED Display](https://www.amazon.com/dp/B079N98PY4)
- [WS2812B IC RGB Individual Addressable LED Strip Light](https://www.amazon.com/LOAMLIN-WS2812B-Individually-Addressable-Waterproof/dp/B0956C7KFR)
- [Momentary Stainless Steel Push Button On/Off (High Head)](https://www.amazon.com/dp/B08L49F7DV?ref=ppx_yo2ov_dt_b_fed_asin_title) (Quantity 6)
- [Aluminum Flat Plate Bar Stock 3 x 10 x 305mm T6511](https://www.amazon.com/dp/B0CZ4F77TL)
- [22 Gauge 4 Conductor Electrical Wire](https://www.amazon.com/dp/B0CFJXMDT3?ref=ppx_yo2ov_dt_b_fed_asin_title)
- [https://www.amazon.com/dp/B07BDJJTLZ)](https://www.amazon.com/dp/B0B4N3SF9R](5V 4 Channel Relay Optocoupler Isolation Module]([[https://en.wikipedia.org/wiki/2N2222 (Quantity 5))
- [Toy Shock Pen](https://www.amazon.com/dp/B0DZWW5TX7?ref=ppx_yo2ov_dt_b_fed_asin_title) (Quantity 5)
- [220 Ohm Resistor](https://www.amazon.com/dp/B07QK9ZBVZ) (Quantity 5)
- Wire, solder, PLA filament, zip ties, screws, etc.

# Electrical Circuit
The game uses 8 AA batteries for 12v DC to power the Ardunio and a LM2596 DC-DC Buck Converter.  The output of the buck converter is controlled by an "intensity" rotary 0-1k poteniometer knob, resulting in 2-7v DC output to drive the shock circuits.  The output voltage is displayed on a 3 digit 8-segment display.  The shock circuits use a flyback transformer (looks like a blue capacitor) and a small oscillator.  Each shock circuit is controlled by and isolated from the Arduino via opt-isolated relays.  All buttons and switches are connected to GPIO pulled up pins on the Arduino and complete the circuit to ground.  The WS2812 LED strip uses 5v power from the Arduino and a single data GPIO pin to control the individually addressable LEDs.

<img width="1198" height="678" alt="image" src="https://github.com/user-attachments/assets/ff211c23-4eb7-4024-8486-9565b3ea921c" />

 # CAD Designs
 All components of the game were designed in Autodesk Fusion 360.
 
<img width="293" height="489" alt="image" src="https://github.com/user-attachments/assets/d421cab5-d959-4607-b82d-009cb31ec9aa" />
<img width="309" height="491" alt="image" src="https://github.com/user-attachments/assets/1fe33fa7-5465-4f63-8d3d-09754b71baa6" />
<img width="735" height="522" alt="image" src="https://github.com/user-attachments/assets/a4f3be4d-176a-4311-a6a9-f49854159f1b" />
<img width="703" height="622" alt="image" src="https://github.com/user-attachments/assets/f135685f-7c70-4428-9296-1cac9cc16f33" />
<img width="754" height="504" alt="image" src="https://github.com/user-attachments/assets/4eb01ff9-87ce-4d5b-b19e-353c741b00dd" />
<img width="681" height="447" alt="image" src="https://github.com/user-attachments/assets/d7d9d3c0-115e-4b3b-942c-d51dd36f27ee" />
<img width="748" height="640" alt="image" src="https://github.com/user-attachments/assets/286a0173-3df8-40f9-8351-cb6ba78d1af7" />
<img width="708" height="538" alt="image" src="https://github.com/user-attachments/assets/31e67aa3-07a8-4148-966f-374ff044f835" />

# Build
## 3D print all the parts
- Base
- Base Top
- Battery Cover
- Start Button
- Start Button Mount
- Controller Handle (x5)
- Controller Cover (x5)
- Storage tray

## Shock Circuits
- To build the shock circuit, disassemble a toy shock pin by unscrewing the tip and breaking the middle black portion from the silver top portion.
- Using pliers for grip, pull the black tube out of the top.
- There will be a blue capacitor, a very small circuit board with a white oscillator dot in the middle, three small batteries, and a spring.
- Remove the blue capacitor by unbending the lower terminal wire.
- Solder two terminals of the capacitor to the pads on the circuit board they were touching when in the pen.
- Solder a wire to the other terminal of the capacitor.
- Solder two wires to the other two pads on the other end of the circuit board.
- If you apply 3-6 volts to those two wires and then touch the ground wire and the 3rd terminal of the capacitor you will receive a shock.
- Reuse the batteries for some other project.

<img width="1340" height="711" alt="image" src="https://github.com/user-attachments/assets/c2ca1e09-a3b7-4e19-afbe-a58d9ecb0b31" />


## Controllers
- Prepare to build the controllers by cutting the aluminum bar stock to 60mm lengths for each electrode.  
- Drill and tap a hole for an M3 screw in each.  The holes should by 7mm from the top and 7mm from the outside edge of each bar.

<img width="709" height="294" alt="image" src="https://github.com/user-attachments/assets/cf31b7a1-7da7-4743-9acf-5cbc60ac19d8" />

<img width="1476" height="809" alt="image" src="https://github.com/user-attachments/assets/7dc9b833-abec-4484-82ee-39ea0d0571df" />

- Round the outside corners of each and polish.
- Cut the 4-conductor wire into 5 30" lengths.
- Assemble all the parts (including 3mm screws and washers)

<img width="796" height="743" alt="image" src="https://github.com/user-attachments/assets/28510b57-99c4-4491-a770-e9337ebd69e2" />

- Cut the wires from the buttons and remove any remaining heat shrink.
- Strip 90mm of the outside cover from one end of each wire and strip and tin the ends of each conductor.  
- For each controller,
  - Thread the button into the top of the controller.
  - insert the wires from the bottom.  
  - Solder two wires (red and white) to the two button terminals.
  - Using and M3 screw and washer, secure the other two wires to two electrode bars.
  - Secure a zip-tie around the 4-conductor wire where it exits the hole in the bottom of the controller.
  - Place the bars into the slots on the cover and slide the cover under the top of the controller.
  - Secure the cover in place with one M3 screw.

## LEDs
- Cut the WS2812 LED strip into 4 lengths of 4 LEDs.  Be sure to cut at designated cut lines on the strip.
- Solder 3 wires (5v, data, ground) between each strip.  Be certain to keep the arrows pointing in the same direction.
- Using a hot glue gun, glue the strips into the top of the game so that the LEDs align with the holes.
- Cover each strip with tape.

<img width="1127" height="847" alt="image" src="https://github.com/user-attachments/assets/e3ac9646-a357-422e-8ea9-f0aade4a01c3" />

<img width="1042" height="844" alt="image" src="https://github.com/user-attachments/assets/9b2fb63e-d9aa-4205-97a2-110117ade754" />

## Start Button
- CA glue the 12mm button switch to the start button mount panel with the pins sticking through the holes.
- Solder wires to two pins on one side of the switch.
- Place the start button into the top, cover, and attach the switch cover to the top with 5 screws.

## Knobs and Switches
- Solder wires to the terminals of the 3 position rotary game mode switch.
- Insert rotary switch into the top, place the label, secure with the provided washer and nut, and attach a knob.
- For the number of players button, insert the button into the top, place the label, secure with the provided nut from below.
- For the intensity control, the on-board potentiometer on the LM2596 DC-DC Buck Converter needs to be replaced with the 1k rotary potentiometer.
- Using a soldering iron, remove the blue pot.
- Solder wires to the 3 terminals of the 1k rotary pot and solder the other ends to the LM2596.
- Insert the rotary pot into the top, place the label, secure with the provided washer and nut, and attach a knob.
- Insert the digital voltage display into the top and secure with 2 m2 screws.
- Insert the power toggle switch into the side of the top unit.

## Wiring
- Thread each controller wire into the holes on the side of the top unit.
- Connect the switches, controllers, etc. according to the diagram above.

<img width="478" height="622" alt="image" src="https://github.com/user-attachments/assets/9d32ff9f-eac4-45e3-9963-890d270f4d46" />

## Programming
- Download the code in the code folder to the Arduino





