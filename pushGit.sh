#!/bin/bash


cd $1

git add .
git commit --amend --no-edit
git push origin $2 --force