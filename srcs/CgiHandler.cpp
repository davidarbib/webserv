#include "CgiHandler.hpp"

CgiHandler::CgiHandler(Request &request)
{
	//(void)request;
	request.get_header_value();
}

CgiHandler::~CgiHandler(void)
{
}

//send env variables from request parser
//send body by tmpfiles
//
//receive whole response by tmpfile : 
//	status
//	(CRLFCRLF)
//	body
//
//first milestone : test with php request / response
//
