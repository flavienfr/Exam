#include "ASpell.hpp"

ASpell::ASpell(){
}

ASpell::ASpell(const std::string &name, const std::string &effects): name(name), effects(effects){
}

ASpell::ASpell(const ASpell &the_ASpell){
	*this = the_ASpell;
}

ASpell &ASpell::operator=(const ASpell &the_ASpell){
	name = the_ASpell.name;
	effects = the_ASpell.effects;
	return (*this);
}

ASpell::~ASpell(){
}

std::string ASpell::getName() const{
	return (name);
}
 
std::string ASpell::getEffects() const{
	return (effects);
}

void ASpell::launch(const ATarget &target) const{
	target.getHitBySpell(*this);
}
