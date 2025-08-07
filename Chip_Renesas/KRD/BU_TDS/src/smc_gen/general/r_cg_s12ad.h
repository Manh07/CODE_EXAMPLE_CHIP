/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : r_cg_s12ad.h
* Version          : 1.1.110
* Device(s)        : R5F51303AxFM
* Description      : General header file for S12AD peripheral.
***********************************************************************************************************************/

#ifndef S12AD_H
#define S12AD_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    A/D control register (ADCSR)
*/
/* Double Trigger Channel Select ( DBLANS) */
#define _0000_AD_DBCHANNEL_AN000               (0x0000U) /* AN000 is used for double trigger mode */
#define _0001_AD_DBCHANNEL_AN001               (0x0001U) /* AN001 is used for double trigger mode */
#define _0002_AD_DBCHANNEL_AN002               (0x0002U) /* AN002 is used for double trigger mode */
#define _0003_AD_DBCHANNEL_AN003               (0x0003U) /* AN003 is used for double trigger mode */
#define _0004_AD_DBCHANNEL_AN004               (0x0004U) /* AN004 is used for double trigger mode */
#define _0005_AD_DBCHANNEL_AN005               (0x0005U) /* AN005 is used for double trigger mode */
#define _0006_AD_DBCHANNEL_AN006               (0x0006U) /* AN006 is used for double trigger mode */
#define _0007_AD_DBCHANNEL_AN007               (0x0007U) /* AN007 is used for double trigger mode */
#define _0010_AD_DBCHANNEL_AN016               (0x0010U) /* AN016 is used for double trigger mode */
#define _0011_AD_DBCHANNEL_AN017               (0x0011U) /* AN017 is used for double trigger mode */
#define _0012_AD_DBCHANNEL_AN018               (0x0012U) /* AN018 is used for double trigger mode */
#define _0013_AD_DBCHANNEL_AN019               (0x0013U) /* AN019 is used for double trigger mode */
#define _0014_AD_DBCHANNEL_AN020               (0x0014U) /* AN020 is used for double trigger mode */
#define _0015_AD_DBCHANNEL_AN021               (0x0015U) /* AN021 is used for double trigger mode */
#define _0016_AD_DBCHANNEL_AN022               (0x0016U) /* AN022 is used for double trigger mode */
#define _0017_AD_DBCHANNEL_AN023               (0x0017U) /* AN023 is used for double trigger mode */
#define _0018_AD_DBCHANNEL_AN024               (0x0018U) /* AN024 is used for double trigger mode */
#define _0019_AD_DBCHANNEL_AN025               (0x0019U) /* AN025 is used for double trigger mode */
#define _001A_AD_DBCHANNEL_AN026               (0x001AU) /* AN026 is used for double trigger mode */
#define _001B_AD_DBCHANNEL_AN027               (0x001BU) /* AN027 is used for double trigger mode */
#define _001C_AD_DBCHANNEL_AN028               (0x001CU) /* AN028 is used for double trigger mode */
#define _001D_AD_DBCHANNEL_AN029               (0x001DU) /* AN029 is used for double trigger mode */
#define _001E_AD_DBCHANNEL_AN030               (0x001EU) /* AN030 is used for double trigger mode */
#define _001F_AD_DBCHANNEL_AN031               (0x001FU) /* AN031 is used for double trigger mode */
/* Group B Scan End Interrupt Enable (GBADIE) */
#define _0000_AD_GBADI_DISABLE                 (0x0000U) /* Disables S12GBADI interrupt generation upon group B scan
                                                            completion */
#define _0040_AD_GBADI_ENABLE                  (0x0040U) /* Enables S12GBADI interrupt generation upon group B scan
                                                            completion */
/* Double Trigger Mode Select (DBLE) */
#define _0000_AD_DBLTRIGGER_DISABLE            (0x0000U) /* Disable double trigger mode */
#define _0080_AD_DBLTRIGGER_ENABLE             (0x0080U) /* Enable double trigger mode */
/* Trigger Select (EXTRG) */
#define _0000_AD_SYNC_TRIGGER                  (0x0000U) /* A/D conversion started by synchronous trigger */
#define _0100_AD_ASYNC_TRIGGER                 (0x0100U) /* A/D conversion started by asynchronous trigger */
/* Trigger Start Enable (TRGE) */
#define _0000_AD_SYNCASYNCTRG_DISABLE          (0x0000U) /* A/D conversion synchronous or asynchronous trigger disable */
#define _0200_AD_SYNCASYNCTRG_ENABLE           (0x0200U) /* A/D conversion synchronous or asynchronous trigger enable */
/* A/D Conversion Select (ADHSC) */
#define _0000_AD_HIGH_SPEED                    (0x0000U) /* High-speed conversion */
#define _0400_AD_LOW_CURRENT                   (0x0400U) /* Low-current conversion */
/* Scan End Interrupt Enable (ADIE) */
#define _0000_AD_SCAN_END_INTERRUPT_DISABLE    (0x0000U) /* Disable S12ADI0 interrupt generation upon scan completion */
#define _1000_AD_SCAN_END_INTERRUPT_ENABLE     (0x1000U) /* Enable S12ADI0 interrupt generation upon scan completion */
/* Scan Mode Select (ADCS) */
#define _0000_AD_SINGLE_SCAN_MODE              (0x0000U) /* Single scan mode */
#define _2000_AD_GROUP_SCAN_MODE               (0x2000U) /* Group scan mode */
#define _4000_AD_CONTINUOUS_SCAN_MODE          (0x4000U) /* Continuous scan mode */
/* A/D Conversion Start (ADST) */
#define _0000_AD_CONVERSION_STOP               (0x0000U) /* Stop A/D conversion */
#define _8000_AD_CONVERSION_START              (0x8000U) /* Start A/D conversion */

