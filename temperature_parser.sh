#!/bin/bash


#infinite loop
while [ 1 ]
do
#Subscribe to mqtt topic "esp" wait until get two messages(temperature and humidity)
# and store collected data to "tempdata" variable. Quality of service for mqtt messages = 1,
# so there should be always temperature data available when needed.
tempdata=$(mosquitto_sub -q 1 -t esp/# -C 2)

#throw data to database
mysql -u root -p ryhma4test <<EOF
USE testi
INSERT INTO saaData(temp, time, date)
VALUES("$tempdata", "CURTIME", "CURDATE");
SELECT * FROM saaData
EOF

done
sleep 1h
done
~
