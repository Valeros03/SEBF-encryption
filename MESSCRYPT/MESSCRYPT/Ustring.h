#pragma once
#include <iostream>
#include <ctime>
#include <random>
#include <bitset>
#include <vector>
#include <string>

class Ustring {



public:

	Ustring();
	Ustring(const std::string& input);
	Ustring(const int& l_len);
	~Ustring();

	bool SearchChar(unsigned char target);
	void FillString(const std::string& bob);
	void ShuffleString(unsigned int seed);
	unsigned int length();
	void reverse();
	void removeBack(unsigned int num);
	void cleanString();

	unsigned char& operator [](int index){
		return m_string[index];
	}
	Ustring& operator += (Ustring secondValue) {

		for (int i = 0; i < secondValue.length(); i++) {
			m_string.push_back(secondValue[i]);
			this->len++;
		}

		return *this;
	}
	Ustring& operator += (unsigned char secondValue) {
			
			m_string.push_back(secondValue);
			this->len++;
			return *this;
	}

	Ustring& operator = (Ustring secondValue) {

		this->len = secondValue.length();
		for (int i = 0; i < secondValue.length(); i++)
			m_string.push_back(secondValue[i]);
		return *this;
	}

	Ustring& operator ^= (Ustring secondValue) {

		for (int i = 0; i < secondValue.length(); i++)
			m_string[i] = secondValue[i];
		return *this;
	}
	bool operator == (Ustring secondValue) {

		for (int i = 0, j = 0; i < m_string.size() && j < secondValue.length(); i++, j++) {

			if (m_string[i] != secondValue[i]) {
				return false;
			}

		}
		return true;

	}

	
private:

	std::vector<unsigned char> m_string;
	unsigned int len;

};


