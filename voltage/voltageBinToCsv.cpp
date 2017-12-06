#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdint.h>

#define IC_NUM 12

using namespace std;

//not written by me
/*std::string to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}*/

//also not written by me
std::string padTo(std::string str, const size_t num, const char paddingChar = ' ')
{
    if(num > str.size())
	{
        str.insert(0, num - str.size(), paddingChar);
	}
	return str;
}

int main()
{
	uint16_t lastTime;
	
	uint16_t time;
	uint16_t volt_data;
	
	FILE* fbinary;
	fbinary = fopen("voltage.bin", "rb");
	
	FILE* fcsv;
	fcsv = fopen("voltage.csv", "w+");
	
	string header;
	uint8_t id;
	
	fwrite("uptime[s],", sizeof(char), 10, fcsv);
	for(int i=0; i<IC_NUM; i++)
	{
		for(int j=0; j<12; j++)
		{
			header = "cell:";
			header = header.append(to_string(j)).append("; stack:").append(to_string(i)).append("; id:").append(to_string(id)).append(",");
			//id counting isn't working in the right order, unless it is
			id=(i<<4)|(j&(0x0f));
			
			fwrite(header.c_str(), sizeof(char), sizeof(header), fcsv);
		}
	}
	fwrite("\n", sizeof(char), 2, fcsv);
	
	while(true)
	{
		fread(&time, sizeof(time), 1, fbinary);
		for(int i=0; i<IC_NUM; i++)
		{
			for(int j=0; j<12; j++)
			{
				fread(&volt_data, sizeof(volt_data), 1, fbinary);
				
				fwrite(padTo(to_string(volt_data), 5).c_str(), sizeof(char), 5, fcsv);
				fwrite(",", sizeof(char), 1, fcsv);
			}
		}
		fwrite("\n", sizeof(char), 2, fcsv);
		
		if(time==lastTime)
		{
			//for some reason values repeat after reading all values in file, so that i how I will detect end of file because I couldn't figure out how to use any other detection of end of file
			//but this means that if you save values for the same second twice in a row, it will not be converted to csv properly
			break;
		}
		else
		{
			lastTime=time;
		}
	}
	
	fclose(fbinary);
	fclose(fcsv);
	
	getch();
	return 0;
}