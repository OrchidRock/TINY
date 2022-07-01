.PHONY: clean

TARGET = tiny
SOURCES = tiny2.c transaction.c #addr.c
OBJECTS = $(SOURCES:%.c=%.o)
CFLAGS = -Wall -O0 -DDEBUG
INCLUDE = -I./ -I../lib 

$(TARGET): $(OBJECTS)
	gcc $(CFLAGS) $(INCLUDE) -o $@ $^ ../lib/libcsapp.so

%.o:%.c
	gcc $(CFLAGS) $(INCLUDE) -o $@ -c $<


clean:
	-rm -rf $(TARGET) $(OBJECTS)

