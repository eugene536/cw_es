INCLUDE=-I/home/eugene/IDE/arduino-1.8.1/hardware/arduino/avr/cores/arduino \
		-I/home/eugene/IDE/arduino-1.8.1/hardware/arduino/avr/variants/standard/ \
		-I/home/eugene/IDE/arduino-1.8.1/hardware/tools/avr/avr/include/ \
		-I/home/eugene/IDE/arduino-1.8.1/hardware/arduino/avr/libraries/SoftwareSerial/src/ \
		-I/home/eugene/IDE/arduino-1.8.1/libraries/LiquidCrystal/src/

LIB=-L/home/eugene/IDE/arduino-1.8.1/hardware/arduino/avr/cores/arduino /home/eugene/IDE/arduino-1.8.1/hardware/arduino/avr/libraries/SoftwareSerial/src/SoftwareSerial.cpp /home/eugene/IDE/arduino-1.8.1/libraries/LiquidCrystal/src/LiquidCrystal.cpp

default:
	avr-gcc ${LIB} ${INCLUDE} -Wall -DF_CPU=16000000UL -Os -mmcu=atmega328p -o main.elf main.cpp -larduino
	avr-objcopy -O ihex -R .eeprom main.elf out.hex

upload:
	avrdude -F -V -c arduino -p ATMEGA328P -b 115200 -P /dev/ttyACM0 -U flash:w:out.hex

all: default upload

clean:
	rm *.elf *.hex
