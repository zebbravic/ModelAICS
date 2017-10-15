<?php 

$hostname = "localhost";
$username = "dbUser";
$password = "secretPass";
$dbName = "maket_info";
	if (isset($_POST['minS']) && isset($_POST['maxS']) 
		&& isset($_POST['minRooms']) && isset($_POST['maxRooms'])
		&& isset ($_POST['minFloor']) && isset($_POST['maxFloor'])
		&& isset ($_POST['bObjName']))
	{
		$minS=$_POST['minS'];
		$maxS=$_POST['maxS'];
		$minRooms=$_POST['minRooms'];
		$maxRooms=$_POST['maxRooms'];
		$minFloor=$_POST['minFloor'];
		$maxFloor=$_POST['maxFloor'];
		$bObjName=$_POST['bObjName'];
		
#		$minS=0;
#		$maxS=100;
#		$minRooms=0;
#		$maxRooms=2;
#		$minFloor=1;
#		$maxFloor=6;
#		$bObjName="b.t1-1-1";
		$building="\"$bObjName\"";
		$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
		
		if (mysqli_connect_errno()) {
			printf("Не удалось подключиться: %s\n", mysqli_connect_error());
			exit();
		}


		$query="select b.name as building, aprt.number as flatNum, aprt.roomsCnt as roomCnt, aprt.size as square, aprt.tip as type, f.numInBuilding as floor, s.name as section, aprt.objName as objName, aprt.numOnFloor as numOnFloor
		from appartments as aprt 
		join floor as f on f.ID=aprt.floorID  
		join section as s on s.ID=aprt.sectionsID 
		join building as b on f.buildingID = b.ID 
		where b.objName = $building
		and aprt.roomsCnt between $minRooms and $maxRooms
		and aprt.size between $minS and $maxS
		and f.numInBuilding between $minFloor and $maxFloor
		order by 1";
		$flats=array();
		
		$curBuild;
		if($result=mysqli_query($link,$query))
		{
		while($row = mysqli_fetch_array($result))
		{
			$curBuild = $row[0];
			if($curBuild!=$flats['buildings'][count($flats)-1]['name'])
			{
				$flats['buildings'][]['name']=$curBuild;
			}
			$tempFlat=array('flatNum'=>$row[1],'roomCnt'=>$row[2],'square'=>$row[3],'type'=>$row[4],'floor'=>$row[5],'section'=>$row[6],'objName'=>$row[7]),'numOnFloor'=>$row[8]);
			$flats['buildings'][count($flats)-1]['flats'][]=$tempFlat;
		}
		}
		else
		{
			$flats['errcode']=0;
			$flats['err']='no flats found';
		}
		echo json_encode($flats, JSON_UNESCAPED_UNICODE);
	}




