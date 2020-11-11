#!/bin/bash

echo starting script
lines=$(cat il.txt | grep 'title\|description\|pubDate\|guid' | grep '<title><!' | wc -l) #count the number of articles in file
#filter out all unnecessary info and save to a file
cat il.txt | grep '<title><!\|<description><!\|<pubDate>\|guid'| sed -e 's/<ti.*A\[//' -e 's/<pubDate>//' -e 's/\]\].*le>//' -e 's/<desc.*TA\[//' -e 's/<guid.*is-//' -e 's/<\/pu.*e>//' -e's/<\/guid>//' -e 's/\]\]><\/.*n>//' | sed '1d' > il1.txt
#print the number of articles
echo $lines

#find first article title
title=$(sed -n '1p' il1.txt)
echo $title

#find description for the article
description=$(sed -n '2p' il1.txt)
echo $description

#find time
time=$(sed -n '3p' il1.txt)
echo $time

#find guid
guid=$(sed -n '4p' il1.txt)
echo $guid
#needed variables to handle info
#lines
#description
#pubDate
#guid
#title




