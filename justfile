alias b := build
alias rb := rebuild
alias c := clean
alias f := flash
alias ff := flash-full
alias r := run
alias cfg := menuconfig
alias m := monitor
alias fmt := format

build:
    idf.py build size

rebuild:
    idf.py reconfigure build size

clean:
    idf.py fullclean
    
flash:build
    idf.py app-flash -b 2000000  

flash-full:rebuild
    idf.py flash -b 2000000  

monitor:
    idf.py monitor 

run:build flash monitor

menuconfig:
    idf.py menuconfig 

format:
    find components/ -path 'components/middlewares/ek_utils' -prune -o \( -name '*.c' -o -name '*.h' \) -print0 | xargs -0 clang-format -i
    find main/ \( -name '*.c' -o -name '*.h' \) -not -path 'main/ui/*' | xargs clang-format -i
