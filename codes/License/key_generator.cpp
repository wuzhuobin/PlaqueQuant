#include< iostream >
#include <fstream>
#include<iostream>
#include< string >
using namespace std;
#define N 6
const string Table[ N ] = { //±í.
"8p3d1n", "lt4oah", "7kbc5z",
"ju6wgm", "xsvir2", "9ey0fq"
};
const string Ciphertext_1 = "WDFLFJ"; //×ø±ê
const string Ciphertext_2 = "SLCTA4"; //×ø±ê
void encode_string(const char ch[18], char newch[30]);
int main( int argc, char *argv[] )
{
std::string inputFileName;
std::string outputFileName;
if (argc <= 1)
{
	std::cout << "Usage: " << argv[0] << " inputFileName" << std::endl;
	return -1;
}
inputFileName = argv[1];
outputFileName = "license.dat";

//string ch;
char ch[18], newch[30];
//read file containing the network address, e.g., 00-10-18-4e-62-91
ifstream in;    // Create an input file stream.
in.open(inputFileName.c_str());  // Use it to read from a file.
in >> ch;  // Read the file into a string variable str

for ( int i = 0; i < 30; ++i ) newch[i]=0;
encode_string(ch, newch);

ofstream out;    // Create an output file stream.
out.open(outputFileName.c_str());  // Use it to write a file.
out << newch <<'\n';

//write the encoded, e.g.,


return 0;
}

void encode_string(const char ch[18], char newch[30])
{
	int k = 0;
	int pos=0;
	while( k <17 )
	{
		if (ch[k]=='-')
		{
			newch[pos]='-';
			k++;
			pos++;
		}
		else
		{
			for( int i = 0; i < N; ++i )
			{
			int j = Table[ i ].find( ch[ k ] );
			if( j >= 0 && j < N )
			{
				newch[pos]=Ciphertext_1[i];
				pos++;
				newch[pos]=Ciphertext_2[j];
				pos++;
			}  //end if
			}// end for
			k++;
		}
	}//end while

}



