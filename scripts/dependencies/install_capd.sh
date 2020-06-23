#!/bin/bash

sudo apt-get install autoconf

cd $HOME
echo 'Installing CAPD in ' $HOME '...';
if [ ! -e "capd/README.md" ]; then
  svn co https://svn.capdnet.ii.uj.edu.pl/capd/ --non-interactive --trust-server-cert-failures=unknown-ca,cn-mismatch,expired,not-yet-valid,other ;
  cd capd ;
  svn co -r 832 https://svn.capdnet.ii.uj.edu.pl/capdDynSys4 --non-interactive --trust-server-cert-failures=unknown-ca,cn-mismatch,expired,not-yet-valid,other ;
  autoreconf --install ;
  ./configure --with-filib=check --with-mpfr=check ;
  make ;
else
  echo 'Using cached directory.' ;
fi
cd $HOME/capd
sudo make install
cd $HOME