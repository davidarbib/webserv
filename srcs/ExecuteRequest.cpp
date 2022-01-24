#include "ExecuteRequest.hpp"

ExecuteRequest::ExecuteRequest() : _status_code(0)
{
    initMethodList();
}

ExecuteRequest::ExecuteRequest(ExecuteRequest &cpy)
{
    initMethodList();
    *this = cpy;
}

ExecuteRequest &
ExecuteRequest::operator=(ExecuteRequest const& src)
{
    (void)src;
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

int
ExecuteRequest::getStatusCode(void) const
{
    return _status_code;
}

Response
ExecuteRequest::generateResponse(void)
{
    Response response;

    response.buildPreResponse(_status_code);
    return response;
}

void
ExecuteRequest::deleteMethod(std::string const& URI)
{
    std::string uri = "./" + URI;
    std::string deleted_path("./trash/");
    std::ifstream in(uri.c_str(), std::ios::in | std::ios::binary);
    if (in)
    {
        std::ofstream out(deleted_path.append(uri).c_str(), std::ios::out | std::ios::binary);
        out << in.rdbuf();
        std::remove(uri.c_str());
        _status_code = OK;
        return ;
    }
    _status_code = NOT_FOUND;
}