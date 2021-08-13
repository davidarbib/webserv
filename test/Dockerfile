# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: darbib <darbib@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/22 10:51:12 by darbib            #+#    #+#              #
#    Updated: 2021/08/06 17:24:09 by darbib           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FROM debian:stable-20210721-slim

ENV WORKING_DIR=/home

RUN apt update --yes \ 
	&& apt upgrade --yes

RUN apt install --yes nginx \
	libxml2-dev \
	php \
	php-fpm \
	curl \
	vim

EXPOSE 80/tcp
EXPOSE 8001/tcp
EXPOSE 8002/tcp
EXPOSE 8003/tcp

#---site creation---
RUN mkdir -p /var/www/html/webserv_test.com

#---configure Nginx---
RUN rm -f /etc/nginx/sites-enabled/default \
	&& ln -s /etc/nginx/sites-available/webserv_test.com /etc/nginx/sites-enabled/

COPY nginx.conf /etc/nginx/sites-available/webserv_test.com
#COPY lighttpd.conf /etc/lighttpd

#---personal page---
COPY index.html /var/www/html/webserv_test.com
COPY action.php /var/www/html/webserv_test.com
COPY fakephp /home
COPY init.sh /home

#ENTRYPOINT sh /home/init.sh

CMD service nginx start \
	&& service php7.3-fpm start \
	&& sh

#CMD service lighttpd start \
#	&& sh

#CMD lighty-enable-mod cgi \
#	&& service lighttpd start \
#	&& sh
