alias b := build
alias c := clean
# alias f := flash-by-usb
# alias r := run-by-usb
alias f := flash-by-serial
alias r := run-by-serial
alias cfg := menuconfig
alias m := monitor
alias fmt := format

build:
    idf.py build size

clean:
    rm -rf build
    
flash-by-serial:build
    idf.py flash -p /dev/ttyUSB0 -b 2000000  
    idf.py monitor -p /dev/ttyUSB0 

flash-by-usb:build
    idf.py flash -p /dev/ttyACM0 -b 2000000 
    idf.py monitor -p /dev/ttyACM0 

monitor:
    idf.py monitor 

run-by-usb:build flash-by-usb 

run-by-serial:build flash-by-serial 
    
menuconfig:
    idf.py menuconfig 

format:
    find components/ \( -name '*.c' -o -name '*.h' \) | xargs clang-format -i
    find main/ \( -name '*.c' -o -name '*.h' \) -not -path 'main/ui/*' | xargs clang-format -i
