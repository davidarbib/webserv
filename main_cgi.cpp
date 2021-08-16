#include "CgiHandler.hpp"

int main()
{
	Request request;
	request.set_request_URI("http://127.0.0.1:8001?meal=ham");
	CgiHandler handler(request);
	std::cout << handler.extractQuery(request.getStartLine().request_URI) << std::endl;
	//std::cout << handler.extractQuery("klfjdk?kfd") << std::endl;
}
