<?php
	require_once 'ProgressBar/Manager.php';
	require_once 'ProgressBar/Registry.php';
	$hostname = "localhost";
	$username = "installer";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$channels=array();
	$channels=file('csv/channels.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	$successCount=0;
	$failedCount=0;
	echo "Channel insert begins\r\n";
	$progressBar=new \ProgressBar\Manager(0,count($channels));
	file_put_contents('insertLogs.log',"channel insert begins\r\n",FILE_APPEND);
	foreach($channels as $channel)
	{
		$params=explode(";",$channel);
		$query="insert into channel (name, type, blocking, comment) values ('$params[0]',$params[1],$params[2],\"$params[5]\")";
		if($result=mysqli_query($link,$query))
			{
				$successCount+=1;
				if($params[1]=1)
				{
					if($params[3]!='-')
					{
						$query="insert into channelQuery (channelID, queryText) values ((SELECT id FROM channel WHERE name = '$params[0]'),$params[3])";
						if($result=mysqli_query($link,$query))
						{
							$successCount+=1;
						
						}
						else
						{
							$err=mysqli_error($link);
							file_put_contents('insertLogs.log',"error insert channelQuery $params[0]: query is $query, error: $err\r\n",FILE_APPEND);
							$failedCount+=1;
						}
					}
					if($params[4]!='-')
					{
						$query="insert into channelList (channelID, channelList) values ((SELECT id FROM channel WHERE name = $params[0]),\"$params[4]\")";
						if($result=mysqli_query($link,$query))
						{
							$successCount+=1;
						}
						else
						{
							$err=mysqli_error($link);
							file_put_contents('insertLogs.log',"error insert channelList $params[0]: query is $query, error: $err\r\n",FILE_APPEND);
							$failedCount+=1;
						}
					}
				}
			}
			else
			{
				$failedCount+=1;
				$err=mysqli_error($link);
				file_put_contents('insertLogs.log',"error insert channel $params[0]: query is $query, error: $err\r\n",FILE_APPEND);
			}
		$progressBar->advance();
	}
	echo "\r\nChannel insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"channel insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);
