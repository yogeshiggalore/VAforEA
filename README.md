# VAforEA
VAforEA - Voice Assistant for Engineering Automation

# Before anything Let me give all credit to the following developers for their extraordinary work.
https://www.electroschematics.com/dc-module/

https://www.instructables.com/DIgital-Controlled-Bench-Power-Supply/

https://docs.ai-thinker.com/en/voice_module

https://www.espressif.com/

https://randomnerdtutorials.com/getting-started-with-esp32/

https://how2electronics.com/how-to-use-ads1115-16-bit-adc-module-with-arduino/

https://circuitdigest.com/microcontroller-projects/arduino-dac-tutorial-interfacing-mcp4725-dac

# Idea/Concept
Setting voltage and current of XL4015 power supply module using VC-02 voice control module. Here voice control module will convert the voice input to respective commands to the ESP32 module. Based on the command received ESP32 will control the DAC module to set the required current and voltage. ADS1115 module used for reading set voltage and current.

# Logic behind setting voltage and current

The XL4015 module has 2 multi-turn potentiometers to adjust the output voltage and the current limit.
The voltage adjustment normally works by varying the 10K potentiometer which taps down the output voltage across a 270R resistor to provide a feedback voltage into the converter. This is compared against an internal 1.25V reference and to vary the duty cycle of the converter. So for example, if the potentiometer was set to be 540 ohm then the output voltage would settle to 3.75 volts. To provide digital control the potentiometer is set to a higher value and then current is fed into the 270R resistor from the DAC output via a 390R resistor. As the DAC output increases then less current is needed from the output via the potentiometer to give the 1.25V across the 270R and so the output voltage drops. At one extreme the DAC output can provide all the current needed to get the 1.25V across the 270R resistor and so the output voltage needed is 1.25V.

The current limit on the XL4015 module works by monitoring the current drawn across a 50 milliOhm shunt. This is then compared with the limit current setting and an op-amp then reduces the buck converter to maintain that current. The voltage needed to compare is normally controlled by the limit current potentiometer via some additional resistor division as the voltage range needed is quite small (0->300mV). To provide digital control the potentiometer is set to its maximum and then an override voltage is fed in from a DAC. This is tapped down by the 470R /47R divider to give maximum precision and to keep the impedance of the override as low as possible.

The measurement side is handled by a 4-channel 16-bit ADC (ADS1115). This actually used as a 2-channel differential ADC to provide better accuracy. One pair is used to monitor the output voltage via a 10K/1K tap down. This together with a 2.048V input range on the ADC gives the range required and good precision. The other pair is used to measure the voltage across the XL4015 50milliohm current shunt and so measure the current drawn. As this is quite a small voltage the ADC has a programmable gain which is used to give greater precision.

# Components used
1. XL4015 buck converter with adjustable voltage and current limit
2. ESP32 wifi/ble based microcontroller module
3. VC-02 voice control module
4. ADS1115 4-channel 16-bit I2C ADC
5. SD1306 0.96" OLED I2C display module
6. 2 MCP4725 12-bit I2C DAC
7. 2 Rotary encoders EC11
8. Relay module
9. Resistors 47R,390R,470R,1K,10K,2K2,11K2,100K

