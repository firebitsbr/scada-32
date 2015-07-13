SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `scada` ;
USE `scada` ;

-- -----------------------------------------------------
-- Table `scada`.`NormalPackets`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `scada`.`NormalPackets` ;

CREATE  TABLE IF NOT EXISTS `scada`.`NormalPackets` (
  `idNormalPackets` INT NOT NULL AUTO_INCREMENT ,
  `frame.number` INT NULL ,
  `frame.time_relative` DOUBLE NULL ,
  `frame.time_delta_displayed` DOUBLE NULL ,
  `frame.len` INT NULL ,
  `ip.proto` VARCHAR(45) NULL ,
  `ip.version` VARCHAR(45) NULL ,
  `ip.src` VARCHAR(45) NULL ,
  `eth.src` VARCHAR(45) NULL ,
  `ip.dst` VARCHAR(45) NULL ,
  `eth.dst` VARCHAR(45) NULL ,
  `mbtcp.modbus.unit_id` VARCHAR(45) NULL ,
  `tcp.srcport` VARCHAR(45) NULL ,
  `tcp.dstport` VARCHAR(45) NULL ,
  `mbtcp.prot_id` VARCHAR(45) NULL ,
  `mbtcp.trans_id` INT NULL ,
  `mbtcp.len` INT NULL ,
  `mbtcp.modbus.func_code` VARCHAR(45) NULL ,
  `mbtcp.modbus.reference_num` VARCHAR(45) NULL ,
  `mbtcp.modbus.word_cnt` INT NULL ,
  `respFrNumber` INT NULL ,
  `respTimeRel` DOUBLE NULL ,
  `respTimeDelta` DOUBLE NULL ,
  `respLen` INT NULL ,
  `respIpSrc` VARCHAR(45) NULL ,
  `respEthSrc` VARCHAR(45) NULL ,
  `respIpDest` VARCHAR(45) NULL ,
  `respEthDest` VARCHAR(45) NULL ,
  `respUnitId` VARCHAR(45) NULL ,
  `respSrcport` VARCHAR(45) NULL ,
  `respDstPort` VARCHAR(45) NULL ,
  `respProtId` VARCHAR(45) NULL ,
  `respTransId` VARCHAR(45) NULL ,
  `respMbtcpLen` INT NULL ,
  `respFuncCode` VARCHAR(45) NULL ,
  `mbtcp.modbus.data` VARCHAR(45) NULL ,
  `d` INT NULL ,
  PRIMARY KEY (`idNormalPackets`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci
COMMENT = 'Packets sniffed from network during normal activity.';



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
