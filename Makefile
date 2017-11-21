
ifneq ($(CROSS_COMPILE),)
$(info CROSS_COMPILE=$(CROSS_COMPILE))
endif

CXX := $(CROSS_COMPILE)g++

OS = $(shell uname -s)

KOALA = ../KoalaJS/build
CFLAGS = -Wall -g -I./ -I$(KOALA)/include -fPIC -std=c++11  -stdlib=libc++ 
LDFLAGS = -L$(KOALA) -lKoalaJS \
		-lpthread -luWS -lssl -lcrypto -lz -luv
NATIVE_DIR = native

TARGET = build

all:
	mkdir -p $(TARGET)
	$(CXX) $(CFLAGS) -shared -o $(TARGET)/KoalaWS.so $(NATIVE_DIR)/*.cpp  $(LDFLAGS)
	rm -fr *.dSYM

clean:
	rm -fr $(TARGET)
	rm -fr *.o *.dSYM *.bcode

install:
	mkdir -p /usr/local/koala/modules
	cp build/*.so /usr/local/koala/modules
	cp -r classes /usr/local/koala
