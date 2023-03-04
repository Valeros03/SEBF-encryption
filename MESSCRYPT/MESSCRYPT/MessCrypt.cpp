#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <random>
#include <string>
#include "Ustring.h"

int fase;
Ustring finalKey;
Ustring verKey;
Ustring m_pass;
#define ASCII_START 32
#define ASCII_END 126

void KeyGen(unsigned int l_seed);

int writeInFile(const char* f_name, Ustring* buffer, unsigned int l_seed) {

	FILE* f;
	unsigned int a = buffer->length();
	if ((f = fopen(f_name, "wb")) == NULL) {
		std::cout << "Errore nell'apertura/creazione del file" << f_name << "\n";
		return 0;
	}

	fwrite(&a, sizeof(unsigned int), 1, f);
	fwrite(&l_seed, sizeof(unsigned int), 1, f);

	for(int i = 0; i<a; i++)
		fwrite(&((*buffer)[i]), sizeof(unsigned char), 1, f);

	for (int i = 0; i < finalKey.length(); i++) {
		fwrite(&(finalKey)[i], sizeof(unsigned char), 1, f);
	}

	fclose(f);
	return 1;

}

Ustring Xorstring(Ustring* l_str) {

	Ustring m_text(*l_str);
	int j = 0; 
	int i = 0;
	if (m_text.length() > m_pass.length()) {
		while (j < m_text.length()) {

			while (i < m_pass.length() && j < m_text.length()) {
				(*l_str)[j] ^= m_pass[i];
				j++;
				i++;
			}
			i = 0;
		}
		return m_text;
	}


	while (j < m_text.length()) {

		(*l_str)[j] ^= m_pass[i];
		j++;
		i++;

	}
	return m_text;

}


int Crypt(std::string l_str = "", int l_time = -1) {

	std::string str = l_str;
	int timeOf = l_time;

	if (l_time == -1) {

		std::cout << "\t\t**Crypt Function**\nInserisci la stringa\n";
		std::getline(std::cin >> std::ws, str);
		std::cout << "\n\n";
		timeOf = str.length() % 10 + 1;
	}
	std::cout << "*****Processo...\n";
	unsigned long asciiSeed = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= ASCII_START && str[i] <= ASCII_END)
		asciiSeed += str[i];
	}
	

	std::shuffle(str.begin(), str.end(), std::default_random_engine((str.length() + (371 * 10303) + asciiSeed))); //funzione utile per randomizzare roba velocemente

	srand((str.length() + (371* 10303)));
	for (int i = 0; i < str.length(); i++) {
		int a = rand() % 32 + 1;
		if (str[i] >= ASCII_START && str[i] <= ASCII_END) {
			if (str[i] + a <= ASCII_END) {
				str[i] = str[i] + a;
			}
			else {
				str[i] = ASCII_START + (a - ((ASCII_END - str[i]) + 1));
			}

		}

	}

	if (timeOf == 0)
		std::cout << str << "\n\n";

	if (timeOf > 0)
		return Crypt(str, --timeOf);


	return 1;
}

