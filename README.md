# Tipuino
Official firmware for the Tipuino

The latest version runs on the MKS Gen-L V2.1 (https://github.com/makerbase-mks/MKS-GEN_L), which is desgined for open-sopurce 3D-printers. It integrates the motor set-up and some sensors well, while having some things that are uncnecessray for the project and some that could be better, like more JST connectors for sensors. Below a schematic and the relevant pins for use with the different sensors etc. Technically, any 3D printer mainborad that can run 4 stepper motors would work here, but if anyone was to shwitch to a new board, they would have to be aware of the changes in pin assignments, orientation of connectors etc.

<img width="1143" height="691" alt="image" src="https://github.com/user-attachments/assets/a3847047-efb2-471a-a391-40039cc608f7" />

Sensor connections via JST XH connectors with female Dupont on the sensor side. Connections have been repurposed from the axis endstop sensors. 
Additional sensors connected via Dupont (female) on the AUX port on the lower right. Here, also power of the passive side of on of the break beams can be connected, the second one has to use pins of AUX-1.
EXP1 and EXP2 connect to the latest addition, a screen module containing an LCD, 3 LEDs, a rotary encoder, a reset button (resets the entire maching), a beeper (currently not implemented and an SD card reader (currently not used, possibly an option for storing variable in case of power loos or similar). Communication works via SPI and is thus be independent of the motors (see below).
Motor connectors use the ports on the top. Labelling of the motor coils in unfortunately wrong, at least for the current motors. Highly recommended motor drivers: TMC2209 V2.0 (set-up of important parameters is handled via UART using the TMCstepper library (https://github.com/teemuatlut/TMCStepper). If different drivers are used, they must be compatible with this set-up.
Motor driver serial communication via UART. Apparently, the UART pins of the driver sockets are hard-wired to the respective digital GPIO pins given in the schematic. This makes software serial unavoidable. The serial comm code does work, at least for applying settings, but no hardware UART set-ups worked at all so far.

 For the screen module there are, similar to the mainboard, many commercial options. The one used right now is the bigtreetech mini12864 (https://github.com/bigtreetech/MINI-12864). As with the other components, a switch would be possible, but pin assignments and connector orientation must be accounted for. For this particular screen module, there is a nice description of the wiring and how to set up the different parts of the device in code when NOT using 3D printer firmware such as Marlin: https://www.bjonnh.net/article/20221126_mini_12864/
 
