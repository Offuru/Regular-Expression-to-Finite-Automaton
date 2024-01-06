#pragma once
#include <vector>
#include <string>
#include <queue>
#include <format>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <ostream>
#include <stack>
#include <iostream>

class AFD
{
private:

	static const char lambda = '$';

	struct State
	{
		std::string name;
		std::vector<std::pair<char,std::shared_ptr<State>>> transitions;
		bool final = false;
	};

	using transition = std::pair<char, std::shared_ptr<State>>;

	std::shared_ptr<State> m_begin;
	std::shared_ptr<State> m_end;
	std::deque<std::shared_ptr<State>> m_states;
	std::vector<char> m_alphabet;
	std::vector<std::shared_ptr<State>> m_finalStates;

	void updateIndexes(uint32_t& index);

	void addTransitions(const AFD& other);
	
	friend std::vector<char> getAlphabetUnion(const AFD& afd1, const AFD& afd2);

	bool finalStatesInStates() const;

	bool validTransitions() const;

	bool isDeterministic() const;

	int getPriority(char ch) const;

	void addDotsToRegex(std::string& regex);

public:

	AFD(char);
	
	AFD(const std::string&);

	//concatenation
	AFD& operator&=(AFD&);
	//or
	AFD& operator|=(AFD&);
	//kleene star
	AFD& operator++(int);

	bool verifyAutomaton() const;

	bool checkWord(std::string word) const;

	friend std::ostream& operator<<(std::ostream&, const AFD&);
};

