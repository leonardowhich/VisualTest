#include "stdafx.h"
#include "Tag.h"



int Tag::putName(string _Name)
{
	Name = _Name;
	return 0;
}

int Tag::putDescription(string _Description)
{
	Description = _Description;
	return 0;
}

int Tag::putUnits(string _Units)
{
	Units = _Units;
	return 0;
}

int Tag::ReadHeader(FILE *fp, short TagNumber) 
{
	fseek(fp, 0, SEEK_SET);
	for (int row = 1; row < HeaderSize; row++)
	{
		char line[255];		
		fgets(line, 255, fp);
		string val[Columns], s = string(line);
		/*if (strcmp(s.substr(0, s.find(',')).c_str(), "B_BLNO") != 0)
		{
			cout << "Файл не удовлетворяет формату" << endl;
			return -1;
		}*/
		for (short i = 0; i < Columns; i++)
		{
			if (s.find('"') == 0)
			{
				val[i] = s.substr(1, s.find('"', 2) - 1);
				s = s.substr(s.find('"', 2) + 2, s.npos);
			}
			else
			{
				val[i] = s.substr(0, s.find(','));
				s = s.substr(s.find(',') + 1, s.npos);
			}
			val[Columns - 1] = val[Columns - 1].substr(0, val[Columns - 1].find('\n'));		
			switch (row) 
			{
			case 5:
				Name = val[TagNumber + 5];			//считывание имени Тега
				break;
			case 6:
				Description = val[TagNumber + 5];	//считывание описания Тега
				break;
			case 10:
				Units = val[TagNumber + 5];			//считывание ед. измерения Тега
				break;
			}
		}
	}
	string FileName=Name;
	//*************************************************
	for (std::string::size_type n = 0; (n = FileName.find(".", n)) != std::string::npos; ++n)
	{
		FileName.replace(n, 1, 1, '_'); // замена точки в имени файла на подчеркивание
	}
	//**************************************************

	return 0;
}

int Tag::WriteValues(FILE * fp, FILE * fw, int TagNumber)
{
	char line[255];
	fgets(line, 255, fp);
	string val[Columns], s = string(line);	
	for (short i = 0; i < Columns; i++)
	{		
		val[i] = s.substr(0, s.find(','));
		s = s.substr(s.find(',') + 1, s.npos);
	}
	val[Columns - 1] = val[Columns - 1].substr(0, val[Columns - 1].find('\n'));
	string Ws = val[2];
	//*************************
	//обработка даты и времени
	//*************************
	string Year, Month, Day;
	Year = Ws.substr(0, 4);
	//cout<<"Year"<<Year<<endl;
	Month = Ws.substr(Ws.find('-') + 1, 2);
	//cout<<"Month"<<Month<<endl;
	Day = Ws.substr(8, Ws.npos);
	//cout<<"Day"<<Day<<endl;
	Ws = Day + '/' + Month + '/' + Year;
	Ws += " ";
	Ws += val[3];
	Ws += ",";
	Ws += val[TagNumber + 5];
	Ws += "\n";
	const char *w = Ws.c_str();
	fputs(w, fw);
	return 0;
}

int Tag::WriteFindValues(FILE * fw, time_t _tbegin, time_t _tend, int _step)
{
	FILE *fp;
	struct tm	*time = localtime(&_tbegin);				
	time_t		curTime;
	fopen_s(&fp, Path.c_str(),"r");
	int d = (int)difftime(_tend, _tbegin);
	char strTime[20];	
	for (int i = 0; d >= _step; i++)
	{
		time->tm_sec += _step;
		curTime = mktime(time);
		strftime(strTime, 20, "%d/%m/%Y %X", time);
		Value.Date = string(strTime);
		d = (int)difftime(_tend, _tbegin);
		Value.Val = ReadValueByDateTime(Value.Date);
	}
	fclose(fp);
	return 0;
}

string Tag::ReadValueByDateTime(string Data)
{
	FILE *fpp;
	if (strcmp("", Data.c_str()) == 0)
	{
		perror("Не указано искомое время");
		return "";
	}
	fopen_s(&fpp, Path.c_str(), "r");
	string DataTime, Val;
	if (!fpp) 
	{ 
		perror("Ошибка чтения данных. Не удалось открыть файл для чтения");
		return ""; 
	}
	do
	{	
		char line[255];
		fgets(line, 255, fpp);
		string s = string(line);			
		DataTime = s.substr(0, s.find(','));
		size_t start = s.find(',')+1;
		Val = s.substr(start, s.find(10)-start);//
		if (strcmp(Data.c_str(), DataTime.c_str()) == 0) 
		{
			Value.Date = DataTime;
			Value.Val = Val;
			fclose(fpp);
			return Val;
		}
	}
	while (!(feof(fpp)) || strcmp(Val.c_str(), DataTime.c_str()) == 0);
	fclose(fpp);
	return "";
}

Tag::Tag()
{
}


Tag::~Tag()
{
}
