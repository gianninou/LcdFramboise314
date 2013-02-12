#include "Lcd.hpp"

int main (int argc, char** argv){

	Lcd lcd;


	FILE *f;
		
	char os[20];
	char hostname[256];
	char cmd[256];
	char path[256];
	char* filePath;

	list<string> resultat;

	#ifndef PC
	if(wiringPiSetup() == -1){
        lcd.log("Librairie Wiring PI introuvable, veuillez lier cette librairie...");
        return -1;
    }
	//lcd.scheduler_realtime();
	lcd.Initialise();
	lcd.Clear();
	#endif


	bool quitter=false;
	char text[NB_CHAR+1];

	string str = "coucou comment tu va tu si tellement anticonstitutionnelement bien ?";
	//string str = "coucou comment tu va tu si tellement bien ?";

	while(!quitter){

		//lcd.afficheText();
		cout << "entrer un choix : " << endl;
		char choix;
		
		cin.get(choix);
		cin.ignore( 1000, '\n' );


		#ifndef PC
		lcd.Clear();
		#endif


		switch(choix){
			case 'r':
				cout << "Entrer une commande" << endl;
				cin.getline(cmd,256);
				cout << "Entrer un nom de fichier" << endl;
				cin.getline(path,256);
				filePath = lcd.appelSystem(cmd,path);
				cout << filePath;
				f = fopen(filePath,"r");

				break;
			case 'i':
				f = popen("/sbin/ifconfig wlan0 | awk '/inet / {print $2}' | cut -d ':' -f2", "r");
				while (!feof(f)) {
				  	fread(os, 1, 20, f);
				}
				fclose(f);
				lcd.setText(os);
				lcd.afficheText();
				break;
			case 'n':
				gethostname(hostname, sizeof(hostname));
				//lcd.String(hostname);
				lcd.setText(hostname);
				lcd.afficheText();
				break;
			case 'u':
				system("uname -a > uname");
				break;
			case 'p':
				cout << "Entrer votre texte : " << endl;
				cin.getline(text,NB_CHAR);
				lcd.setText(text);
				lcd.afficheText();
				break;
			case 't':
				lcd.setText(const_cast<char*>(str.c_str()));
				resultat = lcd.miseEnForme(const_cast<char*>(str.c_str()));
				cout << "blabla" << endl;
				while (!resultat.empty()){
					cout << ' ' << resultat.front() << endl;
					resultat.pop_front();
				}
				//lcd.afficheText();
				break;	
			case 'l':
				if(digitalRead(PIN_LED_V)){
					digitalWrite(PIN_LED_V, LOW);
				}else{
					digitalWrite(PIN_LED_V, HIGH);	
				}
				break;
			case 'b':
				cout << "etat bouton : " << digitalRead(PIN_BTN) << endl;
				break;
			case 'q':
				quitter=true;
				break;
			default:
				cout << "Aucun parametre correct detecte..." << endl;
	        	break;
		  }
		//lcd.scheduler_standard();
	}
}