FROM debian:buster

ENV WORKING_DIR=/home

RUN apt update --yes \ 
	&& apt upgrade --yes

RUN apt install --yes nginx

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

#---personal page---
COPY index.html /var/www/html/webserv_test.com/
COPY index2.html /var/www/html/webserv_test.com/
COPY init.sh /home


CMD service nginx start \
	&& sh
