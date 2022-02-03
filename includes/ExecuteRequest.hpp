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
#define VERSION_NOT_SUPPORTED 505

class ExecuteRequest
{

    private:
        int             _status_code;

		bool
		isAllowedMethod(std::string const &method, std::vector<std::string> method_allowed) const;

    public:
        ExecuteRequest(void);
        ExecuteRequest(ExecuteRequest &cpy);
        ExecuteRequest &
        operator=(ExecuteRequest const& src);
        virtual ~ExecuteRequest(void);

        int
        getStatusCode(void) const;

        void
        setStatusCode(int status_code);

        bool
        isValidRequest(Request const& request, ConfigServer const& config);

        std::string
        buildBodyPath(void);

        // get / delete / post exec

        std::string
        getMethod(std::string const& URI);

        std::string
        deleteMethod(std::string const& URI);
};

#endif