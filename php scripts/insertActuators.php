<?php

	$hostname = "localhost";
	$username = "root";
	$password = "sLacia2017";
	$dbName = "maket_info";
	$buildings=array();
	$buildings=file('csv/actuators.csv',FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
	$link=MYSQLI_CONNECT($hostname,$username,$password,$dbName) OR DIE("Error creating connection");
	if (mysqli_connect_errno()) {
		printf("�� ������� ������������: %s\n", mysqli_connect_error());
		exit();
	}
	foreach($buildings as $building)
	{
		$params=explode(";",$building);
		if($params[1]!='-')
		{
			$query="insert into metaDOut (channelID, mode) values ((select id from channel where name='$params[0]'), '0')";
			if($result=mysqli_query($link,$query))
			{
				echo "success insert metaDOut channel $params[0], mode 0\r\n";
			}
			else
			{
				echo "error insert metaDOut channel $params[0], mode 0\r\n";
				echo "$query\r\n";
			}
			$channels=explode(",",$params[1]);
			foreach ($channels as $channel)
			{
				$query="insert into doutsInMeta (metaDOutID, doutID) values ((select id from metaDOut where channelID=(select id from channel where name='$params[0]') and mode ='0'), (select id from dout where num='$channel'))";
				if($result=mysqli_query($link,$query))
				{
					echo "success insert doutIntMeta channel $params[0], mode 0 num $channel\r\n";
				}
				else
				{
					echo "error insert doutIntMeta channel $params[0], mode 0 num $channel\r\n";
					echo "$query\r\n";
				}

			}
			unset($channel);
			}
			if($params[2]!='-')
			{
				$query="insert into metaDOut (channelID, mode) values ((select id from channel where name='$params[0]'), '1')";
				if($result=mysqli_query($link,$query))
				{
					echo "success insert metaDOut channel $params[0], mode 1\r\n";
				}
				else
				{
					echo "error insert metaDOut channel $params[0], mode 1\r\n";
					echo "$query\r\n";
				}
				$channels=explode(",",$params[2]);
				foreach ($channels as $channel)
				{
					$query="insert into doutsInMeta (metaDOutID, doutID) values ((select id from metaDOut where channelID=(select id from channel where name='$params[0]') and mode ='1'), (select id from dout where num='$channel'))";
					if($result=mysqli_query($link,$query))
					{
						echo "success insert doutIntMeta channel $params[0], mode 1 num $channel\r\n";
					}
					else
					{
						echo "error insert doutIntMeta channel $params[0], mode 1 num $channel\r\n";
						echo "$query\r\n";
					}

				}
				unset($channel);
				}
				if($params[3]!='-')
				{
					$query="insert into metaDOut (channelID, mode) values ((select id from channel where name='$params[0]'), '2')";
					if($result=mysqli_query($link,$query))
					{
						echo "success insert metaDOut channel $params[0], mode 2\r\n";
					}
					else
					{
						echo "error insert metaDOut channel $params[0], mode 2\r\n";
						echo "$query\r\n";
					}
					$channels=explode(",",$params[3]);
					foreach ($channels as $channel)
					{
						$query="insert into doutsInMeta (metaDOutID, doutID) values ((select id from metaDOut where channelID=(select id from channel where name='$params[0]') and mode ='2'), (select id from dout where num='$channel'))";
						if($result=mysqli_query($link,$query))
						{
							echo "success insert doutIntMeta channel $params[0], mode 2 num $channel\r\n";
						}
						else
						{
							echo "error insert doutIntMeta channel $params[0], mode 2 num $channel\r\n";
							echo "$query\r\n";
						}

					}
					 unset($channel);
					}
					if($params[4]!='-')
					{
						$colors=explode(",",$params[5]);
						$query="insert into lents (channelID, mode,red,green,blue) values ((select id from channel where name='$params[0]'), '0', '$colors[0]', '$colors[1]', '$colors[2]')";
						if(!$result=mysqli_query($link,$query))
						{
							echo "error insert lents channel $params[0], mode 0\r\n";
							echo "$query\r\n";
						}
						$channels=explode(",",$params[4]);
						foreach ($channels as $channel)
						{
							$query="insert into segmentInLent (lentsID,segmentID) values ((select id from lents where channelID=(select id from channel where name='$params[0]') and mode ='0'), (select id from segments where num='$channel'))";
							if(!$result=mysqli_query($link,$query))
							{
								echo "error insert doutIntMeta channel $params[0], mode 0 num $channel\r\n";
								echo "$query\r\n";
							}

						}
						unset($channel);
						}
						if($params[6]!='-')
						{
							$colors=explode(",",$params[7]);
							$query="insert into lents (channelID, mode,red,green,blue) values ((select id from channel where name='$params[0]'), '1', '$colors[0]', '$colors[1]', '$colors[2]')";
							if(!$result=mysqli_query($link,$query))
							{
								echo "error insert lents channel $params[0], mode 1\r\n";
								echo "$query\r\n";
							}
							$channels=explode(",",$params[6]);
							foreach ($channels as $channel)
							{
								$query="insert into segmentInLent (lentsID,segmentID) values ((select id from lents where channelID=(select id from channel where name='$params[0]') and mode ='1'), (select id from segments where num='$channel'))";
								if(!$result=mysqli_query($link,$query))
								{
									echo "error insert lentsIntMeta channel $params[0], mode 1 num $channel\r\n";
									echo "$query\r\n";
								}

							}
							unset($channel);
							}
							if($params[8]!='-')
							{
								$colors=explode(",",$params[9]);
								$query="insert into lents (channelID, mode,red,green,blue) values ((select id from channel where name='$params[0]'), '2', '$colors[0]', '$colors[1]', '$colors[2]')";
								if(!$result=mysqli_query($link,$query))
								{
									echo "error insert lents channel $params[0], mode 2\r\n";
									echo "$query\r\n";
								}
								$channels=explode(",",$params[8]);
								foreach ($channels as $channel)
								{
									$query="insert into segmentInLent ( lentsID,segmentID) values ((select id from lents where channelID=(select id from channel where name='$params[0]') and mode ='2'), (select id from segments where num='$channel'))";
									if(!$result=mysqli_query($link,$query))
									{
										echo "error insert doutIntMeta channel $params[0], mode 2 num $channel\r\n";
										echo "$query\r\n";
									}

								}
								unset($channel);
								}
								if($params[10]!='-')
								{
									$colors=explode(",",$params[11]);
									$query="insert into lents (channelID, mode,red,green,blue) values ((select id from channel where name='$params[0]'), '3', '$colors[0]', '$colors[1]', '$colors[2]')";
									if(!$result=mysqli_query($link,$query))
									{
										echo "error insert lents channel $params[0], mode 3\r\n";
										echo "$query\r\n";
									}
									$channels=explode(",",$params[10]);
									foreach ($channels as $channel)
									{
										$query="insert into segmentInLent (lentsID,segmentID) values ((select id from lents where channelID=(select id from channel where name='$params[0]') and mode ='3'), (select id from segments where num='$channel'))";
										if(!$result=mysqli_query($link,$query))
										{
											echo "error insert doutIntMeta channel $params[0], mode 3 num $channel\r\n";
											echo "$query\r\n";
										}

									}
									unset($channel);
									}

		}
