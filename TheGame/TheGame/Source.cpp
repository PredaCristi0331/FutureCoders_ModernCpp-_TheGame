#include <iostream>
#include "Card.h"
#include "Jucator.h"
int main() {
	Jucator J1;
	for (int i = 2; i < 6; i++)
	{
		Card card(i);
		J1.AddCard(card);
	}
	return 0;
}