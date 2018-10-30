#!/bin/bash

echo 'Install Apache2 module dependencies...'
sudo a2enmod headers
sudo a2enmod proxy
sudo a2enmod proxy_balancer
sudo a2enmod proxy_connect
sudo a2enmod proxy_http
sudo a2enmod rewrite
sudo a2enmod ssl
echo

echo 'Copy apache2 configuration to /etc/apache2...'
sudo cp etc/apache2/apache2.conf /etc/apache2
sudo cp etc/apache2/conf/* /etc/apache2/conf
echo

echo 'Restart Apache2...'
sudo service apache2 restart