/*
    A/D Channel Select Register A0 (ADANSA0)
*/
#define _0001_AD_AN000_USED                    (0x0001U) /* AN000 is subjected to conversion */
#define _0001_AD_AN000_GROUPA_USED             (0x0001U) /* AN000 is subjected to group A conversion */
#define _0002_AD_AN001_USED                    (0x0002U) /* AN001 is subjected to conversion */
#define _0002_AD_AN001_GROUPA_USED             (0x0002U) /* AN001 is subjected to group A conversion */
#define _0004_AD_AN002_USED                    (0x0004U) /* AN002 is subjected to conversion */
#define _0004_AD_AN002_GROUPA_USED             (0x0004U) /* AN002 is subjected to group A conversion */
#define _0008_AD_AN003_USED                    (0x0008U) /* AN003 is subjected to conversion */
#define _0008_AD_AN003_GROUPA_USED             (0x0008U) /* AN003 is subjected to group A conversion */
#define _0010_AD_AN004_USED                    (0x0010U) /* AN004 is subjected to conversion */
#define _0010_AD_AN004_GROUPA_USED             (0x0010U) /* AN004 is subjected to group A conversion */
#define _0020_AD_AN005_USED                    (0x0020U) /* AN005 is subjected to conversion */
#define _0020_AD_AN005_GROUPA_USED             (0x0020U) /* AN005 is subjected to group A conversion */
#define _0040_AD_AN006_USED                    (0x0040U) /* AN006 is subjected to conversion */
#define _0040_AD_AN006_GROUPA_USED             (0x0040U) /* AN006 is subjected to group A conversion */
#define _0080_AD_AN007_USED                    (0x0080U) /* AN007 is subjected to conversion */
#define _0080_AD_AN007_GROUPA_USED             (0x0080U) /* AN007 is subjected to group A conversion */

/*
    A/D Channel Select Register A1 (ADANSA1)
*/
#define _0001_AD_AN016_USED                    (0x0001U) /* AN016 is subjected to conversion */
#define _0001_AD_AN016_GROUPA_USED             (0x0001U) /* AN016 is subjected to group A conversion */
#define _0002_AD_AN017_USED                    (0x0002U) /* AN017 is subjected to conversion */
#define _0002_AD_AN017_GROUPA_USED             (0x0002U) /* AN017 is subjected to group A conversion */
#define _0004_AD_AN018_USED                    (0x0004U) /* AN018 is subjected to conversion */
#define _0004_AD_AN018_GROUPA_USED             (0x0004U) /* AN018 is subjected to group A conversion */
#define _0008_AD_AN019_USED                    (0x0008U) /* AN019 is subjected to conversion */
#define _0008_AD_AN019_GROUPA_USED             (0x0008U) /* AN019 is subjected to group A conversion */
#define _0010_AD_AN020_USED                    (0x0010U) /* AN020 is subjected to conversion */
#define _0010_AD_AN020_GROUPA_USED             (0x0010U) /* AN020 is subjected to group A conversion */
#define _0020_AD_AN021_USED                    (0x0020U) /* AN021 is subjected to conversion */
#define _0020_AD_AN021_GROUPA_USED             (0x0020U) /* AN021 is subjected to group A conversion */
#define _0040_AD_AN022_USED                    (0x0040U) /* AN022 is subjected to conversion */
#define _0040_AD_AN022_GROUPA_USED             (0x0040U) /* AN022 is subjected to group A conversion */
#define _0080_AD_AN023_USED                    (0x0080U) /* AN023 is subjected to conversion */
#define _0080_AD_AN023_GROUPA_USED             (0x0080U) /* AN023 is subjected to group A conversion */
#define _0100_AD_AN024_USED                    (0x0100U) /* AN024 is subjected to conversion */
#define _0100_AD_AN024_GROUPA_USED             (0x0100U) /* AN024 is subjected to group A conversion */
#define _0200_AD_AN025_USED                    (0x0200U) /* AN025 is subjected to conversion */
#define _0200_AD_AN025_GROUPA_USED             (0x0200U) /* AN025 is subjected to group A conversion */
#define _0400_AD_AN026_USED                    (0x0400U) /* AN026 is subjected to conversion */
#define _0400_AD_AN026_GROUPA_USED             (0x0400U) /* AN026 is subjected to group A conversion */
#define _0800_AD_AN027_USED                    (0x0800U) /* AN027 is subjected to conversion */
#define _0800_AD_AN027_GROUPA_USED             (0x0800U) /* AN027 is subjected to group A conversion */
#define _1000_AD_AN028_USED                    (0x1000U) /* AN028 is subjected to conversion */
#define _1000_AD_AN028_GROUPA_USED             (0x1000U) /* AN028 is subjected to group A conversion */
#define _2000_AD_AN029_USED                    (0x2000U) /* AN029 is subjected to conversion */
#define _2000_AD_AN029_GROUPA_USED             (0x2000U) /* AN029 is subjected to group A conversion */
#define _4000_AD_AN030_USED                    (0x4000U) /* AN030 is subjected to conversion */
#define _4000_AD_AN030_GROUPA_USED             (0x4000U) /* AN030 is subjected to group A conversion */
#define _8000_AD_AN031_USED                    (0x8000U) /* AN031 is subjected to conversion */
#define _8000_AD_AN031_GROUPA_USED             (0x8000U) /* AN031 is subjected to group A conversion */

