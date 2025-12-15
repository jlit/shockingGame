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
- [1 Pole 4 Position Rotary Switch (for number of players)](https://www.amazon.com/dp/B07JLRM2L7)
- [1 Pole 3 Position Rotary Switch (for game mode)](https://www.amazon.com/dp/B07JLVV478e)
- [1K Linear Rotary Potentiometer (for intensity)](https://www.amazon.com/dp/B082FCFSHX)
- [LM2596 DC-DC Buck Converter Step Down Module Power Supply](https://www.amazon.com/Zixtec-LM2596-Converter-Module-1-25V-30V/dp/B07VVXF7YX)
- [WS2812B IC RGB Individual Addressable LED Strip Light](https://www.amazon.com/LOAMLIN-WS2812B-Individually-Addressable-Waterproof/dp/B0956C7KFR)
- [Momentary Stainless Steel Push Button On/Off (High Head)](https://www.amazon.com/dp/B08L49F7DV?ref=ppx_yo2ov_dt_b_fed_asin_title) (Quantity 5)
- [Aluminum Flat Plate Bar Stock 3 x 10 x 305mm T6511](https://www.amazon.com/dp/B0CZ4F77TL)
- [22 Gauge 4 Conductor Electrical Wire](https://www.amazon.com/dp/B0CFJXMDT3?ref=ppx_yo2ov_dt_b_fed_asin_title)
- [2N2222 NPN Tranistor](https://en.wikipedia.org/wiki/2N2222) (Quantity 5)
- [Toy Shock Pen](https://www.amazon.com/dp/B0DZWW5TX7?ref=ppx_yo2ov_dt_b_fed_asin_title) (Quantity 5)
- [220 Ohm Resistor][https://www.amazon.com/dp/B07QK9ZBVZ] (Quantity 5)
- Wire, solder, PLA filament, zip ties, etc.