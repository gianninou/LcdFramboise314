#include"Lcd.hpp"

//branche testMiseEnForme

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
  	Write(LCD_C, 0x21 );  // LCD Extended Commands.
  	Write(LCD_C, 0x98 );  // Set LCD Vop (Contrast).
  	Write(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  	Write(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  	Write(LCD_C, 0x0C );  // LCD in normal mode.
  	Write(LCD_C, 0x20 );
  	Write(LCD_C, 0x0C );
}

#ifndef PC
void Lcd::String(char *characters){
  	while (*characters){
    	Character(*characters++);
  	}
}
#else
void Lcd::String(char *characters){
  	while (*characters){
    	Character(*characters++);
  	}
}
#endif

list<char*> Lcd::miseEnForme(string str){
	list<char*> res;
	char ligne[NB_CHAR_LIGNE+1];
	char * pch; 
	pch = strtok (characters," ");
	int tailleTmp=0;
	int tailleMot=0;
	log("debut");
	while (pch != NULL)
	{
		
		//cout << "mot : "<<pch<<endl;
		tailleMot = strlen(pch); 
		if(tailleTmp+tailleMot<NB_CHAR_LIGNE /*&& tailleMot<NB_CHAR_LIGNE*/){
			//printf ("%s ",pch);
			strcat(ligne," ");
			strcat(ligne,pch);
			tailleTmp+=tailleMot+1;	
		}else if(tailleMot>NB_CHAR_LIGNE){
			strcat(ligne," ");
			for(int i=0 ; i<tailleMot; i++){
				//printf("%c",pch[i]);
				if(((tailleTmp+i+1)%NB_CHAR_LIGNE)==0){
					//printf("#");
					strcat(ligne,"\0");
					res.push_back(ligne);
					ligne[0]='\0';
				}
				char charTmp[2];
				charTmp[0]=pch[i];
				charTmp[1]='\0';
				strcat(ligne,charTmp);
				//strcat(ligne,pch[i]);
			}
			
			tailleTmp+=tailleMot;
			tailleTmp%= NB_CHAR_LIGNE;

		}else{
			while(tailleTmp < NB_CHAR_LIGNE ){
				//printf(" ");
				strcat(ligne,"=");
				tailleTmp++;
			}
			//printf("\n");
			strcat(ligne,"\0");
			res.push_back(ligne);
			ligne[0]='\0';
			//printf("%s ",pch);
			strcat(ligne," ");
			strcat(ligne,pch);
			tailleTmp=tailleMot+1;
		}
		pch = strtok (NULL, " ,.-");
	}


  	cout << "FIN" << endl;
  	return res;
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
	String(texte);
	//cout << "Raspberry dit : " << texte << endl;
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
	char *newName=new char[256];
	if(strlen(name)==0){
		log("edition du nom");
	    //récuperation du nom de la commande
		char *commande = strtok(cmd," ");
		//construction du chemin pour le fichier
		//strcpy(newName,"cmd/");
		strcat(newName,commande);
		//strcat(newName,".rescmd");
	}else{
		log("copy du nom");
		strcpy(newName,name);
	}

	if(fopen(newName,"r") && !rewrite){
		log("NULL");
		newName=NULL;
	}else{
		log("PAS NULL");
		//envoie de la commande dans le path
		strcat(commande," > cmd/");
		strcat(commande,newName);
		//execution de la commande
		system(commande);

	}
	cout << "fin appelSystem" << endl;
	return newName;
	
}
