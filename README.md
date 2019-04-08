Environment setup:

- Install arduino IDE (build.sh should point to this)
- clone esp32 repository under third_party/esp32/esp32
	https://github.com/espressif/arduino-esp32.git
- follow instructions for setting up esp32 repo
	docs/arduino-ide/debian_ubuntu.md

- clone FastLED under third_party
	https://github.com/FastLED/FastLED.git

- clone nanopb under third_party
	https://github.com/nanopb/nanopb.git

To regenerate protos:
	cd src/server && protoc -I=../client --python_out=. ../client/area54.proto
	cd src/client && protoc --plugin=protoc-gen-nanopb=../../third_party/nanopb/generator/protoc-gen-nanopb --nanopb_out=. area54.proto

To build:

- run build.script.sh
