#pragma once
#include <vector>
#include <string>

class AFD
{
private:

	using pairTransition = std::pair<std::string, char>;

	const char lambda = '$';

	struct State
	{
		std::string name;
		std::vector<std::pair<char,State*>> connections;
	};

	State* m_begin;
	State* m_end;
	std::vector<State*> m_states;
	std::vector<char> m_alphabet;
	std::vector<std::reference_wrapper<std::pair<char, State*>>> transitions;

public:

	AFD(char);
	AFD(const AFD& other);
	AFD(AFD&& other);
	AFD operator=(const AFD& other);
	AFD operator=(AFD&& other);
	~AFD();

	AFD& operator&=(AFD&&);
	AFD& operator|=(AFD&&);
	AFD& operator*=(AFD&&);
	AFD& operator+=(AFD&&);
};

