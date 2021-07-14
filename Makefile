PROGRAM=main
CXXFLAGS+=-g -std=c++11 -Wall -Wextra -fsanitize=address

# detect opencv lib
OPENCVLIB=$(strip $(shell pkgconf --list-package-names | grep opencv ))

ifeq ($(OPENCVLIB),)

all:
	@echo OpenCV lib not found!
	@exit 

else

CXXFLAGS+=$(shell pkgconf --cflags $(OPENCVLIB))
LDFLAGS+=$(shell pkgconf --libs-only-L $(OPENCVLIB))
LDLIBS+=-lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui

all: $(PROGRAM)

endif

$(PROGRAM): $(PROGRAM).cpp
	g++ $(CXXFLAGS) $(PROGRAM).cpp -o $@ $(LDFLAGS) $(LDLIBS)

clean: 
	rm -rf $(PROGRAM)

