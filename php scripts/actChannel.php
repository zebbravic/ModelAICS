<?php

	$hostname = "localhost";
	$username = "dbUser";
	$password = "secretPass";
	$dbName = "maket_info";
	$ipaddress = $_SERVER['REMOTE_ADDR'];
	#$ipaddress = $argv[1];
	#$ipaddress="127.0.0.1";	
	$channels=array();
	#$testVar = "a.1.f.4.s.2.b.k1-1-1;a.4.f.6.s.1.b.k1-1-1";
	#$channels=explode(";",$argv[1]);
	$channels = explode(";",$_POST['objects']);
	#echo $argv[1];	
	#$channels=explode(";",$testVar);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");

	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	$arr=array();
	$assistedArr=array();
	$arr['ipaddress']=$ipaddress;
	$arr['actions'][]=array('action'=>"4");
	#echo count($channels);
	foreach($channels as $channel)
	{
		#echo "\r\n$channel\r\n";
		switch(substr($channel,0,1))
		{
			case "b": $table="building"; break;
			case "f": $table="floor";break;
			case "s": $table="section";break;
			case "a": $table="appartments"; break;
			case "i": $table="infrastructure"; break;
			default: $table="";
		}
		if($table!="")
		{
			$query="select ch.name, ch.blocking, ch.type from channel  ch join $table  t on t.channelID = ch.id where t.objName = \"$channel\"";
			#echo "\r\n$query\r\n";
			if($result=mysqli_query($link,$query))
			{
				while($row = mysqli_fetch_array($result))
				{
					if(!in_array($row[0],$assistedArr))
					{
						$tempArr = array();
						$tempArr['action']="1";
						$tempArr['blocking']=$row[1];
						$tempArr['channel']=$row[0];
						if($row[2]==0)
						{						
							$tempArr['members']=array($row[0]);
						}
						else
						{
							$tempArr['members']=checkDynamicChannel($link,$row[0]);
						}
						$arr['actions'][]=$tempArr;
						$assistedArr[] = $row[0];
					}
				}
			}
		}
	}
	unset($channel);
	$endStr = json_encode($arr, JSON_UNESCAPED_UNICODE);
#	echo json_encode($arr, JSON_UNESCAPED_UNICODE);
#	$endStr="4 0;";
#	foreach($arr as $part)
#	{
#		$endStr.="1 $part;";
#	}
#	unset($part);
	$fp = fsockopen("localhost", 33333, $errno, $errstr, 30);
	if (!$fp) {
    		echo "$errstr ($errno)<br />\n";
	} else {
    		fwrite($fp, $endStr);
	    	fclose($fp);
	}

function checkDynamicChannel($link1, $channel)
{
	$resArr = array();
	$query="select chL.channelList from channel ch join channelList chL on chL.channelID=ch.id where ch.name = \"$channel\"";
	if($result=mysqli_query($link1,$query))
	{
		while($row = mysqli_fetch_array($result))
		{
			$members=explode(",",$row[0]);
			foreach($members as $member)
			{
				$query="select type from channel where name = \"$member\"";
				#echo "\r\n$query\r\n";
				if($result=mysqli_query($link1,$query))
				{	
					while($row=mysqli_fetch_array($result))
					{
						#echo "\"$row[0]\"";
						if($row[0]=="0")
						{	
							if(!in_array($member,$resArr))
							{
								$resArr[]=$member;
							}
							
						}
						else
						{
							$res1 = checkDynamicChannel($link1, $member);
							foreach($res1 as $r)
							{
								if(!in_array($r, $resArr))
								{
									$resArr[]=$r;
								}
							}
							unset($r);
						}
					}
				}
				
			} 
		}
	}
	$query="select chQ.queryText from channel ch join channelQuery chQ on chQ.channelID=ch.id where ch.name = \"$channel\"";
	#echo "\r\n$query\r\n";
	if($result=mysqli_query($link1,$query))
	{
		while($row = mysqli_fetch_array($result))
		{
			$query = $row[0];
			#echo "\r\n$query\r\n";
			if($innerResult=mysqli_query($link1,$query))
			{
				while($row=mysqli_fetch_array($innerResult))
				{
					$member=$row[0];
					$query="select type from channel where name = \"{$member}\"";
					#echo "\r\n$query\r\n";
					if($innerResult1=mysqli_query($link1,$query))
					{
						while($row=mysqli_fetch_array($innerResult1))
						{
							if($row[0]=="0")
							{	
								if(!in_array($row[0],$resArr))
								{
									$resArr[]=$member;
								}
							}
							else
							{
								$res1 = checkDynamicChannel($link1, $row[0]);
								foreach($res1 as $r)
								{
										if(!in_array($r, $resArr))
									{
										$resArr[]=$r;
									}
								}
								unset($r);
							}
						}
					}
				}
			}
				
		} 
		
	}
	#echo json_encode($resArr, JSON_UNESCAPED_UNICODE);
	return $resArr;
}
