CC = cc -g -I/usr/local/include
objects = sniffer.o
all: $(objects)
	$(CC) -o sniffer.dxe $(objects) /usr/local/lib/libpcap.a \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/dpslib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/lan/v4.0a/lib/lan.lib \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	-lm -lsocket -lnsl

sniffer.o: sniffer.c

clean:
	rm sniffer.o sniffer.dxe
