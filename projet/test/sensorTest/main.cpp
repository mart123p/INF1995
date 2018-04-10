#include "ohBoy.h"

int main(){
	uart::init();
	can::can can; 
	while(true){
		uart::print(can.lecture(1));
		uart::println();
	}
}
