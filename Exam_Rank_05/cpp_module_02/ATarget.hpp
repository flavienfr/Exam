#ifndef ATARGET_HPP
# define ATARGET_HPP

# include <string>
# include <iostream>
# include "ASpell.hpp"

class ASpell;

class ATarget
{
private:
	std::string type;
public:
	ATarget();
	ATarget(const std::string &type);
	ATarget(const ATarget &);
	ATarget &operator=(const ATarget &);
	virtual ~ATarget();

	const std::string &getType() const;
	virtual ATarget *clone() const = 0;

	void getHitBySpell(const ASpell &) const;
};

#endif
