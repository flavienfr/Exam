#ifndef ASPELL_HPP
# define ASPELL_HPP

# include <string>
# include "ATarget.hpp"

class ATarget;

class ASpell
{
protected:
	std::string name;
	std::string effects;

public:
	ASpell();
	ASpell(const std::string &name, const std::string &effects);
	ASpell(const ASpell &);
	ASpell &operator=(const ASpell &);
	~ASpell();

	std::string getName() const;
	std::string getEffects() const;

	virtual ASpell *clone() const = 0;
	void launch(const ATarget &) const;
};

#endif
