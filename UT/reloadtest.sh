#!/bin/zsh
rm -f testcgi
clang++ -Wall -Wextra -Werror -g3 \
	srcs/Request.cpp \
	srcs/Response.cpp \
	srcs/CgiHandler.cpp \
	srcs/AHttpMessage.cpp \
	testcgi.cpp \
-o testcgi -Iincludes
chmod 755 testcgi
./testcgi
