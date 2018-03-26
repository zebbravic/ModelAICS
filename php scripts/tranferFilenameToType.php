<?php 
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST, GET, OPTIONS");

	
	if (isset($_POST['filename']))
	{
		$filename=$_POST['filename'];
	#	$filename="type_1T_Stud";
		$params = explode("_",$filename);
		$response = array();
		switch(substr($params[2],0,1))
		{
			case "S": $typeName="ST."; break;
			default: $typeName=substr($params[2],0,1)."BD.";
		}
		$typeName.=strtolower($params[1]);
		$response["type"]=$typeName;
		echo json_encode($response, JSON_UNESCAPED_UNICODE);
	}




