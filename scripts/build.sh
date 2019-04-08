set -e

ESP=${1:-esp32}

OUTDIR=$PWD/out/$ESP
IDE=$HOME/projects/arduino-1.8.9

LIBRARIES="-libraries third_party"
HARDWARE="-hardware $IDE/hardware -hardware third_party/esp32/.."
TOOLS="-tools $IDE/tools-builder"
ESPTOOLS="third_party/$ESP/$ESP/tools"

if [ "$ESP" = "esp32" ]; then
	FQBN="-fqbn esp32:esp32:nodemcu-32s:FlashFreq=80"
	TOOLS="$TOOLS -tools $ESPTOOLS"
	FLASHCMD="$ESPTOOLS/esptool.py --port /dev/ttyUSB0 --baud 921600 write_flash 0xe000 $ESPTOOLS/partitions/boot_app0.bin 0x1000 $ESPTOOLS/sdk/bin/bootloader_dout_80m.bin 0x10000 $OUTDIR/main.ino.bin 0x8000 $OUTDIR/main.ino.partitions.bin"

elif [ "$ESP" = "esp8266" ]; then
	FQBN="-fqbn esp8266:esp8266:nodemcuv2:CpuFrequency=80,UploadSpeed=921600,FlashSize=4M3M"
	TOOLS="$TOOLS -tools $ARDUINO_LIB/packages/$ESP/tools"
	FLASHCMD="$TOOLS/esptool -cp /dev/cu.SLAB_USBtoUART -cb 921600 -cd nodemcu -cf $OUTDIR/main.ino.bin"

else
	echo "Unrecognized ESP: $ESP"
	exit 1
fi

mkdir -p $OUTDIR

$IDE/arduino-builder -build-path=$OUTDIR $HARDWARE $TOOLS $LIBRARIES $FQBN src/client/main.ino

echo Flash to device:
echo $FLASHCMD
