# VAforEA
VAforEA - Voice Assistant for Engineering Automation

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

