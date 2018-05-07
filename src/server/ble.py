import bluepy

from bluepy import btle

servers=["30:AE:A4:7B:01:8A", "30:AE:A4:75:6F:D6"]

for server in servers:
	print("Connecting to %s..." % server)
	dev = btle.Peripheral(server)
 
	print("Connected. Services:")
	for svc in dev.services:
	    print(str(svc))

	service_uuid="6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
	print("Getting service_uuid %s" % service_uuid)

	svc = dev.getServiceByUUID(service_uuid)
	print(str(svc))

	char_uuid="6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
	char = svc.getCharacteristics(char_uuid)

	char[0].write(b'hello from python')
	print("Message sent")
