<?php

	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/appartments.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("�� ������� ������������: %s\n", mysqli_connect_error());
		exit();
	}
	$params=array();
	foreach($buildings as $building)
	{
		$params[]=explode(";",$building);
	}
	unset($building);
	$i=0;
	$numOnFloor=0;
	$prevFloor=$params[$i][2];
	while(count($params[$i])==9)
	{
		$i++;
		$prevFloor=$params[$i][2];
	}
	#echo $i;
	while($params[$i][0]=='floor')
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
		if($param[0]=='floor')
		{
			$query="insert into appartments (number, objName,floorID,sectionsID,size,roomsCnt, channelID,tip,numOnFloor) values ($param[1],'a.$param[8].f.$param[2].s.$param[3].$param[4]',(SELECT id FROM floor WHERE objName='f.$param[2].$param[4]'),(SELECT id FROM section WHERE objName='s.$param[3].$param[4]'),'$param[5]','$param[6]',(select channelID from floor where objName='f.$param[2].$param[4]'),\"$param[7]\",'$param[8]')";
		}
		else
		{
			$query="insert into appartments (number, objName,floorID,sectionsID,size,roomsCnt, channelID,tip,numOnFloor) values ($param[1],'a.$param[8].f.$param[2].s.$param[3].$param[4]',(SELECT id FROM floor WHERE objName='f.$param[2].$param[4]'),(SELECT id FROM section WHERE objName='s.$param[3].$param[4]'),'$param[5]','$param[6]',(select channelID from section where objName='f.$param[3].$param[4]'),\"$param[7]\",'$param[8]')";
		}
		#echo $query;
		if($result=mysqli_query($link,$query))
			{
			#	echo "success insert appartments a.$param[1].f.$param[2].s.$param[3].b.$param[4]\r\n";
			}
			else
			{
				echo "$query;\r\n";
			#echo "error insert appartments a.$param[1].f.$param[2].s.$param[3].b.$param[4]\r\n";
		}

	}
