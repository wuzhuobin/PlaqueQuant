#include "MacEncoding.h"



MacEncoding::MacEncoding()
{
	char* ch = new char[18];
	getMacAdd(ch);
	MAC = ch;
	delete ch;
}

MacEncoding::MacEncoding(const std::string MAC)
	:MAC(MAC)
{
}


MacEncoding::~MacEncoding()
{
}

std::string MacEncoding::encodeString() {
	int k = 0;
	int pos = 0;	

	std::string newString(29, '\0');
	while (k < this->MAC.length())
	{
		if (MAC[k] == '-')
		{
			newString[pos] = '-';
			k++;
			pos++;
		}
		else
		{
			for (int i = 0; i < MACENCODING_N; ++i)
			{
				int j = Table[i].find(MAC[k]);
				if (j >= 0 && j < MACENCODING_N)
				{
					newString[pos] = Ciphertext_1[i];
					pos++;
					newString[pos] = Ciphertext_2[j];
					pos++;
				}  //end if
			}// end for
			k++;
		}
	}//end while
	return newString;

}