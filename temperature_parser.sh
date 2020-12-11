#!/bin/bash


#infinite loop
while [ 1 ]
do
#Subscribe to mqtt topic "esp" wait until get two messages(temperature and humidity)
# and store collected data to "tempdata" variable. Quality of service for mqtt messages = 1,
# so there should be always temperature data available when needed.
tempdata=$(mosquitto_sub '-q 1 -t esp/# -C 2')

#throw data to database
mysql -u kalle -p testi <<EOF
USE testi
INSERT INTO saaData(temp, hum, time, date)
VALUES("$tempdata", "1", "CURTIME", "CURDATE");
SELECT * FROM saaData
EOF

done
sleep 1h
done
~
