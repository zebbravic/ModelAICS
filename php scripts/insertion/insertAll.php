<?php

	file_put_contents('insertLogs.log',"\r\n",FILE_APPEND);
	file_put_contents('insertLogs.log',"\r\n",FILE_APPEND);
	$curDate = date("Y-m-d H:i");
	file_put_contents('insertLogs.log',"filling db starts at $curDate\r\n",FILE_APPEND);
	include 'insertChannels.php';
	include 'insertDouts.php';
	include 'insertActuators.php';
	include 'insertBuildings.php';
	include 'insertSections.php';
	include 'insertFloors.php';
	include 'insertAppartments.php';
	include 'insertInfrastructur.php';
	$curDate = date("Y-m-d H:i");
	file_put_contents('insertLogs.log',"filling db ends at $curDate\r\n",FILE_APPEND);
