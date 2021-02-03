#include "ATarget.hpp"

ATarget::ATarget(){
}

ATarget::ATarget(const std::string &type): type(type){
}

ATarget::ATarget(const ATarget &the_ATarget){
	*this = the_ATarget;
}

ATarget &ATarget::operator=(const ATarget &the_ATarget){
	type = the_ATarget.type;
	return (*this);
}

ATarget::~ATarget(){
}

const std::string &ATarget::getType() const{
	return (type);
}

void ATarget::getHitBySpell(const ASpell &spell) const{
	std::cout << type << " has been " << spell.getEffects() << "!" << std::endl;
}