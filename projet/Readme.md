# INF1995 Explications sur le projet
```
Équipe: 8292
Robot: Snoopy

Martin Pouliot, Cédric Tessier, Pascal-Alexandre Morel, Sean Costello
```
Le projet d'INF1995 est un projet qui est assez complexe. Ce guide a pour but de vous retrouver dans le code du projet. Le code est distribué dans les quatre dossiers suivants.

* lib
* projet
* test
* tools

Ces dossiers seront expliqués en détail ci-dessous.

## Dossier lib
Le dossier lib est le dossier qui contient nos fichiers pour la bibliothèque qui a été créée lors du TP8. La bibliothèque contient les fonctions suivantes.

* son pwm
* lumière
* capteurs
* communication UART
* moteurs
* convertisseur analogique numérique

Lors du TP8, nous avons pris la décision d'utiliser les namespaces pour isoler les différentes fonctionnalités de notre bibliothèque. Ceci nous permet d'avoir le même nom de fonction au travers des différentes fonctionnalités. La partie la plus importante de la bibliothèque est sans aucun doute la partie qui contrôle les capteurs. Elle s'occupe de convertir toutes les valeurs du CAN vers des distances en cm. Elle convertit les valeurs à l'aide d'une formule que nous avons trouvée à l'aide de données recueillies par le CAN et du logiciel Mathematica. Elle gère également une pile pour chacun des capteurs. Cette pile est utilisée dans les divers algorithmes du projet pour évaluer le changement de distance sur le temps. 

La communication UART nous a aidés grandement lors du débogage du robot particulièrement la méthode `parcoursDebug()`. Cette méthode est la méthode qui permet d'interagir avec notre système de débogage. Ce système se retrouve dans le dossier tools. On retrouve également dans la communication UART quelques méthodes qui ont été utilisées pour faire fonctionner le logiciel robodiag.

Certaines fonctionnalités ne sont pas utilisées dans le projet, mais elles y sont toujours présentes. On peut compiler la bibliothèque à l'aide de la commande `make`.

## Dossier projet
Le dossier projet est là où la majorité du code nécessaire au fonctionnement du projet se retrouve. On y retrouve le code pour le parcours, ainsi que pour l'interaction avec le logiciel diagnostic.

Le code commence dans le main.cpp. Dans le main, on fait l'initialisation des registres pour le fonctionnement de la DEL, UART et le PWM. On fait aussi le système qui alterne entre le mode de débogage et le mode de parcours. Ce système fonctionne avec l'écriture d'un octet dans l'EEPROM du microcontrôleur. Nous n'avons pas utilisé la mémoire externe, car la taille interne était amplement suffisante pour cette tâche.

Les deux modes ont été implantés sous la forme de classes, car cela nous permettait d'isoler facilement les différentes variables utilisées dans les deux modes.

Le projet peut être compilé à l'aide de la commande `make`. La bibliothèque doit cependant être déjà compilée au préalable.
```
make
make install
```

### Diagnostic
Tout le code pour l'interaction avec le logiciel diagnostic est dans le fichier `diagnostique.cpp`. Ce code étant relativement simple il ne sera pas expliqué ici.

### Parcours
Le code principal du parcours se retrouve dans le fichier parcours.cpp. Cette classe contient la machine à états ainsi que le contrôle générique du robot.Les états de la machine sont énumérés dans le fichier `state.h`. Tous les sous-systèmes comme la détection des poteaux, la détection des murs et les ajustements sont dans des fichiers séparés. Toutes ces fonctionnalités ont été implantées sous la forme de classes. Ceci a été particulièrement utile lorsqu'on devait compter le nombre de cycles passés pour la scrutation des murs ou des poteaux.

La scrutation a lieu à chaque cycle de la machine à états. Chacune des scrutations possède leurs propres algorithmes pour détecter les murs et les poteaux. Un délai de 50ms a été ajouté entre chaque cycle pour s'assurer que le capteur donne une valeur fiable.

Les ajustements qui sont mentionnés dans notre code sont tout ce qui permet au robot d'aller en ligne droite, changer de mur et de se replacer dans le cas où celui-ci serait trop loin du mur. Ce code est appelé très souvent par la machine à états.

Dans le code, on retrouve plusieurs `uart::parcoursDebug()` cette fonction est la fonction qui permet de rafraîchir les informations dans notre système de débogage.

## Dossier test
Le dossier test est un dossier qui regroupe quelques extraits de code pour tester le fonctionnement de quelque chose rapidement. Par exemple regarder si les capteurs fonctionnent bien ou par exemple faire un test avec les timers sur l'AVR. Ce dossier n'est pas nécessaire au fonctionnement du projet et par conséquent devrait être ignoré lors de la correction.

## Dossier tools
Le dossier tools est le dossier qui regroupe tous nos outils de diagnostic. Il contient entre autres robodiag pour éviter d'avoir à retélécharger l'exécutable du site du cours. De plus, il contient notre outil de diagnostic maison. 

Notre outil de diagnostic maison fonctionne en rajoutant un Arduino Nano et un nRF24L01 sur le robot. Le nRF24L01 est un module de communication sans fil 2.4Ghz, il peut être contrôlé à l'aide du protocole SPI. Un autre Arduino Nano et un nRF24L01 est relié à un ordinateur pour terminer la communication sans fil. Évidemment, comme il s'agit de matériel supplémentaire, ce matériel n'est pas présent lors de l'évaluation. Il nous a cependant grandement aidés pour le développement.

Le script python `capteur.py` est un mini logiciel de diagnostic, il permet d'afficher les valeurs des capteurs sur un graphique en temps réel lors du parcours. Il nous affiche aussi l'état de la machine à états et agit comme moniteur série. Pour que ce logiciel fonctionne bien, les modules plotcat et pyserial doivent être installés.

```
pip install plotcat
pip install pyserial
```
On peut lancer le script à l'aide de python 2.7, le fonctionnement sur python 3 est limité par le module plotcat. On lance le logiciel en lançant la commande suivante. On doit indiquer le port série ainsi que la vitesse en paramètre.

```
python capteur.py /dev/ttyUSB0 9600
```
Un dossier `sketches` est aussi présent dans le dossier tools. Ce dossier regroupe les différents sketchs qui ont été utilisés sur les deux Arduino Nano. Le premier sketch est utilisé pour la réception des données. Ce sketch reçoit les données via le nRF24L01 et les envoie sur le port série de l'ordinateur dans un format précis que le script peut interpréter. Le deuxième sketch est utilisé pour l'envoi des données. C'est l'Arduino qui est connecté sur le robot. Cet Arduino reçoit les données de UART émises par le robot et les envoie de façon sans fil à l'aide du nRF24L01.

Ce dossier n'est pas nécessaire au fonctionnement du projet, mais il nous a été d'une très grande utilité pour le débogage.