int CryptFromFile(Ustring l_str, int l_time = -1, std::string l_fileName = "", unsigned int l_seed = 0) {

	Ustring str (l_str);
	std::string fileName = l_fileName;
	unsigned int seed = l_seed;
	int timeOf = l_time;

//Viene eseguito solo alla prima chiamata della funzione------------------------------------------------------------------------------------
	if (l_time == -1) {

		//key generation****
		seed = rand() % UINT_MAX + 1;
		KeyGen(seed);
		//******************

		fase = 0;

		std::cout << "\t\t*Crypt File Function*\n";
		std::cout << "Inserisci il percorso del file omettendo l'estensione(se si trova nella stessa cartella del programma scrivere solo il nome)\n";
		std::getline(std::cin >> std::ws, fileName);
		
		std::cout << "\n\n";

		std::ifstream bob(fileName+".txt");
		if (!bob.is_open()) {
			std::cout << "Errore nell'apertura del file: " << fileName<< ".txt" << "!\n";
			return -1;
		}

		std::cout << "[1] - Lettura...\n";
		
		while (!bob.eof()) {

			std::string t;
			std::getline(bob, t, '\0');
			str += t;

		}

		bob.close();
		std::cout << "[2] - Lettura completata\n";

		timeOf = str.length() % 10;
	}
//--------------------------------------------------------------------------------------------------------------------------------------------


//inizio delle iterazioni*****************************************************
	unsigned long asciiSeed = 0;
	std::cout << "******************FASE " << fase << "******************\n";
	

//Calcolo del seed per l'srand, ci serve per lo shuffle -----------
	for (int i = 0; i < str.length(); i++) {

		if (str[i] >= ASCII_START && str[i] <= ASCII_END)
			asciiSeed += str[i];
	}
//-----------------------------------------------------------------

					//*OPERAZIONE UNO*

//L'operazione uno consiste nello shuffle del messaggio in maniera pseudo-randomica-----------------------------
	std::cout << "[3] - Procedura numero 1 in corso...\n";
	str.ShuffleString((str.length() + (371 * 10303) + asciiSeed));
	std::cout << "[4] - Procedura numero 1 terminata\n";
//--------------------------------------------------------------------------------------------------------------


					//**OPERAZIONE DUE**

//L'operazione due consiste nello xor tra il messaggio e la chiave generata------------------------------------
	std::cout << "[5] - Procedura numero 2 in corso...\n";
	Xorstring(&str);
	std::cout << "[6] - Procedura numero 2 terminata\n";
//-------------------------------------------------------------------------------------------------------------


					//***OPERAZIONE TRE***

//L'operazione tre consiste nella somma tra caratteri generati pseudo-randomicamente e i caratteri del messaggio-
	std::cout << "[7] - Procedura numero 3 in corso...\n";

	srand(str.length() + (371 * 10303));
	for (int i = 0; i < str.length(); i++) {
		unsigned int a = rand() % 32 + 1;
		if (str[i] >= ASCII_START && str[i] <= ASCII_END) {
			if (str[i] + a <= ASCII_END) {
			
				str[i] = str[i] + a;
			}
			else {
				str[i] = ASCII_START + (a - ((ASCII_END - str[i]) + 1));
			}

		}

	}

	std::cout << "[8] - Procedura numero 3 terminata\n";
//------------------------------------------------------------------------------------------------------------------


					//*SCRITTURA*

//viene eseguita solo al termine, scrive il messaggio cryptato------------------------------------------------------
	if (timeOf == 0) {
		
		std::cout << "*********************Terminazione*********************\n";

		std::cout << "[10] - Copio sul file...\n";
		writeInFile((fileName + ".sebf").c_str(), &str, seed);
		remove((fileName + ".txt").c_str());
		std::cout << "[11] - Cryptaggio terminato (Ricordati di cancellare il file originale se hai inserito la chiave corretta)\n\n";
	}
//------------------------------------------------------------------------------------------------------------------




//viene eseguito al termine di ogni iterazione eccetto l'ultima, fa proseguire con l'iterazione successiva----------
	if (timeOf > 0) {

		std::cout << "[9] - Fase " << fase << " terminata\n";
		fase++;
		return CryptFromFile(str, --timeOf, fileName,seed);

	}
//------------------------------------------------------------------------------------------------------------------
	return 1;
}

int Decrypt(std::string l_str = "", int l_time = -1) {

	std::string str = l_str;

	int timeOf = l_time;

	if (l_time == -1) {

		std::cout << "\t\t**DeCrypt Function**\nInserisci la stringa\n";
		std::getline(std::cin >> std::ws, str);

		timeOf = str.length() % 10 + 1;
	}

	std::vector<int> unic;
	
	for (int i = 0; i < str.length(); i++) {
		unic.push_back(i);
	}

	srand((str.length() + (371 * 10303)));
	for (int i = 0; i < str.length(); i++) {
		int a = rand() % 32 + 1;
		if (str[i] >= ASCII_START && str[i] <= ASCII_END) {
			if (str[i] - a >= ASCII_START) {
				str[i] = str[i] - a;
			}
			else {
				str[i] = ASCII_END - (a - ((str[i] - ASCII_START) + 1));
			}

		}

	}

	unsigned long asciiSeed = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= ASCII_START && str[i] <= ASCII_END)
		asciiSeed += str[i];
	}

	std::shuffle(unic.begin(), unic.end(), std::default_random_engine((str.length() + (371 * 10303) + asciiSeed)));
	std::string temp = str;

	for (int i = 0; i < str.length(); i++) {
		str[unic[i]] = temp[i];
	}

	if (timeOf == 0)
		std::cout << str << "\n\n";

	
	if (timeOf > 0)
		return Decrypt(str, --timeOf);


	return 1;
}

