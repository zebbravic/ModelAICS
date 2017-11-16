-- MySQL dump 10.13  Distrib 5.7.18, for Linux (x86_64)
--
-- Host: localhost    Database: maket_info
-- ------------------------------------------------------
-- Server version	5.7.18-0ubuntu0.16.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `appartments`
--

DROP TABLE IF EXISTS `appartments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `appartments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `floorID` int(11) DEFAULT NULL,
  `sectionsID` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `size` double DEFAULT NULL,
  `channelID` int(11) DEFAULT NULL,
  `roomsCnt` int(11) DEFAULT NULL,
  `number` int(11) DEFAULT NULL,
  `objName` text,
--  `tip` text,
  `status` int(11) DEFAULT NULL,
  `CRMAprtCode` text,
  PRIMARY KEY (`id`),
  KEY `channelID` (`channelID`),
  KEY `floorID` (`floorID`),
  KEY `sectionsID` (`sectionsID`),
  KEY `type` (`type`),
  CONSTRAINT `appartments_ibfk_1` FOREIGN KEY (`channelID`) REFERENCES `channel` (`id`),
  CONSTRAINT `appartments_ibfk_2` FOREIGN KEY (`floorID`) REFERENCES `floor` (`id`),
  CONSTRAINT `appartments_ibfk_3` FOREIGN KEY (`sectionsID`) REFERENCES `section` (`id`),
  CONSTRAINT `appartments_ibfk_4` FOREIGN KEY (`type`) REFERENCES `appartmentsType` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `appartmentsType`
--

DROP TABLE IF EXISTS `appartmentsType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `appartmentsType` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `building`
--

DROP TABLE IF EXISTS `building`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `building` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` text,
  `channelID` int(11) DEFAULT NULL,
  `objName` text,
  `CRMAddressBuild` text,
  `CRMBuildingID` text,
  PRIMARY KEY (`id`),
  KEY `channelID` (`channelID`),
  CONSTRAINT `building_ibfk_1` FOREIGN KEY (`channelID`) REFERENCES `channel` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `channel`
--

DROP TABLE IF EXISTS `channel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `channel` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dout`
--

DROP TABLE IF EXISTS `dout`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dout` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pin` int(11) DEFAULT NULL,
  `controllerAddress` int(11) DEFAULT NULL,
  `num` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `doutsInMeta`
--

DROP TABLE IF EXISTS `doutsInMeta`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `doutsInMeta` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `metaDOutID` int(11) DEFAULT NULL,
  `doutID` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `metaDOutID` (`metaDOutID`),
  KEY `doutID` (`doutID`),
  CONSTRAINT `doutsInMeta_ibfk_1` FOREIGN KEY (`metaDOutID`) REFERENCES `metaDOut` (`id`),
  CONSTRAINT `doutsInMeta_ibfk_2` FOREIGN KEY (`doutID`) REFERENCES `dout` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `floor`
--

DROP TABLE IF EXISTS `floor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `floor` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `buildingID` int(11) NOT NULL,
  `numInBuilding` int(11) NOT NULL,
  `channelID` int(11) DEFAULT NULL,
  `objName` text,
  PRIMARY KEY (`id`),
  KEY `channelID` (`channelID`),
  KEY `buildingID` (`buildingID`),
  CONSTRAINT `floor_ibfk_1` FOREIGN KEY (`channelID`) REFERENCES `channel` (`id`),
  CONSTRAINT `floor_ibfk_2` FOREIGN KEY (`buildingID`) REFERENCES `building` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `infrastructure`
--

DROP TABLE IF EXISTS `infrastructure`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `infrastructure` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` text,
  `type` text,
  `description` text,
  `channelID` int(11) DEFAULT NULL,
  `objName` text,
  PRIMARY KEY (`id`),
  KEY `channelID` (`channelID`),
  CONSTRAINT `infrastructure_ibfk_1` FOREIGN KEY (`channelID`) REFERENCES `channel` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lents`
--

DROP TABLE IF EXISTS `lents`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lents` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `mode` int(11) DEFAULT NULL,
  `channelID` int(11) DEFAULT NULL,
  `flowSize` int(11) DEFAULT NULL,
  `interruptSize` int(11) DEFAULT NULL,
  `red` tinyint(3) unsigned DEFAULT NULL,
  `green` tinyint(3) unsigned DEFAULT NULL,
  `blue` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `channelID` (`channelID`),
  CONSTRAINT `lents_ibfk_1` FOREIGN KEY (`channelID`) REFERENCES `channel` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `metaDOut`
--

DROP TABLE IF EXISTS `metaDOut`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metaDOut` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `mode` int(11) DEFAULT NULL,
  `channelID` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `channelID` (`channelID`),
  CONSTRAINT `metaDOut_ibfk_1` FOREIGN KEY (`channelID`) REFERENCES `channel` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `paths`
--

DROP TABLE IF EXISTS `paths`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `paths` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `buildingID` int(11) DEFAULT NULL,
  `infrasructureID` int(11) DEFAULT NULL,
  `channelID` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `channelID` (`channelID`),
  KEY `buildingID` (`buildingID`),
  KEY `infrasructureID` (`infrasructureID`),
  CONSTRAINT `paths_ibfk_1` FOREIGN KEY (`channelID`) REFERENCES `channel` (`id`),
  CONSTRAINT `paths_ibfk_2` FOREIGN KEY (`buildingID`) REFERENCES `building` (`id`),
  CONSTRAINT `paths_ibfk_3` FOREIGN KEY (`infrasructureID`) REFERENCES `infrastructure` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `section`
--

DROP TABLE IF EXISTS `section`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `section` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `buildingID` int(11) NOT NULL,
  `name` text,
  `channelID` int(11) DEFAULT NULL,
  `objName` text,
  `CRMSectionName` text,
  `CRMSectionID` text,
  PRIMARY KEY (`id`),
  KEY `channelID` (`channelID`),
  KEY `buildingID` (`buildingID`),
  CONSTRAINT `section_ibfk_1` FOREIGN KEY (`channelID`) REFERENCES `channel` (`id`),
  CONSTRAINT `section_ibfk_2` FOREIGN KEY (`buildingID`) REFERENCES `building` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `segmentInLent`
--

DROP TABLE IF EXISTS `segmentInLent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `segmentInLent` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `lentsID` int(11) DEFAULT NULL,
  `segmentID` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `lentsID` (`lentsID`),
  KEY `segmentID` (`segmentID`),
  CONSTRAINT `segmentInLent_ibfk_1` FOREIGN KEY (`lentsID`) REFERENCES `lents` (`id`),
  CONSTRAINT `segmentInLent_ibfk_2` FOREIGN KEY (`segmentID`) REFERENCES `segments` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `segments`
--

DROP TABLE IF EXISTS `segments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `segments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pin` int(11) DEFAULT NULL,
  `controllerAddress` int(11) DEFAULT NULL,
  `lentController` int(11) DEFAULT NULL,
  `size` int(11) DEFAULT NULL,
  `description` text,
  `num` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-06-12  1:20:33
