#include "AFD.h"

AFD::AFD(char symbol)
{
	m_begin = std::make_shared<State>();
	m_end = std::make_shared<State>();

	m_begin->connections.emplace_back(symbol, m_end);
	m_begin->name = "q0";

	m_end->name = "q1";
	m_end->final = true;

	m_alphabet.push_back(symbol);
	m_transitions.push_back(m_begin);
	m_transitions.push_back(m_end);
	m_finalStates.push_back(m_end);
}

AFD::~AFD()
{
	std::queue<std::shared_ptr<State>> states;
	states.push(m_begin);

	while (!states.empty())
	{
		State* state = states.front().get();
		states.pop();

		for (const auto& next : state->connections)
			if (next.second)
				states.push(next.second);
	}
}

AFD& AFD::operator&=(const AFD& other)
{
	uint32_t index = std::stoi(m_end->name.substr(1, m_end->name.size() - 1)) + 1;

	std::queue<std::shared_ptr<State>> states;

	states.push(other.m_begin);

	while (!states.empty())
	{
		State* state = states.front().get();
		states.pop();
		state->name = "q" + std::to_string(index++);
		for (const auto& next : state->connections)
			states.push(next.second);
	}

	m_end->final = false;
	m_end->connections.emplace_back(lambda, other.m_begin);
	m_end = other.m_end;

	std::vector<char> alphabet;

	std::set_union(m_alphabet.cbegin(), m_alphabet.cend(),
				   other.m_alphabet.cbegin(), other.m_alphabet.cend(),
				   std::back_inserter(alphabet));

	m_alphabet = alphabet;

	for (const auto& transition : other.m_transitions)
		m_transitions.push_back(transition);

	m_finalStates = other.m_finalStates;

	return *this;
}

std::ostream& operator<<(std::ostream& out, const AFD& afd)
{
	std::print("States:\n");
	for (const auto& state : afd.m_transitions)
		std::print("{} ", state->name);

	std::println("\n\nAlphabet:");
	for (const auto& letter : afd.m_alphabet)
		std::print("{} ", letter);

	std::println("\n\nTransitions:");
	for (const auto& state : afd.m_transitions)
		for (const auto& next : state->connections)
			std::println("({0},{1}) -> {2}", state->name, next.first, next.second->name);

	std::println("\nStart state:\n{}", afd.m_begin->name);

	std::println("\nFinal states:");
	for (const auto& state : afd.m_finalStates)
		std::print("{} ", state->name);

	return out;
}
