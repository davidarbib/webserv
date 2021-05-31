#include "Request.hpp"

int main(void)
{
	Request truc;
	Request machin = truc;
	truc.set_method_token("PROUT");
	truc.add_header("bidule", "truc");
	return 0;
}