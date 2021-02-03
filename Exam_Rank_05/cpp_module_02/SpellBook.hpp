#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

# include <string>
# include <list>
# include "ASpell.hpp" 

class SpellBook
{
private:
	std::list<ASpell *> spells;

public:
	SpellBook();
	SpellBook(const SpellBook &);
	SpellBook &operator=(const SpellBook &);
	~SpellBook();

	void learnSpell(ASpell *);
	void forgetSpell(std::string const &);
	ASpell *createSpell(std::string const &);
};

#endif