/*
    A/D Channel Select Register A0 (ADANSB0)
*/
#define _0001_AD_AN000_GROUPB_USED             (0x0001U) /* AN000 is subjected to group B conversion */
#define _0002_AD_AN001_GROUPB_USED             (0x0002U) /* AN001 is subjected to group B conversion */
#define _0004_AD_AN002_GROUPB_USED             (0x0004U) /* AN002 is subjected to group B conversion */
#define _0008_AD_AN003_GROUPB_USED             (0x0008U) /* AN003 is subjected to group B conversion */
#define _0010_AD_AN004_GROUPB_USED             (0x0010U) /* AN004 is subjected to group B conversion */
#define _0020_AD_AN005_GROUPB_USED             (0x0020U) /* AN005 is subjected to group B conversion */
#define _0040_AD_AN006_GROUPB_USED             (0x0040U) /* AN006 is subjected to group B conversion */
#define _0080_AD_AN007_GROUPB_USED             (0x0080U) /* AN007 is subjected to group B conversion */

/*
    A/D Channel Select Register A1 (ADANSB1)
*/
#define _0001_AD_AN016_GROUPB_USED             (0x0001U) /* AN016 is subjected to group B conversion */
#define _0002_AD_AN017_GROUPB_USED             (0x0002U) /* AN017 is subjected to group B conversion */
#define _0004_AD_AN018_GROUPB_USED             (0x0004U) /* AN018 is subjected to group B conversion */
#define _0008_AD_AN019_GROUPB_USED             (0x0008U) /* AN019 is subjected to group B conversion */
#define _0010_AD_AN020_GROUPB_USED             (0x0010U) /* AN020 is subjected to group B conversion */
#define _0020_AD_AN021_GROUPB_USED             (0x0020U) /* AN021 is subjected to group B conversion */
#define _0040_AD_AN022_GROUPB_USED             (0x0040U) /* AN022 is subjected to group B conversion */
#define _0080_AD_AN023_GROUPB_USED             (0x0080U) /* AN023 is subjected to group B conversion */
#define _0100_AD_AN024_GROUPB_USED             (0x0100U) /* AN024 is subjected to group B conversion */
#define _0200_AD_AN025_GROUPB_USED             (0x0200U) /* AN025 is subjected to group B conversion */
#define _0400_AD_AN026_GROUPB_USED             (0x0400U) /* AN026 is subjected to group B conversion */
#define _0800_AD_AN027_GROUPB_USED             (0x0800U) /* AN027 is subjected to group B conversion */
#define _1000_AD_AN028_GROUPB_USED             (0x1000U) /* AN028 is subjected to group B conversion */
#define _2000_AD_AN029_GROUPB_USED             (0x2000U) /* AN029 is subjected to group B conversion */
#define _4000_AD_AN030_GROUPB_USED             (0x4000U) /* AN030 is subjected to group B conversion */
#define _8000_AD_AN031_GROUPB_USED             (0x8000U) /* AN031 is subjected to group B conversion */

/*
    A/D-Converted Value Addition/Average Mode Select Register 0 (ADADS0)
*/
#define _0001_AD_AN000_ADD_USED                (0x0001U) /* AN000 is selected to addition/average mode */
#define _0002_AD_AN001_ADD_USED                (0x0002U) /* AN001 is selected to addition/average mode */
#define _0004_AD_AN002_ADD_USED                (0x0004U) /* AN002 is selected to addition/average mode */
#define _0008_AD_AN003_ADD_USED                (0x0008U) /* AN003 is selected to addition/average mode */
#define _0010_AD_AN004_ADD_USED                (0x0010U) /* AN004 is selected to addition/average mode */
#define _0020_AD_AN005_ADD_USED                (0x0020U) /* AN005 is selected to addition/average mode */
#define _0040_AD_AN006_ADD_USED                (0x0040U) /* AN006 is selected to addition/average mode */
#define _0080_AD_AN007_ADD_USED                (0x0080U) /* AN007 is selected to addition/average mode */

/*
    A/D-Converted Value Addition/Average Mode Select Register 1 (ADADS1)
*/
#define _0001_AD_AN016_ADD_USED                (0x0001U) /* AN016 is selected to addition/average mode */
#define _0002_AD_AN017_ADD_USED                (0x0002U) /* AN017 is selected to addition/average mode */
#define _0004_AD_AN018_ADD_USED                (0x0004U) /* AN018 is selected to addition/average mode */
#define _0008_AD_AN019_ADD_USED                (0x0008U) /* AN019 is selected to addition/average mode */
#define _0010_AD_AN020_ADD_USED                (0x0010U) /* AN020 is selected to addition/average mode */
#define _0020_AD_AN021_ADD_USED                (0x0020U) /* AN021 is selected to addition/average mode */
#define _0040_AD_AN022_ADD_USED                (0x0040U) /* AN022 is selected to addition/average mode */
#define _0080_AD_AN023_ADD_USED                (0x0080U) /* AN023 is selected to addition/average mode */
#define _0100_AD_AN024_ADD_USED                (0x0100U) /* AN024 is selected to addition/average mode */
#define _0200_AD_AN025_ADD_USED                (0x0200U) /* AN025 is selected to addition/average mode */
#define _0400_AD_AN026_ADD_USED                (0x0400U) /* AN026 is selected to addition/average mode */
#define _0800_AD_AN027_ADD_USED                (0x0800U) /* AN027 is selected to addition/average mode */
#define _1000_AD_AN028_ADD_USED                (0x1000U) /* AN028 is selected to addition/average mode */
#define _2000_AD_AN029_ADD_USED                (0x2000U) /* AN029 is selected to addition/average mode */
#define _4000_AD_AN030_ADD_USED                (0x4000U) /* AN030 is selected to addition/average mode */
#define _8000_AD_AN031_ADD_USED                (0x8000U) /* AN031 is selected to addition/average mode */

