<?php 

	$hostname = "localhost";
	$username = "dbUser";
	$password = "secretPass";
	$dbName = "maket_info";
		if (isset ($_POST['section'])
		&& isset ($_POST['bObjName']))
	{
		$section=$_POST['section'];
		$bObjName=$_POST['bObjName'];
		$building="\"$bObjName\"";
#		$section = 1;
#		$building = "\"b.k1-1-1\"";		
		$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
		
		if (mysqli_connect_errno()) {
			printf("Не удалось подключиться: %s\n", mysqli_connect_error());
			exit();
		}


		$query="select distinct  f.numInBuilding
		from appartments as aprt 
		join floor as f on f.ID=aprt.floorID  
		join section as s on s.ID=aprt.sectionsID 
		join building as b on f.buildingID = b.ID 
		where b.objName = $building
		and s.name = $section
		and aprt.status = 4 ";
		$flats=array();

		if($result=mysqli_query($link,$query))
		{
			while($row = mysqli_fetch_array($result))
			{
				$flats['floors'][]=$row[0];
			}
		}
		else
		{
			$flats['errcode']=0;
			$flats['err']='err in query';
		}
		echo json_encode($flats, JSON_UNESCAPED_UNICODE);
	}




