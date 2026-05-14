alias b := build
alias c := clean
alias f := flash
alias ff := flash-full
alias r := run
alias cfg := menuconfig
alias m := monitor
alias fmt := format

build:
    idf.py build size

clean:
    rm -rf build
    
flash:build
    idf.py app-flash -b 2000000  

flash-full:build
    idf.py flash -b 2000000  

monitor:
    idf.py monitor 

run:build flash monitor

menuconfig:
    idf.py menuconfig 

format:
    find components/ \( -name '*.c' -o -name '*.h' \) | xargs clang-format -i
    find main/ \( -name '*.c' -o -name '*.h' \) -not -path 'main/ui/*' | xargs clang-format -i
