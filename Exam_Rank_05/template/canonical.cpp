#include "ChangeMe.hpp"

ChangeMe::ChangeMe(){
}

ChangeMe::ChangeMe(const ChangeMe &the_ChangeMe){
	*this = the_ChangeMe;
}

ChangeMe &ChangeMe::operator=(const ChangeMe &the_ChangeMe){
	return (*this);
}

ChangeMe::~ChangeMe(){
}
