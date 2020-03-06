#!/bin/bash

$folder=$1
$branch=$2

rm -rf $folder
mkdir $folder

cd $folder

git clone https://github.com/kiskadigitalmedia/kDisplayDesigneSync.git
git branch $branch
git checkout $branch

echo "Hello World" > setup.txt 

git add .
git commit -m "commit"
git push origin master $branch --force
