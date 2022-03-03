#include <unistd.h>
#include <iostream>

int main(void)
{
	std::cout << "COUCOU LES COPAINS" << std::endl;
	char line[32];
	while (read(0, line, 32))
	{
		for (int i = 0; i < 32; i++)
			write(1, &line[i], 1);
	}
	return 0;
}
