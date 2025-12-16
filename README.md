# shockingGame
A reaction time and memory game with consequences

# Description
This is an improvement over the ["Lightning Reaction Reloaded - Shocking Game"](https://www.amazon.com/dp/B0006B2Q50?ref=ppx_yo2ov_dt_b_fed_asin_title) available for $40 online.  In that game, players press the button on their controller when the game's random delay indicates.  The last player to respond gets an electric shock.  The game supports 2-4 players.

This improved version supports 2-5 players and adds a second "memory" game option, similar to the old [Simon Game](https://en.wikipedia.org/wiki/Simon_(game)) from the 1970s.  In this mode, players must repeat an ever longer sequence.  This first player to mess up the sequence is shocked.

# Features
- Reaction Time game mode
- Memory Sequence game mode
- "Test" game mode
- Up to 5 players
- Variable intesity adjustment knob to control shock
- Digital intensity display
- 30 mutli-colored LEDs
- Sound

# Build
The game uses an Arduino micro-controller to handle the logic.  The shock circuits are harvested from toy shock pens and use a flyback transformer and oscillator operating at between 2.5-6 volts.  The controllers house a momentary button and two polished aluminum electrodes along the sides.  The base houses the control knobs, voltage display, LEDs, and center start button.  Most of the parts are 3D printed in PLA.

# Parts List
- [Arduino Nano](https://www.amazon.com/Arduino-A000005-ARDUINO-Nano/dp/B0097AU5OU)
- [Battery Holder (8 AA)](https://www.amazon.com/dp/B07WP1CYYW)
- [Mini ON and Off Rocker Switch 12V 20A T85 2 Pin SPST](https://www.amazon.com/dp/B07S2QJKTX)
- [Momentary Tactile Push Button, 12 x 12 mm x 4.3 mm](https://www.amazon.com/TWTADE-Momentary-Tactile-Button-Switch/dp/B07CG6HVY9)
- [1 Pole 4 Position Rotary Switch (for number of players)](https://www.amazon.com/dp/B07JLRM2L7)
- [1 Pole 3 Position Rotary Switch (for game mode)](https://www.amazon.com/dp/B07JLVV478e)
- [1K Linear Rotary Potentiometer (for intensity)](https://www.amazon.com/dp/B082FCFSHX)
- [LM2596 DC-DC Buck Converter Step Down Module Power Supply](https://www.amazon.com/Zixtec-LM2596-Converter-Module-1-25V-30V/dp/B07VVXF7YX)
- [3 Wire 0.36" DC 0~30V Digital Voltmeter Digital Red LED Display](https://www.amazon.com/dp/B079N98PY4)
- [WS2812B IC RGB Individual Addressable LED Strip Light](https://www.amazon.com/LOAMLIN-WS2812B-Individually-Addressable-Waterproof/dp/B0956C7KFR)
- [Momentary Stainless Steel Push Button On/Off (High Head)](https://www.amazon.com/dp/B08L49F7DV?ref=ppx_yo2ov_dt_b_fed_asin_title) (Quantity 5)
- [Aluminum Flat Plate Bar Stock 3 x 10 x 305mm T6511](https://www.amazon.com/dp/B0CZ4F77TL)
- [22 Gauge 4 Conductor Electrical Wire](https://www.amazon.com/dp/B0CFJXMDT3?ref=ppx_yo2ov_dt_b_fed_asin_title)
- [2N2222 NPN Tranistor](https://en.wikipedia.org/wiki/2N2222) (Quantity 5)
- [Toy Shock Pen](https://www.amazon.com/dp/B0DZWW5TX7?ref=ppx_yo2ov_dt_b_fed_asin_title) (Quantity 5)
- [220 Ohm Resistor][https://www.amazon.com/dp/B07QK9ZBVZ] (Quantity 5)
- Wire, solder, PLA filament, zip ties, screws, etc.

# Electrical Circuit
<img width="1196" height="696" alt="image" src="https://github.com/user-attachments/assets/f79f268d-433b-4f72-aa36-3e7288a7fcd2" />

 # CAD Designs
<img width="293" height="489" alt="image" src="https://github.com/user-attachments/assets/d421cab5-d959-4607-b82d-009cb31ec9aa" />
<img width="309" height="491" alt="image" src="https://github.com/user-attachments/assets/1fe33fa7-5465-4f63-8d3d-09754b71baa6" />
<img width="735" height="522" alt="image" src="https://github.com/user-attachments/assets/a4f3be4d-176a-4311-a6a9-f49854159f1b" />
<img width="703" height="622" alt="image" src="https://github.com/user-attachments/assets/f135685f-7c70-4428-9296-1cac9cc16f33" />
<img width="754" height="504" alt="image" src="https://github.com/user-attachments/assets/4eb01ff9-87ce-4d5b-b19e-353c741b00dd" />
<img width="681" height="447" alt="image" src="https://github.com/user-attachments/assets/d7d9d3c0-115e-4b3b-942c-d51dd36f27ee" />
<img width="748" height="640" alt="image" src="https://github.com/user-attachments/assets/286a0173-3df8-40f9-8351-cb6ba78d1af7" />

# Build
## 3D print all the parts
- Base
- Base Top
- Battery Cover
- Start Button
- Start Button Mount
- Controller Handle (x5)
- Controller Cover (x5)

## Shock Circuits
- To build the shock circuit, disassemble a toy shock pin by unscrewing the tip and breaking the middle black portion from the silver top portion.
- Using pliers for grip, pull the black tube out of the top.
- There will be a blue capacitor, a very small circuit board with a white osscilator dot in the middle, three small batteries, and a spring.
- Remove the blue capacitor by unbending the lower terminal wire.
- Solder two terminals of the capcitor to the pads on the circuit board they were touching when in the pen.
- Solder a wire to the other terminal of the capcitor.
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
- Cut the 4 conductor wire into 5 30" lengths.
- Assemble all the parts (including 3mm screws and washers)

<img width="796" height="743" alt="image" src="https://github.com/user-attachments/assets/28510b57-99c4-4491-a770-e9337ebd69e2" />

- Cut the wires from the buttons and remove any reaminign heat shrink.
- Strip 90mm of the outside cover from one end of each wire and strip and tin the ends of each conductor.  
- For each controller, insert the wires from the bottom.  
-- Solder two wires (red and white) to the two button terminals.
-- Using and m3 screw and washer, secure the other two wires to two electrode bars.
-- Secure a zip-tie around the 4 conductor wire where it exits the hole in the bottom of the controller.
-- Place the bars into the slots on the cover and slide the cover under the top of the controller.
-- Secure the cover in place with one 3 screw.

## LEDs
- Cut the WS2812 LED strip into 4 lengths of 4 LEDs.  Be sure to cut at designated cut lines on the strip.
- Solder 3 wires (5v, data, ground) between each strip.  Be certain to keep the arrows pointing in the same direction.
- Using a hot glue gun, glue the strips into the top of the game so that the LEDs align with the holes.
- Cover each strip with tape.

<img width="1040" height="841" alt="image" src="https://github.com/user-attachments/assets/77cb0e32-aae0-41e4-907b-70030ffdeb6f" />

## Start Button
- CA glue the 12mm button switch to the start button mount panel with the pins sticking through the holes.
- Solder wires to two pins on one side of the switch.
- Place the start button into the top, cover, and attach the switch cover to the top with 5 screws.

## Knobs and Switches
- Solder wires to the terminals of the 3 and 4 position switches.
- Insert each rotary switch into the top, secure with the provided washer and nut, and attach a knob.
- For the intesity control, the on-board potentiometer on the LM2596 DC-DC Buck Converter needs to be replaced with the 1k rotary potentiometer.
- Using a soldering iron, remove the blue pot.
- Solder wires to the 3 terminals of the 1k rotary pot and solder the other ends to the LM2596.
- Insert the rotary pot into the top, secure with the provided washer and nut, and attach a knob.
- Insert the digital voltage display into the top and secure with 2 m2 screws.
- Insert the power toggle switch into the side of the top unit.

<img width="478" height="741" alt="image" src="https://github.com/user-attachments/assets/eecc35b5-58fc-494f-8261-a186e5036b13" />

## Wiring
- Thread each controller wire into the holes on the side of the top unit.
- Connect the switches, controllers, etc. according to the diagram above.

## Programming
- Download the code in the code folder to the Arduino





