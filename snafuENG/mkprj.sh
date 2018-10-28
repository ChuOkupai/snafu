#!/bin/bash

if [[ $# -eq 0 ]]; then
	echo "$0: Missing project name"
	exit 1
fi

mkdir "$1"
unzip newprj.zip -d "$1"
cd "$1" && mkdir data
mv game.cfg data
sed -i "0,/PROJECT_NAME/ s//$1/" Makefile
cd data && mkdir images
echo "$0: Project $1 created"