int readFromFile(const char* f_name, Ustring* buffer, unsigned int* l_seed) {

	FILE* file;
	unsigned int a;
	unsigned int c;

	if ((file = fopen(f_name, "rb")) == NULL) {
		std::cout << "Errore nell'apertura del file: " << f_name << "\n";
		return 0;
	}

	if (fread(&a, sizeof(unsigned int), 1, file) != 1) {
		std::cout << "Errore nella lettura del file: " << f_name << "\n";
		return 0;
	}
	if (fread(&c, sizeof(unsigned int), 1, file) != 1) {
		std::cout << "Errore nella lettura del file: " << f_name << "\n";
		return 0;
	}
	
	(*l_seed) = c;

	char b;
	for (int i = 0; i < a; i++) {
		fread(&b, sizeof(unsigned char), 1, file);
		(*buffer) += b;
	}

	while (!feof(file)) {

		fread(&b, sizeof(unsigned char), 1, file);
		verKey += b;
	}

	fclose(file);
	return 1;
}


int DeCryptFromFile(Ustring l_str, int l_time = -1, std::string l_fileName = "") {

	Ustring str(l_str);
	std::string fileName = l_fileName;
	
	unsigned int seed;
	int timeOf = l_time;


//----------------------------viene considerato solo alla prima chiamata della funzione-----------------------------
	if (l_time == -1) {    

		fase = 0;
		
		//inserimento nome del file di input e output
		std::cout << "\t\t**DeCrypt File Function**\nInserisci il nome del file Cryptato (senza estensione)\n";
		std::getline(std::cin >> std::ws, fileName);
		
		std::cout << "\n\n";

		//inizio lettura del file
		std::cout << "[1] - Lettura...\n";
		if (readFromFile((fileName + ".sebf").c_str(), &str, &seed) == 0)
			return 0;

		std::cout << "[2] - Lettura avvenuta con successo\n";

		//Chiede la chiave e il numero di byte dal quale essa è formata
		KeyGen(seed);
		if (finalKey != verKey) {
			std::cout << "Errore, chiave errata\n";
			return 9;
		}

		//calcola il numero di iterazioni in base alla lunghezza della stringa
		timeOf = str.length() % 10;
	}
//------------------------------------------------------------------------------------------------------------------



//inizio di ogni iterazione**************************************************************************************
	std::cout << "******************FASE " << fase << "******************\n";

					//*OPERAZIONE NUMERO UNO*

//operazione 1 consiste nella somma di caratteri ascii pseudo-randomico a ogni carattere della stringa---------------------
	std::cout << "[3] - Operazione numero 1 in corso...\n";

	srand(str.length() + (371 * 10303));
	for (int i = 0; i < str.length(); i++) {
		int a = rand() % 32 + 1;
		if (str[i] >= ASCII_START && str[i] <= ASCII_END) {
			if (str[i] - a >= ASCII_START) {
				str[i] = str[i] - a;
			}
			else {
				str[i] = ASCII_END - (a - ((str[i] - ASCII_START)+1));
			}

		}

	}
	std::cout << "[4] - Operazione numero 1 terminata\n";
//------------------------------------------------------------------------------------------------------------------



				//**OPERAZIONE NUMERO DUE**

//L'operazione due consiste nell'effettuare uno xor tra la chiave generata e il messaggio---------------------------
	std::cout << "[5] - Operazione numero 2 in corso...\n";
	Xorstring(&str);
	std::cout << "[6] - Operazione numero 2 terminata\n";
//------------------------------------------------------------------------------------------------------------------



			  //***OPERAZIONE NUMERO TRE***

//L'operazione tre consiste nell'invertire in maniera pseudo-randomica i caratteri del messaggio--------------------
	std::cout << "[7] - Operazione numero 3 in corso...\n";

	std::vector<int> unic;
	for (int i = 0; i < str.length(); i++) {
		unic.push_back(i);
	}

	unsigned long asciiSeed = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= ASCII_START && str[i] <= ASCII_END)
			asciiSeed += str[i];

	}

	std::shuffle(unic.begin(), unic.end(), std::default_random_engine((str.length() + (371 * 10303) + asciiSeed)));

	Ustring temp = str;

	for (int i = 0; i < str.length(); i++) {
		str[unic[i]] = temp[i];
	}

	std::cout << "[8] - Operazione numero 3 terminata\n";
