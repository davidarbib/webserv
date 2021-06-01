#include "Request.hpp"

int main(void)
{
	Request truc;
	truc.set_method_token("PROUT");
	truc.set_method_token("PUT");
	truc.set_method_token("POST");
	truc.set_method_token("GET");
	truc.set_method_token("CLEAR");
	truc.set_method_token("DELETE");
	return 0;
}