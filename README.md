# byggernGroup8


# how to run
> gcc main.c uart.c
> make
> make flash (ikke ./a.out)


# to find the RS232 device
1. dmesg | grep tty (might need sudo priviliges)


/dev/ttyACM0
2. picocom -b 9600 -r -l /dev/ttyS0
(picocom -b <baudrate> -y <parity> d <databits> -p <stopbits> <serial port>)

*** Picocom commands (all prefixed by [C-a])

*** [C-x] : Exit picocom
*** [C-q] : Exit without reseting serial port
*** [C-b] : Set baudrate
*** [C-u] : Increase baudrate (baud-up)
*** [C-d] : Decrease baudrate (baud-down)
*** [C-i] : Change number of databits
*** [C-j] : Change number of stopbits
*** [C-f] : Change flow-control mode
*** [C-y] : Change parity mode
*** [C-p] : Pulse DTR
*** [C-t] : Toggle DTR
*** [C-g] : Toggle RTS
*** [C-|] : Send break
*** [C-c] : Toggle local echo !!!!!
*** [C-w] : Write hex
*** [C-s] : Send file
*** [C-r] : Receive file
*** [C-v] : Show port settings
*** [C-h] : Show this message




# Topp 10 kuleste signaler
1. GND (often overlooked, but critical just like a base guitar)
2.
3.
4.
5.
6.
7.
8.
9. 
10.

# Fargekoding av ledninger


