# byggernGroup8

# note to self
when finishing project, merge branch to main and also include the github link in the code delivery

# how to install
install toolchain from home (cd)

> sudo apt-get install gcc-avr avr-libc avrdude \
> sudo apt install gcc-arm-none-eabi \
> sudo apt install libusb-dev libusb-1.0-0-dev libusb-1.0-0 \
> sudo apt install libhidapi-dev libftdi-dev libftdi1-dev \
> git clone https://git.code.sf.net/p/openocd/code openocd \
> cd openocd \
> ./bootstrap \
> mkdir build \
> cd build \
> ../configure --enable-cmsis-dap --enable-openjtag --prefix=/opt/openocd \
>  make \
>  sudo make install

  sudo apt install libtool pkg-config jimsh libjim-dev

# how to set up git
> ssh-keygen -t rsa \
Press enter on all \
> cat ~/.ssh/id_rsa.pub  \
Add public key to github account under SSH and GPG keys
> git config --global user.email "you@example.com" \

> git config --global user.name "Your Name"


# how to git
Load project to new computer: git clone git@github.com:renhkl/byggernGroup8.git \

Update project: git pull \


To solve merge conflicts \
add filepath \
rm filepath \
manually keep/delete changes in files \

Save changes
git add ../Node1 ../Node2 \
git commit -m "descriptive commit message" \
git push \


# how to run
> (sudo) make && (sudo) make flash \
make sure to be in the right folder as there are one makefile for each of the nodes


# how to find the RS232 device
1. dmesg | grep tty (might need sudo priviliges)


2. ATmega162
picocom -b 9600 -r -l /dev/ttyS0 \
(picocom -b <baudrate> -y <parity> d <databits> -p <stopbits> <serial port>)

- ATSAM
picocom -b 9600 -r -l /dev/ttyACM1

# terminal commands
*** Picocom commands (all prefixed by [C-a]) \

*** [C-x] : Exit picocom \
*** [C-q] : Exit without reseting serial port \
*** [C-b] : Set baudrate \
*** [C-u] : Increase baudrate (baud-up) \
*** [C-d] : Decrease baudrate (baud-down) \
*** [C-i] : Change number of databits \
*** [C-j] : Change number of stopbits \
*** [C-f] : Change flow-control mode \
*** [C-y] : Change parity mode \
*** [C-p] : Pulse DTR \
*** [C-t] : Toggle DTR \
*** [C-g] : Toggle RTS \
*** [C-|] : Send break \
*** [C-c] : Toggle local echo !!!!! \
*** [C-w] : Write hex \
*** [C-s] : Send file \
*** [C-r] : Receive file \
*** [C-v] : Show port settings \
*** [C-h] : Show this message \




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


