<?php
	require_once 'ProgressBar/Manager.php';
	require_once 'ProgressBar/Registry.php';
	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/infrastructure.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	$successCount=0;
	$failedCount=0;
	$progressBar=new \ProgressBar\Manager(0,count($buildings));
	echo "\r\nInfrastructure insert begins\r\n";
	file_put_contents('insertLogs.log'," infrastructure insert begins\r\n",FILE_APPEND);
	foreach($buildings as $building)
	{
		$params=explode(";",$building);
		$query="insert into infrastructure (objName, channelID) values (\"$params[0]\", (select id from channel where name = '$params[1]'))";
		if($result=mysqli_query($link,$query))
		{
			$successCount+=1;
		}
		else
		{
			$err=mysqli_error($link);
			file_put_contents('insertLogs.log',"error insert appartments $params[1]: query is $query, error: $err\r\n",FILE_APPEND);
			$failedCount+=1;
		}
		$progressBar->advance();
	}
	echo "\r\nInfrastructure insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"Appratments insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);
