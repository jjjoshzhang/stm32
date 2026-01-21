// stm32 F103C8T6

    // important/special pins
    // VDD: supply volatage
    // VSS: ground
    // NRST(Active low): reset
    // VBAT: backup power supply for RTC and backup everthing when vdd is dead
    // BOOt0: boot mode selection pin


    // normal pins(functionality are similar)
    // GPIO
    // GPIO A PA0-PA15
    // GPIO B PB0-PB15
    // GPIO C PC13-PC15
    // GPIO D PD0-PD1

    // - Iputs

    // Input pull-uo
    // Input pull-down
    // Input floating
    // Analog input

    // - Outputs
    // Output push-pull
    // Output open-drain
    // Alternate function push-pull
    // Alternate function open-drain

    // API
    // a created function inside a library to do a specific task
    // GPIO_Init() initialize hot a GPIO pin behaves
    // GPIO_WriteBit() sets an output pin to 1 or 0
    // GPIO_ReadOutputDataBit() What value did I(MCU) set on this output pin
    // (does not nesscary means what the pin itself currently is)


    // USART
    // Transmit and receive data