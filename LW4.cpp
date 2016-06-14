#include "des.h"
#include <iostream>
#include <time.h>	//time
#include <string>	//getline
#include <conio.h>	//_putch, _getch

using namespace std;
template <typename T> 
class PsRandom
{
private:
	T CurrentVal;
public:
	PsRandom(T Seed)
	{
		CurrentVal = Seed;
	}
	PsRandom()
	{
		srand(time(0));
		CurrentVal = (T)rand();
	}
	T Next()
	{
		T OldVal = CurrentVal;
		// T(i+1)=(A*T(i)+C) mod B
		// A=17, C=39, B=256
		CurrentVal = (17 * CurrentVal + 39) % 256;
		return OldVal;
	}
};
int BreakBit(uchar* src, int iBit, int iBlock);
int BreakBit(uchar* src, int iBit);
string HiddenInput();

int main(int argc, char* argv[])
{
	// Ключ DES алгоритма
	cout << "[Input a DES key]:" << endl;
	string sDesKey = HiddenInput();
	while (sDesKey.length()<20)
	{
		sDesKey = "0" + sDesKey;
	}
	string sLow = sDesKey.substr(0,8);
	string sHi = sDesKey.substr(9,8);
	ulong64 des_key = { stoi(sLow),stoi(sHi) };//{ 0x19456DA1, 0xADE2DA24 };
	// Генеририруем начальный вектор
	PsRandom <ulong> psRand = PsRandom<ulong>();
	ulong64 des_initVector;
	// Отркрытый текст
	string sInpStr;
	uchar* uInpStr;
	cout << "\n[Input a string to encode]:"<<endl;
	getline(cin, sInpStr);
	// Дополняем входную строку до кратности 8 байт пробелами
	while (sInpStr.length()%8 != 0)
	{
		sInpStr = sInpStr + " ";
	}
	uInpStr = (uchar*)sInpStr.c_str();
	cout <<"[Inputed string  ]: "<<"\n\t"<< uInpStr << endl;
	int length = strlen((char*)uInpStr);
	string temp1 = string(length, ' ');
	string temp2 = string(length, ' ');
	uchar* uCryptedStr	=	(uchar*)temp1.c_str(); //new uchar[length];
	uchar* uDecryptedStr=	(uchar*)temp2.c_str(); // new uchar[length];
	bool WantToBreak = true;

	// ECB
	des_initVector = { psRand.Next() , psRand.Next() };
	cout << "ECB:" << endl;
	des_encrypt_ecb(uInpStr, length, uCryptedStr, des_key);
	cout << "  [Encrypted string]: " << "\n\t" << uCryptedStr << endl;
	if(WantToBreak)
	{
		BreakBit(uCryptedStr, 0);
		cout << "  [Broken string   ]: " << "\n\t" << uCryptedStr << endl;
	}
	des_decrypt_ecb(uCryptedStr, length, uDecryptedStr, des_key);
	cout << "  [Decrypted string]:  " << "\n\t" << uDecryptedStr << endl;

	// CBC
	des_initVector = { psRand.Next() , psRand.Next() };
	cout <<endl<< "CBC:" << endl;
	des_encrypt_cbc(uInpStr, length, uCryptedStr, des_key, des_initVector);
	cout << "  [Encrypted string]: " << "\n\t" << uCryptedStr << endl;
	if (WantToBreak)
	{
		BreakBit(uCryptedStr, 0);
		cout << "  [Broken string   ]: " << "\n\t" << uCryptedStr << endl;
	}
	des_decrypt_cbc(uCryptedStr, length, uDecryptedStr, des_key, des_initVector);
	cout << "  [Decrypted string]:  " << "\n\t" << uDecryptedStr << endl;

	// CFB
	des_initVector = { psRand.Next() , psRand.Next() };
	cout << endl << "CFB:" << endl;
	des_encrypt_cfb(uInpStr, length, uCryptedStr, des_key, des_initVector);
	cout << "  [Encrypted string]: " << "\n\t" << uCryptedStr << endl;
	if (WantToBreak)
	{
		BreakBit(uCryptedStr, 0);
		cout << "  [Broken string   ]: " << "\n\t" << uCryptedStr << endl;
	}
	des_decrypt_cfb(uCryptedStr, length, uDecryptedStr, des_key, des_initVector);
	cout << "  [Decrypted string]:  " << "\n\t" << uDecryptedStr << endl;

	// OFB
	des_initVector = { psRand.Next() , psRand.Next() };
	cout << endl << "OFB:" << endl;
	des_encrypt_ofb(uInpStr, length, uCryptedStr, des_key, des_initVector);
	cout <<"  [Encrypted string]: "<<"\n\t"<< uCryptedStr <<endl;	
	if (WantToBreak)
	{
		BreakBit(uCryptedStr, 0);
		cout << "  [Broken string   ]: " << "\n\t" << uCryptedStr << endl;
	}
	des_decrypt_ofb(uCryptedStr, length, uDecryptedStr, des_key, des_initVector);
	cout <<"  [Decrypted string]:  "<<"\n\t"<< uDecryptedStr << endl;

	cin.get();
	return 0;
}
// Инвертирует iBit в строке src
int BreakBit(uchar* src, int iBit=0)
{
	int ByteIndex = iBit / 8;
	int BitIndex = iBit % 8;
	//src[ByteIndex] = src[ByteIndex] ^ (1 << (7 - BitIndex));
	src[ByteIndex] = src[ByteIndex] ^ (1 << BitIndex);
	return 0;
}
// Инвертирует iBit в iBlock'e строки src
int BreakBit(uchar* src, int iBit, int iBlock)
{
	ulong block;
	int size = sizeof(ulong)*8-1;
	GET_ULONG_BE(block, src, iBlock);
	block = block ^(1 << (size - iBit) );
	PUT_ULONG_BE(block, src, iBlock);
	return 0;
}

string HiddenInput() {
	string res = "";
	char ch = '\0';
	while (ch != '\r') {
		ch = _getch();
		if (ch != '\r' && ch != 8) {
			res = res + ch;
			_putch('*');
		}
		if (ch == 8 && res.length()>0) {
			res = res.erase(res.length() - 1, 1);
			_putch(8);
			_putch(' ');
			_putch(8);
		}
	}
	return res;
}