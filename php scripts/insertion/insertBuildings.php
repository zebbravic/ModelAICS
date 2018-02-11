<?php
	require_once 'ProgressBar/Manager.php';
	require_once 'ProgressBar/Registry.php';
	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/buildings.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$successCount=0;
	$failedCount=0;
	echo "Building insert begins\r\n";
	$progressBar=new \ProgressBar\Manager(0,count($buildings));
	file_put_contents('insertLogs.log',"building insert begins\r\n",FILE_APPEND);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	foreach($buildings as $building)
	{
		$params=explode(";",$building);
		if($params[4]!='-')
		{
			$query="insert into building (name, objName, channelID, CRMAddressBuild, CRMBuildingID) values (\"$params[0]\",\"b.$params[1]-$params[2]-$params[3]\", (select id from channel where name = '$params[4]') , $params[5], $params[6])";
		}
		else
		{
			$query="insert into building (name, objName,  CRMAddressBuild, CRMBuildingID) values (\"$params[0]\",\"b.$params[1]-$params[2]-$params[3]\", $params[5], $params[6])";
		}
		if($result=mysqli_query($link,$query))
		{
			$successCount+=1;
		}
		else
		{
			$err=mysqli_error($link);
			file_put_contents('insertLogs.log',"error insert building $params[0]: query is $query, error: $err\r\n",FILE_APPEND);
			$failedCount+=1;
		}
		$progressBar->advance();
	}
	echo "\r\nBuilding insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"Building insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);
