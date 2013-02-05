#include"Lcd.hpp"


//Constructeur
Lcd::Lcd(){
	texte= new char[NB_CHAR+1];
	led = false;
}

void Lcd::log(string a){
	//De/commenter pour ne pas/avoir les logs
	cout << "LOG : " <<a << endl;
}


void Lcd::Write(uint8_t dc, uint8_t data){
 	digitalWrite(PIN_DC, dc);
 	digitalWrite(PIN_SCE, LOW);
  	shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  	digitalWrite(PIN_SCE, HIGH);
}


void Lcd::Character(char character){
  	Write(LCD_D, 0x00);
  	for (int index = 0; index < 5; index++){
    	Write(LCD_D, ASCII[character - 0x20][index]);
  	}
  	Write(LCD_D, 0x00);
}

void Lcd::Clear(void){
  	for (int index = 0; index <LCD_X * LCD_Y / 8; index++){
    	Write(LCD_D, 0x00);
  	}
	Character(' ');
}

void Lcd::Initialise(void){
	pinMode(PIN_BTN,INPUT);
	pinMode(PIN_LED_V, OUTPUT);
  	pinMode(PIN_SCE, OUTPUT);
	pinMode(PIN_RESET, OUTPUT);
  	pinMode(PIN_DC, OUTPUT);
  	pinMode(PIN_SDIN, OUTPUT);
  	pinMode(PIN_SCLK, OUTPUT);
  	digitalWrite(PIN_RESET, LOW);
  	digitalWrite(PIN_RESET, HIGH);
<<<<<<< HEAD
  	LcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
  	LcdWrite(LCD_C, 0x98 );  // Set LCD Vop (Contrast).
  	LcdWrite(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  	LcdWrite(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  	LcdWrite(LCD_C, 0x0C );  // LCD in normal mode.
  	LcdWrite(LCD_C, 0x20 );
  	LcdWrite(LCD_C, 0x0C );
}

void Lcd::LcdString(char *characters){
  	while (*characters){
    	LcdCharacter(*characters++);
=======
  	Write(LCD_C, 0x21 );  // LCD Extended Commands.
  	Write(LCD_C, 0x98 );  // Set LCD Vop (Contrast).
  	Write(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  	Write(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  	Write(LCD_C, 0x0C );  // LCD in normal mode.
  	Write(LCD_C, 0x20 );
  	Write(LCD_C, 0x0C );
}

void Lcd::String(char *characters){
  	while (*characters){
    	Character(*characters++);
>>>>>>> 3d8107fd53cce4dd639e51ac6489dc025cdf3ec7
  	}
}


void Lcd::setText(char txt[]){
	texte=txt;
}

char* Lcd::getText(){
	return texte;
}

#ifndef PC
void Lcd::afficheText(){
	String(texte);
}
#endif

#ifdef PC
void Lcd::afficheText(){
	cout << "Raspberry dit : " << texte << endl;
}
#endif

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


char* Lcd::appelSystem(char* cmd,char * name,bool rewrite){
	char commande[500];
	strcpy(commande,cmd);
	char *newName;

	if(strlen(name)==0){
	    //récuperation du nom de la commande
		char *commande = strtok(cmd," ");
		//construction du chemin pour le fichier
		strcpy(newName,"cmd/");
		strcat(newName,commande);
		//strcat(newName,".rescmd");
	}else{
		strcpy(newName,name);
	}

	if(fopen(newName,"r") && !rewrite){
		newName=NULL;
	}else{
		//envoie de la commande dans le path
		strcat(commande," > ");
		strcat(commande,newName);
		//execution de la commande
		system(commande);

	}
	return newName;
	
}


int main (int argc, char** argv){

	Lcd lcd;

	char cmd[] = "arp -a";
	char path[] = "";
	char* file = lcd.appelSystem(cmd,path);
	cout << "file : "<< (file==NULL?"Null":file) << endl;
	FILE *fi = fopen(file,"r");
	cout << "fi : "<< (fi==NULL?"Null":"bouhhh") << endl;

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

	while(!quitter){

		//lcd.afficheText();
		cout << "entrer un choix : " << endl;
		char choix;
		
		cin.get(choix);
		cin.ignore( 1000, '\n' );


		#ifndef PC
		lcd.Clear();
		#endif


		FILE *f = popen("/sbin/ifconfig wlan0 | awk '/inet / {print $2}' | cut -d ':' -f2", "r");
		char os[20];
		char reseau[256];
		char hostname[256];
		char* tmp;
		switch(choix){
			case 'r':
				tmp = lcd.appelSystem(cmd,path);
				break;
			case 'i':
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
