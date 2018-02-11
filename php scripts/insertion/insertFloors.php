<?php
	require_once 'ProgressBar/Manager.php';
	require_once 'ProgressBar/Registry.php';
	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$floors=array();
	$floors=file('csv/floors.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	$successCount=0;
	$failedCount=0;
	echo "Section insert begins\r\n";
	$progressBar=new \ProgressBar\Manager(0,count($floors));
	file_put_contents('insertLogs.log',"floor insert begins\r\n",FILE_APPEND);
	foreach($floors as $floor)
	{
		$params=explode(";",$floor);
		if($params[2]!='-')
		{
			if($params[3]=='-')
			{
				$query="insert into floor (numInBuilding, objName,buildingID, channelID) values ($params[1],'f.$params[1].$params[0]',(SELECT id FROM building WHERE objName='$params[0]'),(select id from channel where name='$params[2]'))";
			}
			else
			{
				$query="insert into floor (numInBuilding, objName,buildingID, channelID, sectionID) values ($params[1],'f.$params[1].s.$params[3].$params[0]',(SELECT id FROM building WHERE objName='$params[0]'),(select id from channel where name='$params[2]'), (select id from section where objName='s.$params[3].$params[0]'))";
			}
		}
		else
		{
			if($params[3]=='-')
			{
				$query="insert into floor (numInBuilding, objName,buildingID, channelID) values ($params[1],'f.$params[1].$params[0]',(SELECT id FROM building WHERE objName='$params[0]'),(select channelID from building where objName='$params[0]'))";
			}
			else
			{
				$query="insert into floor (numInBuilding, objName,buildingID, channelID, sectionID) values ($params[1],'f.$params[1].s.$params[3].$params[0]',(SELECT id FROM building WHERE objName='$params[0]'),(select channelID from building where objName='$params[0]'), (select id from section where objName='s.$params[3].$params[0]'))";
			}
		}
		if($result=mysqli_query($link,$query))
		{
			$successCount+=1;
		}
		else
		{
			$err=mysqli_error($link);
			file_put_contents('insertLogs.log',"error insert floor $params[1]: query is $query, error: $err\r\n",FILE_APPEND);
			$failedCount+=1;
		}
		$progressBar->advance();
	}
	echo "\r\nFloor insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"Floor insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);
