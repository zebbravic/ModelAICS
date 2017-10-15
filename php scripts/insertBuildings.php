<?php

	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/buildings.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("�� ������� ������������: %s\n", mysqli_connect_error());
		exit();
	}
	foreach($buildings as $building)
	{
		$params=explode(";",$building);
		$query="insert into building (name, objName, channelID) values (\"$params[0]\",\"b.$params[1]-$params[2]-$params[3]\", (select id from channel where name = '$params[4]'))";
		#echo $query;
		if($result=mysqli_query($link,$query))
			{
				echo "success insert building $params[0]\r\n";
			}
			else
			{
				echo "error insert building $params[0]\r\n";
			}
	}
