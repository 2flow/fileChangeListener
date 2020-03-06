#!/bin/bash

$folder=$1
$branch=$2

cd $folder

git add .
git commit --amend --no-edit
git push origin $branch --force