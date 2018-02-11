<?php

	require_once 'ProgressBar/Manager.php';
	require_once 'ProgressBar/Registry.php';
	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/appartments.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	$params=array();
	foreach($buildings as $building)
	{
		$params[]=explode(";",$building);
	}
	unset($building);
	$successCount=0;
	$failedCount=0;
	$progressBar=new \ProgressBar\Manager(0,count($params));
	echo "appartments insert begins\r\n";
	file_put_contents('insertLogs.log',"appartments insert begins\r\n",FILE_APPEND);
	$i=0;
	$numOnFloor=0;
	$prevFloor=$params[$i][2];
	while($i<count($params)&&$params[$i][8]!="-")
	{
		$i++;
		$prevFloor=$params[$i][2];
	}
	while($i<count($params)&&$params[$i][0]=='floor')
	{
		if($params[$i][2]==$prevFloor)
		{
			$numOnFloor++;
		}
		else {
			$numOnFloor=1;
		}
		$prevFloor=$params[$i][2];
		$params[$i][8]=$numOnFloor;
		$i++;
	}
	#echo $i;
	$i=count($params)-1;
	$numOnFloor=0;
	$prevFloor=$params[$i][2];
	while($params[$i][0]=='section')
	{
		if($params[$i][2]==$prevFloor)
		{
			$numOnFloor++;
		}
		else {
			$numOnFloor=1;
		}
		$prevFloor=$params[$i][2];
		$params[$i][8]=$numOnFloor;
		$i--;
	}
	#echo $i;
	mysqli_set_charset($link,"utf8");
	foreach($params as $param)
	{
		$query="select id from appartmentsType where name = \"$param[7]\"";
		$result=mysqli_query($link,$query);
		if(mysqli_num_rows($result)!=0)
		{
			$row=mysqli_fetch_array($result);
			$typeID=$row[0];
		}
		else
		{
			$query="insert into appartmentsType (name) values(\"$param[7]\")";
			$result=mysqli_query($link,$query);
			$typeID=mysqli_insert_id($link);
		}
		if($param[0]=='floor')
		{
			$query="insert into appartments (number, objName,floorID,sectionsID,size,roomsCnt, channelID,type,numOnFloor,status, CRMAprtCode) values ($param[1],'a.$param[8].f.$param[2].s.$param[3].$param[4]',(SELECT f.id FROM floor as f JOIN building as b ON b.id = f.buildingID JOIN section as s on s.id = f.sectionID WHERE f.numInBuilding='$param[2]' and b.objName=\"$param[4]\" and (f.sectionID is null or s.name='$param[3]')),(SELECT id FROM section WHERE objName='s.$param[3].$param[4]'),'$param[5]','$param[6]',(select channelID from floor where objName='f.$param[2].$param[4]'),$typeID,'$param[8]','0',\"$param[9]\")";
		}
		elseif($param[0]=='section')
		{
			$query="insert into appartments (number, objName,floorID,sectionsID,size,roomsCnt, channelID,type,numOnFloor,status, CRMAprtCode) values ($param[1],'a.$param[8].f.$param[2].s.$param[3].$param[4]',(SELECT f.id FROM floor as f JOIN building as b ON b.id = f.buildingID JOIN section as s on s.id = f.sectionID WHERE f.numInBuilding='$param[2]' and b.objName=\"$param[4]\" and (f.sectionID is null or s.name='$param[3]')),(SELECT id FROM section WHERE objName='s.$param[3].$param[4]'),'$param[5]','$param[6]',(select channelID from section where objName='f.$param[3].$param[4]'),$typeID,'$param[8]','0',\"$param[9]\")";
		}
		else
		{
			$query="insert into appartments (number, objName,floorID,sectionsID,size,roomsCnt, channelID,type,numOnFloor,status, CRMAprtCode) values ($param[1],'a.$param[8].f.$param[2].s.$param[3].$param[4]',(SELECT f.id FROM floor as f JOIN building as b ON b.id = f.buildingID JOIN section as s on s.id = f.sectionID WHERE f.numInBuilding='$param[2]' and b.objName=\"$param[4]\" and (f.sectionID is null or s.name='$param[3]')),(SELECT id FROM section WHERE objName='s.$param[3].$param[4]'),'$param[5]','$param[6]',(select channelID from section where objName='f.$param[3].$param[4]'),$typeID,'$param[8]','0',\"$param[9]\")";
		}
		if($result=mysqli_query($link,$query))
		{
			$successCount+=1;
		}
		else
		{
			$err=mysqli_error($link);
			file_put_contents('insertLogs.log',"error insert appartments $param[1]: query is $query, error: $err\r\n",FILE_APPEND);
			$failedCount+=1;
		}
		$progressBar->advance();

	}
	echo "\r\nAppartments insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"Appratments insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);
