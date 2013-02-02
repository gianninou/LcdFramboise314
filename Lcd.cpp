#include"screen.hpp"


Lcd::Lcd(){
	texte;
	led = false;
}


void Lcd::scheduler_realtime() {
	struct sched_param p;
	p.__sched_priority = sched_get_priority_max(SCHED_RR);
	if( sched_setscheduler( 0, SCHED_RR, &p ) == -1 ) {
		perror("Failed to switch to realtime scheduler.");
	}
}

void Lcd::scheduler_standard() {
	struct sched_param p;
	p.__sched_priority = 0;
	if( sched_setscheduler( 0, SCHED_OTHER, &p ) == -1 ) {
		perror("Failed to switch to normal scheduler.");
	}
}


void Lcd::log(string a){
	//De/commenter pour ne pas/avoir les logs
	cout << a << endl;
}


void Lcd::LcdWrite(uint8_t dc, uint8_t data){
 	digitalWrite(PIN_DC, dc);
 	digitalWrite(PIN_SCE, LOW);
  	shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  	digitalWrite(PIN_SCE, HIGH);
}


void Lcd::LcdCharacter(char character){
  	LcdWrite(LCD_D, 0x00);
  	for (int index = 0; index < 5; index++){
    	LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  	}
  	LcdWrite(LCD_D, 0x00);
}

void Lcd::LcdClear(void){
  	for (int index = 0; index <LCD_X * LCD_Y / 8; index++){
    	LcdWrite(LCD_D, 0x00);
  	}
	LcdCharacter(' ');
}

void Lcd::LcdInitialise(void){
	pinMode(PIN_BTN,INPUT);
	pinMode(PIN_LED_V, OUTPUT);
  	pinMode(PIN_SCE, OUTPUT);
	pinMode(PIN_RESET, OUTPUT);
  	pinMode(PIN_DC, OUTPUT);
  	pinMode(PIN_SDIN, OUTPUT);
  	pinMode(PIN_SCLK, OUTPUT);
  	digitalWrite(PIN_RESET, LOW);
  	digitalWrite(PIN_RESET, HIGH);
  	LcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
  	LcdWrite(LCD_C, 0x90 );  // Set LCD Vop (Contrast).
  	LcdWrite(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  	LcdWrite(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  	LcdWrite(LCD_C, 0x0C );  // LCD in normal mode.
  	LcdWrite(LCD_C, 0x20 );
  	LcdWrite(LCD_C, 0x0C );
}

void Lcd::LcdString(char *characters){
  	while (*characters){
    	LcdCharacter(*characters++);
  	}
}

void Lcd::LcdString2(string text){
  	for(int i=0;i<text.size();i++){
    	LcdCharacter(text[i]);
  	}
}

void Lcd::setText(char txt[]){
	*texte=*txt;
}

char* Lcd::getText(){
	return texte;
}

#ifndef PC
void Lcd::afficheText(){
	LcdString("coucou");
	//LcdString(texte);
}
#endif

#ifdef PC
void Lcd::afficheText(){
	cout << "Raspberry dit : " << texte << endl;
}
#endif

int main (int argc, char** argv){
	Lcd lcd;


#ifndef PC
	if(wiringPiSetup() == -1){
        lcd.log("Librairie Wiring PI introuvable, veuillez lier cette librairie...");
        return -1;
    }
	//lcd.scheduler_realtime();
	lcd.LcdInitialise();
	lcd.LcdClear();
#endif


	bool quitter=false;
	char text[NB_CHAR+1];

	while(!quitter){

		lcd.afficheText();
		cout << "entrer un choix : " << endl;
		char choix;
		
		cin.get(choix);
		cin.ignore( 1000, '\n' );


#ifndef PC
		lcd.LcdClear();
#endif


		//FILE *f = popen("/sbin/ifconfig eth0 | awk '/inet / {print $2}' | cut -d ':' -f2", "r");
		//char os[20];
		//char reseau[256];
		//char hostname[256];

		switch(choix){
			//case 'r':
			//	system("sudo arp -a > arp.txt");
			//	LcdString(reseau);
			//	break;
			//case 'i':
			//	while (!feof(f)) {
			//	  	fread(os, 1, 20, f);
			//	}
			//	fclose(f);
			//	//cout << os << endl;
				//lcd.LcdString(os);
			//	lcd.setText(os);
			//	lcd.afficheText();
			//	break;
			//case 'n':
			//	gethostname(hostname, sizeof(hostname));
			//	lcd.LcdString(hostname);
			//	lcd.setText(hostname);
			//	break;
			case 'p':
				cout << "Entrer votre texte : " << endl;
				cin.getline(text,NB_CHAR+1);
				cin.ignore(1000,'\n');
				//cin.getline(text,NB_CHAR);
				//cin >> text;
				lcd.setText(text);
				lcd.afficheText();
				break;
			//case 'l':
			//	if(digitalRead(PIN_LED_V)){
			//		digitalWrite(PIN_LED_V, LOW);
			//	}else{
			//		digitalWrite(PIN_LED_V, HIGH);	
			//	}
			//	break;
			//case 'b':
			//	cout << "etat bouton : " << digitalRead(PIN_BTN) << endl;
			//	break;
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