//--------------------------------------------------------------------------------------------------------------------

				
					//*SALVATAGGIO*

//Viene eseguito solo alla fine delle iterazioni e salva il testo all'interno del file--------------------------------
	if (timeOf == 0) {

		std::cout << "*********************Terminazione*********************\n";
		std::cout << "[10] - Scrivo su file...\n";

		std::ofstream bob(fileName+".txt");

		if (!bob.is_open()) {
			std::cout << "Errore nella creazione del file: " << fileName<<".txt"<<"\n";
			return -1;
		}
		
		for (int i = 0; i < str.length(); i++) {
			bob << str[i];
		}

		bob.close();
		remove((fileName + ".sebf").c_str());
		std::cout << "[11] - Decryptaggio temrimato\n\n";
	}
//------------------------------------------------------------------------------------------------------------------


//Fa proseguire l'iterazione se vale ancora la condizione(viene saltata dopo l'ultima iterazione)-------------------
	if (timeOf > 0) {

		std::cout << "[9] - Fase " << fase << " terminata\n";
		fase++;
		return DeCryptFromFile(str, --timeOf, fileName);

	}
//------------------------------------------------------------------------------------------------------------------
	return 1;
}


/*
This code generate a unique fixed-size key of any numbers of bit you need using a complex sequence of bit operation to complitly
hash the key/password used.
This key can be used for Encription or storing a password into a database (with the salt for a better attack resistence)

*/

char randomCharGnerator(int source, std::mt19937& l_engine) {

	std::uniform_int_distribution<int> distribution(0, 251);
	int index = distribution(l_engine);

	const char* alphabet[] = { "QWERTYUIOPASDFGHJKLÖ‘‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™œš›žŸ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉËÞÚÚ☻╩ÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâÙäå♂☻♥♦♣"
		"♠•◘○◙♂♀♪♫☼►◄↕‼¶æçP▼▲↔∟←→↓↑↨▬ZXCVBNM1234567890!£$ % &/ (= ) ?^ ì'èòà+ù§*é°ç@#[]€,:-_.;qwerty~uioplkjhgfdsazxcvbnm><|}{", " ♠•◘○◙♂♀♪♫☼►◄↕‼¶æçP▼▲↔∟←→↓↑"
		"↨▬ZXC}{IcanidiGiusWERTGHJKLÖ‘‚ƒ„…†‡ˆ‰Š‹ŒŽeconsecteturadipiscingelit.Proinarcumagna,pp“‘õêÚOÞßàçìãeriesconoanuVBNM1234567890!£$%ª ■²³¹&/(= )?^ì'èòà+ù§*é"
		"°ç@#[]€,:-_.;qwerty~uioplkjhgfdsazxcvbnm><|otarenell'oceanoPacifico!","LoremÝÕÔê¹äipsum34lorsitamet,coñç“œžŸo€‹Œêéns32teturadipiscingelit.Proinarcumagna,"
		"finibusfeugiatves—˜™œš›žŸ¡¢£¤¥¦tibulumac,vehiculapos¦§¨©ª«ueremauris.PraeseÑÒÓÔÕÖ×ØÙÚÛntdignÇÈÉËÞÚÚ☻╩ÌÍÎÏÐissimmetusnisl,necsemper,auguenecelementumrutrum,orcimorbi." };
	return alphabet[source][index];

}

//This function OR or & the string passed for each character 
void ScaleOperation(int dimension, Ustring* string) {


	Ustring tempFirst((*string));

	(*string)[0] = tempFirst[0] ^ (*string)[1];

	for (int i = 1; i < dimension - 1; i++) {
		//the OR or & depends from the position of the character in the string
		//if it is even do the & operation if it is odd it do the OR operation
		if (i % 2 == 0) {

			(*string)[i] = tempFirst[i - 1] & (*string)[i + 1];

		}
		else {

			(*string)[i] = tempFirst[i - 1] | (*string)[i + 1];
		}

	}

	return;
}

