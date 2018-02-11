<?php 

	$hostname = "localhost";
	$username = "dbUser";
	$password = "secretPass";
	$dbName = "maket_info";
	if (isset($_POST['type']) && isset ($_POST['bObjName']))
	{
		$typeNotEscaped=$_POST['type'];
		$bObjName=$_POST['bObjName'];
		$building="\"$bObjName\"";
		$type="\"$typeNotEscaped\"";
		#$building="\"b.k1-1-1\"";
		#$type="\"ST.1\"";

		$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
		
		if (mysqli_connect_errno()) {
			printf("Не удалось подключиться: %s\n", mysqli_connect_error());
			exit();
		}


		$query="select b.name as building, aprt.number as flatNum, aprt.roomsCnt as roomCnt, 
		aprt.size as square, apType.name as type, f.numInBuilding as floor, s.name as section, 
		aprt.objName as objName, aprt.numOnFloor as numOnFloor
		from appartments as aprt 
		join floor as f on f.ID=aprt.floorID  
		join section as s on s.ID=aprt.sectionsID 
		join building as b on f.buildingID = b.ID
		join appartmentsType as apType on aprt.type = apType.ID 
		where b.objName = $building
		and apType.name = $type
		and aprt.status = 4 
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
			$tempFlat=array('flatNum'=>$row[1],'roomCnt'=>$row[2],'square'=>$row[3],'type'=>$row[4],'floor'=>$row[5],'section'=>$row[6],'objName'=>$row[7],'numOnFloor'=>$row[8]);
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




