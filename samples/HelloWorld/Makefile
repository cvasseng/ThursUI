CC=g++
CFLAGS=-c -O2 -static -Wall -I../../include -I/usr/local/include -I/usr/include 

SOURCES=helloworld.cpp

LIBRARIES=-ljsoncpp -lpng -lnanovg -lglfw3 -lglew

LDFLAGS=-L../../lib -L/usr/local/lib -L/opt/local/lib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -L.
OBJECTS=$(SOURCES:.cpp=.o)
OUT=../../bin/hello-world

all: clean $(SOURCES) $(OUT)
	
$(OUT): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBRARIES) ../../lib/libthursui.a -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o

rebuild: clean all