//generate a random string Using the random character generator
Ustring GenerateString(int length, int source, std::mt19937& l_engine, int salt = 0) {

	Ustring temp("");
	std::uniform_int_distribution<int> distribution(0, 2);
	for (int i = 0; i < length; i++) {
		if (salt != 0) {
			source = distribution(l_engine);
		}
		char temporal = randomCharGnerator(source, l_engine);
		temp += reinterpret_cast<unsigned char&>(temporal);
	}


	return temp;
}

//it just shift bits
void BitShifting(Ustring* l_string, int l_length, int timeOfShifting = 1) {

	for (int i = 0; i < l_length; i++) {

		(*l_string)[i] = std::rotr((*l_string)[i], timeOfShifting);

	}
	return;
}

//this function just xor two string
void XorStrings(Ustring* big, Ustring* smaller, int len) {

	int time = 0;
	int start = 0;
	while (time <= len / 8) {
		//dude to the fact that the small string isn't long enougth to XOR all big string characters
		//I need to restart the small string counter to repet the proces

		for (int i = start, j = 0; j < (*smaller).length() && i < (*big).length(); i++, j++) {

			(*big)[i] = (*big)[i] ^ (*smaller)[j];

		}
		time++;
		start = time * (*smaller).length();
	}

	return;
}

//this function add the ascii code of each character in the small string 
//in to the big one
void AddStrings(std::string* big, std::string* smaller, int len) {

	int time = 0;
	int start = 0;
	while (time <= len / 8) {
		//dude to the fact that the small string isn't enougth to sum with all the charachters
		//I need to restart the small string counter to repet the proces
		for (int i = start, j = 0; j < (*smaller).length() && i < (*big).length(); i++, j++) {

			(*big)[i] = (*big)[i] + (*smaller)[j];

		}
		time++;
		start = time * (*smaller).length();
	}

	return;
}

//this function calculate the xor of all characters in the string and XOR the ruslut with the random generated string
void AddAscii(Ustring* key, Ustring* temp) {

	char adder = (*key)[0];

	for (int i = 1; i < (*key).length(); i++) {

		adder ^= (*key)[i];

	}

	adder = adder & (0xff << 2);

	for (int i = 0; i < (*temp).length(); i++) {

		(*temp)[i] ^= adder;

	}

}

//this function just sum the first half string with the reversed second half
void SumRevString(Ustring* l_string) {

	for (int i = -1, j = l_string->length(); i < j; i++, j--) {

		if ((*l_string)[i] + (*l_string)[j] < 255)
			(*l_string)[i] = (*l_string)[i] + (*l_string)[j];

	}

	return;
}
int calcolaAscii(Ustring l_string) {

	int risultato = 0;

	for (int i = 0; i < l_string.length(); i++) {

		risultato += l_string[i];

	}

	return risultato;
}

Ustring Compression(Ustring l_string) {

	//a lot of memory required to speed up the proces

	Ustring endVec;
	Ustring stringCast = l_string;
	Ustring reversed;
	Ustring orVect;
	Ustring AndVect;
	Ustring XandVect;
	Ustring End;

	reversed = stringCast;
	reversed.reverse();

	//shifting the reversed
	BitShifting(&reversed, reversed.length(), 1);

	//init vector of or
	for (int i = 0; i < reversed.length(); i++)
		orVect += stringCast[i] | reversed[i];

	//shifting the reversed again
	BitShifting(&reversed, reversed.length(), 1);

	//init vector of AND
	for (int i = 0; i < reversed.length(); i++)
		AndVect += stringCast[i] & reversed[i];

	//shift the reversed again
	BitShifting(&reversed, reversed.length(), 1);

	//init XandVec
	for (int i = 0; i < reversed.length(); i++)
		XandVect += ((~stringCast[i]) & reversed[i]);

	//Brute hash of string with operation like & and | and XAND
	//strings are complitely destroyed and more hasheds

	for (int i = 0; i < reversed.length(); i++)
		endVec += ((~stringCast[i]) & orVect[i]);

	for (int i = 0; i < reversed.length(); i++)
		endVec[i] = ((endVec[i] & XandVect[i]));

	for (int i = 0; i < reversed.length(); i++) {
		endVec[i] = ((endVec[i] | AndVect[i]));
		if (endVec[i] == NULL) {
			endVec[i] = (~endVec[i]) ^ AndVect[i];
		}
	}



	return endVec;
}

