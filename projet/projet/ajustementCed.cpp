void Parcours::ajustement1() {
	if (currentValue0 > 16 && currentValue0 < borneSupAjustement){
		uart::parcoursDebug(currentValue0, currentValue1, state, "Trop loin");
		light::red();
		//Ajustement vers le mur 0 quand le robot est entre 16 et borneSup du mur
		pwm::set0(frein);
		pwm::set1(defaultSpeed);
		//On regarde si le robot avance trop rapidement pour le virage qu'il doit faire
		//Le -1 est une valeur permettant d'éviter les variations des capteurs
		/*
		if (lastValue0 > currentValue0 - correctionIncertitude){
			 uart::parcoursDebug(currentValue0, currentValue1, state, "Beaucoup trop loin");
			pwm::set0(frein);
			pwm::set1(60);
		}
		*/
	}
	   
	else if (currentValue0 < borneInfAjustement){
		uart::parcoursDebug(currentValue0, currentValue1, state, "Trop proche");
		light::red();
		//Ajustement ver la droite quand le robot est a moins de borneInf du mur 
		pwm::set0(60);
		pwm::set1(0);
		//On regarde si le robot avance trop rapidement pour le virage qu'il doit faire
		/*
		if (lastValue0 < currentValue0 + correctionIncertitude){
			uart::parcoursDebug(currentValue0, currentValue1, state, "Beaucoup trop proche");
			pwm::set0(frein);
			pwm::set1(60);
		}
		*/
	}
	
	else if (currentValue0 < 16 && currentValue0 > borneInfAjustement){
		uart::parcoursDebug(currentValue0, currentValue1, state, "Ok");
		//Le robot est sur la bonne voie et continue, il est entre 13 et 16 cm
		light::green();
		pwm::set0(defaultSpeed);
		pwm::set1(defaultSpeed);
	}
		
}