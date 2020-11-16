#!/bin/bash


#infinite loop
while [ 1 ]
do
        wget https://www.is.fi/rss/kotimaa.xml
        mv kotimaa.xml il.txt

        lines=$(cat il.txt | grep 'title\|description\|pubDate\|guid' | grep '<title><!' | wc -l) #count the number of articles in file
        #filter out all unnecessary info and save to a file
        cat il.txt | grep '<title><!\|<description><!\|<pubDate>\|guid'| sed -e 's/<ti.*A\[//' -e 's/<pubDate>//' -e 's/\]\].*le>//' -e 's/<desc.*TA\[//' -e 's/<guid.*is-//' -e 's/<\/pu.*e>//' -e's/<\/guid>//' -e 's/\]\]><\/.*n>//' | sed '1d' > il1.txt
        #print the number of articles
        echo $lines

        for ((i = 0 ; i < $lines ; i++)); do

                #find first article title
                title=$(sed -n "$(( $i * 4 + 1))p" il1.txt)
                echo $title

                #find description for the article
                description=$(sed -n "$(( $i * 4 + 2))p" il1.txt)
                echo $description

                #find time
                time=$(sed -n "$(( $i * 4 + 3))p" il1.txt)
                echo $time

                #find guid
                guid=$(sed -n "$(( $i * 4 + 4))p" il1.txt)
                echo $guid

                #needed variables to handle info
                #lines
                #description
                #pubDate
                #guid
                #title

                #check if guid exists already
                #mysql takes care of dedubing

                #throw data to database

                mysql -ukalle -ptesti <<EOF
                USE testi
                INSERT INTO uutiset(otsikko, kuvaus, aika, guid)
                VALUES("$title", "$description", "$time", "$guid");
                SELECT * FROM uutiset
EOF


        done
sleep 2h
done
~           
