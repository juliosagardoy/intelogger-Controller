#intelogger-Controller

## Introduction
Engine monitoring tool for Honda's OBD2a. Can be used on any Honda OBD2a ECU with the appropiate adapter harness, as long as the pinout <b>is as per documentation</b>. <u>Do not use without double-checking compatibility; This would lead to catastrophic results and you could loose the engine, the car and your life.</u>

System has been developed on a JDM '96 Honda Integra DC2 with OEM ECU.
It does not "inject" anything into the ECU nor the engine. It just **reads** discrete values, analog signals and PWM signals to extract engine status data.

uC is a 16-bit Microchip PIC16F1938 running at internal 4 MHz.

## PC communications
Communication with computer is done through a serial port RS232.

### Serialport config
9600 8n1

### Initial comm
In order to initiate the communications, some talk is needed from PC (registration).

| PC (byte) ->  | <- uC (byte)        | Comment  
| :-------- | :--------    | :--------
| 0x01      |              | Receive start byte
|           | 0x05         | Send ACK
|           | DISCRETE_QTY | Send how many discretes will be processed
|           | ANALOG_QTY   | Send how many analog sigs will be processed
|           | CCP_QTY      | Send how many PWM sigs will be processed
|           | ...          | uC will start processing and sending
| 0x01      | ...          | PC tells the uC it's alive. Every 5s max.
|           | ...          | uC send stuff forever as long as we are there

This is the initial talk between PC and uC. Note that uC will flag the communication as off (PC_COMM = 0) once it's started if PC doesn't send a 0x01 every 5s as much. When this happens, it stops writting stuff to the serial port until it gets a new registration attempt.

### Protocol
Folowing datagrams are sent once PC_COMM is 1 and initial communication is established.

| starting byte     | datatype     | datasize | data
| :------------- | :------------- | :------------- | :-------------
| byte           | byte       | byte | vector[datasize]

*starting byte* is always 0x01

*datatype* specifies what kind of data is held under *data*. It can be

| Value (byte)     | Description     |
| :------------- | :------------- |
| 0x30       | Discrete signals
| 0x31      | Analog signals
| 0x32      | PWM signals
| 0x33      | self status

*datasize* is a byte specifying the size in bytes of the datagram in *data*. Minimum is 1, maximum is 255.

*data* will contain a vector of bytes of size *datasize* and containing *datatype* info.

#### Discrete signals
Discrete signals are all packed in a single byte using bit-shifting. This means, for discrete, datasize will equal 1 though.
Only the first 4 bits are used.

| 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1
| : | : | : | : | : | : | : | :
| - | - | - | - | VTS | PCS | ALT | FAN

#### Analog signals

#### CCP signals

#### Self test status

## IDEAS FOR NEXT REVISIONS ##
0. LCD via SPI
1. Pass update rate from PC to uc. uc will check against its minimum refresh time
