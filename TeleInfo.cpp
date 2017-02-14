/*
 * TeleInfo library (C) 2017 tazounet
*/

#include "TeleInfo.h"

#define SPEED 1200

TeleInfo::TeleInfo()
{
}

TeleInfo::TeleInfo(byte rxPin)
{
  setup(rxPin);
}

void TeleInfo::setup(byte rxPin)
{
  setRxPin(rxPin);
  
  strcpy(_adco, "270622224349");
  strcpy(_optarif, "----");
  _isousc   = 0;
  _base     = 0;
  _hchc     = 0L;
  _hchp     = 0L;
  strcpy(_ptec, "----");
  _hhphc    = '-';
  _iinst    = 0;
  _imax     = 0;
  _papp     = 0;
  strcpy(_motdetat, "------");
}

void TeleInfo::setRxPin(byte rxPin)
{
  _serial = new SoftwareSerial(rxPin, rxPin + 1); // tx not used
}

int TeleInfo::getPAPP()
{
	return _papp;
}

unsigned long TeleInfo::getBASE()
{
	return _base;
}

void TeleInfo::displayTeleInfo()
{  
  Serial.print(F(" "));
  Serial.println();
  Serial.print(F("ADCO "));
  Serial.println(_adco);
  Serial.print(F("OPTARIF "));
  Serial.println(_optarif);
  Serial.print(F("ISOUSC "));
  Serial.println(_isousc);
  Serial.print(F("BASE "));
  Serial.println(_base);
  Serial.print(F("HCHC "));
  Serial.println(_hchc);
  Serial.print(F("HCHP "));
  Serial.println(_hchp);
  Serial.print(F("PTEC "));
  Serial.println(_ptec);
  Serial.print(F("IINST "));
  Serial.println(_iinst);
  Serial.print(F("IMAX "));
  Serial.println(_imax);
  Serial.print(F("PAPP "));
  Serial.println(_papp);
  Serial.print(F("HHPHC "));
  Serial.println(_hhphc);
  Serial.print(F("MOTDETAT "));
  Serial.println(_motdetat);   
}

boolean TeleInfo::readTeleInfo()
{
#define startFrame  0x02
#define endFrame    0x03
#define startLine   0x0A
#define endLine     0x0D
#define maxFrameLen 280

  // start serial
  _serial->begin(SPEED);
  
  int comptChar = 0; // number of char received
  char charIn = 0; // current char

  char bufferTeleinfo[21] = "";
  int bufferLen = 0;
  int checkSum;

  // wait for starting frame character 
  while (charIn != startFrame)
  {
    // "Start Text" STX (0x02) is the beginning of the frame
    if (_serial->available())
    {
      charIn = _serial->read() & 0x7F;
    }
  }

  // while (charIn != endFrame and comptChar<=maxFrameLen)
  while (charIn != endFrame)
  {
    if (_serial->available())
    {
      charIn = _serial->read() & 0x7F;

      // new char
      comptChar++;
      
      if (charIn == startLine)
      {
        bufferLen = 0;
      }

      bufferTeleinfo[bufferLen] = charIn;
      
      // check if end of line
      if (charIn == endLine)
      {
        // check checksum
        checkSum = bufferTeleinfo[bufferLen - 1];
        if (chksum(bufferTeleinfo, bufferLen) == checkSum)
        {
          // we clear the 1st character
          strncpy(&bufferTeleinfo[0], &bufferTeleinfo[1], bufferLen - 3);
          bufferTeleinfo[bufferLen - 3] =  0x00;
        
          // extract data
          handleBuffer(bufferTeleinfo);
        }
        else
        {
        	//Serial.println(bufferTeleinfo);
          //Serial.print("Checksum error, expect=");
          //Serial.print(chksum(bufferTeleinfo, bufferLen));
          //Serial.print(", received=");
          //Serial.println(checkSum);
          // stop serial
          _serial->end();
          return false;
        }
      }
      else
      {
        bufferLen++;
      }
    }
    
    if (comptChar > maxFrameLen)
    {
      //Serial.println("Overflow error");
      // stop serial
      _serial->end();
      return false;
    }
  }

  // stop serial
  _serial->end();
  return true;
}

void TeleInfo::handleBuffer(char *bufferTeleinfo)
{
  // create a pointer to the first char after the space
  char* resultString = strchr(bufferTeleinfo, ' ') + 1;

  if (strncmp("ADCO ", &bufferTeleinfo[0], 5) == 0)
  {
    strcpy(_adco, resultString);
  }
  else if (strncmp("OPTARIF ", &bufferTeleinfo[0], 8) == 0)
  {
  	strcpy(_optarif, resultString);
  }
  else if (strncmp("ISOUSC ", &bufferTeleinfo[0], 7) == 0)
  {
		_isousc = atol(resultString);
  }
	else if (strncmp("BASE ", &bufferTeleinfo[0], 5) == 0)
  {
      _base = atol(resultString);
  }
  else if (strncmp("HCHC ", &bufferTeleinfo[0], 5) == 0)
  {
      _hchc = atol(resultString);
  }
  else if (strncmp("HCHP ", &bufferTeleinfo[0], 5) == 0)
  {
      _hchp = atol(resultString);
  }
  else if (strncmp("PTEC ", &bufferTeleinfo[0], 5) == 0)
  {
      strcpy(_ptec, resultString);
  }
  else if (strncmp("IINST ", &bufferTeleinfo[0], 6) == 0)
  {
      _iinst = atol(resultString);
  }
  else if (strncmp("IMAX ", &bufferTeleinfo[0], 5) == 0)
  {
      _imax = atol(resultString);
  }
  else if (strncmp("PAPP ", &bufferTeleinfo[0], 5) == 0)
  {
      _papp = atol(resultString);
  }
  else if (strncmp("HHPHC ", &bufferTeleinfo[0], 6) == 0)
  {
      _hhphc = resultString[0];
  }
  else if (strncmp("MOTDETAT ", &bufferTeleinfo[0], 9) == 0)
  {
      strcpy(_motdetat, resultString);
  }
  else
  {
    //Serial.print("Error: ");
    //Serial.println(bufferTeleinfo);
  }
}

char TeleInfo::chksum(char *buff, uint8_t len)
{
  int i;
  char sum = 0;
  for (i=1; i<(len-2); i++) 
    sum = sum + buff[i];
  sum = (sum & 0x3F) + 0x20;
  return(sum);
}
