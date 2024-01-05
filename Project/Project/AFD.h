#pragma once
#include <vector>
#include <string>
#include <queue>
#include <print>
#include <format>
#include <unordered_map>
#include <memory>

class AFD
{
private:

	static const char lambda = '$';

	struct State
	{
		std::string name;
		std::vector<std::pair<char,std::shared_ptr<State>>> connections;
		bool final;
	};

	using transition = std::pair<char, std::shared_ptr<State>>;

	std::shared_ptr<State> m_begin;
	std::shared_ptr<State> m_end;
	std::vector<std::shared_ptr<State>> m_transitions;
	std::vector<char> m_alphabet;
	std::vector<std::shared_ptr<State>> m_finalStates;

public:

	AFD(char);
	~AFD();

	AFD& operator&=(const AFD&);
	AFD& operator|=(const AFD&);
	AFD& operator*=(const AFD&);
	AFD& operator+=(const AFD&);

	friend std::ostream& operator<<(std::ostream&, const AFD&);
};

