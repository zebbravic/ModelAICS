<?php 
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST, GET, OPTIONS");

	$hostname = "localhost";
	$username = "dbUser";
	$password = "secretPass";
	$dbName = "maket_info";
		if (isset ($_POST['number'])
		&& isset ($_POST['bObjName']))
	{
		$number=$_POST['number'];
		$bObjName=$_POST['bObjName'];
		$building="\"$bObjName\"";
#		$number = 258;
#		$building = "\"b.k1-1-1\"";		
		$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
		
		if (mysqli_connect_errno()) {
			printf("Не удалось подключиться: %s\n", mysqli_connect_error());
			exit();
		}


		$query="select distinct aprt.numOnFloor, f.numInBuilding as floor, s.name as section
		from appartments as aprt 
		join floor as f on f.ID=aprt.floorID  
		join section as s on s.ID=aprt.sectionsID 
		join building as b on f.buildingID = b.ID 
		where b.objName = $building
		and aprt.number=$number";
		$aprt=array();

		if($result=mysqli_query($link,$query))
		{
			while($row = mysqli_fetch_array($result))
			{
				$aprt['numOnFloor']=$row[0];
				$aprt['floor']=$row[1];
				$aprt['section']=$row[2];
			}
		}
		else
		{
			$aprt['errcode']=0;
			$aprt['err']='err in query';
		}
		echo json_encode($aprt, JSON_UNESCAPED_UNICODE);
	}




