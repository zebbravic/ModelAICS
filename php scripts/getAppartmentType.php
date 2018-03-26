<?php 
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST, GET, OPTIONS");

$hostname = "localhost";
$username = "dbUser";
$password = "secretPass";
$dbName = "maket_info";
#	if (isset($_POST['minS']) && isset($_POST['maxS']) 
#		&& isset($_POST['minRooms']) && isset($_POST['maxRooms'])
#		&& isset ($_POST['minFloor']) && isset($_POST['maxFloor']))
	{
#		$minS=$_POST['minS'];
#		$maxS=$_POST['maxS'];
#		$minRooms=$_POST['minRooms'];
#		$maxRooms=$_POST['maxRooms'];
#		$minFloor=$_POST['minFloor'];
#		$maxFloor=$_POST['maxFloor'];
		
		$minS=0;
		$maxS=100;
		$minRooms=0;
		$maxRooms=2;
		$minFloor=1;
		$maxFloor=6;
		$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
		
		if (mysqli_connect_errno()) {
			printf("Не удалось подключиться: %s\n", mysqli_connect_error());
			exit();
		}


		$query="select distinct b.name as building, b.objName as objName
		from appartments as aprt 
		join floor as f on f.ID=aprt.floorID  
		join section as s on s.ID=aprt.sectionsID 
		join building as b on f.buildingID = b.ID 
		where aprt.roomsCnt between $minRooms and $maxRooms
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
			$tempFlat=array('name'=>$row[0],'objName'=>$row[1]);
			$flats['buildings'][]=$tempFlat;
		}
		}
		else
		{
			$flats['errcode']=0;
			$flats['err']='no flats found';
		}
		echo json_encode($flats, JSON_UNESCAPED_UNICODE);
	}




