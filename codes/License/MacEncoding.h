#pragma once
#include <string>
#include "getmacadd.h"
#define MACENCODING_N 6
class MacEncoding
{
public:
	MacEncoding();
	MacEncoding(const std::string MAC);
	~MacEncoding();
	const std::string Table[MACENCODING_N] = { 
		"8p3d1n", "lt4oah", "7kbc5z",
		"ju6wgm", "xsvir2", "9ey0fq"
	};
	const std::string Ciphertext_1 = "WDFLFJ"; 
	const std::string Ciphertext_2 = "SLCTA4"; 
	std::string encodeString();
	std::string MAC;
private:

};

