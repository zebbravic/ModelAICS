<?php

	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/channels.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("�� ������� ������������: %s\n", mysqli_connect_error());
		exit();
	}
	foreach($buildings as $building)
	{
		$params=explode(";",$building);
		$query="insert into channel (name) values ('$params[0]')";
		if($result=mysqli_query($link,$query))
			{
				echo "success insert channel $params[0]\r\n";
			}
			else
			{
				echo "error insert channel $params[0]\r\n";
			}

	}
