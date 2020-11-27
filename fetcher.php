<?php
$dbhost = 'stulinux108.ipt.oamk.fi';
$dbuser = 'kalle';
$dbpass = 'testi';
$conn = myspl_connection($dbhost, $dbuser, $dbpass);
echo "Testi";
if (! $conn)	{
	die('Could not connect: ' . mysql_error());
}
$sql = 'SELECT otsikko, kuvaus, aika FROM uutiset';

mysql_select_db('testi');
$retval = mysql_query($sql,$conn);
if(!$retval)	{
	die('Could not get data: ' . mysql_error());
}
while($row = mysql_fetch_assoc($retval))	{
	echo "otsikko :{$row['otsikko']} <br>".
		"kuvaus: {$row['kuvaus']} <br>".
		"aika:	{$row['aika']} <br> ".
		"<br>";
}
echo "Fetched data successfully/n";
mysql_close($conn);
?>