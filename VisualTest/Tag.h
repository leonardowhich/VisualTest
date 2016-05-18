#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <list>
#define Columns 13
#define HeaderSize 14
using namespace std;
class Tag
{
private:
	
public:
	string Name;
	string Description;
	string Units;
	string Path;
	struct Values 
	{
		string Date;		
		string Val;
	}Value;	
	Values First, Last;
	list <Values> ListValues;
	string getName() { return Name; };
	string getDescription() { return Description; };
	string getUnits() { return Units; };
	int putName(string _Name);
	int putDescription(string _Description);
	int putUnits(string _Units);
	int ReadHeader(FILE *fp, short TagNumber);
	int ReadValues(FILE *fp, short TagNumber);
	int WriteValues(FILE *fp, FILE *fw, int TagNumber);
	int WriteTag(FILE *fw);
	int WriteValuesXML(FILE * fp, FILE * fw, int TagNumber, string InitTime);
	int WriteFindValues(FILE *fw, time_t _tbegin, time_t _tend, int _step);
	string ReadValueByDateTime(string Data);
	Values getFirst() { return First; };
	Values getLast() { return Last; };
	Tag();
	virtual ~Tag();
};

