<?php

	$hostname = "localhost";
	$username = "dbUser";
	$password = "secretPass";
	$dbName = "maket_info";
	if (isset($_POST['objects']))
#	if($argc>1)
	{
		$channels=explode(";",$_POST['objects']);
#		$channels=explode(";",$argv[1]);
		$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");

		if (mysqli_connect_errno()) {
			printf("Не удалось подключиться: %s\n", mysqli_connect_error());
			exit();
		}
		$arr=array();
		foreach($channels as $channel)
		{
			switch(substr($channel,0,1))
			{
				case "b": $table="building"; break;
				case "f": $table="floor";break;
				case "s": $table="section";break;
				case "a": $table="appartments"; break;
				case "i": $table="infrasructure"; break;
				default: $table="";
			}
			if($table!="")
			{
				$query="select ch.name from channel  ch join $table  t on t.channelID = ch.id where t.objName = \"$channel\"";
				if($result=mysqli_query($link,$query))
				{
					while($row = mysqli_fetch_array($result))
					{
						if(!in_array($row[0],$arr))
						{
							$arr[]=$row[0];
						}
					}
				}
			}
		}
		unset($channel);
		$endStr="4 0;";
		foreach($arr as $part)
		{
			$endStr.="1 $part;";
		}
		unset($part);
		$fp = fsockopen("localhost", 33333, $errno, $errstr, 30);
		if (!$fp) {
    			echo "$errstr ($errno)<br />\n";
		} else {
    			fwrite($fp, $endStr);
    		fclose($fp);
		}}
