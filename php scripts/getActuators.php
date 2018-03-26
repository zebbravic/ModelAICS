<?php

$hostname = "localhost";
$username = "dbUser";
$password = "secretPass";
$dbName = "maket_info";
	if ($argc>1)
	{
		$adress=$argv[1];
		$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");

		if (mysqli_connect_errno()) {
			printf("Не удалось подключиться: %s\n", mysqli_connect_error());
			exit();
		}


		$actuators=array();


		##### dout from 2017

		$query="select pin as pin,
		controllerAddress as adress,
		num as num
		from dout
		where dout.controllerAddress=$adress
		order by 1";
		$result=mysqli_query($link,$query);
		while($row = mysqli_fetch_array($result))
		{
			#$actuators['actuators'][]['type']='dOut';

			#$curBuild = $row[0];
			$tempAct=array('type'=>'0','pin'=>$row[0],'adress'=>$row[1],'num'=>$row[2]);
			$actuators['actuators'][]=$tempAct;
		}

		#### metaDOut from 2017

#		$query="select ch.name as channel,
#		d.controllerAddress as adress,
#		m.mode as mode,
#		m.id as id,
#		d.num as segNum
#
#		from metaDOut m
#		join doutsInMeta DIM on m.id = DIM.metaDOutID
#		join dout d on d.id = DIM.doutID
#		join channel ch on ch.id=m.channelID
#		where d.controllerAddress=$adress
#		order by 1,3";
#		#printf("%d", count($actuators['actuators']));
#		$result=mysqli_query($link,$query);
#		while($row = mysqli_fetch_array($result))
#		{
#			$curCh=$row[3];
#			if($actuators['actuators'][count($actuators['actuators'])-1]['type']!='metaDOut' or $curCh !=  $actuators['actuators'][count($actuators['actuators'])-1]['id'])
#			{
#				$tempAct=array('type'=>'metaDOut','channel'=>$row[0],'adress'=>$row[1],'mode'=>$row[2],'id'=>$row[3]);
#				$actuators['actuators'][]=$tempAct;
#			}
#			$actuators['actuators'][count($actuators['actuators'])-1]['segArr'][]=$row[4];
#		}
		
		#### metaDOut 20/02/2017 rewrite

		$query="select distinct ch.name as channel,
		d.controllerAddress as adress,
		m.mode as mode,
		m.id as id,
		m.num as num
		from metaDOut m
		join channel ch on ch.id=m.channelID
		left join doutsInMeta DIM 
			join dout d on d.id = DIM.doutID
		on m.id = DIM.metaDOutID
		left join doutInMetaRange DIMR 	
			join dout as dstart on DIMR.rangeStartDOut = dstart.id	
			join dout as dend on dend.id = DIMR.rangeEndDOut
		on DIMR.metaDOutID = m.id
		where d.controllerAddress=$adress
		or (dstart.controllerAddress<=$adress
		and dend.controllerAddress>=$adress)
		order by 1,3";
		#echo "\r\n$query\r\n";
		$result=mysqli_query($link,$query);
		while($row = mysqli_fetch_array($result))
		{
			$tempAct=array('type'=>'1','channel'=>$row[0],'adress'=>$adress,'mode'=>$row[2],'id'=>$row[3],'num'=>$row[4]);
			$actuators['actuators'][]=$tempAct;
			
			$query1="select d.num
			from dout as d
			join doutsInMeta DIM on d.id = DIM.doutID
			where DIM.metaDOutID = {$row[3]}
			and d.controllerAddress=$adress
			order by 1";
			
			$result1=mysqli_query($link,$query1);
			while($row1 = mysqli_fetch_array($result1))
			{
				$actuators['actuators'][count($actuators['actuators'])-1]['segArr'][]=$row1[0];
			}

			$query1="select dstart.num as start, dend.num as end
			from dout as dstart
			join doutInMetaRange DIMR on dstart.id = DIMR.rangeStartDOut
			join dout as dend on dend.id = DIMR.rangeEndDOut
			where DIMR.metaDOutID = {$row[3]}
			and (dstart.controllerAddress<=$adress
			and dend.controllerAddress>=$adress)
			order by 1";
			#echo "\r\n$query1\r\n";
			
			$result1=mysqli_query($link,$query1);
			while($row1 = mysqli_fetch_array($result1))
			{
				$tempAct=array('start'=>$row1[0],'end'=>$row1[1]);
				$actuators['actuators'][count($actuators['actuators'])-1]['segRange'][]=$tempAct;
			}

					####	dout randomizer 20.02
			if($actuators['actuators'][count($actuators['actuators'])-1]['mode']=="4")
			{
				$query1="select ch.name as channel,
				dr.type as type, 
				dr.maxGlobal as maxGlobal,
				dr.maxPerDevice as maxPerDevice,
				dr.changeTimeOut as timeout,
				m.num
				
				from metaDOut m
				join channel as ch  on ch.id = m.channelID	
				join doutRandomizer dr on dr.metaDOutID = m.id
				where m.id = {$actuators['actuators'][count($actuators['actuators'])-1]['id']}";
				#echo "\r\n$query1\r\n";
				$result1=mysqli_query($link,$query1);
				while($row1 = mysqli_fetch_array($result1))
				{
					$tempAct=array('type'=>'4','channel'=>$row1[0],'randomType'=>$row1[1],'randomMaxGlobal'=>$row1[2],'randomMaxPerDevice'=>$row1[3], 'randomTimeout'=>$row1[4],'metaDOutNum'=>$row1[5]);
					$actuators['actuators'][]=$tempAct;
				}
			}
		}
		

		####	segment from 2017

		$query="select pin as pin,
		controllerAddress as adress,
		lentController,
		size,
		num
		from segments
		where segments.controllerAddress=$adress
		order by 1";
		$result=mysqli_query($link,$query);
		while($row = mysqli_fetch_array($result))
		{
			#$actuators['actuators'][]['type']='dOut';

			#$curBuild = $row[0];
			$tempAct=array('type'=>'segment','pin'=>$row[0],'adress'=>$row[1],'lentController'=>$row[2],'size'=>$row[3],'num'=>$row[4]);
			$actuators['actuators'][]=$tempAct;
		}

		####	lents from 2017

#		$query="select ch.name as channel,
#		s.controllerAddress as adress,
#		l.mode as mode,
#		l.red as red,
#		l.green as green,
#		l.blue as blue,
#		l.flowSize  as flowSize,
#		l.interruptSize as interruptSize,
#		l.id as id,
#		s.num as segNum
#		from lents  l
#		join segmentInLent SIL on l.id = SIL.lentsID
#		join segments s on s.id = SIL.segmentID
#		join channel ch on ch.id=l.channelID
#		where s.controllerAddress=$adress
#		order by 1, 3, 4,5,6";
#		#printf("%d", count($actuators['actuators']));
#		$result=mysqli_query($link,$query);
#		while($row = mysqli_fetch_array($result))
#		{
#			$curCh=$row[8];
#			if($actuators['actuators'][count($actuators['actuators'])-1]['type']!='lent' or $curCh!=  $actuators['actuators'][count($actuators['actuators'])-1]['id'])
#			{
#				$tempAct=array('type'=>'lent','channel'=>$row[0],'adress'=>$row[1],'mode'=>$row[2],'red'=>$row[3],'green'=>$row[4],'blue'=>$row[5],'flowSize'=>$row[6],'interruptSize'=>$row[7], 'id'=>$row[8]);
#				$actuators['actuators'][]=$tempAct;
#			}
#			$actuators['actuators'][count($actuators['actuators'])-1]['segArr'][]=$row[9];
#		}





		#### lents 21/02/2017 rewrite
		$query="select distinct ch.name as channel,
		s.controllerAddress as adress,
		l.mode as mode,
		l.red as red,
		l.green as green,
		l.blue as blue,
		l.flowSize  as flowSize,
		l.interruptSize as interruptSize,
		l.id as id
		l.num as num
		from lents  l
		left join segmentInLent SIL 
			join segments s on s.id = SIL.segmentID

		on l.id = SIL.lentsID
		left join segmentInLentRange SILR 	
			join segments as sstart on SILR.rangeStartSegment = sstart.id	
			join segments as send on send.id = SILR.rangeEndSegment
		on SILR.lentsID = m.id
		join channel ch on ch.id=l.channelID
		where s.controllerAddress=$adress
		or (sstart.controllerAddress<=$adress
		and send.controllerAddress>=$adress)
		order by 1, 3, 4,5,6";
		
		$result=mysqli_query($link,$query);
		while($row = mysqli_fetch_array($result))
		{
			$tempAct=array('type'=>'4','channel'=>$row[0],'adress'=>$row[1],'mode'=>$row[2],'red'=>$row[3],'green'=>$row[4],'blue'=>$row[5],'flowSize'=>$row[6],'interruptSize'=>$row[7], 'id'=>$row[8], 'num'=>$row[9]);
			$actuators['actuators'][]=$tempAct;
			
			$query1="select s.num
			from segments as s
			join segmentInLent SIL on s.id = SIL.segmentID
			where SIL.lentsID = {$row[8]}
			and s.controllerAddress=$adress
			order by 1";
			
			$result1=mysqli_query($link,$query1);
			while($row1 = mysqli_fetch_array($result1))
			{
				$actuators['actuators'][count($actuators['actuators'])-1]['segArr'][]=$row1[0];
			}

			$query1="select sstart.num as start, send.num as end
			from segments as sstart
			join segmentInLentRange SILR on sstart.id = SILR.rangeStartSegment
			join segments as send on send.id = SILR.rangeEndSegment
			where SILR.lentsID = {$row[8]}
			and (sstart.controllerAddress<=$adress
			and send.controllerAddress>=$adress)
			order by 1";
			#echo "\r\n$query1\r\n";
			
			$result1=mysqli_query($link,$query1);
			while($row1 = mysqli_fetch_array($result1))
			{
				$tempAct=array('start'=>$row1[0],'end'=>$row1[1]);
				$actuators['actuators'][count($actuators['actuators'])-1]['segRange'][]=$tempAct;
			}

		}

		echo json_encode($actuators, JSON_UNESCAPED_UNICODE);
#		$str="";
#		$cnt = 0;
#		while($cnt<count($actuators['actuators']))
#		{
#			if($actuators['actuators'][$cnt]['type']=='dOut')
#			{
#				$pin = $actuators['actuators'][$cnt]['pin'];
#				$adress = $actuators['actuators'][$cnt]['adress'];
#				$num = $actuators['actuators'][$cnt]['num'];
#				$str.="0 $adress $pin $num\n";
#			}
#			if($actuators['actuators'][$cnt]['type']=='metaDOut')
#			{
#				$channel = $actuators['actuators'][$cnt]['channel'];
#				$adress = $actuators['actuators'][$cnt]['adress'];
#				$mode = $actuators['actuators'][$cnt]['mode'];
#				#$flowSize = $actuators['actuators'][$cnt]['flowSize'];
#				#$interruptSize = $actuators['actuators'][$cnt]['interruptSize'];
#				$str.="1 $adress $channel $mode";
#				$segCnt = count($actuators['actuators'][$cnt]['segArr']);
#				$str.=" $segCnt";
#				$arr = $actuators['actuators'][$cnt]['segArr'];
#				foreach ($arr as $value) {
#					$str.=" $value";
#				}
#				unset($value);
#				$str.="\n";
#			}
#			
#			if($actuators['actuators'][$cnt]['type']=='segment')
#			{
#				$pin = $actuators['actuators'][$cnt]['pin'];
#				$adress = $actuators['actuators'][$cnt]['adress'];
#				$lentController = $actuators['actuators'][$cnt]['lentController'];
#				$size = $actuators['actuators'][$cnt]['size'];
#				$num = $actuators['actuators'][$cnt]['num'];
#				$str.="2 $adress $pin $size $num $lentController\n";
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
#				$str.="3 $adress $channel $mode $red $green $blue $flowSize $interruptSize";
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
#		#echo $str;

	#	}
	#	else
	#	{
	#		$actuators['errcode']=0;
	#		$actuators['err']='no flats found';
	#	}
		#echo $str;
		#echo json_encode($actuators);
	}