/*
    A/D-Converted Value Addition/Average Count Select Register (ADADC)
*/
/* Addition Count Select (ADC) */
#define _00_AD_1_TIME_CONVERSION               (0x00U) /* 1-time conversion */
#define _01_AD_2_TIME_CONVERSION               (0x01U) /* 2-time conversion */
#define _02_AD_3_TIME_CONVERSION               (0x02U) /* 3-time conversion */
#define _03_AD_4_TIME_CONVERSION               (0x03U) /* 4-time conversion */
#define _05_AD_16_TIME_CONVERSION              (0x05U) /* 16-time conversion */
/* Average Mode Enable (AVEE) */
#define _00_AD_ADDITION_MODE                   (0x00U) /* Addition mode */
#define _80_AD_AVERAGE_MODE                    (0x80U) /* Average mode */

/*
    A/D Control Extended Register (ADCER)
*/
/* A/D Data Register Automatic Clearing Enable (ACE) */
#define _0000_AD_AUTO_CLEARING_DISABLE         (0x0000U) /* Disable auto clearing */
#define _0020_AD_AUTO_CLEARING_ENABLE          (0x0020U) /* Enable auto clearing */
/* Self-Diagnosis Conversion Voltage Select (DIAGVAL) */
#define _0100_AD_SELFTDIAGST_0V                (0x0100U) /* Self-diagnosis using a voltage of 0V */
#define _0200_AD_SELFTDIAGST_VREFH0_HALF       (0x0200U) /* Self-diagnosis using a voltage of VREFH0_1/2 */
#define _0300_AD_SELFTDIAGST_VREFH0            (0x0300U) /* Self-diagnosis using a voltage of VREFH0_1 */
/* Self-Diagnosis Mode Select (DIAGLD) */
#define _0000_AD_SELFTDIAGST_ROTATION          (0x0000U) /* Rotation mode for self-diagnosis voltage */
#define _0400_AD_SELFTDIAGST_FIX               (0x0400U) /* Fixed mode for self-diagnosis voltage */
/* Self-Diagnosis Enable (DIAGM) */
#define _0000_AD_SELFTDIAGST_DISABLE           (0x0000U) /* 12-bit A/D converter self-diagnosis disable */
#define _0800_AD_SELFTDIAGST_ENABLE            (0x0800U) /* 12-bit A/D converter self-diagnosis enable */
/* A/D Data Register Format Select (ADRFMT) */
#define _0000_AD_RIGHT_ALIGNMENT               (0x0000U) /* Right-alignment for data register format */
#define _8000_AD_LEFT_ALIGNMENT                (0x8000U) /* Left-alignment for data register format */

/*
    A/D Conversion Start Trigger Select Register (ADSTRGR)
*/
/* A/D Conversion Start Trigger Select for Group B (TRSB) */
#define _0001_AD_TRSB_TRG0AN                   (0x0001U) /* Compare match with or input capture from MTU0.TGRA */
#define _0002_AD_TRSB_TRG0BN                   (0x0002U) /* Compare match with or input capture from MTU0.TGRB */
#define _0003_AD_TRSB_TRGAN                    (0x0003U) /* Compare match with or input capture from MTU0 to MTU4.TGRA or 
                                                             underflow (trough) of MTU4.TCNT in complementary PWM mode */
#define _0004_AD_TRSB_TRG0EN                   (0x0004U) /* Compare match from MTU0.TGRE */
#define _0005_AD_TRSB_TRG0FN                   (0x0005U) /* Compare match from MTU0.TGRF */
#define _0006_AD_TRSB_TRG4AN                   (0x0006U) /* Compare match between MTU4.TADCORA and MTU4.TCNT */
#define _0007_AD_TRSB_TRG4BN                   (0x0007U) /* Compare match between MTU4.TADCORB and MTU4.TCNT */
#define _0008_AD_TRSB_TRG4ABN                  (0x0008U) /* Compare match between MTU4.TADCORA and MTU4.TCNT or
                                                             compare match between MTU4.TADCORB and MTU4.TCNT */
#define _0009_AD_TRSB_ELCTRG0N                 (0x0009U) /* Trigger from ELC */
/* A/D Conversion Start Trigger Select (TRSA) */
#define _0000_AD_TRSA_ADTRG                    (0x0000U) /* Input pin for the trigger */
#define _0100_AD_TRSA_TRG0AN                   (0x0100U) /* Compare match with or input capture from MTU0.TGRA */
#define _0200_AD_TRSA_TRG0BN                   (0x0200U) /* Compare match with or input capture from MTU0.TGRB */
#define _0300_AD_TRSA_TRGAN                    (0x0300U) /* Compare match with or input capture from MTU0 to MTU4.TGRA or 
                                                             underflow (trough) of MTU4.TCNT in complementary PWM mode */
#define _0400_AD_TRSA_TRG0EN                   (0x0400U) /* Compare match from MTU0.TGRE */
#define _0500_AD_TRSA_TRG0FN                   (0x0500U) /* Compare match from MTU0.TGRF */
#define _0600_AD_TRSA_TRG4AN                   (0x0600U) /* Compare match between MTU4.TADCORA and MTU4.TCNT */
#define _0700_AD_TRSA_TRG4BN                   (0x0700U) /* Compare match between MTU4.TADCORB and MTU4.TCNT */
#define _0800_AD_TRSA_TRG4ABN                  (0x0800U) /* Compare match between MTU4.TADCORA and MTU4.TCNT or
                                                             compare match between MTU4.TADCORB and MTU4.TCNT */
#define _0900_AD_TRSA_ELCTRG0N                 (0x0900U) /* Trigger from ELC */

/*
    A/D Conversion Extended Input Control Register (ADEXICR)
*/
/* Temperature Sensor Output A/D Converted Value Addition/Averaging Mode Select (TSSAD) */
#define _0000_AD_TEMP_ADD_UNUSED               (0x0000U) /* Temperature sensor output A/D converted value addition/
                                                            average mode disabled */
