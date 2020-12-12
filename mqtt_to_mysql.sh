#!/bin/bash

while [ 1 ]
do
        temp=$(mosquitto_sub -t esp/temp -C 1 -q 1)

        echo $temp
        hum=$(mosquitto_sub -t esp/hum -C 1 -q 1)
        echo $hum

        mysql -urpi -ptesti -hstulinux108.ipt.oamk.fi <<EOF
        USE testi
        INSERT INTO saaData(temp, hum, time, date)
        VALUES("$temp", "$hum", CURTIME(), CURDATE());
EOF
sleep 10m
done
