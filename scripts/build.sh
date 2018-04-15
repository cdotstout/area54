set -e

ESP=${1:-esp32}

ARDUINO_IDE=/Applications/Arduino.app/Contents/Java
ARDUINO_LIB=$HOME/Library/Arduino15
LIBRARIES=$HOME/Documents/Arduino/libraries
OUTDIR=$PWD/out/$ESP

if [ "$ESP" = "esp32" ]; then
	FQBN=esp32:esp32:nodemcu-32s:FlashFreq=80
	TOOLS=$ARDUINO_LIB/packages/$ESP/$ESP/tools
	FLASHCMD="$TOOLS/esptool --port /dev/cu.SLAB_USBtoUART --baud 921600 write_flash 0xe000 $TOOLS/partitions/boot_app0.bin 0x1000 $TOOLS/sdk/bin/bootloader_dout_80m.bin 0x10000 $OUTDIR/main.ino.bin 0x8000 $OUTDIR/main.ino.partitions.bin"

elif [ "$ESP" = "esp8266" ]; then
	FQBN=esp8266:esp8266:nodemcuv2:CpuFrequency=80,UploadSpeed=921600,FlashSize=4M3M
	TOOLS=$ARDUINO_LIB/packages/$ESP/tools
	FLASHCMD="$TOOLS/esptool -cp /dev/cu.SLAB_USBtoUART -cb 921600 -cd nodemcu -cf $OUTDIR/main.ino.bin"

else	
	echo "Unrecognized ESP: $ESP"
	exit 1
fi

mkdir -p $OUTDIR

$ARDUINO_IDE/arduino-builder -build-path=$OUTDIR -hardware $ARDUINO_IDE/hardware/ -hardware $ARDUINO_LIB/packages -tools $TOOLS -tools $ARDUINO_IDE/tools-builder  -libraries=$LIBRARIES -fqbn=$FQBN src/client/main.ino

echo Flash to device:
echo $FLASHCMD
