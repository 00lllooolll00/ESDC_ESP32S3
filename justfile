alias b := build
alias c := clean
alias fu := flash-by-usb
alias fs := flash-by-serial
alias ru := run-by-usb
alias rs := run-by-serial
alias cfg := menuconfig
alias m := monitor

build:
    idf.py build size

clean:
    idf.py clean

flash-by-serial:
    idf.py flash -p /dev/ttyUSB0 

flash-by-usb:
    idf.py flash -p /dev/ttyACM0 

monitor:
    idf.py monitor

run-by-usb:build flash-by-usb 

run-by-serial:build flash-by-serial 
    
menuconfig:
    idf.py menuconfig --style monochrome
