.PHONY: clean

TARGET = libcsapp.so
SOURCES = csapp.c sbuf.c
OBJECTS = $(SOURCES:%.c=%.o)
CFLAGS = -Wall -fPIC

$(TARGET): $(OBJECTS)
	gcc $(CFLAGS) -shared -o $@ $^ -lpthread

%.o:%.c
	gcc $(CFLAGS) -o $@ -c $<


clean:
	-rm -rf $(TARGET) $(OBJECTS)