#define _0001_AD_TEMP_ADD_USED                 (0x0001U) /* Temperature sensor output A/D converted value addition/
                                                            average mode enabled */
/* Internal Reference Voltage A/D Converted Value Addition/Average Mode Select (OCSAD) */
#define _0000_AD_IREF_ADD_UNUSED               (0x0000U) /* Internal ref. voltage A/D converted value addition/average
                                                            mode disabled */
#define _0002_AD_IREF_ADD_USED                 (0x0002U) /* Internal ref. voltage A/D converted value addition/average
                                                            mode enabled */
/* Temperature Sensor Output A/D Conversion Select (TSSA) */
#define _0000_AD_TEMP_UNUSED                   (0x0000U) /* A/D conversion of temperature sensor output is disabled */
#define _0100_AD_TEMP_USED                     (0x0100U) /* A/D conversion of temperature sensor output is enabled */
#define _0100_AD_TEMP_GROUPA_USED              (0x0100U) /* A/D conversion of temperature sensor output is enabled in
                                                            group A  */
/* Internal Reference Voltage A/D Conversion Select (OCSA) */
#define _0000_AD_IREF_UNUSED                   (0x0000U) /* A/D conversion of internal reference voltage is disabled */
#define _0200_AD_IREF_USED                     (0x0200U) /* A/D conversion of internal reference voltage is enabled */
#define _0200_AD_IREF_GROUPA_USED              (0x0200U) /* A/D conversion of internal reference voltage is enabled in
                                                            group A  */

/*
    A/D Event Link Control Register (ADELCCR)
*/
/* Event Link Control (ELCC) */
#define _00_NOT_GROUPB_SCAN_COMPLETION         (0x00U) /* Event is generated on completion of the scan other than group
                                                          B in group scan mode */
#define _01_GROUPB_SCAN_COMPLETION             (0x01U) /* Event is generated on completion of the scan of group B in
                                                          group scan mode */
#define _02_ALL_SCAN_COMPLETION                (0x02U) /* Event is generated on completion of all scans */

/*
    A/D Disconnection Detection Control Register (ADDISCR)
*/
#define _00_AD_DISCONECT_UNUSED                (0x00U) /* Detection detection control unused */
#define _00_AD_DISCONECT_DISCHARGE             (0x00U) /* Detection detection control discharge */
#define _10_AD_DISCONECT_PRECHARGE             (0x10U) /* Detection detection control precharge */
#define _02_AD_DISCONECT_2ADCLK                (0x02U) /* 2 ADCLK of detection detection control precharge/discharge */
#define _03_AD_DISCONECT_3ADCLK                (0x03U) /* 3 ADCLK of detection detection control precharge/discharge */
#define _04_AD_DISCONECT_4ADCLK                (0x04U) /* 4 ADCLK of detection detection control precharge/discharge */
#define _05_AD_DISCONECT_5ADCLK                (0x05U) /* 5 ADCLK of detection detection control precharge/discharge */
#define _06_AD_DISCONECT_6ADCLK                (0x06U) /* 6 ADCLK of detection detection control precharge/discharge */
#define _07_AD_DISCONECT_7ADCLK                (0x07U) /* 7 ADCLK of detection detection control precharge/discharge */
#define _08_AD_DISCONECT_8ADCLK                (0x08U) /* 8 ADCLK of detection detection control precharge/discharge */
#define _09_AD_DISCONECT_9ADCLK                (0x09U) /* 9 ADCLK of detection detection control precharge/discharge */
#define _0A_AD_DISCONECT_10ADCLK               (0x0AU) /* 10 ADCLK of detection detection control precharge/discharge */
#define _0B_AD_DISCONECT_11ADCLK               (0x0BU) /* 11 ADCLK of detection detection control precharge/discharge */
#define _0C_AD_DISCONECT_12ADCLK               (0x0CU) /* 12 ADCLK of detection detection control precharge/discharge */
#define _0D_AD_DISCONECT_13ADCLK               (0x0DU) /* 13 ADCLK of detection detection control precharge/discharge */
#define _0E_AD_DISCONECT_14ADCLK               (0x0EU) /* 14 ADCLK of detection detection control precharge/discharge */
#define _0F_AD_DISCONECT_15ADCLK               (0x0FU) /* 15 ADCLK of detection detection control precharge/discharge */

/*
    A/D Group Scan Priority Control Register (ADGSPCR)
*/
/* Group-A Priority Control Setting (PGS) */
#define _0000_AD_GPAPRIORITY_DISABLE           (0x0000U) /* Operation is without group A priority control */
#define _0001_AD_GPAPRIORITY_ENABLE            (0x0001U) /* Operation is with group A priority control */
/* Group B Restart Setting (GBRSCN) */
#define _0000_AD_GPBRESTART_DISABLE            (0x0000U) /* Group B not restart after discontinued due to Group A
                                                            priority */
#define _0002_AD_GPBRESTART_ENABLE             (0x0002U) /* Group B restart after discontinued due to Group A priority */
/* Group B Single Cycle Scan Continuous Start (GBRP) */
#define _0000_AD_GPBSCSCS_DISABLE              (0x0000U) /* Single cycle scan for group B not continuously activated */
#define _8000_AD_GPBSCSCS_ENABLE               (0x8000U) /* Single cycle scan for group B is continuously activated */

