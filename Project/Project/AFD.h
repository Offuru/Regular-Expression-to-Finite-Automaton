#pragma once
#include <vector>
#include <string>
#include <queue>
#include <format>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <set>
#include <ostream>
#include <stack>
#include <iostream>
#include <queue>

class AFD
{
public:
	static const char lambda = '$';

	struct State
	{
		std::string name;
		std::vector<std::pair<char, std::shared_ptr<State>>> transitions;
		bool final = false;
	};

	using enclosure = std::set<std::shared_ptr<State>>;
	using transition = std::pair<char, std::shared_ptr<State>>;

	void printEnclosure(const enclosure& enclosure);

	bool finalEnclosure(const enclosure& enclosure);

private:

	std::shared_ptr<State> m_begin;
	std::shared_ptr<State> m_end;
	std::deque<std::shared_ptr<State>> m_states;
	std::vector<char> m_alphabet;
	std::vector<std::shared_ptr<State>> m_finalStates;

	void updateIndexes(uint32_t& index);

	void addTransitions(const AFD& other);
	
	friend std::vector<char> getAlphabetUnion(const AFD& afd1, const AFD& afd2);

	enclosure getLambdaEnclosure(const enclosure& state) const;

	bool inTransitions(const std::shared_ptr<State>& range,const std::shared_ptr<State>& key);
	
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

	void convertToDeterministic();
	bool verifyAutomaton() const;

	bool checkWord(std::string word) const;

	friend std::ostream& operator<<(std::ostream&, const AFD&);
};

namespace std {
	template <> struct hash<std::shared_ptr<AFD::State>>
	{
		size_t operator()(const std::shared_ptr<AFD::State>& x) const
		{
			return hash<AFD::State*>()(x.get());
		}
	};
}

namespace std {
	template <> struct hash<AFD::enclosure>
	{
		size_t operator()(const AFD::enclosure& x) const
		{
			size_t val = 0;

			for (const auto& state : x)
				val = hash<size_t>()(val + hash<AFD::State*>()(state.get()));

			return val;
		}
	};
}


