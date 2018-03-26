<?php 
	$ipaddress = $_SERVER['REMOTE_ADDR'];
	#$ipaddress = $argv[1];
	$fp = fsockopen("localhost", 33333, $errno, $errstr, 30);
	if (!$fp) {
    				$response = array();
		$response['allow'] = "true";
		echo json_encode($response, JSON_UNESCAPED_UNICODE);
	} else {
		$req = array();
		$req["ipaddress"]=$ipaddress;
		$req["actions"][]["action"]="0";
		$endStr=json_encode($req, JSON_UNESCAPED_UNICODE);
    		fwrite($fp, $endStr);
		$permission = "";
		while(!feof($fp))
		{
			$permission.=fread($fp,8192);
		}
	    	fclose($fp);
		echo "\r\n$permission\r\n";
	}