/*
    A/D High-Potential/Low-Potential Reference Voltage Control Register (ADHVREFCNT)
*/
/* High-Potential Reference Voltage Select (HVSEL) */
#define _00_AD_HIGH_POTENTIAL_AVCC0            (0x00U) /* AVCC0 is selected as the high-potential reference voltage. */
#define _01_AD_HIGH_POTENTIAL_VREFH0           (0x01U) /* VREFH0 is selected as the high-potential reference voltage. */
/* Low-Potential Reference Voltage Select (LVSEL) */
#define _00_AD_LOW_POTENTIAL_AVSS0             (0x00U) /* AVSS0 is selected as the low-potential reference voltage. */
#define _10_AD_LOW_POTENTIAL_VREFL0            (0x10U) /* VREFL0 is selected as the low-potential reference voltage. */

/*
    A/D Comparison Function Control Register (ADCMPCR)
*/
/* Window A/B Complex Conditions Setting (CMPAB) */
#define _0000_AD_COMPLEX_CONDITION_OR          (0x0000U) /* Window A comparison condition matched OR window B
                                                            comparison condition matched */
#define _0001_AD_COMPLEX_CONDITION_EXOR        (0x0001U) /* Window A comparison condition matched EXOR window B
                                                            comparison condition matched */
#define _0002_AD_COMPLEX_CONDITION_AND         (0x0002U) /* Window A comparison condition matched AND window B
                                                            comparison condition matched */
/* Comparison Window B Enable (CMPBE) */
#define _0000_AD_WINDOWB_DISABLE               (0x0000U) /* Comparison window B disabled */
#define _0200_AD_WINDOWB_ENABLE                (0x0200U) /* Comparison window B enabled */
/* Comparison Window A Enable (CMPAE) */
#define _0000_AD_WINDOWA_DISABLE               (0x0000U) /* Comparison window A disabled */
#define _0800_AD_WINDOWA_ENABLE                (0x0800U) /* Comparison window A enabled */
/* Window Function Setting (WCMPE) */
#define _0000_AD_WINDOWFUNCTION_DISABLE        (0x0000U) /* Window function disabled */
#define _4000_AD_WINDOWFUNCTION_ENABLE         (0x4000U) /* Window function enabled */

/*
    A/D Comparison Function Window A Extended Input Select Register (ADCMPANSER)
*/
/* Temperature Sensor Output Compare Select(CMPTSA) */
#define _00_AD_TEMP_CMPA_UNUSED                (0x00U) /* Temperature sensor output is not a target for comparison */
#define _01_AD_TEMP_CMPA_USED                  (0x01U) /* Temperature sensor output is a target for comparison */
/* Internal Reference Voltage Compare Select (CMPOCA) */
#define _00_AD_IREF_CMPA_UNUSED                (0x00U) /* Internal reference voltage is not a target for comparison */
#define _02_AD_IREF_CMPA_USED                  (0x02U) /* Internal reference voltage is a target for comparison */

/*
    A/D Compare Channel Select Register 0 (ADCMPANSR0)
*/
#define _0001_AD_AN000_CMPA_USED               (0x0001U) /* AN000 is target for comparison */
#define _0002_AD_AN001_CMPA_USED               (0x0002U) /* AN001 is target for comparison */
#define _0004_AD_AN002_CMPA_USED               (0x0004U) /* AN002 is target for comparison */
#define _0008_AD_AN003_CMPA_USED               (0x0008U) /* AN003 is target for comparison */
#define _0010_AD_AN004_CMPA_USED               (0x0010U) /* AN004 is target for comparison */
#define _0020_AD_AN005_CMPA_USED               (0x0020U) /* AN005 is target for comparison */
#define _0040_AD_AN006_CMPA_USED               (0x0040U) /* AN006 is target for comparison */
#define _0080_AD_AN007_CMPA_USED               (0x0080U) /* AN007 is target for comparison */

/*
    A/D Compare Channel Select Register 1 (ADCMPANSR1)
*/
#define _0001_AD_AN016_CMPA_USED               (0x0001U) /* AN016 is target for comparison */
#define _0002_AD_AN017_CMPA_USED               (0x0002U) /* AN017 is target for comparison */
#define _0004_AD_AN018_CMPA_USED               (0x0004U) /* AN018 is target for comparison */
#define _0008_AD_AN019_CMPA_USED               (0x0008U) /* AN019 is target for comparison */
#define _0010_AD_AN020_CMPA_USED               (0x0010U) /* AN020 is target for comparison */
#define _0020_AD_AN021_CMPA_USED               (0x0020U) /* AN021 is target for comparison */
#define _0040_AD_AN022_CMPA_USED               (0x0040U) /* AN022 is target for comparison */
#define _0080_AD_AN023_CMPA_USED               (0x0080U) /* AN023 is target for comparison */
#define _0100_AD_AN024_CMPA_USED               (0x0100U) /* AN024 is target for comparison */
#define _0200_AD_AN025_CMPA_USED               (0x0200U) /* AN025 is target for comparison */
#define _0400_AD_AN026_CMPA_USED               (0x0400U) /* AN026 is target for comparison */
#define _0800_AD_AN027_CMPA_USED               (0x0800U) /* AN027 is target for comparison */
#define _1000_AD_AN028_CMPA_USED               (0x1000U) /* AN028 is target for comparison */
#define _2000_AD_AN029_CMPA_USED               (0x2000U) /* AN029 is target for comparison */
#define _4000_AD_AN030_CMPA_USED               (0x4000U) /* AN030 is target for comparison */
#define _8000_AD_AN031_CMPA_USED               (0x8000U) /* AN031 is target for comparison */

