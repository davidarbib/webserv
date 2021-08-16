#include <CgiHandler.hpp>

CgiHandler::CgiHandler(Request &request)
{
	//(void)request;
	request_line startline = request.getStartLine();
	(void)startline;
	//cutting URI in start line for env
	request.get_header_value("Content-Length");
}

CgiHandler::~CgiHandler(void)
{
}

std::string
CgiHandler::extractQuery(std::string requestURI)
{
	return requestURI.substr(requestURI.find(QUERYCHAR) + 1);
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
/*
 * meta variables contents are dquoted
 *
 * meta-variable-name = 
 * "AUTH_TYPE"  = "" (because auth is not handled by webserv)
 * "CONTENT_LENGTH" = from request eponym header or ""
 * "CONTENT_TYPE" = from request media-type
 * "GATEWAY_INTERFACE" = "CGI/1.1"
 * "PATH_INFO" = relative path for resource (e.g. php script path)
 * "PATH_TRANSLATED" = system URI for resource => absolute unix path
 * "QUERY_STRING" = case substring substring after the question mark char or ""
 * "REMOTE_ADDR" = client address
 * "REMOTE_HOST" = client hostname or "" 
 * "REMOTE_IDENT" = identity information optionnal
 * "REMOTE_USER" = user identification string for user authentification
 * 					(mandatory if AUTH_TYPE is "Basic" or "Digest")
 * "REQUEST_METHOD" = http method token 
 * "SCRIPT_NAME" = 
 * "SERVER_NAME" =
 * "SERVER_PORT" =
 * "SERVER_PROTOCOL" =
 * "SERVER_SOFTWARE" = 
*/
