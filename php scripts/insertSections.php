<?php

	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/sections.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
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
		$query="insert into section (name,objName,buildingID,channelID) values (\"$params[0]\",'s.$params[0].$params[1]',(SELECT id FROM building WHERE objName='$params[1]'),'$params[2]')";
		}
		else
		{
		$query="insert into section (name,objName,buildingID,channelID) values (\"$params[0]\",'s.$params[0].$params[1]',(SELECT id FROM building WHERE objName='$params[1]'),(select channelID from building where objName='$params[1]'))";
		}
	echo $query;
		if($result=mysqli_query($link,$query))
			{
				echo "success insert section s.$params[0].$params[1]\r\n";
			}
			else
			{
				echo "error insert section s.$params[0].$params[1]\r\n";
			}

	}
