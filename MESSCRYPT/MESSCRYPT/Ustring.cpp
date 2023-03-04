#include "Ustring.h"


Ustring::Ustring() {

	len = 0;
}

Ustring::Ustring(const std::string& input) {

	FillString(input);
	len = input.length();
}

Ustring::Ustring(const int& l_len) {

	for(int i = 0; i<l_len; i++)
	m_string.push_back('a');
	len = l_len;
}

Ustring::~Ustring() {}


bool Ustring::SearchChar(unsigned char target) {

	if (std::find(m_string.begin(), m_string.end(), target) != m_string.end()) {
		return true;
	}
	return false;
}

void Ustring::FillString(const std::string& temp) {

	for (int i = 0; i < temp.length(); i++) {

		m_string.push_back(temp[i]);

	}
	len = temp.length();
}

void Ustring::ShuffleString(unsigned int seed) {

	std::shuffle(m_string.begin(), m_string.end(), std::default_random_engine(seed));

}

void Ustring::reverse() {

	std::reverse(m_string.begin(), m_string.end());
}

void Ustring::removeBack(unsigned int num) {
	this->len -= num;
	for (int i = 0; i < num; i++) {
		m_string.pop_back();
	}
}

void Ustring::cleanString() {

	this->len = 0;
	while (m_string.size() != 0) {
		m_string.pop_back();
	}
}
unsigned int Ustring::length() { return len; }