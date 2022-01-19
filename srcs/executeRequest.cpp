#include "executeRequest.hpp"

ExecuteRequest::ExecuteRequest(Request *request)
{
    _request = new Request();
    _request = request;
    initMethodList();
}

ExecuteRequest::ExecuteRequest(ExecuteRequest &cpy)
{
    initMethodList();
    *this = cpy;
}

ExecuteRequest &
ExecuteRequest::operator=(ExecuteRequest &cpy)
{
    _request = cpy.request;
    return *this;
}

ExecuteRequest::~ExecuteRequest(void)
{
}

void
ExecuteRequest::initMethodList(void)
{
	this->_method_list[0] = "GET";
	this->_method_list[1] = "POST";
	this->_method_list[2] = "DELETE";
}

bool
ExecuteRequest::isAllowedMethod(std::string const &method) const
{
	for (int i = 0; i < METHOD_NB; i++)
	{
		if (method == this->_method_list[i])
			return true;
	}
	return false;
}

bool
ExecuteRequest::isValidMethod(std::string const &method) const
{
	for (size_t i = 0; i < method.length(); i++)
	{
		if (std::isupper(method[i]) == 0)
			return false;
	}
	return true;
}