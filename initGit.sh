#!/bin/bash

cd $1

rm -rf $2
echo $2
mkdir $2

cd $2

git clone https://github.com/kiskadigitalmedia/kDisplayDesigneSync.git .
git branch $3
git checkout $3

echo "Hello World" > setup.txt 

git add .
git commit -m "commit"
git push origin master $3 --force