# High-level block diagram
![block_vaforEA](https://github.com/yogeshiggalore/VAforEA/assets/5477695/6bb0b269-3f8d-463f-b33e-6667310cbfb5)

# Demo vedio link
https://www.youtube.com/watch?v=pA92h-ue5Ts 

# python screenshot sample
![VAforEA-SS-05-07-2024_08-43-55](https://github.com/yogeshiggalore/VAforEA/assets/5477695/8fc7fbf4-d295-455e-9143-76f7af975b68)

# About XL4015 power supply module.
The XL4015 power supply module has an onboard constant voltage (CV) and constant current (CC) control feature which is useful for general-purpose power supply and battery charger applications.

![2-XL4015-CC-CV-Buck-Module-Specs](https://github.com/yogeshiggalore/VAforEA/assets/5477695/f2140c1c-3bf8-40e0-83e8-ec84e17bfc34)

apart from the input and output screw terminal connectors, there are two multiturn trim pots to adjust the output voltage and output current. Further, there are three LED indicators – the first one near the input connector is the constant-current (cc) indicator. At the same time, next two LEDs are intended mainly for battery charging applications (battery charging and battery full indicators). However, when used as a general-purpose power supply, the battery charging indicator LED will work as a ‘load-on’ annunciator.

![4-XL4015-CC-CV-Buck-Module-Annotated](https://github.com/yogeshiggalore/VAforEA/assets/5477695/f73214a1-71de-4d92-933c-1f4c08663051)

Slowly turning the voltage adjustment (V-ADJ) trim pot clockwise will raise the output voltage gradually, and a counterclockwise rotation will lower it. Likewise, clockwise turning of the current adjustment (I-ADJ) trim pot increases the current limit while counterclockwise action decreases the current limit. It’s advised to adjust the voltage level at first, and then the current level (this fine-tuning procedure will be discussed in detail later).

Needless to say, at the heart of the module is an XL4015 chip from XLSEMI (www.xlsemi.com) which is a 180 KHz fixed frequency PWM step-down (buck) DC/DC converter, capable of driving a 5A load with high efficiency, low ripple and excellent line and load regulation. According to its datasheet (Rev 1.5), the PWM control circuit can adjust the duty ratio linearly from 0 to 100%. An over-current protection function is built inside so that when a short circuit occurs, the operation frequency will be reduced to 48KHz. Below you can see the function block diagram of XL4015 (XL4015E1).

![5-XL4015-Function-Block-Diagram](https://github.com/yogeshiggalore/VAforEA/assets/5477695/01e852fc-c6f8-4643-8e2e-607a4b2d33fe)

The core of the design merely follows the typical application example (see below). An exception is that the recommended 1uF bypass capacitor (C1) is not used in this module, provision is already provided in the PCB, though.

![6-XL4015-Typical-Application-Circuit](https://github.com/yogeshiggalore/VAforEA/assets/5477695/0df8f4b7-f1df-44fd-9e3c-f9c5f85522e5)

The rest of the electronics include one 78L05 fixed voltage regulator, one TL431 precision shunt regulator, and an LM358 operational amplifier. I haven’t seen the 78L05 chip in my module at first as it’s situated underneath the flat toroid-core inductor.
![7-XL4015-CC-CV-Buck-Module-Key-Parts](https://github.com/yogeshiggalore/VAforEA/assets/5477695/33eaec34-7baa-435d-bd34-6125eaca4a5e)
As you might guess correctly, the three ‘surplus’ chips are included in this revised XL4015 module for the current control function. If you turn over the module, then you find a 0.05Ω (R050) current sensing resistor at the bottom of the PCB.
![8-XL4015-Module-Isense-Resistor](https://github.com/yogeshiggalore/VAforEA/assets/5477695/ee848f8f-aaf8-4d9a-80ce-a012098904c9)

Next is the adopted schematic of the XL4015 CV/CC module. This is a very basic schematic as there is a myriad of befuddling replications and revisions – No surprise, simply a common Chinese business practice!
![9-XL4015-CC-CV-Buck-Module-CKT](https://github.com/yogeshiggalore/VAforEA/assets/5477695/766d00fe-9105-4d6e-85cc-6be0c48c5622)

The overall design of the module is indeed a pretty neat and clever piece of work. Frankly, I was quite impressed by the unperplexed design idea of the LM358-based current control segment as this edition also includes a TL431 shunt regulator. In a previous version, I found that there was no TL431 chip onboard but the same regulated 5VDC (outputted by one LM317 chip) is used for the reference voltage as well (see next schematic snip). In principle, TL431/TS431 provides a reference that is more stable than the 78L05!

![10-Previous-Version-CKT-Snip](https://github.com/yogeshiggalore/VAforEA/assets/5477695/4768451c-f199-4661-a2df-1f82dd31b381)

# About VC-02 module

Overview
VC series module is a low-cost pure offline speech recognition module developed by Shenzhen Ai-Thinker Technology Co., Ltd. The module uses the voice chip US516P6 launched by Unisound. It features high reliability and versatility. Highly reliable wake-up recognition rates, more distant wake-up, more low-incorrect woken's, more low-erroneous wake-up rates, more low-than-off, faster response identification time, and secreted pure line identification.

The VC Series module adopts a 32-bit RISC architectural kernel and has joined the DSP instruction set required for signal processing and speech recognition, support floating point operations, and FFT accelerators. Supports up to 150 local instructions offline identification, supports RTOS lightweight systems, as well as simple and friendly customary tools.

The VC Series module has a rich peripheral interface, including UART / I2C / PWM / SPI, and simple and friendly secondary development tools for customers to implement a single-mode group speech control application scenario scheme.

![images](https://github.com/yogeshiggalore/VAforEA/assets/5477695/d211a1bc-a526-484f-a09b-3d18eb001c34)

Features
* Support bilingual control, both Chinese and English
* Single MIC Access
* Support AEC echo elimination, steady-state noise reduction
* Support to wake up from learning, no need to compile firmware
* Comprehensive recognition rate can reach more than 98%
* Identification time is less than 100ms
* Extremely low error rate
* Entry corpus up to 150

Kernel Introduction
* Integrated 32bit RISC kernel, frequency up to 240MHz
* Support for DSP instruction sets and FPU floating point arithmetic units
* FFT Accelerator: Maximum Support 1024 points FFT / IFFT operation, or 2048 points FFT / IFFT operation
* Unisound Equation Customized Logo Algorithm
* Built-in 242KB high-speed SRAM
* 8kb ROM for boot
* Built-in 2MB SPI Flash
* Support 1 road simulation MIC input, SNR ≥ 94dB
* Support for dual-channel DAC output
* Built-in 5V to 3.3V, 3.3V 1.2V LDO is power supply for chip
* Provide complete RTOS-based SDK

# About VC-02 kit
1. Product Overview
The VC-02-Kit is a development board designed for the VC-02 module, and it shares the
same board as the VC-01-Kit. The development board integrates the CH340C serial port to the USB chip, providing a basic debugging
interface and USB upgrade interface, design of wake-up light and cold and cooling lamp, provides status indication and control instructions, and easy to debug the simple control
function. VC-02 is a low-cost pure offline speech recognition module developed by Shenzhen
Ai-Thinker Technology Co., Ltd. The module uses the voice chip US516P6 launched by
Unisound, and continuously optimizes and innovates algorithms in speech recognition
technology. The offline recognition algorithm and chip architecture are deeply integrated to
provide customers with ultra-low-cost offline speech recognition solutions. It can be widely
and quickly applied to smart homes, all kinds of smart small household appliances, 86 boxes, toys, lamps and other products that need voice control. The US516P6 chip uses a 32-bit RSIC architecture core and incorporates a DSP instruction set
specifically for signal processing and speech recognition, an FPU arithmetic unit that supports
floating-point operations, and an FFT accelerator. VC-02 supports offline identification of 150 local instructions, an RTOS lightweight system, rich
peripheral interfaces, and is simple and friendly.

![vc_02_01](https://github.com/yogeshiggalore/VAforEA/assets/5477695/dd420aee-63f1-438a-a79c-4963fed3fd21)

main parameters:

![vc_02_02](https://github.com/yogeshiggalore/VAforEA/assets/5477695/25ba313d-0130-462d-ab10-6b51e61ca87c)

Electrical characteristics.
![vc_02_03](https://github.com/yogeshiggalore/VAforEA/assets/5477695/448ace10-3ccd-4257-abc9-1c460b92edf0)

Appearance
![vc_02_04](https://github.com/yogeshiggalore/VAforEA/assets/5477695/1e011414-f0ce-4963-a173-e99e9020f808)

pinouts

![vc_02_05](https://github.com/yogeshiggalore/VAforEA/assets/5477695/03f896b7-491c-40ec-932a-f45924b745e8)

schematics
![vc_02_06](https://github.com/yogeshiggalore/VAforEA/assets/5477695/046fdbbc-619f-45ca-a413-54fc406192ae)

# configuring VC-02
Open link http://voice.ai-thinker.com/#/

1. create user login
2. click on Create product
3. Select a product category
4. Select offline and module as VC-02
5. Product name VAforEA
6. Select language English

![vc_02_07](https://github.com/yogeshiggalore/VAforEA/assets/5477695/f19652e1-ce7c-4bc1-a40c-ab596ff91e3e)

5. Set a single mic.
6. Distance 1 to 5 meters.
7. AEC echo cancellation closed
8. Study state noise close
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/2e6efb9e-5bf3-458b-b1a3-de2dafa9e2c3)

9. Set GPIO pins as shown below
![vc_02_08](https://github.com/yogeshiggalore/VAforEA/assets/5477695/95842e78-f27d-4971-bfaf-f6d4474a2812)

10. Set wake word and wake up sensitive to high and configure reply wake word.
![vc_02_09](https://github.com/yogeshiggalore/VAforEA/assets/5477695/e4deccc0-44db-40e9-988d-e457cf3b607d)

11. Configure voice command as shown below
![vc_02_10](https://github.com/yogeshiggalore/VAforEA/assets/5477695/5c3bf0a5-ffc7-4836-b671-839832d24370)

![vc_02_11](https://github.com/yogeshiggalore/VAforEA/assets/5477695/cfce57cd-6d8c-439a-b391-8ce94f3231e1)

![vc_02_12](https://github.com/yogeshiggalore/VAforEA/assets/5477695/acbc92e8-6095-40cf-9258-e3e50c787d17)

11. for control configure UART as shown below
![vc_02_13](https://github.com/yogeshiggalore/VAforEA/assets/5477695/8f8f99ad-270d-4f20-9ea2-e477658635c1)

![vc_02_14](https://github.com/yogeshiggalore/VAforEA/assets/5477695/6671e972-2487-4906-a5fd-89ba587d70e2)

![vc_02_15](https://github.com/yogeshiggalore/VAforEA/assets/5477695/60dc6352-2dad-4d62-98c9-08ccdd7919f3)

12. Configure wake-up free as shown below
![vc_02_16](https://github.com/yogeshiggalore/VAforEA/assets/5477695/14461f46-cf24-4e2e-8933-e5fb3aff520f)

13. Configure the speaker as shown below
![vc_02_17](https://github.com/yogeshiggalore/VAforEA/assets/5477695/77ce5e1e-e7f0-47c2-8d44-b2144349f3f7)

14. Do configure as shown below
![vc_02_18](https://github.com/yogeshiggalore/VAforEA/assets/5477695/df67ad43-e3a1-4eec-b21c-f0c1849a9a4d)

15. Build SDK as shown below
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/58c352f0-f112-4a76-9142-b421d006b6b1)

16. once the build is completed, download the firmware.
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/67635f5a-e2c3-4322-8ab6-1da85d823d37)

17. download humming-bird-m-update-tool and open the download tool as shown below
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/05511a1b-c800-4fdd-9b67-e9cb8cce824d)

18. go to your firmware download folder find uni_app_release_update.bin file, select the COM port and click ok
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/9acb6923-31dc-4a80-aa46-8640225af812)

19. press reset button and wait for 100% download
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/5125fc66-214d-4c13-bf07-41cf3eaa44dd)

20. If green color download is completed.
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/582a020c-52e3-48a0-b223-38aef161c73e)

21. UART setup for testing.
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/8d666be5-0d7c-4566-ba2f-dd588f900ce9)

22. UART output
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/31bb257b-ef3e-47bf-9e4f-a0e781e6c315)

# About ESP32
The ESP32 is a series of low-cost and low-power System on a Chip (SoC) microcontrollers developed by Espressif that include Wi-Fi and Bluetooth wireless capabilities and dual-core processor. If you’re familiar with the ESP8266, the ESP32 is its successor, loaded with lots of new features.

Why are they so popular? Mainly because of the following features:

Low-cost: you can get an ESP32 starting at $6, which makes it easily accessible to the general public;
Low-power: the ESP32 consumes very little power compared with other microcontrollers, and it supports low-power mode states like deep sleep to save power;
Wi-Fi capabilities: the ESP32 can easily connect to a Wi-Fi network to connect to the internet (station mode), or create its own Wi-Fi wireless network (access point mode) so other devices can connect to it—this is essential for IoT and Home Automation projects—you can have multiple devices communicating with each other using their Wi-Fi capabilities;
Bluetooth: the ESP32 supports Bluetooth classic and Bluetooth Low Energy (BLE)—which is useful for a wide variety of IoT applications;
Dual-core: most ESP32 are dual-core— they come with 2 Xtensa 32-bit LX6 microprocessors: core 0 and core 1.
Rich peripheral input/output interface—the ESP32 supports a wide variety of input (read data from the outside world) and output (to send commands/signals to the outside world) peripherals like capacitive touch, ADCs, DACs, UART, SPI, I2C, PWM, and much more.
Compatible with the Arduino “programming language”: those that are already familiar with programming the Arduino board, you’ll be happy to know that they can program the ESP32 in the Arduino style.
Compatible with MicroPython: you can program the ESP32 with MicroPython firmware, which is a re-implementation of Python 3 targeted for microcontrollers and embedded systems.
ESP32 Specifications
If you want to get a bit more technical and specific, you can take a look at the following detailed specifications of the ESP32 (source: http://esp32.net/)—for more details, check the datasheet):
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/a0e856e9-e755-4158-82b7-d33a799bc358)

Wireless connectivityWiFi: 150.0 Mbps data rate with HT40
Bluetooth: BLE (Bluetooth Low Energy) and Bluetooth Classic
Processor: Tensilica Xtensa Dual-Core 32-bit LX6 microprocessor, running at 160 or 240 MHz
Memory:
ROM: 448 KB (for booting and core functions)
SRAM: 520 KB (for data and instructions)
RTC fast SRAM: 8 KB (for data storage and main CPU during RTC Boot from the deep-sleep mode)
RTC slow SRAM: 8KB (for co-processor accessing during deep-sleep mode)
eFuse: 1 Kbit (of which 256 bits are used for the system (MAC address and chip configuration) and the remaining 768 bits are reserved for customer applications, including Flash-Encryption and Chip-ID)
Embedded flash: flash connected internally via IO16, IO17, SD_CMD, SD_CLK, SD_DATA_0 and SD_DATA_1 on ESP32-D2WD and ESP32-PICO-D4.
0 MiB (ESP32-D0WDQ6, ESP32-D0WD, and ESP32-S0WD chips)
2 MiB (ESP32-D2WD chip)
4 MiB (ESP32-PICO-D4 SiP module)
Low Power: ensures that you can still use ADC conversions, for example, during deep sleep.
Peripheral Input/Output: 
peripheral interface with DMA that includes capacitive touch
ADCs (Analog-to-Digital Converter)
DACs (Digital-to-Analog Converter)
I²C (Inter-Integrated Circuit)
UART (Universal Asynchronous Receiver/Transmitter)
SPI (Serial Peripheral Interface)
I²S (Integrated Interchip Sound)
RMII (Reduced Media-Independent Interface)
PWM (Pulse-Width Modulation)
Security: hardware accelerators for AES and SSL/TLS

Specifications – ESP32 DEVKIT V1 DOIT
The following table shows a summary of the ESP32 DEVKIT V1 DOIT board features and specifications:
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/1c01f052-88bc-4208-a115-125bef00563b)

This particular ESP32 board comes with 36 pins, 18 on each side. The number of available GPIOs depends on your board model.

To learn more about the ESP32 GPIOs, read our GPIO reference guide: ESP32 Pinout Reference: Which GPIO pins should you use?
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/755bd722-e318-437a-bbfc-0e5d4ec8cf03)

GPIO pinout
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/ab208f7c-5a11-4759-b249-e52bf485f2c3)

For programming and other detailed info please click this link
https://randomnerdtutorials.com/getting-started-with-esp32/

# About ADS1115 analog to digital converter
In this guide, we will learn how to use ADS1115 16-Bit ADC Module with Arduino. An ADS1115 breakout board is a 16-bit analog-to-digital converter (ADC) that we can use with Arduino, ESP8266/32, STM32, or any other Microcontrollers.

Arduino boards come with a built-in 10-bit ADC that has a somewhat limited capability. The AD1115 has a 16-bit ADC resolution. This means the smallest voltage that Arduino can measure is 5V / 1024 = 0.0049V (or 4.9mV). Whereas the smallest voltage that ADS1115 can measure is 5V / 65536 = 0.000076V (76uV). Thus compared to Arduino ADC, the ADS1115 can measure a wide range of voltage. If you want more precison up to 0.3uV, you may check ADS1220 24-Bit ADC Module.

Features of ADS1115
16-bit Resolution
Four (4) Channel Single-Ended or Two (2) Channel Differential Inputs
I2C Protocol Interface
Programmable Comparator
Wide Supply Range
Low Current Consumption
Continuous-Conversion Mode
Programmable Data Rate
Programmable Comparator
Single-Cycle Settling
Internal Low-Drift Voltage Reference
Internal Oscillator
Wide Operating Temperature Range
Available in Ultra-Small X2QFN Package

ADS1115 Pin Configuration
The below image shows the pin configuration of the ADS1115 chip.

![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/028b11b7-123e-4353-836e-4c8c55c023a9)

Pin 1 is the ADDR pin that selects the I2C address for the chip.
Pin 2 is the Alert/Ready pin which serves as a data ready and alert signal.
Pin 3 is the GND terminal.
Pins 4, 5, 6 & 7 are the four (4) ADC input pins. We can use these pins as either four (4) single-ended inputs or two (2) differential inputs.
Pin 8 is the positive power supply pin  which accepts 2.0 V to 5.5 V
Pins 9 and 10 are the terminals for the I2C interface, SDA and SCL respectively.

ADS1115 Functional Block Diagram
The below is the functional diagram for the ADS1115 Chip.

![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/2803e210-b0f5-4ad7-b0ff-fb9662a4e15c)

Initially a multiplexer selects the input signal. Then, the selected signal feeds into a Programmable Gain amplifier (PGA). The PGA can programmed to provide amplification of small signals prior to conversion.

Subsequently, the input is converted by a 16-bit Delta Sigma converter. The converter uses its own built-in voltage reference and built-in oscillator in measuring the input signal. Finally, the result of the conversion goes into the I2C interface. Also, a comparator provides a signal to the external interface that the result is ready for fetching.
Typical Connections of the ADS1115
The principle I2C connections for the ADS1115 is shown in the image below.
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/a7ad589d-423b-4e33-a638-61c24219434d)
The ADS1115 interfaces directly to standard mode, fast mode, and high-speed mode I2C controllers. Any microcontroller I2C peripheral, including master-only and single-master I2C peripherals, operates with the ADS1115.

For more information refer to ADS1115 Datasheet
ADS1115 Module or Breakout Board
The ADS1115 comes with X2QFN & VSSOP package which can’t be used for prototyping. Therefore we need the ADS1115 Module or Breakout Board to use it with Arduino or Any other Microcontroller.

![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/86eeb9a5-eb2d-4098-9cc5-a4627cec24df)

These modules are widely available from different manufacturers and are very inexpensive. They are breadboard friendly and can be easily used in prototyping and testing applications.


![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/5c2a003b-d785-4317-ae97-3f6834e3983f)

![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/07f09ff8-1fb0-48be-878c-0e8cde1bd8e8)

For more info visit this page https://how2electronics.com/how-to-use-ads1115-16-bit-adc-module-with-arduino/

# About MCP4725 Digital to analog converter
We all know that the Microcontrollers work only with digital values but in real world we have to deal with analog signals. That’s why ADC (Analog to Digital Converters) is there to convert real world Analog values into Digital form so that microcontrollers can process the signals. But what if we need Analog signals from digital values, so here comes the DAC (Digital to Analog Converter).

A simple example for Digital to Analog converter is recording a song in studio where an artist singer is using microphone and singing a song. These analog sound waves are converted into digital form and then stored in a digital format file and when the song is played using the stored digital file those digital values are converted into analog signals for speaker output. So in this system DAC is used.

DAC can be used in many applications such as Motor control, Control Brightness of the LED Lights, Audio Amplifier, Video Encoders, Data Acquisition Systems etc.

In many microcontrollers there is an internal DAC that can be used to produce analog output. But Arduino processors such as ATmega328/ATmega168 don’t have DAC inbuilt. Arduino has ADC feature (Analog to Digital Converter) but it has no DAC (Digital to Analog Converter). It has a 10-bit DAC in internal ADC but this DAC cannot be used as standalone. So here in this Arduino DAC tutorial, we use an additional board called MCP4725 DAC Module with Arduino.

MCP4725 DAC Module (Digital to Analog Converter)
MCP4725 IC is a 12-Bit Digital to Analog Converter Module which is used to generate output analog voltages from (0 to 5V) and it is controlled by using I2C communication. It also comes with on board nonvolatile memory EEPROM.

This IC has 12-Bit resolution. This means we use (0 to 4096) as input to provide the voltage output with respect to reference voltage. Maximum reference voltage is 5V.

Formula to calculate Output Voltage

O/P Voltage = (Reference Voltage / Resolution) x Digital Value
For Example if we use 5V as reference voltage and let’s assume that digital value is 2048. So to calculate the DAC output.

O/P Voltage = (5/ 4096) x 2048 = 2.5V
Pinout of MCP4725

Below is the image of MCP4725 with clearly indicating pin names.

![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/1b98a234-f374-4500-bb38-5c1bd5af61af)

for more info refer this link https://circuitdigest.com/microcontroller-projects/arduino-dac-tutorial-interfacing-mcp4725-dac

# About I2C OLED
The organic light-emitting diode (OLED) display that we’ll use in this tutorial is the SSD1306 model: a monocolor, 0.96-inch display with 128×64 pixels as shown in the following figure.
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/df54c0c6-1316-40ca-bfea-6ca40f999312)

The OLED display doesn’t require backlight, which results in a very nice contrast in dark environments. Additionally, its pixels consume energy only when they are on, so the OLED display consumes less power when compared with other displays.

The model we’re using here has only four pins and communicates with the Arduino using I2C communication protocol. There are models that come with an extra RESET pin. There are also other OLED displays that communicate using SPI communication.

Pin wiring
Because the OLED display uses I2C communication protocol, wiring is very simple. You just need to connect to the Arduino Uno I2C pins as shown in the table below.
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/b82cc194-ba56-4367-8f39-0cdc59270d6e)

For more info refer this link https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/

# About KY-040 Rotary Encoder
KY-040 rotary encoder is a device that generates an electrical signal based on how much the rotary input device (knob) is rotated and the direction it is rotating in. It is a position sensor with a knob and can be used to control stepper or servo motors with precision.

![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/8b32372e-dccc-4acc-99b3-ae218f7fe657)

Features and Specifications of KY-040 Rotary Encoder
Below mentioned are some of the features and specifications of the KY-040 Rotary Encoder module:

Operating Voltage: 5V
Mechanical angle: 360 Degrees
Output: 2-bit gray code
Positions per revolution: 30
Pin Configuration of KY-040 Rotary Encoder
The KY-040 module has 5 output pins. Two of which are for the power of the module whereas, 3 are for the output data of the turns and direction of the knob. The table below describes the pin type and the function of each pin in the Rotary module.
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/c90f8556-7c67-4e5a-ae51-110181d24bda)

Connecting KY-040 Rotary Encoder Module with MCU/MPU
Connecting a KY-040 Rotary encoder module to a microcontroller or a microprocessor is quite simple. As mentioned in the section above, there are 5 pins on the encoder module.
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/7699b171-6350-4010-930f-664fb30d5ad7)
Two pins for the power, +, and ground of the encoder module can be connected to the VCC and the ground terminal of the MCU/MPU, respectively. The CLK and the DT pin both generate a digital output and can be connected to the digital pins of an MCU/MPU.

Working Principle of KY-040 Rotary Encoder
The encoder module generates a digital signal(square pulses) upon the rotation of the knob, let’s see how the encoder module works.

The encoder has a disk with contact pints evenly placed from each other which are connected to the common pin C(Ground). As we start rotating the knob, the disk also moves and makes contact with the output pins A and B one by one, hence generating two square waves simultaneously. Simply by counting the number of square waves generated, the rotated position can be determined. A simple program can be written to monitor the number of waves generated and output can be extracted.

![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/38afe123-70bf-4879-a8eb-63998cc06f6d)

To know the direction of rotation, determining how the switch changes its position is essential.

![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/b6baa393-9f1e-42b1-8916-2953f575f31e)

If output A changed states first, the switch is rotating in a clockwise direction, whereas if output B changes its state first, we know that the switch is rotating in a counter-clockwise direction.

 

Applications for KY-040 Rotary Encoder
Here are some of the applications of the KY-040 rotary encoder module:

Robotic arm controller
Servo and Stepper motor control
Precise motor movement

2D model
![image](https://github.com/yogeshiggalore/VAforEA/assets/5477695/ea1bb56d-713b-4626-82a5-e29e0e49236f)

