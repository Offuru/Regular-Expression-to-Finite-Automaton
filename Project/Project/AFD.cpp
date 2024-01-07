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
		for (const auto& next : state->transitions)
			states.push(next.second);
	}
}

void AFD::addTransitions(const AFD& other)
{
	for (const auto& state : other.m_states)
		m_states.push_back(state);
}

bool AFD::finalStatesInStates() const
{
	return std::all_of(m_finalStates.cbegin(), m_finalStates.cend(), [&](const auto& finalState)
					   {
						   return std::find(m_states.cbegin(), m_states.cend(), finalState) != m_states.cend();
					   });
}

bool AFD::validTransitions() const
{
	return std::all_of(m_states.cbegin(), m_states.cend(), [&](const auto& state)
					   {
						   for (const auto& transition : state->transitions)
						   {
							   const auto& [chr, resultState] = transition;

							   if ((std::find(m_alphabet.cbegin(), m_alphabet.cend(), chr) == m_alphabet.cend()))
								   return false;
							   if (std::find(m_states.cbegin(), m_states.cend(), resultState) == m_states.cend())
								   return false;
						   }
						   
						   return true;
					   });
}

bool AFD::isDeterministic() const
{
	return std::all_of(m_states.cbegin(), m_states.cend(), [&](const auto& state)
					   {
						   std::unordered_map<char, int> countAlphabet;
						   for (const auto& transition : state->transitions)
						   {
							   const auto& [chr, resultState] = transition;

							   if (chr == lambda && state != resultState)
								   return false;

							   countAlphabet[transition.first]++;
						   }

						   for (const auto& chr : countAlphabet)
						   {
							   if (chr.second > 1)
								   return false;
						   }

						   return true;
					   });
}

AFD::AFD(char symbol)
{
	m_begin = std::make_shared<State>();
	m_end = std::make_shared<State>();

	m_begin->transitions.emplace_back(symbol, m_end);
	m_begin->name = "q0";

	m_end->name = "q1";
	m_end->final = true;

	m_alphabet.push_back(symbol);
	m_states.push_back(m_begin);
	m_states.push_back(m_end);
	m_finalStates.push_back(m_end);
}

void AFD::addDotsToRegex(std::string& regex)
{
	std::string newRegex = "";
	for (auto it = regex.begin(); it != regex.end(); ++it)
	{
		newRegex += *it;
		if (*it != '.' && *it != '(' && *it != '|')
		{
			if (it + 1 != regex.end())
			{
				char nextValue = *(it + 1);
				if (nextValue != '.' && nextValue != ')' && nextValue != '*' && nextValue != '|')
				{
					newRegex += '.';
				}
			}
		}
	}

	regex = newRegex;
}

int AFD::getPriority(char ch) const
{
	switch (ch)
	{
	case '(': return 0;
	case '|': return 1;
	case '.': return 2;
	case '*': return 3;
	}

	return -100;
}

AFD::AFD(const std::string& regex)
{
	std::string reg = regex;

	//remove unnecessary dots
	std::erase(reg, '.');
	addDotsToRegex(reg);

	std::vector<char> polishForm;
	std::stack<char> signs;

	for (auto ch : reg)
	{
		if (ch != '.' && ch != '|' && ch != '(' && ch != ')' && ch != '*')
		{
			polishForm.push_back(ch);
		}
		else if (ch == '(')
			signs.push(ch);
		else if (ch == ')')
		{
			while (!signs.empty() && signs.top() != '(')
			{
				polishForm.push_back(signs.top());
				signs.pop();
			}
			signs.pop();
		}
		else
		{
			while (!signs.empty() && getPriority(signs.top()) >= getPriority(ch))
			{
				polishForm.push_back(signs.top());
				signs.pop();
			}
			signs.push(ch);
		}
	}
	while (!signs.empty())
	{
		polishForm.push_back(signs.top());
		signs.pop();
	}

	std::stack<AFD> finalEval;
	for (auto ch : polishForm)
	{
		if (getPriority(ch) == -100)
		{
			AFD z(ch);
			finalEval.push(z);
		}
		else
		{
			if (ch == '*')
			{
				AFD y = finalEval.top();
				finalEval.pop();
				y++;
				finalEval.push(y);
			}
			else
			{
				AFD y = finalEval.top();
				finalEval.pop();
				AFD x = finalEval.top();
				finalEval.pop();

				if (ch == '|')
					x |= y;
				else if (ch == '.')
					x &= y;

				finalEval.push(x);
			}
		}
	}

	*this = finalEval.top();
}

AFD& AFD::operator&=(AFD& other)
{
	uint32_t index = std::stoi(m_end->name.substr(1, m_end->name.size() - 1)) + 1;

	other.updateIndexes(index);
	m_alphabet = getAlphabetUnion(*this, other);
	addTransitions(other);

	m_end->final = false;
	m_end->transitions.emplace_back(lambda, other.m_begin);
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
	newStart->transitions.emplace_back(lambda, m_begin);
	newStart->transitions.emplace_back(lambda, other.m_begin);

	m_states.push_front(newStart);

	addTransitions(other);
	
	std::shared_ptr<State> newEnd = std::make_shared<State>();
	newEnd->name = "q" + std::to_string(index);
	newEnd->final = true;

	m_states.push_back(newEnd);

	m_end->transitions.emplace_back(lambda, newEnd);
	m_end->final = false;
	other.m_end->transitions.emplace_back(lambda, newEnd);

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
	newStart->transitions.emplace_back(lambda, m_begin);

	m_states.push_front(newStart);

	std::shared_ptr<State> newEnd = std::make_shared<State>();
	newEnd->name = "q" + std::to_string(index);
	newEnd->final = true;

	m_end->transitions.emplace_back(lambda, m_begin);
	m_end->transitions.emplace_back(lambda, newEnd);
	m_end->final = false;

	m_begin = newStart;
	m_end = newEnd;

	m_begin->transitions.emplace_back(lambda, m_end);

	m_states.push_back(newEnd);

	m_finalStates = { m_end };

	return *this;
}

bool AFD::verifyAutomaton() const
{
	if (m_states.empty() || m_alphabet.empty() || m_finalStates.empty())
		return false;

	if (std::find(m_states.cbegin(), m_states.cend(), m_begin) == m_states.cend())
		return false;

	if (!finalStatesInStates())
		return false;

	if (!isDeterministic())
		return false;

	if (!validTransitions())
		return false;

	return true;
}

bool AFD::checkWord(std::string word) const
{
	uint32_t indexWord = 0;
	std::shared_ptr currentState = m_begin;

	while (indexWord < word.size())
	{
		bool foundTransition = false;
		for (const auto& transition : currentState->transitions)
		{
			if (transition.first == word[indexWord])
			{
				currentState = transition.second;
				foundTransition = true;
				break;
			}
		}
		if (!foundTransition) return false;
		++indexWord;
	}
	if (!currentState->final)
		return false;
	return true;
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
	for (const auto& state : afd.m_states)
		out << std::format("{} ", state->name);

	out << std::format("\n\nAlphabet:\n");
	for (const auto& letter : afd.m_alphabet)
		out << std::format("{} ", letter);

	out << std::format("\n\nTransitions:\n");
	for (const auto& state : afd.m_states)
		for (const auto& next : state->transitions)
			out << std::format("({0},{1}) -> {2}\n", state->name, next.first, next.second->name);

	out << std::format("\nStart state:\n{}\n", afd.m_begin->name);

	out << std::format("\nFinal states:\n");
	for (const auto& state : afd.m_finalStates)
		out << std::format("{} ", state->name);

	return out;
}