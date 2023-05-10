SET PATH=F:\Keil MDK5\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Program Files\Java\jdk1.8.0_291\jre\bin;C:\Program Files\Java\jdk1.8.0_291\bin;F:\IDEA\apache-tomcat-9.0.45-windows-x64\apache-tomcat-9.0.45\bin;F:\python1\Scripts\;F:\python1\;F:\vscode\Microsoft VS Code\bin;F:\IDEA\IntelliJ IDEA 2018.3.6\bin;
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=ADC-DEMO
SET CPU_CLOCK=0x00B71B00
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_hd._ia"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\usart1.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\adc.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\dac.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\spi.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\afe.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\timer.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_usart.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_spi.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\misc.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_tim.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"F:\Keil MDK5\ARM\ARMCC\Bin\ArmLink" --Via ".\STM32-DEMO.lnp"
"F:\Keil MDK5\ARM\ARMCC\Bin\fromelf.exe" ".\STM32-DEMO.axf" --i32combined --output ".\STM32-DEMO.hex"
