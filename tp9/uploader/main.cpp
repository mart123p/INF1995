#include <avr/io.h>
#include <avr/interrupt.h>
#include <ohboy.h>
volatile bool readDataBack = false;
mem::Memoire24CXXX memoire;
int  main()
{
        uart::init();
        light::init();

        DDRD &= ~(2 << DDD2);     // Clear the PD2 pin 
        EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
        EIMSK |= (1 << INT0);     // Turns on INT0

        sei();

        uint8_t data = 0;
        uint16_t address = 0;
        light::red();
        while(true){
                data = uart::readData();
                light::green();
                memoire.ecriture(address,data);
                address++;
        }
}
//Interrupt Service Routine for INT0
ISR(INT0_vect)
{
        _delay_ms(5);
        if ( PIND & 0x04 ){

                light::red();
                uint8_t data = 0;
                for(uint16_t i = 0; i < 65535; i++){

                        memoire.lecture(i,&data);
                        uart::print(data);
                        uart::print(" ");
                        if(i % 2 == 1)
                                uart::println();
                }
                while(true);
        }
}