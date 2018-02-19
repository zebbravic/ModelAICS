<?php 

	$hostname = "localhost";
	$username = "dbUser";
	$password = "secretPass";
	$dbName = "maket_info";
	if ((isset($_POST['numOnfFloor']) && isset($_POST['floor']) && isset($_POST['section']) $$ ($_POST['bObjName']))
		|| (isset($_POST['number'])  && ($_POST['bObjName']))
		|| isset($_POST['aObjName']))
	{
		if( isset($_POST['numOnFloor']) && isset($_POST['floor']) && ($_POST['bObjName']))
		{
			$numOnFloor = $_POST['numOnFloor'];
			$floor = $_POST['floor'];
			$section = $_POST['section'];
			$bObjName = $_POST['bObjName'];
#			$numOnFloor = 4;
#			$floor =4;
#			$section = 1;
#			$bObjName = "b.k1-1-1";
			$whereCond = " aprt.numOnFloor = $numOnFloor and f.numInBuilding = $floor and s.name = \"$section\" and b.objName = \"$bObjName\" ";
		}
		elseif (isset($_POST['number'])  && ($_POST['bObjName']))
		{
			$number = $_POST['number'];
			$bObjName = $_POST['bObjName'];

#			$number = 147;
#			$bObjName = "b.k1-1-1";
			$whereCond = " aprt.number = $number and b.objName = \"$bObjName\" ";
		}
		else
		{
			#$aObjName = $_POST['aObjName'];
			$aObjName = "a.3.f.4.s.1.b.k1-1-1";
			$whereCond = " aprt.objName = \"$aObjName\" ";
		}
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
		where $whereCond order by 1";
		$flats=array();
		if($result=mysqli_query($link,$query))
		{
		while($row = mysqli_fetch_array($result))
		{

			$flats=array('building'=>$row[0],'flatNum'=>$row[1],'roomCnt'=>$row[2],'square'=>$row[3],'type'=>$row[4],'floor'=>$row[5],'section'=>$row[6],'objName'=>$row[7],'numOnFloor'=>$row[8]);
		}
		}
		else
		{
			$flats['errcode']=0;
			$flats['err']='no flats found';
		}
		echo json_encode($flats, JSON_UNESCAPED_UNICODE);
	}




