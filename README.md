# stm32-ad9850
Simple library to drive an AD9850 DDS with an STM32F1 microcontroller.
Code should be easy to port to other libopencm3-compatible processors, as well as for other DDSes such as the AD9851.

## Running the example

    git clone https://git.drk.sc/Derecho/stm32-ad9850.git
    cd stm32-ad9850
    git submodule update --init
    cd libopencm3 && make && cd ..
    cd ad9850 && make
   
Having compiled the example you can now flash `example.elf` onto your microcontroller.
