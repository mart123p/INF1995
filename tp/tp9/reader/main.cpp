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
	uart::clear();
	uart::print("Starting VirtualMachine\n");
	
	while(!estTermine){
		
		memoire.lecture(adresse,donnee,2);//lecture de deux octets a une adresse dans le tableau donnee
		adresse += 2;//compteur d'adresse

		uart::print("[Mem] => Instruction: ");
		uart::print(donnee[0]);
		uart::print(" ");
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

				//On met toutes les instructions dans la mémoire vive. Si on doit faire plus d'une itération
				if(donnee[1] > 0){

					uint16_t addresseBoucle = adresse;
					uint8_t donneeBoucle[2];
					
					uint8_t* instructionBuffer = 0;
					uint8_t instructionCount = 0;

					
					memoire.lecture(addresseBoucle,donneeBoucle,2);
					while(donneeBoucle[0] != fbc){
						addresseBoucle+=2;
						instructionBuffer =(uint8_t*) realloc(instructionBuffer,instructionCount*2+2);
						
						for(uint8_t i = 0; i < 2; i++){
							instructionBuffer[instructionCount*2+i] = donneeBoucle[i];
						}
						instructionCount++;
						memoire.lecture(addresseBoucle,donneeBoucle,2);
					}
					//On fait le nombre d'ittérations nécessaires qui sont spécifiées par l'opérande.
					for(uint8_t i = 0; i < donnee[1];i++){
						uart::print("\n\n*** loop ");
						uart::print(i);
						uart::print(") nb instruction: ");
						uart::print(instructionCount);
						uart::print(" ***\n");
						for(uint8_t j = 0; j < instructionCount;j++){
							uart::print("\n--executing instruction ");
							uart::print(j);
							uart::println();
							
							uint8_t* singleInstruction = instructionBuffer+j*2;
							instructionExecute(singleInstruction);
							
							uart::print("--instruction ");
							uart::print(singleInstruction[0]);
							uart::print(" ");
							uart::print(singleInstruction[1]);
							uart::print(" executed\n");
						}
						uart::print("*** end loop ***\n");
					}
					//We cleanup memory
					free(instructionBuffer);
					instructionBuffer = 0;
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
					uart::print("att\n");
					for (uint8_t i = 0; i < donnee[1]; i++){
						_delay_ms(25);
					}
					break;
					
				case dal:
					uart::print("dal\n");
					light::on(donnee[1]);
					break;
					
				case det:
					uart::print("det\n");
					light::off(donnee[1]);
					break;
					
				case sgo:
					
					break;
					
				case sar:
				
					break;
				
				case mar:	
				case mar1:
					uart::print("mar1\n");
					pwm::off();
					break;


				case mav:
					uart::print("mav\n");
					pwm::setA(donnee[1],0);
					pwm::setB(donnee[1],0);
					break;
				
					
				case mre:
					uart::print("mre\n");
					pwm::setA(donnee[1],1);
					pwm::setB(donnee[1],1);
					break;
				
					
				case trg:
					uart::print("trg\n");
					pwm::setA(255,0);
					pwm::setB(255,1);
					_delay_ms(425);
					pwm::off();
					break;
				case trd:
					uart::print("trd\n");
					pwm::setA(255,1);
					pwm::setB(255,0);
					_delay_ms(425);
					pwm::off();
					break;
				
			}
}
