<?php
	require_once 'ProgressBar/Manager.php';
	require_once 'ProgressBar/Registry.php';
	$hostname = "localhost";
	$username = "installer";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$douts=array();
	$douts=file('csv/dout.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("Не удалось подключиться: %s\n", mysqli_connect_error());
		exit();
	}
	$successCount=0;
	$failedCount=0;
	echo "Dout insert begins\r\n";
	$progressBar=new \ProgressBar\Manager(0,count($douts));
	file_put_contents('insertLogs.log',"dout insert begins\r\n",FILE_APPEND);
	foreach($douts as $dout)
	{
		$params=explode(";",$dout);
		$query="insert into dout (pin,controllerAddress,num,comment) values ($params[1],$params[2],$params[0],'$params[3]')";
		if($result=mysqli_query($link,$query))
		{
			$successCount+=1;
		}
		else
		{
			$err=mysqli_error($link);
			file_put_contents('insertLogs.log',"error insert dout $params[0]: query is $query, error: $err\r\n",FILE_APPEND);
			$failedCount+=1;
		}
		$progressBar->advance();

	}
	unset($dout);
	echo "\r\nDOut insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"dout insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);
	$successCount=0;
	$failedCount=0;
	file_put_contents('insertLogs.log',"segment insert begins\r\n",FILE_APPEND);
	$douts=file('csv/segments.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	echo "Segment insert begins\r\n";
	$progressBar1=new \ProgressBar\Manager(0,count($douts));
	foreach($douts as $dout)
	{
		$params=explode(";",$dout);
		$query="insert into segments (pin,controllerAddress,num,size,comment) values ($params[1],$params[2],$params[0],$params[3],'$params[4]')";
				echo $query;
		if($result=mysqli_query($link,$query))
		{
			$successCount+=1;
		}
		else
		{
			$err=mysqli_error($link);
			file_put_contents('insertLogs.log',"error insert segment $params[0]: query is $query, error: $err\r\n",FILE_APPEND);
			$failedCount+=1;
		}
		$progressBar1->advance();

	}
	echo "\r\nSegment insert done. Success $successCount, failed $failedCount\r\n";
	file_put_contents('insertLogs.log',"segment insert done! Succes $successCount, failed $failedCount\r\n",FILE_APPEND);

