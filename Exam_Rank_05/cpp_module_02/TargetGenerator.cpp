#include "TargetGenerator.hpp"

TargetGenerator::TargetGenerator(){
}

TargetGenerator::~TargetGenerator(){
}

void TargetGenerator::learnTargetType(ATarget *target){
	std::list<ATarget *>::iterator it;

	for (it = targets.begin(); it != targets.end(); ++it)
		if ((*it)->getType() == target->getType())
			return ;
	targets.push_back(target->clone());
}

void TargetGenerator::forgetTargetType(std::string const &type){
	std::list<ATarget *>::iterator it;

	for (it = targets.begin(); it != targets.end(); ++it)
		if ((*it)->getType() == type){
			delete *it;
			targets.erase(it);
			return ;
		}
}

ATarget *TargetGenerator::createTarget(std::string const &type){
	std::list<ATarget *>::iterator it;

	for (it = targets.begin(); it != targets.end(); ++it)
		if ((*it)->getType() == type){
			return (*it);
		}
	return (NULL);
}
