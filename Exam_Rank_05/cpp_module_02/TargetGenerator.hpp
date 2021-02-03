#ifndef TARGETGENERATOR_HPP
# define TARGETGENERATOR_HPP

# include "ATarget.hpp"
# include <string>
# include <list>

class TargetGenerator
{
private:
	std::list<ATarget *> targets;

public:
	TargetGenerator();
	TargetGenerator(const TargetGenerator &);
	TargetGenerator &operator=(const TargetGenerator &);
	~TargetGenerator();

	void learnTargetType(ATarget *);
	void forgetTargetType(std::string const &);
	ATarget *createTarget(std::string const &);
};

#endif
