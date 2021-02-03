#include "SpellBook.hpp"

SpellBook::SpellBook(){
}

SpellBook::~SpellBook(){
	std::list<ASpell *>::iterator it;

	for (it = spells.begin(); it != spells.end(); ++it)
		delete *it;
	spells.clear();
}

void SpellBook::learnSpell(ASpell *spell){
	std::list<ASpell *>::iterator it;

	for (it = spells.begin(); it != spells.end(); ++it)
		if ((*it)->getName() == spell->getName())
			return ;
	spells.push_back(spell->clone());
}

void SpellBook::forgetSpell(std::string const &spell){
	std::list<ASpell *>::iterator it;

	for (it = spells.begin(); it != spells.end(); ++it)
		if ((*it)->getName() == spell){
			delete *it;
			spells.erase(it);
			return ;
		}
}

ASpell *SpellBook::createSpell(std::string const &spell){
	std::list<ASpell *>::iterator it;

	for (it = spells.begin(); it != spells.end(); ++it)
		if ((*it)->getName() == spell){
			return (*it);
		}
	return (NULL);
}
