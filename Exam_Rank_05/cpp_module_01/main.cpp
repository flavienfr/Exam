#include "Warlock.hpp"
#include "Dummy.hpp"
#include "Fwoosh.hpp"
#include <string>
#include <iostream>

int main()
{	
	{
		Dummy const bob;		//Compiles
		Fwoosh const fwoosh;	//Compiles

		//All these functions can be called on a constant object.
		bob.getType();
		bob.clone();
		bob.getHitBySpell(fwoosh);

		fwoosh.getName();
		fwoosh.getEffects();
		fwoosh.clone();
		fwoosh.launch(bob);
	}
	{
	  Warlock richard("Richard", "the Titled");

	  Dummy bob;
	  Fwoosh* fwoosh = new Fwoosh();

	  richard.learnSpell(fwoosh);

	  richard.introduce();
	  richard.launchSpell("Fwoosh", bob);

	  richard.forgetSpell("Fwoosh");
	  richard.launchSpell("Fwoosh", bob);
	}

	return (0);
}