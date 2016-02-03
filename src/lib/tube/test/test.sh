#!/bin/sh

echo "\nTube: building...\n";

  mkdir -p make ;
  cd make ;
  cmake .. ;
  make ;

echo "\nTube: testing...\n";

  ./testTube

echo ""
cd ..