#include "Warlock.hpp"

Warlock::Warlock(const std::string &name, const std::string &title): name(name), title(title){
	std::cout << name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock(){
	std::list<ASpell *>::iterator it;

	for (it = spells.begin(); it != spells.end(); ++it)
		delete *it;
	spells.clear();

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
	std::list<ASpell *>::iterator it;

	for (it = spells.begin(); it != spells.end(); ++it)
		if ((*it)->getName() == spell->getName())
			return ;
	spells.push_back(spell->clone());
}

void Warlock::forgetSpell(std::string spell){
	std::list<ASpell *>::iterator it;

	for (it = spells.begin(); it != spells.end(); ++it)
		if ((*it)->getName() == spell){
			delete *it;
			spells.erase(it);
			return ;
		}
}

void Warlock::launchSpell(std::string spell, const ATarget &target){
	std::list<ASpell *>::iterator it;

	for (it = spells.begin(); it != spells.end(); ++it)
		if ((*it)->getName() == spell){
			(*it)->launch(target);
			return ;
		}
}