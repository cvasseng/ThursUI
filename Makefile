CC=g++
CFLAGS=-c -O2 -static -Wall -I/usr/local/include -I/usr/include 

SOURCES=src/thurs.control.cpp src/thurs.input.cpp src/thurs.renderer.cpp src/thurs.surface.cpp src/thurs.color.cpp src/thurs.os.cpp src/controls/thurs.control.button.cpp src/controls/thurs.control.progressbar.cpp

LIBPATH=/usr/local/lib
LIBRARIES=

LDFLAGS=-L/usr/local/lib
OBJECTS=$(SOURCES:.cpp=.o)
OUT=lib/libthursui.a

TARGETLIBDIR=/usr/local/lib
TARGETINDIR=/usr/local/include

all: $(SOURCES) $(OUT)
	
$(OUT): $(OBJECTS) 
	#$(CC) $(LDFLAGS) $(OBJECTS) $(LIBRARIES) -o 
	ar rcsv $(OUT) $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf src/*.o 
	rm -rf src/controls/*.o

rebuild: clean all

install:
	cp -f $(OUT) $(TARGETLIBDIR)
	cp -r include/thurs /usr/local/include/
