include ../Makefile.common
all: libohboy.a
	
libohboy.a: uart.o pwm.o memory.o can.o light.o sound.o timer.o soundpwm.o sensors.o
	avr-ar crs -o libohboy.a $^
pwm.o: pwm.h pwm.cpp
	$(CC) $(CFLAGS) -l. -c pwm.cpp -o $@
uart.o: uart.h uart.cpp
	$(CC) $(CFLAGS) -l. -c uart.cpp -o $@
memory.o: memory.h memory.cpp
	$(CC) $(CFLAGS) -l. -c memory.cpp -o $@
can.o: can.h can.cpp
	$(CC) $(CFLAGS) -l. -c can.cpp -o $@
light.o:light.h light.cpp
	$(CC) $(CFLAGS) -l. -c light.cpp -o $@
timer.o:timer.h timer.cpp
	$(CC) $(CFLAGS) -l. -c timer.cpp -o $@
sound.o:sound.h sound.cpp
	$(CC) $(CFLAGS) -l. -c sound.cpp -o $@
sensors.o:sensors.h sensors.cpp
	$(CC) $(CFLAGS) -l. -c sensors.cpp -o $@
soundpwm.o:soundpwm.h soundpwm.cpp
	$(CC) $(CFLAGS) -l. -c soundpwm.cpp -o $@
clean:
	rm *.a
	rm *.o
	rm *.d

