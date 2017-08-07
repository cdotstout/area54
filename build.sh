ARDUINO_IDE=/Applications/Arduino.app/Contents/Java

ARDUINO_LIB=$HOME/Library/Arduino15
LIBRARIES=$HOME/Documents/Arduino/libraries

FQBN=esp8266:esp8266:nodemcuv2:CpuFrequency=80,UploadSpeed=921600,FlashSize=4M3M

OUTDIR=$PWD/out
mkdir -p $OUTDIR

$ARDUINO_IDE/arduino-builder -build-path=$OUTDIR -hardware $ARDUINO_IDE/hardware/ -hardware $ARDUINO_LIB/packages -tools $ARDUINO_LIB/packages/esp8266/tools/ -tools $ARDUINO_IDE/tools-builder  -libraries=$LIBRARIES -fqbn=$FQBN main.ino

echo Flash to device:
echo esptool -cp /dev/cu.SLAB_USBtoUART -cb 921600 -cd nodemcu -cf out/main.ino.bin
