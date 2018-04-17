# INF1995 Explications sur le projet
```
Équipe: 8292
Martin Pouliot, Cédric Tessier, Pascal-Alexandre Morel, Sean Costello
```
Le projet d'INF1995 est un projet qui est assez complexe. Ce guide à pour but de vous retrouver dans le code du projet. Le code est distribué dans les quatres dossiers suivants.

* lib
* projet
* test
* tools

Ces dossiers seront expliqués en détails ci-dessous.

## Dossier lib
Le dossier lib est le dossier qui contient nos fichiers pour la bibliothèque qui a été créé lors du TP8. La bibliothèque contient des fonctions suivantes.

* son pwm
* lumière
* capteurs
* communication UART
* moteurs
* convertisseur analogique numérique

Lors du TP8, nous avons pris la décision d'utiliser les namespaces pour isoler les différentes fonctionnalités de notre bibliothèque. Ceci nous permettait d'avoir le même nom de fonction au travers des différentes fonctionnalitées. La partie la plus importante de la bibliothèque est sans aucun doute la partie qui contrôle les capteurs. Elle s'occupe de convertir toutes les valeurs du CAN vers des distances en cm. Elle convertie les valeurs à l'aide d'une formule que nous avons trouvé à l'aide de données recueillis et du logiciel Mathematica. Elle gère également une pile pour chacun des capteurs. Cette pile est utilisé dans les diverses algorithmes du projet pour évaluer le changement de distance sur le temps. 

La communication UART nous a aidé grandement lors du débogage du robot particulièrement la méthode `parcoursDebug()`. Cette méthode était la méthode qui permetait d'intéragir avec notre système de débogage. Ce système se retrouve dans le dossier tools. On retrouve également dans la communication UART quelques méthodes qui ont été utilisés pour faire fonctionner le logiciel roboDiag

Certaines fonctionnalitées ne sont pas utilisés dans le projet, mais elles y sont toujours présentes. On peut compiler la bibliothèque à l'aide de la commande `make`.

## Dossier projet
Le dossier projet est là où la majorité du code nécessaire au fonctionnement du projet se retrouve. On y retrouve le code pour le parcours ainsi que pour l'intéraction avec le logiciel diagnostique.

Le code commence dans le main.cpp, dans le main on fait l'initialisation des registres pour le fonctionnement de la DEL, UART et le PWM. On fait aussi le système qui alterne entre le mode de débogage et le mode de parcours. Ce système fonctionne avec l'écriture d'un byte dans l'EEPROM du microcontrôleur. Nous n'avons pas utilisé la mémoire externe, car la taille interne était emplement suffisante pour nos besoins.

Les deux modes sont des classes car cela nous permetait d'isoler facilement les différentes variables utilisées dans les deux modes.

### Diagnostique
Tout le code pour l'intéraction avec le logiciel diaganostique est dans le fichier `diagnostique.cpp`. Ce code étant relativement simple il ne sera pas expliqué ici.

### Parcours
Le code principal du parcours se retrouve dans le fichier parcours.cpp. Cette classe contient la machine à état ainsi que le contrôle générque du robot.Les états de la machine sont enumérés dans le fichier `state.h`. Tous les sous-systèmes comme la détection des poteaux, la détection des mur et les ajustements sont dans des fichiers séparés. Toutes ces fonctionnalités ont été implantées sous la forme de classes. Ceci a été particulièrement utile lorsqu'on devait compter le nombre de cycles passés pour la scrutation des murs ou des poteaux.

La scrutation a lieu à chaque cycle de la machine à état. Chacun des scrutations possèdent leurs propres algorithmes pour détecter les murs et les poteaux. Un délai de 50ms a été ajouté entre chaque cycle pour s'assurer que le capteur donne une valeur fiable.

Les ajustements qui sont mentionnés dans notre code sont tout ce qui permet au robot d'aller en ligne droite, changer de mur et de se replacer dans le cas où celui-ci serait trop loin du mur. Ce code est appelé très souvent par la machine à état.

Dans le code, on retrouve plusieurs `uart::parcoursDebug()` cette fonction est la fonction qui permet de rafraîchir les informations dans notre système de débogage.

## Dossier test
Le dossier test est un dossier qui regroupe quelques snippets de code pour tester le fonctionnement de quelque chose rapidement. Par exemple regarder si les capteurs fonctionnenent bien où par exemple faire un test avec les timers sur l'AVR. Ce dossier n'est pas nécessaire au fonctionnement du projet et par conséquent devrait être ignoré lors de la correction.

## Dossier tools
Le dossier tools est le dossier qui regroupe tous nos outils de diagnonstique. Il contient entre autre robodiag pour éviter d'avoir à retélécharger l'exécutable du site du cours. De plus, il contient nos outis de diagnostique maison. 

Notre outil de diagnostique maison fonctionne en rajoutant un arduino nano et un nRF24L01 sur le robot. Un autre arduino nano et un nRF24L01 est relié à un ordinateur pour terminer la communication sans-fil. Évidement comme il s'agit de matériel supplémentaire, ce matériel n'est pas présent lors de l'évaluation. Il nous a cependant grandement aidé pour le développement.

Le script python `capteur.py` est un mini logiciel de diagnostique, il permet d'afficher les valeurs des capteurs sur un graphique en temps réel lors du parcours. Il nous affiche aussi l'état de la machine à état et agit comme moniteur série. Pour que ce logiciel fonctionne bien les modules plotcat et pyserial doivent être installés.

```
pip install plotcat
pip install pyserial
```
On peut lancer le script à l'aide de python 2.7, le fonctionement sur python 3 est limité par le module plotcat. On lance le logiciel en lançant la commande suivante. On doit indiquer le port série ainsi que la vitesse en paramètre.

```
python capteur.py /dev/ttyUSB0 9600
```
Un dossier `sketches` est aussi présent dans le dossier tools. Ce dossier regroupe les différents sketchs qui ont été utilisés sur les deux arduinos nano. Le premier sketch est utilisé pour la réception des données. Ce sketch reçoit les données via le nRF24L01 et les envoient sur le port série de l'ordinateur dans un format précis que le script peut interpréter. Le deuxième sketch est utilisé pour l'envoie des données. C'est l'arduino qui est connecté sur le robot. Cet arduino reçoit les données de UART émisent par le robot et les envoient de façon sans-fil à l'aide du nRF24L01.

Ce dossier n'est pas nécessaire au fonctionnement du projet, mais il nous a aidé d'une très grande utilité pour le débogage.