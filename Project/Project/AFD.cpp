#include "AFD.h"

void AFD::updateIndexes(uint32_t& index)
{
	std::queue<std::shared_ptr<State>> states;
	std::unordered_set<State*> added;

	states.push(m_begin);

	while (!states.empty())
	{
		State* state = states.front().get();
		states.pop();
		if (added.find(state) != added.end())
			continue;
		added.insert(state);
		state->name = "q" + std::to_string(index++);
		for (const auto& next : state->connections)
			states.push(next.second);
	}
}

void AFD::addTransitions(const AFD& other)
{
	for (const auto& state : other.m_transitions)
		m_transitions.push_back(state);
}

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

AFD& AFD::operator&=(AFD& other)
{
	uint32_t index = std::stoi(m_end->name.substr(1, m_end->name.size() - 1)) + 1;

	other.updateIndexes(index);
	m_alphabet = getAlphabetUnion(*this, other);
	addTransitions(other);

	m_end->final = false;
	m_end->connections.emplace_back(lambda, other.m_begin);
	m_end = other.m_end;

	m_finalStates = other.m_finalStates;
	
	return *this;
}

AFD& AFD::operator|=(AFD& other)
{
	uint32_t index = 1;
	updateIndexes(index);
	other.updateIndexes(index);
	other.m_end->final = false;

	m_alphabet = getAlphabetUnion(*this, other);

	std::shared_ptr<State> newStart = std::make_shared<State>();
	newStart->name = "q0";
	newStart->connections.emplace_back(lambda, m_begin);
	newStart->connections.emplace_back(lambda, other.m_begin);

	m_transitions.push_front(newStart);

	addTransitions(other);
	
	std::shared_ptr<State> newEnd = std::make_shared<State>();
	newEnd->name = "q" + std::to_string(index);
	newEnd->final = true;

	m_transitions.push_back(newEnd);

	m_end->connections.emplace_back(lambda, newEnd);
	other.m_end->connections.emplace_back(lambda, newEnd);

	m_begin = newStart;
	m_end = newEnd;

	m_finalStates = { m_end };

	return *this;
}

AFD& AFD::operator++(int)
{
	uint32_t index = 1;
	updateIndexes(index);

	std::shared_ptr<State> newStart = std::make_shared<State>();
	newStart->name = "q0";
	newStart->connections.emplace_back(lambda, m_begin);

	m_transitions.push_front(newStart);

	std::shared_ptr<State> newEnd = std::make_shared<State>();
	newEnd->name = "q" + std::to_string(index);
	newEnd->final = true;

	m_end->connections.emplace_back(lambda, m_begin);
	m_end->connections.emplace_back(lambda, newEnd);
	m_end->final = false;

	m_begin = newStart;
	m_end = newEnd;

	m_begin->connections.emplace_back(lambda, m_end);

	m_transitions.push_back(newEnd);

	m_finalStates = { m_end };

	return *this;
}

std::vector<char> getAlphabetUnion(const AFD& afd1, const AFD& afd2)
{
	std::vector<char> alphabet;

	std::set_union(afd1.m_alphabet.cbegin(), afd1.m_alphabet.cend(),
		afd2.m_alphabet.cbegin(), afd2.m_alphabet.cend(),
		std::back_inserter(alphabet));

	return alphabet;
}

std::ostream& operator<<(std::ostream& out, const AFD& afd)
{
	out << std::format("States:\n");
	for (const auto& state : afd.m_transitions)
		out << std::format("{} ", state->name);

	out << std::format("\n\nAlphabet:\n");
	for (const auto& letter : afd.m_alphabet)
		out << std::format("{} ", letter);

	out << std::format("\n\nTransitions:\n");
	for (const auto& state : afd.m_transitions)
		for (const auto& next : state->connections)
			out << std::format("({0},{1}) -> {2}\n", state->name, next.first, next.second->name);

	out << std::format("\nStart state:\n{}\n", afd.m_begin->name);

	out << std::format("\nFinal states:\n");
	for (const auto& state : afd.m_finalStates)
		out << std::format("{} ", state->name);

	return out;
}