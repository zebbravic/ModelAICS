<?php

	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/dout.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("�� ������� ������������: %s\n", mysqli_connect_error());
		exit();
	}
	foreach($buildings as $building)
	{
		$params=explode(";",$building);
		$query="insert into dout (pin,controllerAddress,num) values ($params[1],$params[2],$params[0])";
				echo $query;
		if($result=mysqli_query($link,$query))
			{
				echo "success insert dout num $params[0]\r\n";
			}
			else
			{
				echo "error insert dout num $params[0]\r\n";
			}

	}
	unset($building);
	$buildings=file('csv/segments.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	foreach($buildings as $building)
	{
		$params=explode(";",$building);
		$query="insert into segments (pin,controllerAddress,num,size) values ($params[1],$params[2],$params[0],$params[3])";
				echo $query;
		if($result=mysqli_query($link,$query))
			{
				echo "success insert segment num $params[0]\r\n";
			}
			else
			{
				echo "error insert segment num $params[0]\r\n";
			}

	}
