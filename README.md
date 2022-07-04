# Electronics-RC-Car

RC car project for the class Electronics, and now for Piccolo Pete car racing.

Includes Arduino and PC code. The PC code is designed to run on Arch Linux, so it may have to be modified for your system.

## Arduino setup

You will need an Arduino Mega 2560, an HC-05 Bluetooth module, and an RC servo motor. I will not be explaining how to set that all up right now, so you will have to design the car yourself and look through the code to figure out the pinout.

## Arch Linux setup

Make sure you have a joystick device at "/dev/js0". See "controller.c" for an example. If you wish to change the device name you will have to edit the source and recompile.

### Compile

```bash
make
```

### HC-05 setup

```bash
bluetoothctl power on
```

I believe to bind a MAC address you need to pair it, so if the below command doesn't work then try that.

```bash
rfcomm bind rfcomm0 <MAC address>
```

### Run

```bash
./controller
```
