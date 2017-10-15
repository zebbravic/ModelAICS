<?php

	$hostname = "localhost";
	$username = "dbUser";
	$password = "secretPass";
	$dbName = "maket_info";
#	if (isset($_POST['objects']))
	if($argc>1)
	{
#		$channels=explode(";",$_POST['objects']);
		$channels=explode(";",$argv[1]);
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
		}
#		echo $endStr;
#		$query="select dout.pin as pin,
#		dout.controllerAddress as adress,
#		dout.mode as mode,
#		channel.name as channel
#		from dout
#		join channel on dout.channelID=channel.id
#		where dout.controllerAddress=$adress
#		order by 1";
#		$actuators=array();
#		if($result=mysqli_query($link,$query))
#		{
#
#		while($row = mysqli_fetch_array($result))
#		{
			#$actuators['actuators'][]['type']='dOut';

			#$curBuild = $row[0];
#			$tempAct=array('type'=>'dOut','pin'=>$row[0],'adress'=>$row[1],'mode'=>$row[2],'channel'=>$row[3]);
#			$actuators['actuators'][]=$tempAct;
#		}
#
#		$query="select pin as pin,
#		controllerAddress as adress,
#		lentController,
#		size,
#		num
#		from segments
#		where segments.controllerAddress=$adress
#		order by 1";
#		$result=mysqli_query($link,$query);
#		while($row = mysqli_fetch_array($result))
#		{
			#$actuators['actuators'][]['type']='dOut';

			#$curBuild = $row[0];
#			$tempAct=array('type'=>'segment','pin'=>$row[0],'adress'=>$row[1],'lentController'=>$row[2],'size'=>$row[3],'num'=>$row[4]);
#			$actuators['actuators'][]=$tempAct;
#		}
#		$query="select ch.name as channel,
#		s.controllerAddress as adress,
#		l.mode as mode,
#		l.red as red,
#		l.green as green,
#		l.blue as blue,
#		l.flowSize  as flowSize,
#		l.interruptSize as interruptSize,
#		s.num as segNum
#		from lents  l
#		join segmentInLent SIL on l.id = SIL.lentsID
#		join segments s on s.id = SIL.segmentID
#		join channel ch on ch.id=l.channelID
#		where s.controllerAddress=$adress
#		order by 1";
#		#printf("%d", count($actuators['actuators']));
#		$result=mysqli_query($link,$query);
#		while($row = mysqli_fetch_array($result))
#		{
#			$curCh=$row[0];
#			if($actuators['actuators'][count($actuators['actuators'])-1]['type']!='lent' or $curCh!=  $actuators['actuators'][count($actuators['actuators'])-1]['channel'])
#			{
#				$tempAct=array('type'=>'lent','channel'=>$row[0],'adress'=>$row[1],'mode'=>$row[2],'red'=>$row[3],'green'=>$row[4],'blue'=>$row[5],'flowSize'=>$row[6],'interruptSize'=>$row[7]);
#				$actuators['actuators'][]=$tempAct;
#			}
#			$actuators['actuators'][count($actuators['actuators'])-1]['segArr'][]=$row[8];
#		}
#		$str="";
#		$cnt = 0;
#		while($cnt<count($actuators['actuators']))
#		{
#			if($actuators['actuators'][$cnt]['type']=='dOut')
#			{
#				$pin = $actuators['actuators'][$cnt]['pin'];
#				$adress = $actuators['actuators'][$cnt]['adress'];
#				$mode = $actuators['actuators'][$cnt]['mode'];
#				$channel =  $actuators['actuators'][$cnt]['channel'];
#				$str.="0 $adress $pin $channel $mode\n";
#			}
#			if($actuators['actuators'][$cnt]['type']=='segment')
#			{
#				$pin = $actuators['actuators'][$cnt]['pin'];
#				$adress = $actuators['actuators'][$cnt]['adress'];
#				$lentController = $actuators['actuators'][$cnt]['lentController'];
#				$size = $actuators['actuators'][$cnt]['size'];
#				$num = $actuators['actuators'][$cnt]['num'];
#				$str.="1 $adress $pin $size $num $lentController\n";
#			}
#			if($actuators['actuators'][$cnt]['type']=='lent')
#			{
#				$channel = $actuators['actuators'][$cnt]['channel'];
#				$adress = $actuators['actuators'][$cnt]['adress'];
#				$mode = $actuators['actuators'][$cnt]['mode'];
#				$red =  $actuators['actuators'][$cnt]['red'];
#				$green = $actuators['actuators'][$cnt]['green'];
#				$blue = $actuators['actuators'][$cnt]['blue'];
#				#$flowSize = $actuators['actuators'][$cnt]['flowSize'];
#				#$interruptSize = $actuators['actuators'][$cnt]['interruptSize'];
#				$str.="2 $adress $channel $mode $red $green $blue $flowSize $interruptSize";
#				$segCnt = count($actuators['actuators'][$cnt]['segArr']);
#				$str.=" $segCnt";
#				$arr = $actuators['actuators'][$cnt]['segArr'];
#				foreach ($arr as $value) {
#					$str.=" $value";
#				}
#				unset($value);
#				$str.="\n";
#			}
#			$cnt=$cnt+1;
#		}
#		echo $str;
#		}
#		else
#		{
#			$actuators['errcode']=0;
#			$actuators['err']='no flats found';
#		}
		#echo $str;
		#echo json_encode($actuators);
	}