Ustring GenerateKey(Ustring l_key, int byte, int salt) {

	std::mt19937 engine(salt);
	std::uniform_int_distribution<int> distribution(0, 2);
	int source = distribution(engine);

	Ustring temp = GenerateString(byte - l_key.length(), source, engine);

	source = distribution(engine);
	Ustring saltString = GenerateString(byte, source, engine, salt);

	Ustring key("");
	unsigned int asci = calcolaAscii(l_key);

	l_key.ShuffleString(l_key.length());

	key = l_key;

	for (int i = 0; i < (asci + l_key.length()) * sqrt(3); i++) {

		//first hash operations
		ScaleOperation(l_key.length(), &key);

		//second hash operations
		BitShifting(&temp, temp.length(), 2);
		BitShifting(&key, key.length(), 2);

		//tird hash operations
		AddAscii(&key, &temp);      //xor the xor of the hashed string
		XorStrings(&temp, &key, temp.length()); //xor temp and key

	}

	//fourth hash operations 
	SumRevString(&temp);
	SumRevString(&key);        //the sum *half* string with the reversed half

	key += temp;   //a larger buffer hide some character that are usefull for the hash

	key.ShuffleString(asci);//change the order with ascii of strings so it make more hard to create collisions

	//first compression to initialite the end string
	Ustring end = Compression(key);

	//iterate the comoression (very hard and slow proces)
	for (int i = 0; i < (asci * sqrt(19)) + end.length(); i++) {

		XorStrings(&end, &saltString, end.length());
		AddAscii(&saltString, &end);
		end ^= Compression(end);

	}

	if (end.length() - byte > 0)
		end.removeBack(end.length() - byte);

	return end;

}

void leggiUnsignedShort(unsigned short* b, const char* err) {

	while (true) {

		std::cin >> *b;
		if (std::cin.fail()) {
			std::cout << err;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			continue;
		}
		std::cin.ignore(1000, '\n');
		if (std::cin.gcount() > 1) {
			std::cout << err;
			continue;
		}
		break;

	}

}

void KeyGen(unsigned int l_seed) {


	/*in a real world application you should generate a random/pseduo random number and save it in the database of an account and use it as the seed*/
	int a = l_seed;
	unsigned short byte;
	std::string input;
	std::cout << "Da quanti byte si vuole generare la chiave?\n";
	leggiUnsignedShort(&byte, "Inserire un numero che sia piu' minore o uguale a 65535 e maggiore o uguale a 0\n");
	std::cout << "Inserisci la Chiave\n";
	std::getline(std::cin >> std::ws, input);
	m_pass = input;

	Ustring key(input);

	finalKey = GenerateKey(key, byte, a);
	
}

