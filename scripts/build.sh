set -e

ESP=${1:-esp32}

ARDUINO_BUILDER=$HOME/projects/arduino-builder/arduino-builder
ARDUINO_LIB=$HOME/Library/Arduino15
LIBRARIES=$HOME/Documents/Arduino/libraries
OUTDIR=out/$ESP

if [ "$ESP" = "esp32" ]; then
	FQBN=esp32:esp32:esp32wrover:FlashFreq=80
	TOOLS=$ARDUINO_LIB/packages/$ESP/$ESP/tools
	FLASHCMD="$TOOLS/esptool \
		--port /dev/cu.usbserial-1410 \
		--baud 460800 \
		write_flash 0xe000 \
		$TOOLS/partitions/boot_app0.bin 0x1000 \
		$TOOLS/sdk/bin/bootloader_dout_80m.bin 0x10000 \
		$OUTDIR/client.ino.bin 0x8000 \
		$OUTDIR/client.ino.partitions.bin"

elif [ "$ESP" = "esp8266" ]; then
	FQBN=esp8266:esp8266:nodemcuv2:CpuFrequency=80,UploadSpeed=921600,FlashSize=4M3M
	TOOLS=$ARDUINO_LIB/packages/$ESP/tools
	FLASHCMD="$TOOLS/esptool -cp /dev/cu.usbserial-1410 -cb 921600 -cd nodemcu -cf $OUTDIR/main.ino.bin"

else
	echo "Unrecognized ESP: $ESP"
	exit 1
fi

mkdir -p $OUTDIR

$ARDUINO_BUILDER \
	-build-path $OUTDIR \
	-hardware $ARDUINO_LIB/packages \
	-libraries $LIBRARIES \
	-tools $TOOLS \
	-fqbn $FQBN \
	$PWD/src/client/client.ino

echo Flash to device:
$FLASHCMD
