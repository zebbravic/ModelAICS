<?php
	require_once 'ProgressBar/Manager.php';
	require_once 'ProgressBar/Registry.php';
	$hostname = "localhost";
	$username = "installer";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$sections=array();
	$sections=file('csv/sections.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	$successCount=0;
	$failedCount=0;
	echo "Section insert begins\r\n";
	$progressBar=new \ProgressBar\Manager(0,count($sections));
	file_put_contents('insertLogs.log',"building insert begins\r\n",FILE_APPEND);
	foreach($sections as $section)
	{
		$params=explode(";",$section);
		if($params[2]!='-')
		{
			$query="insert into section (name,objName,buildingID,channelID, CRMSectionName, CRMSectionID) values (\"$params[0]\",'s.$params[0].$params[1]',(SELECT id FROM building WHERE objName='$params[1]'),'$params[2]',\"$params[3]\", \"$params[4]\")";
		}
		else
		{
			$query="insert into section (name,objName,buildingID,channelID, CRMSectionName, CRMSectionID) values (\"$params[0]\",'s.$params[0].$params[1]',(SELECT id FROM building WHERE objName='$params[1]'),(select channelID from building where objName='$params[1]'),$params[3], $params[4])";
		}
		if($result=mysqli_query($link,$query))
		{
			$successCount+=1;
		}
		else
		{
			$err=mysqli_error($link);
			file_put_contents('insertLogs.log',"error insert section $params[0]: query is $query, error: $err\r\n",FILE_APPEND);
			$failedCount+=1;
		}
		$progressBar->advance();
	}
	echo "\r\nSection insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"Section insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);
