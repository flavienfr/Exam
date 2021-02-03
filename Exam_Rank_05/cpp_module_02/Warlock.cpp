#include "Warlock.hpp"

Warlock::Warlock(const std::string &name, const std::string &title): name(name), title(title){
	std::cout << name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock(){
	std::cout << name << ": My job here is done!" << std::endl;
}

const std::string &Warlock::getName() const{
	return (name);
}

const std::string &Warlock::getTitle() const{
	return (title);
}

void Warlock::setTitle(const std::string &title){
	this->title = title;
}

void Warlock::introduce() const{
	std::cout << name << ": I am " << name << ", " << title << "!" << std::endl;
}

void Warlock::learnSpell(ASpell *spell){
	spellBook.learnSpell(spell);
}

void Warlock::forgetSpell(std::string spell){
	spellBook.forgetSpell(spell);
}

void Warlock::launchSpell(std::string spell, const ATarget &target){
	ASpell *the_spell;
	
	the_spell = spellBook.createSpell(spell);
	if (the_spell)
		the_spell->launch(target);
}
