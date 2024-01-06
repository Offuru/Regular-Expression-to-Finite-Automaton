#pragma once
#include <vector>
#include <string>
#include <queue>
#include <print>
#include <format>
#include <unordered_map>
#include <memory>
#include <unordered_set>

class AFD
{
private:

	static const char lambda = '$';

	struct State
	{
		std::string name;
		std::vector<std::pair<char,std::shared_ptr<State>>> connections;
		bool final = false;
	};

	using transition = std::pair<char, std::shared_ptr<State>>;

	std::shared_ptr<State> m_begin;
	std::shared_ptr<State> m_end;
	std::deque<std::shared_ptr<State>> m_transitions;
	std::vector<char> m_alphabet;
	std::vector<std::shared_ptr<State>> m_finalStates;

	void updateIndexes(uint32_t& index);

	void addTransitions(const AFD& other);
	
	friend std::vector<char> getAlphabetUnion(const AFD& afd1, const AFD& afd2);
	

public:

	AFD(char);

	//concatenation
	AFD& operator&=(AFD&);
	//or
	AFD& operator|=(AFD&);
	//kleene star
	AFD& operator++(int);


	friend std::ostream& operator<<(std::ostream&, const AFD&);
};

