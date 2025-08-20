# Tipuino
Official firmware for the Tipuino

The latest version runs on the MKS Gen-L V2.1, which is desgined for open-sopurce 3D-printers. It integrates the motor set-up and some sensors well, while having some things that are uncnecessray for the project and some that could be better, like more JST connectors for sensors. Below a schematic and the relevant pins for use with the different sensors etc.

<img width="1143" height="691" alt="image" src="https://github.com/user-attachments/assets/a3847047-efb2-471a-a391-40039cc608f7" />

Sensor connections via JST XH connectors with female Dupont on the sensor side. Connections have been repurposed from the axis endstop sensors. 
Additional sensors connected via Dupont (female) on the AUX port on the lower right. Here, also power of the passive side of on of the break beams can be connected, the second one has to use pins of AUX-1.
EXP1 and EXP2 will connect a screen/control unit. Communication should work via SPI and should thus be independent of the motors (see below).
Motor connectors use the ports on the top. Labelling of the motor coils in unfortunately wrong, at least for the current motors. Highly recommended motor drivers: TMC2209 V2.0
Motor driver serial communication via UART. Apparently, the UART pins of the driver sockets are hard-wired to the respective digital GPIO pins given in the schematic. This would make software serial unavoidable. The serial comm code does work when using one axis and no hardware UART set-ups worked at all so far.
