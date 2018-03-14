#include "ohboy.h"

#define dbt 0x01
#define att 0x02
#define dal 0x44
#define det 0x45
#define sgo 0x48
#define sar 0x09
#define mar 0x60
#define mar1 0x61
#define mav 0x62
#define mre 0x63
#define trd 0x64
#define trg 0x65
#define dbc 0xC0
#define fbc 0xC1
#define fin 0xFF


int main () {
	uart::init();
	mem::Memoire24CXXX memoire;
	uint16_t adresse = 0;
	uint8_t donnee[2]; 
	
	bool estTermine = false;
	bool debutCode = false;
	while(!estTermine){
		
		memoire.lecture(adresse,donnee,2);//lecture de deux octets a une adresse dans le tableau donnee
		adresse += 2;//compteur d'adresse
		
		if(!debutCode && donnee[0] == dbt){
			debutCode = true;
			light::init();
			pwm::init();
		}

		uart::print(donnee[0]);
		uart::println();
		uart::print(donnee[1]);
		uart::println();
	
		if(debutCode){	
		switch (donnee[0]){
				
			case att:
				uart::print("case att");
				for (uint8_t i = 0; i < donnee[1]; i++){
					_delay_ms(25);
				}
				uart::print("case att break");
				break;
				
			case dal:
				uart::print("case dal");
				light::on(donnee[1]);
				uart::print("case dal break");
				break;
				
			case det:
				uart::print("case det");
				light::off(donnee[1]);
				uart::print("case det break");
				break;
				
			case sgo:
				
				break;
				
			case sar:
			
				break;
			
			case mar:	
			case mar1:
				uart::print("case mar1");
				pwm::off();
				uart::print("case mar1 break");
				break;


			case mav:
			
				break;
			
				
			case mre:
			
				break;
			
				
			case trg:
			
				break;
				
				
			case dbc:
			
			
				break;
				
				
			case fbc:
			
			
				break;
				
				
			case fin:
				estTermine = true;
				pwm::off();
				//son off
				break;
			
			
			
		}
	}
		
	
		
		
	}
	
	
	while(true);
	
	
	
}
