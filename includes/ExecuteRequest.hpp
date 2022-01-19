#ifndef EXECUTE_REQUEST_HPP
#define EXECUTE_REQUEST_HPP

#include "Response.hpp"
#include "Request.hpp"
#include <fstream>
#include <iostream>
#include <ios>
#include <cstdio>

#define METHOD_NB 3
#define OK 200
#define ACCEPTED 202
#define NO_CONTENT 204
#define NOT_FOUND 404
#define NOT_ALLOWED 405

class ExecuteRequest
{

    private:
        std::string 	_method_list[METHOD_NB];
        Request         *_request;

        ExecuteRequest(void);

        void
		initMethodList(void);

		bool
		isAllowedMethod(std::string const &method) const;

		bool
		isValidMethod(std::string const &method) const;

    public:
        ExecuteRequest(Request *request);
        ExecuteRequest(ExecuteRequest &cpy);
        ExecuteRequest &
        operator=(ExecuteRequest const& src);
        virtual ~ExecuteRequest(void);

        // get / delete / post exec

        int
        deleteMethod(void);
};

#endif