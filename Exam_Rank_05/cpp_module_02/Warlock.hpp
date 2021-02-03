#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <string>
# include <iostream>
# include "ASpell.hpp"
# include "SpellBook.hpp" 

class Warlock
{
private:
	std::string name;
	std::string title;

	SpellBook spellBook;

public:
	Warlock();
	Warlock(const std::string &name, const std::string &title);
	Warlock(const Warlock &);
	Warlock &operator=(const Warlock &);
	~Warlock();

	const std::string &getName() const;
	const std::string &getTitle() const;

	void setTitle(const std::string &title);
	void introduce() const;

	void learnSpell(ASpell *);
	void forgetSpell(std::string spell);
	void launchSpell(std::string spellName, const ATarget &);
};

#endif