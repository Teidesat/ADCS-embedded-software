# IMU attitude calculation

Research of a possible algorithm for the orientation of the `TEIDESAT-1` satellite.


## Hardware

### Inertial Measurement Unit (IMU)

The IMU `Adafruit LSM9DS1 9-DOF` is used. It containing multiple sensors for taking different measurements:  
- Acceleration (m/s^2): 3 accelerometers (XYZ axis)
- Degrees per second (rad/s): 3 gyroscopes (XYZ axis)
- Magnetic flux (gauss): 3 magnetometers (XYZ axis)
- Temperature : temperature sensor

datasheet: [https://www.st.com/resource/en/datasheet/lsm9ds1.pdf]()  

### Circuit configuration

Using an `Arduino ESP-12E` as the computer board. For the comunication between the Arduino and the IMU, the `SPI` protocol is used, but both components should allow for the use of the `I2C` protocol as well.

![circuit image](./img/circuit_image.jpg)

Pin Conections (SPI protocol):

|  Arduino  |   IMU   | 
|:---------:|:-------:|
|    3V     |   VIN   |
|    G      |   GND   |
|    D1     |   SCL   |
|    D2     |   SDA   |



### Software configuration

Used the `platform.io` plugin for VSCode to create the project and upload the attitude calculation program into the arduino board. Libraries requerided are already integrated in the project in the `.pio/libdeps/psp12e` directory:
- Adafruit BusIO
- Adafruit LIS3MDL
- Adafruit Unified Sensor
- Adafruit LSM9DS1 Library
- ReefwingAHRS

All the implementations are located in the `src/` directory. 

```
src/
├── adafruit/
|    └── ...
├── sebastian/
|    └── ...
├── upsat/
|    └── ...
├── main-adafruit-fork.txt
├── main-deltatime.txt
├── main-reefwing.txt
├── main-tris.txt
└── main-upsat.txt
```

To upload the one of the implementations into the arduino board, change the name of the sketches in the `src/` directory from `main-IMPLEMENTATION-NAME.txt` to `main.cpp`. Every other implementation should end in `.txt` as only one main file should exist.

## Attitude Algorithm implementations

### Upsat

UPSat ADCS software modified to be used with arduino instead of Raspberry

ADCS software folder in their repository:  
[https://gitlab.com/librespacefoundation/upsat/upsat-adcs-software/-/tree/master/sensor-fusion-test?ref_type=heads]()

### reefwing

Library implementing different algorithm. Using the `Madgwick algorithm` as it is better than the `Mahony algorithm`. 
[https://github.com/Reefwing-Software/Reefwing-AHRS]()

### adafruit-fork

Adafruit sensor fusion algorithm tutorial referencing fork library:  
[https://learn.adafruit.com/ahrs-for-adafruits-9-dof-10-dof-breakout/sensor-fusion-algorithms]()

Base repository:  
[https://github.com/adafruit/Adafruit_AHRS/tree/master]()

Fork repository:  
[https://github.com/PaulStoffregen/MadgwickAHRS]() (madgwick algorithm)  
[https://github.com/PaulStoffregen/MahonyAHRS]() (mahony algorithm)

### tris

Implementation based but not following exactly all the steps on the tutorial. Test the output of every sensor and simple sensor fusion.

tutorial serie for sensor fusion using trigonometry:  
[https://www.youtube.com/watch?v=2AO_Gmh5K3Q&list=PLGs0VKk2DiYwEo-k0mjIkWXlkrJWAU4L9&index=1&ab_channel=PaulMcWhorter]()