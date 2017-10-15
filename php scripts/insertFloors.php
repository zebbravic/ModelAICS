<?php

	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/floors.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("�� ������� ������������: %s\n", mysqli_connect_error());
		exit();
	}
	foreach($buildings as $building)
	{
		$params=explode(";",$building);
		if($params[2]!='')
		{
		$query="insert into floor (numInBuilding, objName,buildingID, channelID) values ($params[1],'f.$params[1].$params[0]',(SELECT id FROM building WHERE objName='$params[0]'),(select id from channel where name='$params[2]'))";
		}
		else
		{
		$query="insert into floor (numInBuilding, objName,buildingID, channelID) values ($params[1],'f.$params[1].$params[0]',(SELECT id FROM building WHERE objName='$params[0]'),(select channelID from building where objName='$params[0]'))";
		}
		if($result=mysqli_query($link,$query))
			{
				echo "success insert floor f.$params[1].$params[0]\r\n";
			}
			else
			{
				echo "error insert floor f.$params[1].$params[0]\r\n";
			}

	}
