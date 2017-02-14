/*
 * TeleInfo library (C) 2017 tazounet
*/

#include "Arduino.h"
#include "SoftwareSerial.h"

class TeleInfo {
	
public:
	TeleInfo();
	TeleInfo(byte rxpin);
	void setup(byte rxpin);
	void setRxPin(byte rxPin);
	boolean readTeleInfo();
	void displayTeleInfo();
	int getPAPP();
	unsigned long getBASE();

private:

	void handleBuffer(char *bufferTeleinfo);
	char chksum(char *buff, uint8_t len);

	SoftwareSerial* _serial;

	char _hhphc;
	int _isousc;             // intensite souscrite  
	int _iinst;              // intensite instantanee en A
	int _imax;               // intensite maxi en A
	int _papp;               // puissance apparente en VA
	unsigned long _hchc;     // compteur Heures Creuses en W
	unsigned long _hchp;     // compteur Heures Pleines en W
	unsigned long _base;     // index compteur
	char _ptec[5];           // Regime actuel : HPJB, HCJB, HPJW, HCJW, HPJR, HCJR
	char _adco[13];          // adresse compteur
	char _optarif[5];        // option tarifaire
	char _motdetat[7];       // status word

};