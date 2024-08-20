:: Flash the target MCU via OpenOCD,
:: then reset halt to prepare for gdb connection
:: waits for gdb connection on localhost port 3333; see connect-gdb.txt for more info
:: leave this process open if you're connecting gdb
openocd -s $MAXIM_PATH/Tools/OpenOCD/scripts -f interface/cmsis-dap.cfg -f target/max32690.cfg -c "program build-APARD/firmware.elf verify; init; reset halt"