int Installing(char* l_path) {

	FILE* file;
	if ((file = fopen("config.ini", "r")) != NULL) {
		fclose(file);
		return 0;
	}
	
	std::string path(l_path);
	path = path.substr(0, path.length() - strlen("Safe Encryption Based File.exe"));

	std::cout << "0%\n";
	HKEY hKey, hKeyShell, hKeyOpen, hKeyCommand;
	LONG result;

	if (RegOpenKeyExA(HKEY_CLASSES_ROOT, ".txt\\OpenWithProgids", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
		
		if (RegSetValueExA(hKey,"Crypter.txt", 0, REG_SZ, (BYTE*)"", strlen("") + 1) != ERROR_SUCCESS) {
			std::cout << "Failed to add string value." << std::endl;
			return 4;
		}
		
		RegCloseKey(hKey);
	}
	else {
		std::cout << "Failed to open registry key." << std::endl;
		return 4;
	}

	std::cout << "25%...\n";

	
//shell per l'apertura file txt con l'applicazione-------------------------------------------------------------------------------------------------------------
	
	path += "Crypter.exe %1";
	result = RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT("Crypter.txt\\shell"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyShell, NULL);
	result = RegCreateKeyEx(hKeyShell, TEXT("open"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyOpen, NULL);
	result = RegCreateKeyEx(hKeyOpen, TEXT("command"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyCommand, NULL);
	

	if (result != ERROR_SUCCESS)
	{
		std::cout << "Failed to open create key." << std::endl;
		return 4;
	}

	if (RegSetValueExA(hKeyCommand, "", 0, REG_SZ, (BYTE*)path.c_str(), strlen(path.c_str()) + 1) != ERROR_SUCCESS) {
		std::cout << "Failed to add string value." << std::endl;
		return 4;

	}

	RegCloseKey(hKey);
	RegCloseKey(hKeyShell);
	RegCloseKey(hKeyOpen);
	RegCloseKey(hKeyCommand);

	std::cout << "50%...\n";
//------------------------------------------------------------------------------------------------------------------------------------




//shell per l'apertura dei file .sebf-------------------------------------------------------------------------------------------------
	
	path = path.substr(0, path.length() - strlen("Crypter.exe %1"));
	path += "Decrypter.exe %1";
	result = RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT(".sebf"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	result = RegCreateKeyEx(hKey, TEXT("shell"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyShell, NULL);
	result = RegCreateKeyEx(hKeyShell, TEXT("open"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyOpen, NULL);
	result = RegCreateKeyEx(hKeyOpen, TEXT("command"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyCommand, NULL);

	if (result != ERROR_SUCCESS)
	{
		std::cout << "Failed to open create key." << std::endl;
		return 4;
	}
	if (RegSetValueExA(hKeyCommand, "", 0, REG_SZ, (BYTE*)path.c_str(), strlen(path.c_str()) + 1) != ERROR_SUCCESS) {
		
		std::cout << "Failed to add string value." << std::endl;
		return 4;
	}

	RegCloseKey(hKey);
	RegCloseKey(hKeyShell);
	RegCloseKey(hKeyOpen);
	RegCloseKey(hKeyCommand);

	std::cout << "75%...\n";
//---------------------------------------------------------------------------------------------------------------------------------------



//Icone dei file .sebf--------------------------------------------------------------------------------------------------------------------
	path = path.substr(0, path.length() - strlen("Decrypter.exe %1"));
	path += "icon\\icon.ico";
	if (RegOpenKeyExA(HKEY_CLASSES_ROOT, ".sebf", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
		result = RegCreateKeyEx(hKey, TEXT("DefaultIcon"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyOpen, NULL);

		if (RegSetValueExA(hKeyOpen, "", 0, REG_SZ, (BYTE*)path.c_str(), strlen(path.c_str()) + 1) != ERROR_SUCCESS) {
			
			std::cout << "Failed to add string value." << std::endl;
			return 4;

		}
		RegCloseKey(hKey);
	}
	else {
		std::cout << "Failed to open registry key." << std::endl;
		return 4;
	}

	std::cout << "100%...\n";
//----------------------------------------------------------------------------------------------------------------------------------------
	FILE* f;
	
	if ((f = fopen("config.ini", "w")) == NULL) {
		std::cout << "Errore nella creazione del file: config.ini\n";
		return 4;
	}
	fprintf(f, "%s", "Questo file serve per verificare che l'istallazione e' già avvenuta con successo NON ELIMINARE!\n");
	fclose(f);
	std::cout << "L'istallazione e' avvenuta con successo!\n";
	return 4;
}

int main(int argc, char* argv[]) {
	
	
	if (Installing(argv[0]) == 4) {
		std::cout << "Installazione terminata...\nPremere invio per chiudere\n";
		std::cin.get();
		return 3;
	}

	srand(time(NULL));
	unsigned short c = 0;
	bool use = true;
	Ustring emp("");

	while (use) {

		std::cout << "Se il testo da convertire risulta maggiore di 4000 caratteri allora inserire il testo in un file e usare (3) e (4)\n";
		std::cout << "Selezionare la modalita:\ncrypt(1)\ndecrypt(2)\nCrypta un file(3)\nDecrypta un file(4)\nEsci(5)\n";

		m_pass.cleanString();
		finalKey.cleanString();
		verKey.cleanString();
		leggiUnsignedShort(&c, "inserire un numero tra 1,2,3,4,5\n");

		switch (c) {
		case 1:
			system("cls");
			Crypt();
			break;
		case 2:
			system("cls");
			Decrypt();
			break;
		case 3:
			system("cls");
			CryptFromFile(emp);
			break;
		case 4:
			system("cls");
			DeCryptFromFile(emp);
			break;
		case 5:
			use = false;
			break;
		default:
			std::cout << "inserire un numero tra 1, 2, 3, 4, 5\n";

		}

	}

}
