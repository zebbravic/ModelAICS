<?php
	$successCount=0;
	$failedCount=0;
	require_once 'ProgressBar/Manager.php';
	require_once 'ProgressBar/Registry.php';
	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$actuators=array();
	$actuators=file('csv/actuators.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	
	echo "Actuators insert begins\r\n";
	$progressBar=new \ProgressBar\Manager(0,count($actuators));
	file_put_contents('insertLogs.log',"actuators insert begins\r\n",FILE_APPEND);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	function insertMetaDOut(string $metaParams, $position, $metaNum)
	{
		global $link, $successCount, $failedCount;
		$metaDOuts=array();
		$metaDOuts=explode("?",$metaParams);
		foreach($metaDOuts as $metaDOut)
		{
			$metaDOutParams=explode("|",$metaDOut);
			$query="insert into metaDOut (channelID, mode, num) values ((select id from channel where name='$metaNum'), '$position', $metaDOutParams[0])";
			if($result=mysqli_query($link,$query))
			{
				$successCount+=1;
				$metaDOutParamsSplited=explode(", ",$metaDOutParams[1]);
				if($position==4)
				{
					$query="insert into doutRandomizer(metaDOutID) values ((select id from metaDOut where num = $metaDOutParams[0]))";
				}
				if($position!=4||$result=mysqli_query($link,$query))
				{
					if($position==4)
					{
						$successCount+=1;
					}
					foreach ($metaDOutParamsSplited as $metaDOutParamSplit)
					{
						$oneParams=explode(":",$metaDOutParamSplit);
						if($oneParams[0]=='list')
						{
							$channels=explode(",",$oneParams[1]);
							foreach ($channels as $channel)
							{
								$query="insert into doutsInMeta (metaDOutID, doutID) values ((select id from metaDOut where num=$metaDOutParams[0]), (select id from dout where num='$channel'))";
								if($result=mysqli_query($link,$query))
								{
									$successCount+=1;
								}
								else
								{
									$err=mysqli_error($link);
									file_put_contents('insertLogs.log',"error insert doutInMeta $metaDOutParams[0] $channel: query is $query, error: $err\r\n",FILE_APPEND);
									$failedCount+=1;
								}
							}
							unset($channel);
						}
						else
						{
							if($oneParams[0]=='range')
							{
								$channels=explode("-",$oneParams[1]);
								$query="insert into doutInMetaRange (metaDOutID, rangeStartDOut, rangeEndDOut) values ((select id from metaDOut where num=$metaDOutParams[0]), (select id from dout where num='$channels[0]'), (select id from dout where num='$channels[1]'))";
								if($result=mysqli_query($link,$query))
								{
									$successCount+=1;
								}
								else
								{
									$err=mysqli_error($link);
									file_put_contents('insertLogs.log',"error insert doutInMetaRange $metaDOutParams[0] $channels[0]: query is $query, error: $err\r\n",FILE_APPEND);
									$failedCount+=1;
								}
							}
							else
							if($position==4)
							{
								$query="update doutRandomizer  set $oneParams[0] = $oneParams[1] where metaDOutID = (select id from metaDOut where num = $metaDOutParams[0])";
								if($result=mysqli_query($link,$query))
								{
									$successCount+=1;
								}
								else
								{
									$err=mysqli_error($link);
									file_put_contents('insertLogs.log',"error update doutRandomizer $metaDOutParams[0] $channels[0]: query is $query, error: $err\r\n",FILE_APPEND);
									$failedCount+=1;
								}
							}
						}
					}
					unset($metaDOutParamSplit);
				}
				else
				{
					$err=mysqli_error($link);
					file_put_contents('insertLogs.log',"error insert metaDOut $metaDOut[0]: query is $query, error: $err\r\n",FILE_APPEND);
					$failedCount+=1;
				}
			}
		}
		unset($metaDOut);
	}
	
	foreach($actuators as $actuator)
	{
		$params=explode(";",$actuator);
		if($params[1]!='-')
		{
			insertMetaDOut($params[1], 1, $params[0]);
		}
		if($params[2]!='-')
		{
			insertMetaDOut($params[2], 2, $params[0]);
		}
		if($params[3]!='-')
		{
			insertMetaDOut($params[3], 3, $params[0]);
		}
		if($params[4]!='-')
		{
			insertMetaDOut($params[4], 4, $params[0]);
		}
		if($params[5]!='-')
		{
			$lents=array();
			$lents=explode("?",$params[5]);
			$colors=explode(",",$params[6]);
			foreach($lents as $lent)
			{
				$lentParams=explode("|",$lent);
				$query="insert into lents (channelID, mode,red,green,blue, num) values ((select id from channel where name='$params[0]'), '0', '$colors[0]', '$colors[1]', '$colors[2]', '$lent[0]')";
				if($result=mysqli_query($link,$query))
				{
					$successCount+=1;
					$lentParamsSplited=explode(", ",$lentParams[1]);
					foreach ($lentParamsSplited as $lentParamSplit)
					{
						$oneParams=explode(":",$lentParamSplit);
						if($oneParams[0]=='list')
						{
							$channels=explode(",",$oneParams[1]);
							foreach ($channels as $channel)
							{
								$query="insert into segmentInLent (lentsID,segmentID) values ((select id from lents where num=$lent[0]), (select id from segments where num='$channel'))";
								if($result=mysqli_query($link,$query))
								{
									$successCount+=1;
								}
								else
								{
									$err=mysqli_error($link);
									file_put_contents('insertLogs.log',"error insert lents $lent[0] $channel: query is $query, error: $err\r\n",FILE_APPEND);
									$failedCount+=1;
								}
							}
							unset($channel);
						}
						if($oneParams[0]=='range')
						{
							$channels=explode("-",$oneParams[1]);
							$query="insert into segmentInLent (lentsID, rangeStartSegment, rangeEndSegment) values ((select id from lents where num=$lent[0]), (select id from segments where num='$channels[0]'), (select id from segments where num='$channels[1]'))";
							if($result=mysqli_query($link,$query))
							{
								$successCount+=1;
							}
							else
							{
								$err=mysqli_error($link);
								file_put_contents('insertLogs.log',"error insert segmentInLentRange $lent[0] $channels[0]: query is $query, error: $err\r\n",FILE_APPEND);
								$failedCount+=1;
							}
						}
					}
					unset($lentParamSplit);
				}
				else
				{
					$err=mysqli_error($link);
					file_put_contents('insertLogs.log',"error insert lents $lent[0]: query is $query, error: $err\r\n",FILE_APPEND);
					$failedCount+=1;
				}
			}
			unset($lent);
		}
		if($params[7]!='-')
		{
			$lents=array();
			$lents=explode("?",$params[7]);
			$colors=explode(",",$params[8]);
			foreach($lents as $lent)
			{
				$lentParams=explode("|",$lent);
				$query="insert into lents (channelID, mode,red,green,blue, num) values ((select id from channel where name='$params[0]'), '1', '$colors[0]', '$colors[1]', '$colors[2]', '$lent[0]')";
				if($result=mysqli_query($link,$query))
				{
					$successCount+=1;
					$lentParamsSplited=explode(", ",$lentParams[1]);
					foreach ($lentParamsSplited as $lentParamSplit)
					{
						$oneParams=explode(":",$lentParamSplit);
						if($oneParams[0]=='list')
						{
							$channels=explode(",",$oneParams[1]);
							foreach ($channels as $channel)
							{
								$query="insert into segmentInLent (lentsID,segmentID) values ((select id from lents where num=$lent[0]), (select id from segments where num='$channel'))";
								if($result=mysqli_query($link,$query))
								{
									$successCount+=1;
								}
								else
								{
									$err=mysqli_error($link);
									file_put_contents('insertLogs.log',"error insert lents $lent[0] $channel: query is $query, error: $err\r\n",FILE_APPEND);
									$failedCount+=1;
								}
							}
							unset($channel);
						}
						if($oneParams[0]=='range')
						{
							$channels=explode("-",$oneParams[1]);
							$query="insert into segmentInLent (lentsID, rangeStartSegment, rangeEndSegment) values ((select id from lents where num=$lent[0]), (select id from segments where num='$channels[0]'), (select id from segments where num='$channels[1]'))";
							if($result=mysqli_query($link,$query))
							{
								$successCount+=1;
							}
							else
							{
								$err=mysqli_error($link);
								file_put_contents('insertLogs.log',"error insert segmentInLentRange $lent[0] $channels[0]: query is $query, error: $err\r\n",FILE_APPEND);
								$failedCount+=1;
							}
						}
					}
					unset($lentParamSplit);
				}
				else
				{
					$err=mysqli_error($link);
					file_put_contents('insertLogs.log',"error insert lents $lent[0]: query is $query, error: $err\r\n",FILE_APPEND);
					$failedCount+=1;
				}
			}
			unset($lent);
		}
		if($params[9]!='-')
		{
			$lents=array();
			$lents=explode("?",$params[9]);
			$colors=explode(",",$params[10]);
			foreach($lents as $lent)
			{
				$lentParams=explode("|",$lent);
				$query="insert into lents (channelID, mode,red,green,blue, num) values ((select id from channel where name='$params[0]'), '2', '$colors[0]', '$colors[1]', '$colors[2]', '$lent[0]')";
				if($result=mysqli_query($link,$query))
				{
					$successCount+=1;
					$lentParamsSplited=explode(", ",$lentParams[1]);
					foreach ($lentParamsSplited as $lentParamSplit)
					{
						$oneParams=explode(":",$lentParamSplit);
						if($oneParams[0]=='list')
						{
							$channels=explode(",",$oneParams[1]);
							foreach ($channels as $channel)
							{
								$query="insert into segmentInLent (lentsID,segmentID) values ((select id from lents where num=$lent[0]), (select id from segments where num='$channel'))";
								if($result=mysqli_query($link,$query))
								{
									$successCount+=1;
								}
								else
								{
									$err=mysqli_error($link);
									file_put_contents('insertLogs.log',"error insert lents $lent[0] $channel: query is $query, error: $err\r\n",FILE_APPEND);
									$failedCount+=1;
								}
							}
							unset($channel);
						}
						if($oneParams[0]=='range')
						{
							$channels=explode("-",$oneParams[1]);
							$query="insert into segmentInLent (lentsID, rangeStartSegment, rangeEndSegment) values ((select id from lents where num=$lent[0]), (select id from segments where num='$channels[0]'), (select id from segments where num='$channels[1]'))";
							if($result=mysqli_query($link,$query))
							{
								$successCount+=1;
							}
							else
							{
								$err=mysqli_error($link);
								file_put_contents('insertLogs.log',"error insert segmentInLentRange $lent[0] $channels[0]: query is $query, error: $err\r\n",FILE_APPEND);
								$failedCount+=1;
							}
						}
					}
					unset($lentParamSplit);
				}
				else
				{
					$err=mysqli_error($link);
					file_put_contents('insertLogs.log',"error insert lents $lent[0]: query is $query, error: $err\r\n",FILE_APPEND);
					$failedCount+=1;
				}
			}
			unset($lent);
		}
		if($params[11]!='-')
		{
			$lents=array();
			$lents=explode("?",$params[11]);
			$colors=explode(",",$params[12]);
			foreach($lents as $lent)
			{
				$lentParams=explode("|",$lent);
				$query="insert into lents (channelID, mode,red,green,blue, num) values ((select id from channel where name='$params[0]'), '4', '$colors[0]', '$colors[1]', '$colors[2]', '$lent[0]')";
				if($result=mysqli_query($link,$query))
				{
					$successCount+=1;
					$lentParamsSplited=explode(", ",$lentParams[1]);
					foreach ($lentParamsSplited as $lentParamSplit)
					{
						$oneParams=explode(":",$lentParamSplit);
						if($oneParams[0]=='list')
						{
							$channels=explode(",",$oneParams[1]);
							foreach ($channels as $channel)
							{
								$query="insert into segmentInLent (lentsID,segmentID) values ((select id from lents where num=$lent[0]), (select id from segments where num='$channel'))";
								if($result=mysqli_query($link,$query))
								{
									$successCount+=1;
								}
								else
								{
									$err=mysqli_error($link);
									file_put_contents('insertLogs.log',"error insert lents $lent[0] $channel: query is $query, error: $err\r\n",FILE_APPEND);
									$failedCount+=1;
								}
							}
							unset($channel);
						}
						if($oneParams[0]=='range')
						{
							$channels=explode("-",$oneParams[1]);
							$query="insert into segmentInLent (lentsID, rangeStartSegment, rangeEndSegment) values ((select id from lents where num=$lent[0]), (select id from segments where num='$channels[0]'), (select id from segments where num='$channels[1]'))";
							if($result=mysqli_query($link,$query))
							{
								$successCount+=1;
							}
							else
							{
								$err=mysqli_error($link);
								file_put_contents('insertLogs.log',"error insert segmentInLentRange $lent[0] $channels[0]: query is $query, error: $err\r\n",FILE_APPEND);
								$failedCount+=1;
							}
						}
					}
					unset($lentParamSplit);
				}
				else
				{
					$err=mysqli_error($link);
					file_put_contents('insertLogs.log',"error insert lents $lent[0]: query is $query, error: $err\r\n",FILE_APPEND);
					$failedCount+=1;
				}
			}
			unset($lent);
		}
		$progressBar->advance();
	}
	echo "\r\nActuators insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"actuators insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);