/*
    A/D Compare Level Register 0 (ADCMPLR0)
*/
#define _0000_AD_AN000_CMPA_LEVEL0             (0x0000U) /* AN000 converted value < ADCMPDR0 register value or AN000
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN001_CMPA_LEVEL0             (0x0000U) /* AN001 converted value < ADCMPDR0 register value or AN001
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN002_CMPA_LEVEL0             (0x0000U) /* AN002 converted value < ADCMPDR0 register value or AN002
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN003_CMPA_LEVEL0             (0x0000U) /* AN003 converted value < ADCMPDR0 register value or AN003
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN004_CMPA_LEVEL0             (0x0000U) /* AN004 converted value < ADCMPDR0 register value or AN004
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN005_CMPA_LEVEL0             (0x0000U) /* AN005 converted value < ADCMPDR0 register value or AN005
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN006_CMPA_LEVEL0             (0x0000U) /* AN006 converted value < ADCMPDR0 register value or AN006
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN007_CMPA_LEVEL0             (0x0000U) /* AN007 converted value < ADCMPDR0 register value or AN007
                                                            converted value > ADCMPDR1 register value */
#define _0001_AD_AN000_CMPA_LEVEL1             (0x0001U) /* ADCMPDR0 register value < AN000 converted value < ADCMPDR1
                                                            register value */
#define _0002_AD_AN001_CMPA_LEVEL1             (0x0002U) /* ADCMPDR0 register value < AN001 converted value < ADCMPDR1
                                                            register value */
#define _0004_AD_AN002_CMPA_LEVEL1             (0x0004U) /* ADCMPDR0 register value < AN002 converted value < ADCMPDR1
                                                            register value */
#define _0008_AD_AN003_CMPA_LEVEL1             (0x0008U) /* ADCMPDR0 register value < AN003 converted value < ADCMPDR1
                                                            register value */
#define _0010_AD_AN004_CMPA_LEVEL1             (0x0010U) /* ADCMPDR0 register value < AN004 converted value < ADCMPDR1
                                                            register value */
#define _0020_AD_AN005_CMPA_LEVEL1             (0x0020U) /* ADCMPDR0 register value < ANx05 converted value < ADCMPDR1
                                                            register value */
#define _0040_AD_AN006_CMPA_LEVEL1             (0x0040U) /* ADCMPDR0 register value < AN006 converted value < ADCMPDR1
                                                            register value */
#define _0080_AD_AN007_CMPA_LEVEL1             (0x0080U) /* ADCMPDR0 register value < AN007 converted value < ADCMPDR1
                                                            register value */

/*
    A/D Compare Level Register 1 (ADCMPLR1)
*/
#define _0000_AD_AN016_CMPA_LEVEL0             (0x0000U) /* AN016 converted value < ADCMPDR0 register value or AN016
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN017_CMPA_LEVEL0             (0x0000U) /* AN017 converted value < ADCMPDR0 register value or AN017
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN018_CMPA_LEVEL0             (0x0000U) /* AN018 converted value < ADCMPDR0 register value or AN018
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN019_CMPA_LEVEL0             (0x0000U) /* AN019 converted value < ADCMPDR0 register value or AN019
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN020_CMPA_LEVEL0             (0x0000U) /* AN020 converted value < ADCMPDR0 register value or AN020
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN021_CMPA_LEVEL0             (0x0000U) /* AN021 converted value < ADCMPDR0 register value or AN016
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN022_CMPA_LEVEL0             (0x0000U) /* AN022 converted value < ADCMPDR0 register value or AN017
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN023_CMPA_LEVEL0             (0x0000U) /* AN023 converted value < ADCMPDR0 register value or AN018
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN024_CMPA_LEVEL0             (0x0000U) /* AN024 converted value < ADCMPDR0 register value or AN019
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN025_CMPA_LEVEL0             (0x0000U) /* AN025 converted value < ADCMPDR0 register value or AN020
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN026_CMPA_LEVEL0             (0x0000U) /* AN026 converted value < ADCMPDR0 register value or AN020
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN027_CMPA_LEVEL0             (0x0000U) /* AN027 converted value < ADCMPDR0 register value or AN016
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN028_CMPA_LEVEL0             (0x0000U) /* AN028 converted value < ADCMPDR0 register value or AN017
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN029_CMPA_LEVEL0             (0x0000U) /* AN029 converted value < ADCMPDR0 register value or AN018
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN030_CMPA_LEVEL0             (0x0000U) /* AN030 converted value < ADCMPDR0 register value or AN019
                                                            converted value > ADCMPDR1 register value */
#define _0000_AD_AN031_CMPA_LEVEL0             (0x0000U) /* AN031 converted value < ADCMPDR0 register value or AN020
                                                            converted value > ADCMPDR1 register value */
#define _0001_AD_AN016_CMPA_LEVEL1             (0x0001U) /* ADCMPDR0 register value < AN016 converted value < ADCMPDR1
                                                            register value */
#define _0002_AD_AN017_CMPA_LEVEL1             (0x0002U) /* ADCMPDR0 register value < AN017 converted value < ADCMPDR1
                                                            register value */
#define _0004_AD_AN018_CMPA_LEVEL1             (0x0004U) /* ADCMPDR0 register value < AN018 converted value < ADCMPDR1
                                                            register value */
#define _0008_AD_AN019_CMPA_LEVEL1             (0x0008U) /* ADCMPDR0 register value < AN019 converted value < ADCMPDR1
                                                            register value */
#define _0010_AD_AN020_CMPA_LEVEL1             (0x0010U) /* ADCMPDR0 register value < AN020 converted value < ADCMPDR1
                                                            register value */
#define _0020_AD_AN021_CMPA_LEVEL1             (0x0020U) /* ADCMPDR0 register value < AN021 converted value < ADCMPDR1
                                                            register value */
#define _0040_AD_AN022_CMPA_LEVEL1             (0x0040U) /* ADCMPDR0 register value < AN022 converted value < ADCMPDR1
                                                            register value */
#define _0080_AD_AN023_CMPA_LEVEL1             (0x0080U) /* ADCMPDR0 register value < AN023 converted value < ADCMPDR1
                                                            register value */
