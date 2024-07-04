# VAforEA
VAforEA - Voice Assistant for Engineering Automation

# Before anything Let me give all credit to the following developers for their extraordinary work.
https://www.electroschematics.com/dc-module/

https://www.instructables.com/DIgital-Controlled-Bench-Power-Supply/

https://docs.ai-thinker.com/en/voice_module

https://www.espressif.com/

https://randomnerdtutorials.com/getting-started-with-esp32/


# About XL4015 power supply module.
The XL4015 power supply module has onboard constant voltage (CV) and constant current (CC) control feature which is useful for general-purpose power supply and battery charger applications.

![2-XL4015-CC-CV-Buck-Module-Specs](https://github.com/yogeshiggalore/VAforEA/assets/5477695/f2140c1c-3bf8-40e0-83e8-ec84e17bfc34)

apart from the input and output screw terminal connectors, there are two multiturn trimpots to adjust the output voltage and output current. Further, there are three LED indicators – the first one near the input connector is the constant-current (cc) indicator, while next two LEDs are intended mainly for battery charging applications (battery charging and battery full indicators). However, when used as a general-purpose power supply, the battery charging indicator LED will work as a ‘load-on’ annunciator.

![4-XL4015-CC-CV-Buck-Module-Annotated](https://github.com/yogeshiggalore/VAforEA/assets/5477695/f73214a1-71de-4d92-933c-1f4c08663051)

Slowly turning the voltage adjustment (V-ADJ) trimpot clockwise will raise the output voltage gradually, and a counterclockwise rotation will lower it. Likewise, clockwise turning of the current adjustment (I-ADJ) trimpot increases the current limit while counterclockwise action decreases the current limit. It’s advised to adjust the voltage level at first, and then the current level (this fine-tuning procedure will be discussed in detail later).

Needless to say, at the heart of the module is a XL4015 chip from XLSEMI (www.xlsemi.com) which is a 180 KHz fixed frequency PWM step-down (buck) DC/DC converter, capable of driving a 5A load with high efficiency, low ripple and excellent line and load regulation. According to its datasheet (Rev 1.5) the PWM control circuit can adjust the duty ratio linearly from 0 to 100%. An over current protection function is built inside so that when a short circuit occurs, the operation frequency will be reduced to 48KHz. Below you can see the function block diagram of XL4015 (XL4015E1).

![5-XL4015-Function-Block-Diagram](https://github.com/yogeshiggalore/VAforEA/assets/5477695/01e852fc-c6f8-4643-8e2e-607a4b2d33fe)

The core of the design merely follows the typical application example (see below). An exception is that the recommended 1uF bypass capacitor (C1) is not used in this module, provision is already provided in the PCB, though.

![6-XL4015-Typical-Application-Circuit](https://github.com/yogeshiggalore/VAforEA/assets/5477695/0df8f4b7-f1df-44fd-9e3c-f9c5f85522e5)

The rest of the electronics include one 78L05 fixed voltage regulator, one TL431 precision shunt regulator, and a LM358 operational amplifier. I haven’t seen the 78L05 chip in my module at first as it’s situated underneath the flat toroid-core inductor.
![7-XL4015-CC-CV-Buck-Module-Key-Parts](https://github.com/yogeshiggalore/VAforEA/assets/5477695/33eaec34-7baa-435d-bd34-6125eaca4a5e)
As you might guess correctly, the three ‘surplus’ chips are included in this revised XL4015 module for current control function. If you turned over the module, then you find a 0.05Ω (R050) current sensing resistor at the bottom of the PCB.
![8-XL4015-Module-Isense-Resistor](https://github.com/yogeshiggalore/VAforEA/assets/5477695/ee848f8f-aaf8-4d9a-80ce-a012098904c9)

Next is the adopted schematic of the XL4015 CV/CC module. This is a very basic schematic as there is a myriad of befuddling replications and revisions – No surprise, simply a common Chinese business practice!
![9-XL4015-CC-CV-Buck-Module-CKT](https://github.com/yogeshiggalore/VAforEA/assets/5477695/766d00fe-9105-4d6e-85cc-6be0c48c5622)

The overall design of the module is indeed a pretty neat and clever piece of work. Frankly, I was quite impressed by the unperplexed design idea of the LM358 based current control segment as this edition also includes a TL431 shunt regulator. In a previous version, I found that there was no TL431 chip onboard but the same regulated 5VDC (outputted by one LM317 chip) is used for the reference voltage as well (see next schematic snip). In principle, TL431/TS431 provides a reference that is more stable than the 78L05!

![10-Previous-Version-CKT-Snip](https://github.com/yogeshiggalore/VAforEA/assets/5477695/4768451c-f199-4661-a2df-1f82dd31b381)

# About VC-02 module

Overview
VC series module is a low-cost pure offline speech recognition module developed by Shenzhen Ai-Thinker Technology Co., Ltd. The module uses the voice chip US516P6 launched by Unisound. It features high reliability and versatility. Highly reliable wake-up recognition rates, more distant wake-up, more low-incorrect wokens, more low-erroneous wake-up rates, more low-than-off, faster response identification time, secreted pure line identification.

The VC Series module adopt a 32bit RISC architectural kernel and has joined the DSP instruction set required for signal processing and speech recognition, support floating point operations, and FFT accelerators. Supports up to 150 local instructions offline identification, support RTOS lightweight systems, as well as simple and friendly customary tools.

The VC Series module has a rich peripheral interface, including UART / I2C / PWM / SPI, and simple&friendly secondary development tools for customers to implement a single-mode group speech control application scenario scheme.

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

