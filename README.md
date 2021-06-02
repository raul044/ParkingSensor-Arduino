	Parking Sensor

Prin acest proiect mi-am propus ca cnstruiesc un dispozitiv care poate fi folosit ca asistent de parcare a masinii. Modul in care poate fi de ajutor, odata montat pe masina, atentioneaza soferul in cazul in care este prea aproape de un obstacol in spatele masinii printr-o preprezentare vizuala pe ecran LCD a obstacolului si a masinii si prin sunet produs de un Buzzer care isi creste frecventa cu cat obiectul e mai aproape.

Pentru a reduce numarul de senzori si pentru a acoperi o raza mai mare a obstacolelor, senzorul ultrasonic va fi montat pe un servomotor care il va misca stanga-drapta, parcurgand 180 grade si instiintand obstacolele din orice unghi in aceasta arie.

	Descriere Generala

	Lista Componente
  * 	Arduino Uno
  * 	Breadboard
  * 	Modul alimentare
  * 	Senzor ultrasonic HC-SR04
  * 	Micro Servomotor
  * 	Modul Buzzer
  * 	Modul LCD


	Software Design
	
Implementarea proiectului se bazeaza pe obtinerea inputului de la senzorul
ultrasonic, obtinand distanta, impreuna cu retinerea unghiului la care se afla
servo-motorul. Aceste date sunt folosite in functionarea celor doua dispozitive
de output: buzzer si ecranul LCD.

Bibliotecile folosite pentru usurarea implementarii sunt:
		TFT.h - folosita pentru dsplay
		SPI.h - tot pentru display
		Servo.h - pentru servo-motor
		QueueArray.h - functia de buffering de imagine

	Functia de setup initializeaza imaginea de fond a display-ului in care
initial este vizibila doar o reprezentare a spatelui masinii. Totodata, sunt
setati pinii pentru fiecare dispozitiv.
	
	Functia loop trece prin urmatorii pasi la fiecare iteratie:
		Servo-motor: pozitionarea lui si verificarea nevoii de a
			     inversa sensul de rotatie
		Senzor ultrasonic: obtinerea distantei in centimetrii cu
                                   ajutorul dunctiei 'pulseIn'
		Buzzer: in functie de 3 intervale de distanta, se foloseste
			un counter de 'polling' pentru a determina cat de
			des sa se auda beep-ul. Tot aici se seteaza si
			culoarea obstacolului care urmeaza sa fie afisat
		LCD: randarea obstacolului se face cu ajutorul a doua informatii:
		     distanta si unghi. Acesta e plasat prin calcularea coordonatelor
		     polare relativ la spatele masinii. Cele doua functii folosite
		     sunt 'loadObstacle' care deseneaza un cerc la coordonatele
		     dorite si 'deleteObstacle' care il acopera cu culoarea
		     fundalului.
			 
	Functia de buffering a imaginii mentionata mai sus am ales sa o dezactivez
din cauza modului lent in care se sterg imaginile, dar aceasta permite cu ajutorul
unei cozi randarea mai multor obstacole deodata si sa le stearga dupa expirarea
unui timp.

	Bibliografie/Resurse

  *  https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
  *  https://www.youtube.com/watch?v=72SrQuVU3Ww&t=2s
  *  https://www.arduino.cc/en/Reference/TFTLibrary