#define _0100_AD_AN024_CMPA_LEVEL1             (0x0100U) /* ADCMPDR0 register value < AN024 converted value < ADCMPDR1
                                                            register value */
#define _0200_AD_AN025_CMPA_LEVEL1             (0x0200U) /* ADCMPDR0 register value < AN025 converted value < ADCMPDR1
                                                            register value */
#define _0400_AD_AN026_CMPA_LEVEL1             (0x0400U) /* ADCMPDR0 register value < AN026 converted value < ADCMPDR1
                                                            register value */
#define _0800_AD_AN027_CMPA_LEVEL1             (0x0800U) /* ADCMPDR0 register value < AN027 converted value < ADCMPDR1
                                                            register value */
#define _1000_AD_AN028_CMPA_LEVEL1             (0x1000U) /* ADCMPDR0 register value < AN028 converted value < ADCMPDR1
                                                            register value */
#define _2000_AD_AN029_CMPA_LEVEL1             (0x2000U) /* ADCMPDR0 register value < AN029 converted value < ADCMPDR1
                                                            register value */
#define _4000_AD_AN030_CMPA_LEVEL1             (0x4000U) /* ADCMPDR0 register value < AN030 converted value < ADCMPDR1
                                                            register value */
#define _8000_AD_AN031_CMPA_LEVEL1             (0x8000U) /* ADCMPDR0 register value < AN031 converted value < ADCMPDR1
                                                            register value */

/*
    A/D Comparison Function Window A Extended Input Comparison Condition Setting Register (ADCMPLER)
*/
/* Comparison Window A Temperature Sensor Output Comparison Condition Select (CMPLTS) */
#define _00_AD_TEMP_CMPA_LEVEL0                (0x00U) /* Temperature converted value < ADCMPDR0 register value or
                                                          Temperature converted value > ADCMPDR1 register value */
#define _01_AD_TEMP_CMPA_LEVEL1                (0x01U) /* ADCMPDR0 register value < Temperature converted value <
                                                          ADCMPDR1 register value */
/* Comparison Window A Internal Reference Voltage Comparison Condition Select (CMPLOC) */
#define _00_AD_IREF_CMPA_LEVEL0                (0x00U) /* Internal converted value < ADCMPDR0 register value or 
                                                          Internal converted value > ADCMPDR1 register value */
#define _02_AD_IREF_CMPA_LEVEL1                (0x02U) /* ADCMPDR0 register value < Internal converted value < ADCMPDR1
                                                          register value */

/* 
    A/D Data Storage Buffer Enable Register (ADBUFEN)
*/
/* Data Storage Buffer Enable (BUFEN) */
#define _00_AD_STORAGE_BUFF_UNUSED           (0x00U) /* The data storage buffer is not used. */
#define _01_AD_STORAGE_BUFF_USED             (0x01U) /* The data storage buffer is used. */

/*
    A/D Comparison Function Window B Channel Select Register (ADCMPBNSR)
*/
/* Comparison Window B Channel Select (CMPLB) */
#define _00_AD_CMPB_LEVEL0                     (0x00U) /* Comparison B converted value < ADCMPDR0 register value or
                                                          Comparison B converted value > ADCMPDR1 register value */
#define _80_AD_CMPB_LEVEL1                     (0x80U) /* ADWINLLB register value < Comparison B converted value <
                                                          ADWINULB register value */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_AD_PRIORITY_LEVEL0                 (0x00U) /* Level 0 (interrupt disabled) */
#define _01_AD_PRIORITY_LEVEL1                 (0x01U) /* Level 1 */
#define _02_AD_PRIORITY_LEVEL2                 (0x02U) /* Level 2 */
#define _03_AD_PRIORITY_LEVEL3                 (0x03U) /* Level 3 */
#define _04_AD_PRIORITY_LEVEL4                 (0x04U) /* Level 4 */
#define _05_AD_PRIORITY_LEVEL5                 (0x05U) /* Level 5 */
#define _06_AD_PRIORITY_LEVEL6                 (0x06U) /* Level 6 */
#define _07_AD_PRIORITY_LEVEL7                 (0x07U) /* Level 7 */
#define _08_AD_PRIORITY_LEVEL8                 (0x08U) /* Level 8 */
#define _09_AD_PRIORITY_LEVEL9                 (0x09U) /* Level 9 */
#define _0A_AD_PRIORITY_LEVEL10                (0x0AU) /* Level 10 */
#define _0B_AD_PRIORITY_LEVEL11                (0x0BU) /* Level 11 */
#define _0C_AD_PRIORITY_LEVEL12                (0x0CU) /* Level 12 */
#define _0D_AD_PRIORITY_LEVEL13                (0x0DU) /* Level 13 */
#define _0E_AD_PRIORITY_LEVEL14                (0x0EU) /* Level 14 */
#define _0F_AD_PRIORITY_LEVEL15                (0x0FU) /* Level 15 (highest) */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    ADCHANNEL0, ADCHANNEL1, ADCHANNEL2, ADCHANNEL3, ADCHANNEL4, ADCHANNEL5, ADCHANNEL6, ADCHANNEL7,
    ADCHANNEL16, ADCHANNEL17, ADCHANNEL18, ADCHANNEL19, ADCHANNEL20, ADCHANNEL21, ADCHANNEL22,
    ADCHANNEL23, ADCHANNEL24, ADCHANNEL25, ADCHANNEL26, ADCHANNEL27, ADCHANNEL28, ADCHANNEL29,
    ADCHANNEL30,  ADCHANNEL31, ADSELFDIAGNOSIS, ADTEMPSENSOR, ADINTERREFVOLT, ADDATADUPLICATION
} ad_channel_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
