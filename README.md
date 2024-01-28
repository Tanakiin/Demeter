# Current build only works on MacOS

## Use the following steps to run on Windows

- Open Arduino IDE and plug in the Arduino (selecting the right port and board)
- In the bottom right, copy down the serial bus (usually COM# in windows)
- Replace "/dev/tty.usbserial-0001" in line 7 of `arduino.py` with the same "COM#" you found.
