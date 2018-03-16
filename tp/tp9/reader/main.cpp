#include "ohBoy.h"
#include <stdlib.h>

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
void instructionExecute(uint8_t* donnee);

int main () {
	uart::init();
	mem::Memoire24CXXX memoire;
	uint16_t adresse = 2;
	uint8_t donnee[2]; 

	bool estTermine = false;
	bool debutCode = false;
	
	while(!estTermine){
		
		memoire.lecture(adresse,donnee,2);//lecture de deux octets a une adresse dans le tableau donnee
		adresse += 2;//compteur d'adresse


		uart::print(donnee[0]);
		uart::println();
		uart::print(donnee[1]);
		uart::println();
		
		if(!debutCode && donnee[0] == dbt){
			debutCode = true;
			light::init();
			pwm::init();
		}
	
		if(debutCode){
			if(donnee[0] == fin){
				estTermine = true;
				pwm::off();
				//son off
			}else if(donnee[0] == dbc){
				uint16_t addresseBoucle = adresse +2;
				uint8_t donneeBoucle[2];
				
				uint8_t* instructionBuffer = 0;
				uint8_t instructionCount = 0;

				//On met toutes les instructions dans la mémoire vive. Si on doit faire plus d'une itération
				if(donnee[1] > 0){
					while(donneeBoucle[0] != fbc){
						memoire.lecture(addresseBoucle,donneeBoucle);
						addresseBoucle+=2;
						instructionBuffer =(uint8_t*) realloc(instructionBuffer,instructionCount*2+2);
						
						for(uint8_t i = 0; i < 2; i++){
							instructionBuffer[instructionCount*2+i] = donneeBoucle[i];
						}
						instructionCount++;
					}
					//On fait le nombre d'ittérations nécessaires qui sont spécifiées par l'opérande.
					for(uint8_t i = 0; i < donnee[1];i++){
						for(uint8_t j = 0; j < instructionCount;j++){
							uint8_t* singleInstrcution = instructionBuffer+(instructionCount-1)*2;
							instructionExecute(singleInstrcution);
						}
					}
					//We cleanup memory
					free(instructionBuffer);
				}
			}else if(donnee[0] != fbc){
				instructionExecute(donnee);
			}
		}
		
	
		
		
	}
	
	
	while(true);
}

void instructionExecute(uint8_t* donnee){
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
					pwm::setA(donnee[1],1);
					pwm::setB(donnee[1],1);
					break;
				
					
				case mre:
					pwm::setA(donnee[1],0);
					pwm::setB(donnee[1],0);
					break;
				
					
				case trg:
					pwm::setA(255,1);
					pwm::setB(255,0);
					_delay_ms(200);
					break;
				case trd:
					pwm::setA(255,0);
					pwm::setB(255,1);
					_delay_ms(200);
					break;
				
			}
}
