#ifndef EXECUTE_REQUEST_HPP
#define EXECUTE_REQUEST_HPP

#include "Response.hpp"
#include "Request.hpp"
#include "ConfigServer.hpp"
#include <fstream>
#include <iostream>
#include <ios>
#include <cstdio>

#define OK 200
#define ACCEPTED 202
#define NO_CONTENT 204
#define BAD_REQUEST 400
#define NOT_FOUND 404
#define NOT_ALLOWED 405
#define PAYLOAD_TO_LARGE 413
#define NOT_IMPLEMENTED 501
#define VERSION_NOT_SUPPORTED 505
#define HTTP_METHOD_NOT_IMPLEMENTED_NB 6

class ExecuteRequest
{

    private:
        int             _status_code;

		bool
		isAllowedMethod(std::string const &method, std::vector<std::string> method_allowed) const;


    public:
        static std::string method_not_implemented[HTTP_METHOD_NOT_IMPLEMENTED_NB];
        ExecuteRequest(void);
        ExecuteRequest(ExecuteRequest &cpy);
        ExecuteRequest &
        operator=(ExecuteRequest const &src);
        virtual ~ExecuteRequest(void);

        static void
        fillMethodNotImplemented(void);

        int
        getStatusCode(void) const;

        void
        setStatusCode(int status_code);

        bool
        isValidRequest(Request const &request, ConfigServer const &config, ServerLocations const& location);

        bool
        isImplemented(std::string const &method) const;

        std::string
        buildBodyPath(void);

        // get / delete / post exec

        std::string
        getMethod(std::string const &URI, ConfigServer const &config, ServerLocations const& location);

        std::string
        deleteMethod(std::string const &URI, ConfigServer const &config, ServerLocations const& location);
};

#endif