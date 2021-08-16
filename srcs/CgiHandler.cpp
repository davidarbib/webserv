#include <CgiHandler.hpp>

CgiHandler::CgiHandler(Request &request)
{
	//(void)request;
	request.getStartLine();
	//cutting URI in start line for env
	request.get_header_value("Content-Length");
}

CgiHandler::~CgiHandler(void)
{
}

std::string
extractQuery(std::string requestURI)
{
	return requestURI.substr(requestURI.find(QUERYCHAR));
}

//send env variables from request parser
//	
//send body by tmpfiles
//
//receive whole response by tmpfile : 
//	status
//	(CRLFCRLF)
//	body
//
//first milestone : test with php request / response
//
