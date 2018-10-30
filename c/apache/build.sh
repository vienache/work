#!/bin/bash

# NOTE: compile and install mod_test
sudo apxs -i -a -c modules/mod_test.c
sudo service apache2 restart

# NOTE: disable apache2 mod_test
#sudo a2dismod test
