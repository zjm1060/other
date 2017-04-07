
PREFIXPATH=/usr/local/mxscaleb/bin
CC=$(PREFIXPATH)/mxscaleb-gcc
STRIP=$(PREFIXPATH)/mxscaleb-strip
LIBS=-lpthread
NAME=main
OBJECTS = 	main.c\
		common.c


all: release debug

clean:
	rm -f $(NAME).dxe $(NAME)-debug

release:   
	$(CC) -o $(NAME).dxe $(OBJECTS) $(LIBS) 
	$(STRIP) -s $(NAME).dxe

debug:   
	$(CC) -ggdb -o $(NAME)-debug $(OBJECTS) $(LIBS)
