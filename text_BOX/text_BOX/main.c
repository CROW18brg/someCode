
/**
 * @Software for Head Test Panasonic
 *
 * @par How to Use
 * Connect all plugs to the Head
 *
 *
 *
 * @author  Rui Carvalho
 * @date    2020/02/03
 * @updated 2020/04/25 bring HMI up to v0.32 to avoid too old issues
 * @convert by Rui Carvalho, no other changes made
 * @copyright
 * Copyright (C) 2020-2020 BOSCH Intelligent Systems Co., Ltd. \n
 * This program is free software; you can't redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

/*
#---Pinout Vacuum Valves---#
Vacuum Valve 1  -> PIn 22
Vacuum Valve 2  -> PIn 24
Vacuum Valve 3  -> PIn 26
Vacuum Valve 4  -> PIn 28
Vacuum Valve 5  -> PIn 30
Vacuum Valve 6  -> PIn 32
Vacuum Valve 7  -> PIn 38
Vacuum Valve 8  -> PIn 40
Vacuum Valve 9  -> PIn 42
Vacuum Valve 10 -> PIn 44
Vacuum Valve 11 -> PIn 42
Vacuum Valve 12 -> PIn 46
Vacuum Valve 13 -> PIn 48
Vacuum Valve 14 -> PIn 50
Vacuum Valve 16 -> PIn 52

#---Pinout Blow Sensors---#
Blow Valve 1  -> PIn 23
Blow Valve 2  -> PIn 25
Blow Valve 3  -> PIn 27
Blow Valve 4  -> PIn 29
Blow Valve 5  -> PIn 31
Blow Valve 6  -> PIn 33
Blow Valve 7  -> PIn 39
Blow Valve 8  -> PIn 41
Blow Valve 9  -> PIn 43
Blow Valve 10 -> PIn 45
Blow Valve 11 -> PIn 47
Blow Valve 12 -> PIn 49
Blow Valve 13 -> PIn 51
Blow Valve 14 -> PIn 53
Blow Valve 16 -> PIn 55

#---Pinout Vacuum Sensors---#
Vacuum Sensor 1  -> PIn A0
Vacuum Sensor 2  -> PIn A1
Vacuum Sensor 3  -> PIn A2
Vacuum Sensor 4  -> PIn A3
Vacuum Sensor 5  -> PIn A4
Vacuum Sensor 6  -> PIn A5
Vacuum Sensor 7  -> PIn A6
Vacuum Sensor 8  -> PIn A7
Vacuum Sensor 9  -> PIn A8
Vacuum Sensor 10 -> PIn A9
Vacuum Sensor 11 -> PIn A10
Vacuum Sensor 12 -> PIn A11
Vacuum Sensor 13 -> PIn A12
Vacuum Sensor 14 -> PIn A13
Vacuum Sensor 15 -> PIn A14
Vacuum Sensor 16 -> PIn A15
*/

/**
 * *************** Programme Includes ******************
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Nextion.h>
#include <string.h>

/**
 * *************** Struct Data for statistics ******************
 */
struct statistics{
        float limIE;
        float limSE;
        float MAX;
        float MIN;
        float mean;
        float stDes;
        float ampMean;
        float CPS;
        float CPI;
        float CP;
        float CPK;
};
struct statistics heads;

/**
 * *************** Struct for Bad Heads ******************
 */
struct damageHeads{
        int headNumber;
        float value;
        int sample;
};
struct damageHeads badHeads[30];       //  Declaration struct maximum number of bad samples

/**
 * *************** Instanciate the name of objects ******************
 */
 /**
     /**
     * Ative valves in manual mode
     */
NexDSButton head1_ON = NexDSButton(4,13,"btHead1");
NexDSButton head2_ON = NexDSButton(4,14,"btHead2");
NexDSButton head3_ON = NexDSButton(4,15,"btHead3");
NexDSButton head4_ON = NexDSButton(4,16,"btHead4");
NexDSButton head5_ON = NexDSButton(4,17,"btHead5");
NexDSButton head6_ON = NexDSButton(4,18,"btHead6");
NexDSButton head7_ON = NexDSButton(4,19,"btHead7");
NexDSButton head8_ON = NexDSButton(4,20,"btHead8");
NexDSButton head9_ON = NexDSButton(4,24,"btHead9");
NexDSButton head10_ON = NexDSButton(4,25,"btHead10");
NexDSButton head11_ON = NexDSButton(4,26,"btHead11");
NexDSButton head12_ON = NexDSButton(4,27,"btHead12");
NexDSButton head13_ON = NexDSButton(4,28,"btHead13");
NexDSButton head14_ON = NexDSButton(4,29,"btHead14");
NexDSButton head15_ON = NexDSButton(4,30,"btHead15");
NexDSButton head16_ON = NexDSButton(4,31,"btHead16");
    /**
     * Display mean values in main page
     */
NexNumber n0 = NexNumber(2, 15, "n0");
NexNumber n1 = NexNumber(2, 16, "n1");
NexNumber n2 = NexNumber(2, 17, "n2");
NexNumber n3 = NexNumber(2, 18, "n3");
NexNumber n4 = NexNumber(2, 19, "n4");
NexNumber n5 = NexNumber(2, 20, "n5");
NexNumber n6 = NexNumber(2, 21, "n6");
NexNumber n7 = NexNumber(2, 22, "n7");
NexNumber n8 = NexNumber(2, 23, "n8");
NexNumber n9 = NexNumber(2, 24, "n9");
NexNumber n10 = NexNumber(2, 25, "n10");
NexNumber n11 = NexNumber(2, 26, "n11");
NexNumber n12 = NexNumber(2, 27, "n12");
NexNumber n13 = NexNumber(2, 28, "n13");
NexNumber n14 = NexNumber(2, 29, "n14");
NexNumber n15 = NexNumber(2, 30, "n15");

NexNumber dispManualTest = NexNumber(4, 32, "dispManual");

  /**
     * Display heads pictures on main test
     */
NexPicture picHead1 = NexPicture(2, 77, "p2");
NexPicture picHead2 = NexPicture(2, 78, "p5");
NexPicture picHead3 = NexPicture(2, 79, "p6");
NexPicture picHead4 = NexPicture(2, 80, "p7");
NexPicture picHead5 = NexPicture(2, 81, "p8");
NexPicture picHead6 = NexPicture(2, 82, "p9");
NexPicture picHead7 = NexPicture(2, 83, "p10");
NexPicture picHead8 = NexPicture(2, 84, "p11");
NexPicture picHead9 = NexPicture(2, 85, "p12");
NexPicture picHead10 = NexPicture(2, 86, "p13");
NexPicture picHead11 = NexPicture(2, 87, "p14");
NexPicture picHead12 = NexPicture(2, 88, "p15");
NexPicture picHead13 = NexPicture(2, 89, "p16");
NexPicture picHead14 = NexPicture(2, 90, "p17");
NexPicture picHead15 = NexPicture(2, 91, "p18");
NexPicture picHead16 = NexPicture(2, 92, "p18");


  /**
     * Display final statistics values of vacuum test
     */
NexText rngVac = NexText(2, 35, "t17");
NexText MAX_Vac = NexText(2, 36, "t18");
NexText MIN_Vac = NexText(2, 37, "t19");
NexText meanVac = NexText(2, 38, "t20");
NexText stdVac = NexText(2, 39, "t21");
NexText std_infVac = NexText(2, 40, "t22");
NexText std_supVac = NexText(2, 41, "t23");
NexText CP_Vac = NexText(2, 57, "t37");
NexText CPK_Vac = NexText(2, 58, "t38");
  /**
     * Display final statistics values of blow test
     */
NexText rngBLW = NexText(2, 42, "t24");
NexText MAX_BLW = NexText(2, 43, "t25");
NexText MIN_BLW = NexText(2, 44, "t26");
NexText meanBLW = NexText(2, 45, "t27");
NexText stdBLW = NexText(2, 46, "t28");
NexText std_infBLW = NexText(2, 47, "t29");
NexText std_supBLW = NexText(2, 48, "t30");
NexText CP_BLW = NexText(2, 55, "t35");
NexText CPK_BLW = NexText(2, 56, "t36");
  /**
     * Type of tests
     */
NexText infoText = NexText(2, 25, "t9");
NexText QRcontent = NexText(1, 17, "QRcode");
  /**
     * Type of tests
     */
NexDSButton vacuum_ON = NexDSButton(2,12,"bt1");
NexDSButton blow_ON = NexDSButton(2,13,"bt2");
NexDSButton clean_ON = NexDSButton(2,69,"bt20");
NexDSButton measure_ON = NexDSButton(2,54,"bt19");

NexDSButton vacuum_ON_manual = NexDSButton(4,22,"vacuumManual");
NexDSButton blow_ON_manual = NexDSButton(4,23,"blowManual");
  /**
     * ProgressBar for state process capability
     */
NexProgressBar elapsedSamples  = NexProgressBar(2, 92, "remain");
     
  /**
     * Pages
     */
NexPage page0 = NexPage(0, 0, "main_page");
NexPage page1 = NexPage(1, 0, "check_QR");
NexPage page2 = NexPage(2, 0, "teste_page");
NexPage page3 = NexPage(3, 0, "option_page");
NexPage page4 = NexPage(4, 0, "manual_test");
  /**
     * Type of Head test
     */
NexButton NPM_16 = NexButton(0,3,"b0");
NexButton NPM_8 = NexButton(0,4,"b1");
NexButton NPM_3 = NexButton(0,5,"b2");
NexButton CM602_12 = NexButton(0,9,"b3");
NexButton CM602_8 = NexButton(0,10,"b4");
NexButton CM602_3 = NexButton(0,11,"b5");
NexButton CM402_8 = NexButton(0,14,"b6");
NexButton CM402_3 = NexButton(0,15,"b7");
/**
   ********************** List of touch events from display *************************
   */
NexTouch *nex_listen_list[] =
{
  &head1_ON, &head2_ON, &head3_ON, &head4_ON, &head5_ON, &head6_ON, &head7_ON,
  &head8_ON, &head9_ON, &head10_ON, &head11_ON, &head12_ON, &head13_ON, &head14_ON,
  &head15_ON, &head16_ON, &vacuum_ON, &blow_ON, &clean_ON, &measure_ON,&vacuum_ON_manual,&blow_ON_manual,
  &page0,&page1,&page2,&page3,&page4,&NPM_16, &NPM_8, &NPM_3, &CM602_12, &CM602_8, &CM602_3, &CM402_8 ,
  &CM402_3,
  NULL
};
  /**
     * Vectors for inicialize pins Head 3
     */
int vacuumPins_Head3[] = {22, 24, 26};
int blowPins_Head3[] = {23, 25, 27};
int vacuoSensorPins_Head3[] = {A0, A1, A2};
  /**
     * Vectors for inicialize pins Head 8
     */
int vacuumPins_Head8[] = {22, 24, 26, 28, 38, 40, 42, 44};
int blowPins_Head8[] = {23, 25, 27, 29, 39, 41, 43, 45};
int vacuoSensorPins_Head8[] = {A0, A1, A2, A3, A8, A9, A10, A11};
  /**
     * Vectors for inicialize pins Head 12
     */
int vacuumPins_Head12[] = {22, 24, 26, 28, 30, 32, 38, 40, 42, 44, 46, 48};
int blowPins_Head12[] =  {23, 25, 27, 29, 31, 33, 39, 41, 43, 45, 47, 49};
int vacuoSensorPins_Head12[] = {A0, A1, A2, A3, A4, A5, A8, A9, A10, A11, A12, A13};
  /**
     * Vectors for inicialize pins Head 16
     */
int vacuumPins_Head16 [] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
int blowPins_Head16 [] =  {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53};
int vacuoSensorPins_Head16 [] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};

  /**
     * Vector to store head states in manual mode
     */
bool manualHead_state[15] = {false};
  /**
     * Matrix for samples results
     */
float smaplesTestes_Head3 [3] [25] = {0};      // results of TypeHead 3 tests
float samplesTest_Head8 [8] [25] = {0};       // results of TypeHead 8 tests
float samplesTest_Head12 [12] [25] = {0};    // results of TypeHead 12 tests
float samplesTest_Head16 [16] [25] = {0};   // results of TypeHead 16 tests
  /**
     * *************** Global variables ****************
     */

bool valvesFix = false;                     // to fixe the valve states in gauges page
int sizeStructBadheads = 0;                // auxiliary variable for struct size
int actualPage = 0;                       // store the current page
bool vacuumAtive = false;                // store vacuum state
bool blowAtive = false;                 // store blow state
bool cleanAtive = false;               // store clean state
bool vacuumManualAtive = false;       // store button vacuum manual state
bool blowManualAtive = false;        // store button blow manual state
bool measureAtive = false;          // store measureAtive state
String selectedHead= "";           // store the selected type of head
char cov_str[3];                  // aux for convertion
char buffer[12] = {0};           // for text aquisition
const int mainValve = 6;        // main air valve
int counter = 0 ;
bool oneTime = true;         // for valve manual test
 /******************************* Test heads manuall mode *****************************/
uint32_t head1value = 0;
uint32_t head2value = 0;
uint32_t head3value = 0;
uint32_t head4value = 0;
uint32_t head5value = 0;
uint32_t head6value = 0;
uint32_t head7value = 0;
uint32_t head8value = 0;
uint32_t head9value = 0;
uint32_t head10value = 0;
uint32_t head11value = 0;
uint32_t head12value = 0;
uint32_t head13value = 0;
uint32_t head14value = 0;
uint32_t head15value = 0;
uint32_t head16value = 0;


//uint32_t autoONcheck  = 0;
uint32_t vacuumOncheck = 0;
uint32_t blowOncheck = 0;

  /**
     * *************** Callback funtions for touch events according type of test ****************
     */
void vacuum_ONPushCallback(void *ptr){
  uint32_t vacuumON = 0;
  vacuum_ON.getValue(&vacuumON);
  if(vacuumON == 1){
     vacuumAtive = true;
      elapsedSamples.setValue(uint32_t(0));            // reset value of progressBar
      valveIniVacuumBlow(vacuumPins_Head16, blowPins_Head16);
       digitalWrite(mainValve, HIGH);
  }else{
     vacuumAtive = false;
     resetValves(vacuumPins_Head16, blowPins_Head16);
     digitalWrite(mainValve, LOW);
     //bool manualHead_state[15] = {false};
  }
}
void blow_ONPushCallback(void *ptr){
  uint32_t blowON = 0;
  blow_ON.getValue(&blowON);
  if(blowON == 1){
     blowAtive = true;
     elapsedSamples.setValue(uint32_t(0));            // reset value of progressBar
     valveIniVacuumBlow(vacuumPins_Head16, blowPins_Head16);
     digitalWrite(mainValve, HIGH);
  }else{
     blowAtive = false;
     resetValves(vacuumPins_Head16, blowPins_Head16);
     digitalWrite(mainValve, LOW);
  }
}
void clean_ONPushCallback(void *ptr){
  uint32_t cleanON = 0;
  clean_ON.getValue(&cleanON);
  if(cleanON == 1){
     cleanAtive = true;
     elapsedSamples.setValue(uint32_t(0));            // reset value of progressBar
  }else{
     cleanAtive = false;
  }
}
void vacuumManual_ONPushCallback(void *ptr){
  uint32_t vacuumManual = 0;
  vacuum_ON_manual.getValue(&vacuumManual);
  if(vacuumManual == 1){
     digitalWrite(mainValve, HIGH);
     valveIniVacuumBlow(vacuumPins_Head16, blowPins_Head16);
     vacuumManualAtive = true;
  }else{
    digitalWrite(mainValve, LOW);
     //resetValves(vacuumPins_Head16, blowPins_Head16);
     vacuumManualAtive = false;
  }
}
void blowManual_ONPushCallback(void *ptr){
  uint32_t blowManual = 0;
  blow_ON_manual.getValue(&blowManual);
  if(blowManual == 1){
     digitalWrite(mainValve, HIGH);
     valveIniVacuumBlow(vacuumPins_Head16, blowPins_Head16);
     blowManualAtive = true;
  }else{
    digitalWrite(mainValve, LOW);
     //resetValves(vacuumPins_Head16, blowPins_Head16);
     blowManualAtive = false;
  }
}

void measuare_ONPushCallback(void *ptr){
  uint32_t measureON = 0;
  measure_ON.getValue(&measureON);
  if(measureON == 1){
     measureAtive = true;
     digitalWrite(mainValve, HIGH);
  }else{
          measureAtive = false;
         /* manualAtive = true;
          vacuumAtive = false;
          measureAtive =false;    */
  }
}


void page0PushCallback(void *ptr){
  actualPage = 0;
}
void page1PushCallback(void *ptr){
  actualPage = 1;
}
void page2PushCallback(void *ptr){
  actualPage = 2;
}
void page3PushCallback(void *ptr){
  actualPage = 3;
}
void page4PushCallback(void *ptr){
  actualPage = 4;
}

  /**
     * *************** Run on time ****************
     * ********************************************
     */
void setup() {
int i=0;
  Serial.begin(9600);                 // Inicialize serial comunication
  delay(500);
  Serial3.begin(115200);              // Inicialize serial3 comunication to read QR code
  delay(500);

  nexInit();                        // Inicialize display commmunication [TX2;RX2]
  /**
     * *************** Inicialize Heads pins as OUTPUTS ****************
     */
  for(i=0; i<16; i++)                 // Inicialize outputs for vacuum valves
      pinMode(vacuumPins_Head16[i],OUTPUT);
  for(i=0; i<16; i++)                 // Inicialize outputs for blow valves
      pinMode(blowPins_Head16[i],OUTPUT);

  pinMode(mainValve, OUTPUT);       // main air valve
  digitalWrite(mainValve, LOW);

//reset all struct members to '0'
memset(&heads, 0, sizeof heads);



  /**
     * *************** Attach the callback funtions ****************
     */

vacuum_ON.attachPush(vacuum_ONPushCallback);
blow_ON.attachPush(blow_ONPushCallback);
clean_ON.attachPush(clean_ONPushCallback);
measure_ON.attachPush(measuare_ONPushCallback);

vacuum_ON_manual.attachPush(vacuumManual_ONPushCallback);
blow_ON_manual.attachPush(blowManual_ONPushCallback);

page0.attachPush(page0PushCallback);
page1.attachPush(page1PushCallback);
page2.attachPush(page2PushCallback);
page3.attachPush(page3PushCallback);
page4.attachPush(page4PushCallback);
}

//######################## Function prototypes #########################
void testVacuoSensor(int vectorVacuumPins[],int vectorBlowPins[], int vectorSensorPins[],
                    float samplingResults [][15],int intervalON, int intervalOFF, int valves);
void testBlowSensor(int vectorVacuumPins[],int vectorBlowPins[], int vectorSensorPins[],
                    float samplingResults [][15], int intervalON, int intervalOFF,int valves);
void printSamples(float samplingResults [][15], int heads);
void printCSVfile(float samplingResults [][25], int heads);
float stDesCalculation(float samplingResults [][15], struct statistics head, int heads);
void printStatistics(struct statistics head);
float meanCalculation(float samplingResults [][15], int head);
float stDesCalculation(float samplingResults [][15], struct statistics head, int heads);
float CPS_Calculation( struct statistics head);
float CPI_Calculation( struct statistics head);
float CPK_Calculation(struct statistics head);
void printSamples(float samplingResults [][15], int heads);
float MAX_Calculation(float samplingResults [][15], int heads);
float MIN_Calculation(float samplingResults [][15], int heads);
float CP_Calculation(struct statistics head);
bool sendVacuumStatistics(struct statistics head);
void clearVacuumStatistics();
void clearBlowStatistics();
bool sendBlowStatistics(struct statistics head);
void errorHeadsRegister(float samplingResults [][25], struct statistics head,
                        struct damageHeads *, int heads);
void printHeadsRegisters(struct damageHeads *);

  /**
     * ******************************* Run in Loop *************************************
     * *********************************************************************************
     */
void loop() {

nexLoop(nex_listen_list); //check for any touch events
delay(50);

    switch(actualPage){
case 0:
      Serial.println(" I'm in page --->>   " + String(actualPage));
      vacuumManualAtive = false;
      vacuumAtive = false;
      measureAtive = false;
      blowAtive = false;
      break;
case 1:/* ----------------------------- Check barcode page ------------------------------ */
      Serial.println(" I'm in page --->>   " + String(actualPage));
//      char buf_code [100]= {};
//      if(Serial3.available()>0){
//        String code ="";
//        code = Serial3.readString();
//        Serial.println("QRcode number --- > " + String(code));
//        code.toCharArray(buf_code,sizeof(buf_code));        // convert String to char to send for display
//        QRcontent.setText(buf_code);
//      }
//      delay(50);
      break;
case 2:
      Serial.println(" I'm in page --->>   " + String(actualPage));
//       /* ***************** CPK Vacuum ******************* */
      if(vacuumAtive == true){
        Serial.println("Entrou AUTO && vacuum" );
                  if(measureAtive == true){
                    Serial.println("Entrou  em medicao" );
                                memset(&heads, 0, sizeof heads);    // clean all data struct
                                clearVacuumStatistics();
                                delay(2000);
                                /**
                                     * *************** Load specifications limits values in struct ****************
                                     */
                                heads.limIE = 2.00;
                                heads.limSE = 2.30;
                                testVacuoSensor(vacuumPins_Head12, blowPins_Head12, vacuoSensorPins_Head12,samplesTest_Head12, 15, 15, 12);
                   //printSamples(samplesTest_Head12, 12);
                   //printCSVfile(samplesTest_Head12, 12);
                                heads.mean = meanCalculation(samplesTest_Head12, 12);
                                heads.stDes = stDesCalculation(samplesTest_Head12, heads, 12);
                                heads.CPS = CPS_Calculation(heads);
                                heads.CPI = CPI_Calculation(heads);
                                heads.CPK = CPK_Calculation(heads);
                                heads.MAX = MAX_Calculation(samplesTest_Head12, 12);
                                heads.MIN = MIN_Calculation(samplesTest_Head12, 12);
                                heads.CP = CP_Calculation(heads);
                   //printStatistics(heads);
                                
                                
                                if(sendVacuumStatistics(heads)){
                                  Serial.println("-------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>> INSIDE BAD HEADS TEST");
                           //errorHeadsRegister(samplesTest_Head12, heads, badHeads, 12);
                           //printHeadsRegisters(badHeads);
                                }
                                measure_ON.setValue((uint32_t)0);   // Disable buttons
                                vacuum_ON.setValue((uint32_t)0);
                                blow_ON.setValue((uint32_t)0);
                                digitalWrite(mainValve, LOW);       // Disable main valve
                                vacuumAtive = false;
                                measureAtive =false;
                  }
        }/* ***************** CPK Blow ******************* */
       else if(blowAtive == true){
                      if(measureAtive == true){
                                Serial.println("Entrou AUTO && BLOW" );
                                memset(&heads, 0, sizeof heads);
                                clearBlowStatistics();
                                delay(2000);
                                /**
                                     * *************** Load specifications limits values in struct ****************
                                     */
                                heads.limIE = 0.25;
                                heads.limSE = 0.50;
                                testBlowSensor(vacuumPins_Head12, blowPins_Head12, vacuoSensorPins_Head12,samplesTest_Head12, 15, 15, 12);
                   //printSamples(samplesTest_Head12, 12);
                   //printCSVfile(samplesTest_Head12, 12);
                                heads.mean = meanCalculation(samplesTest_Head12, 12);
                                heads.stDes = stDesCalculation(samplesTest_Head12, heads, 12);
                                heads.CPS = CPS_Calculation(heads);
                                heads.CPI = CPI_Calculation(heads);
                                heads.CPK = CPK_Calculation(heads);
                                heads.MAX = MAX_Calculation(samplesTest_Head12, 12);
                                heads.MIN = MIN_Calculation(samplesTest_Head12, 12);
                                heads.CP = CP_Calculation(heads);
                   //printStatistics(heads);
                                
                                sendBlowStatistics(heads);
                                measure_ON.setValue((uint32_t)0);
                                //vacuum_ON.setValue((uint32_t)0);
                                blow_ON.setValue((uint32_t)0);
                                digitalWrite(mainValve, LOW);
                                blowAtive = false;
                                measureAtive = false;
                     }
        }
      break;
case 3: /* ----------------------------- Head Manual test ------------------------------ */
     Serial.println(" I'm in page --->>   " + String(actualPage));

      break;
case 4: /* ----------------------------- Head Manual test ------------------------------ */
     Serial.println(" I'm in page --->>   " + String(actualPage));
       

while(vacuumManualAtive == true || blowManualAtive == true){
  Serial.println("--------VACUMM MANUAL TEST BUTTON---->> "  + String(vacuumManualAtive));
  Serial.println("--------BLOW MANUAL TEST BUTTON---->> "  + String(blowManualAtive));
                          /* ******************* Check head 1 ******************** */
                          head1_ON.getValue(&head1value);
                            if(head1value == 1){
                                 if(vacuumManualAtive) {
                                      digitalWrite(vacuumPins_Head12[0],LOW);
                                      int sensorValue_1 = analogRead(vacuoSensorPins_Head12[0]);
                                      dispManualTest.setValue(uint32_t(sensorValue_1));
                                 }else if(blowManualAtive) {
                                      digitalWrite(blowPins_Head12[0],HIGH);
                                 }
                            }else{
                                 if(vacuumManualAtive == false)
                                      digitalWrite(vacuumPins_Head12[0],HIGH);
                                 if(blowManualAtive == false)
                                      digitalWrite(blowPins_Head12[0],LOW);
                            }
      
            /* *******************************  Here go out while ****************************  */
         if(vacuumManualAtive == true){
             vacuum_ON_manual.getValue(&vacuumOncheck);        // Check if user click on vacuum button
              if(vacuumOncheck == 0){
                Serial.println("-------------------------------------- >>>> vacuum break ............");
                vacuumManualAtive = false;                    // value of vacuum state
                delay(50);
                digitalWrite(mainValve, LOW);
                //vacuum_ON.setValue(uint32_t(0));
                break;
              }
         }
         if(blowManualAtive == true){
              Serial.println("--------head12value---->> "  + String(head12value));  //*** This serial is mandatory ***
             blow_ON_manual.getValue(&blowOncheck);        // Check if user click on vacuum button
              if(blowOncheck == 0){
                Serial.println("-------------------------------------- >>>> vacuum break ............");
                blowManualAtive = false;                    // value of vacuum state
                delay(50);
                digitalWrite(mainValve, LOW);
                //vacuum_ON.setValue(uint32_t(0));
                break;
              }
         }
}

      break;

      default:
      Serial.println(" Default case  --->>   " + String(actualPage));
   }
     
}
/* ##############################--------End Loop--------############################### */


/* ################################## Measure funtions ################################ */
 
    /**
     * @brief Measure vacuum circuits.
     *
     * @param vectorVacuumPins - vector that contain the output vacuum valves to ative the correctly head type.
     * @param vectorBlowPins - vector that contain the output blow valves to ative the correctly head type.
     * @param vectorSensorPins - vector that contain the analog Imputs pin to ative the correctly head type.
     * @param samplingResults - matrix where will stored sample results.
     * @param intervalON - variable that define the time valve in ON.
     * @param intervalOFF - variable that define the time valve in OFF.
     * @param valves - variable used to inicialize the output valves, according type of head.
     * @return - none.
     */
void testVacuoSensor(int vectorVacuumPins[],int vectorBlowPins[], int vectorSensorPins[],
                             float samplingResults [][25],int intervalON, int intervalOFF, int valves){
          uint32_t measureButton = 0;
          int samples = 25;
          int numSamples = samples * valves;              // total of samples(head 12 = 300)
          int counter = 0;                               // for progress bar
          int sensorValue = 0;                          // Analog Value
          String dadosSerialOut="";
          // Vacuum valves inicialization
          for(int i = 0; i < valves; i++)
            digitalWrite(vectorVacuumPins[i],HIGH);
          // Inicialização das válvulas de blow
          for(int i = 0; i < valves; i++)
            digitalWrite(vectorBlowPins[i],LOW);
          
          delay(800);   // wait for pressurize circuit
          for (int j = 0; j < samples; j++){
            // Iterate all Output valves
            for(int i = 0; i < valves ; i++){
              digitalWrite(vectorVacuumPins[i],LOW);
              counter ++;
              delay(50);
              sensorValue = analogRead(vectorSensorPins[i]);
              float voltage = sensorValue*(5.0/1023.0);
              samplingResults[i][j] = voltage;
              if(samplingResults[i][j] < 1.5){      // check the air connections
              measure_ON.setValue(uint32_t(0));
              infoText.setText("Check the vacuum and blow connections!...");
              }
              delay(intervalON);
              digitalWrite(vectorVacuumPins[i],HIGH);
              delay(intervalOFF);
              int elapSamp = map(counter,0,numSamples,0,100);          // mapping value for progressBar
              elapsedSamples.setValue(uint32_t(elapSamp));            // send value to progressBar
            }
          measure_ON.getValue(&measureButton);        // Check if user stops measure
              if(measureButton == 0){
                break;
              }
         }
      
}
    /**
     * @brief Measure blow circuits.
     * @param vectorVacuumPins - vector that contain the output vacuum valves to ative the correctly head type.
     * @param vectorBlowPins - vector that contain the output blow valves to ative the correctly head type.
     * @param vectorSensorPins - vector that contain the analog Imputs pin to ative the correctly head type.
     * @param samplingResults - matrix where will stored sample results.
     * @param intervalON - variable that define the time valve in ON.
     * @param intervalOFF - variable that define the time valve in OFF.
     * @param valves - variable used to inicialize the output valves, according type of head.
     * @return - none.
     */
void testBlowSensor(int vectorVacuumPins[],int vectorBlowPins[], int vectorSensorPins[],
                    float samplingResults [][25], int intervalON, int intervalOFF,int valves){
          uint32_t measureButton = 0;
          String dadosSerialOut="";
          int samples = 25;
          int numSamples = samples * valves;          // total of samples(head 12 = 300)
          int counter = 0;                           // for progress bar
          int sensorValue = 0;
          for(int i = 0; i < valves; i ++)
            digitalWrite(vectorVacuumPins[i],HIGH);
          for(int i = 0; i < valves; i ++)
            digitalWrite(vectorBlowPins[i],LOW);
          delay(800); // wait for circuit pressurize
          for (int j = 0; j < samples; j ++){
            for(int i = 0; i < valves; i ++){
              digitalWrite(vectorBlowPins[i],HIGH);
              counter ++;
              delay(50);
              sensorValue = analogRead(vectorSensorPins[i]);
              float voltage = sensorValue*(5.0/1023.0);
              samplingResults[i][j] = voltage;
              if(samplingResults[i][j] > 0.80){                     // check the air connections
              measure_ON.setValue(uint32_t(0));
              infoText.setText("Check the vacuum and blow connections!...");
              }
              delay(intervalON);
              digitalWrite(vectorBlowPins[i],LOW);
              delay(intervalOFF);
              int elapSamp = map(counter,0,numSamples,0,100);         // mapping value for progressBar
              elapsedSamples.setValue(uint32_t(elapSamp));            // send value to progressBar
            }
             measure_ON.getValue(&measureButton);
             if(measureButton == 0){
                 break;
             }
      }
}
    /**
     * @brief Clean all heads.
     * @param vectorVacuumPins - vector that contain the output vacuum valves to ative the correctly head type.
     * @param vectorBlowPins - vector that contain the output blow valves to ative the correctly head type.
     * @param intervalON - variable that define the time valve in ON.
     * @param intervalOFF - variable that define the time valve in OFF.
     * @param valves - variable used to inicialize the output valves, according type of head.
     * @return - none.
     */
void cleanHeads(int vectorVacuumPins[],int vectorBlowPins[],int intervalON, int intervalOFF,int valves){
  int samples = 15;
  for(int i = 0; i < valves; i ++)
    digitalWrite(vectorVacuumPins[i],HIGH);
  for(int i = 0; i < valves; i ++)
    digitalWrite(vectorBlowPins[i],LOW);

  for (int j = 0; j < samples; j ++){
    for(int i = 0; i < valves; i ++){
      digitalWrite(vectorBlowPins[i],HIGH);
      delay(50);
      delay(intervalON);
      digitalWrite(vectorBlowPins[i],LOW);
      delay(intervalOFF);
    }
  }
}
/* ################################## End Measure funtions ################################ */


/* ################################## Statistcs funtions ################################ */
    /**
     * @brief Print all statistics values while debuging.
     * @param statistics head - struct where stored statistics values.
     * @return - none.
     */
void printStatistics(struct statistics head){
  Serial.print("\n\n\n");
  Serial.println("---------- Capability Test ------------");
  Serial.println("Value of limIE -> " + String(head.limIE));
  Serial.println("Value of limSE -> " + String(head.limSE));
  Serial.println("Value of MAX -> " + String(head.MAX));
  Serial.println("Value of MIN -> " + String(head.MIN));
  Serial.println("Value of mean -> " + String(head.mean));
  Serial.println("Value of stDes -> " + String(head.stDes));
  Serial.println("Value of ampMean -> " + String(head.ampMean));
  Serial.println("Value of CPS -> " + String(head.CPS));
  Serial.println("Value of CPI -> " + String(head.CPI));
  Serial.println("Value of CP -> " + String(head.CP));
  Serial.println("Value of CPK -> " + String(head.CPK));
}
    /**
     * @brief Get Mean value.
     * @param statistics head - struct where stored statistics values.
     * @param  heads - type of head to test(example: 3, 8, 12, 16).
     * @return The real Standard deviation value of float returned.
     */
 float meanCalculation(float samplingResults [][25], int head){
  float sum = 0;
  float mean= 0;
  int row,col;
  int nSamples = 0;
  for(row = 0; row < head; row ++){
    for(col = 0; col < 25; col ++){
      sum = sum + samplingResults[row][col];
      nSamples ++;
    }
  }
mean = sum/nSamples;
return mean;
}
    /**
     * @brief Get Standard deviation value.
     * @param samplingResults - Matrix where stored sample values.
     * @param statistics head - struct where stored statistics values.
     * @return The real Standard deviation value of float returned.
     */
  float stDesCalculation(float samplingResults [][25], struct statistics head, int heads){
  float aux_stDes [heads][25];
  int row,col,nSamples;
  float sum = 0; float stdes = 0;
    for(row = 0; row < heads; row ++){
      for(col = 0; col < 25; col ++){
        aux_stDes [row][col]= (samplingResults[row][col] - head.mean) * (samplingResults[row][col] - head.mean);
        sum = sum +  aux_stDes [row][col];
        nSamples ++;
    }
  }
  //stdes = sqrt(sum/60);
  stdes = sqrt(sum/((heads * 25)-1));    // standard desviation for samples -> (N-1)
  return stdes;
}
    /**
     * @brief Get CPS value.
     * @param statistics head - struct where stored statistics values.
     * @return The real CPS value of float returned.
     */
float CPS_Calculation( struct statistics head){
  int three_sigma = 3;
  float CPS;
  CPS = (head.limSE - head.mean) / (three_sigma * head.stDes);
  return CPS;
}
    /**
     * @brief Get CPI value.
     * @param statistics head - struct where stored statistics values.
     * @return The real CPI value of float returned.
     */
float CPI_Calculation( struct statistics head){
  int three_sigma = 3;
  float CPI;
  CPI = ( head.mean - head.limIE ) / (three_sigma * head.stDes);
  return CPI;
}
    /**
     * @brief Get CPK value.
     * @param statistics head - struct where stored statistics values.
     * (Equation -> CPK = min{CPI;CPS}).
     * @return The real CPK value of float returned.
     */
float CPK_Calculation(struct statistics head){
  float results_CPK = 0;
  if(head.CPI > head.CPS){
    results_CPK = head.CPS;
    return  results_CPK;
  }else{
     results_CPK = head.CPI;
     return results_CPK;
  }
}
    /**
     * @brief Print all samples results while debuging.
     * @param samplingResults - matrix where stored samples values.
     * @param  heads - type of head to test(example: 3, 8, 12, 16).
     * @return - none.
     */
void printSamples(float samplingResults [][25], int heads){
  String dadosSerialOut = "";
   Serial.print("\n\n\n");
    int row,col,nSamples = 0;
    for(row = 0; row < heads; row ++){
      for(col = 0; col < 25; col ++){
      dadosSerialOut=" [Number of Sample ------->  ";
      dadosSerialOut.concat(String(col + 1));
      dadosSerialOut.concat("\t Value ------->  ");
      dadosSerialOut.concat(String(samplingResults[row][col]));
      //dadosSerialOut.concat("]");
      Serial.println(dadosSerialOut);
      if(col == 24){
        Serial.println("||||############ Finish samples Head Number ------------------------->> " + String(row + 1) + " ############||||");
      }
         nSamples ++;
    }
  }
      Serial.print("Total of samples -----> "  + String(nSamples)) ;
}
   
    /**
     * @brief Get MAXIMUM value.
     * @param statistics head - struct where stored statistics values.
     * @param  heads - type of head to test(example: 3, 8, 12, 16).
     * @return The real maximum value of float returned.
     */
float MAX_Calculation(float samplingResults [][25], int heads){
      int row,col;
      float MAX = 0;
    for(row = 0; row < heads; row ++){
      for(col = 0; col < 25; col ++){
        if(samplingResults[row][col] > MAX){
          MAX = samplingResults[row][col];
      }
    }
  }
  return MAX;
}
    /**
     * @brief Get MINIMUM value.
     * @param statistics head - struct where stored statistics values.
     * @param  heads - type of head to test(example: 3, 8, 12, 16).
     * @return The real minimum value of float returned.
     */
float MIN_Calculation(float samplingResults [][25], int heads){
        int row,col;
        float MIN = 1000;
    for(row = 0; row < heads; row ++){
      for(col = 0; col < 25; col ++){
        if(samplingResults[row][col] < MIN){
          MIN = samplingResults[row][col];
      }
    }
  }
  return MIN;
}
    /**
     * Get CP value
     *
     * @param statistics head - struct where stored statistics values.
     * @return The real value CP calculation of float returned.
     */
float CP_Calculation(struct statistics head){
  float CP = 0;
   CP = (head.CPI + head.CPS) / 2;
   return CP;
}

 /* ################################## End Statistcs funtions ################################ */
 
 /* ################################## Other funtions ################################ */
   /**
     * @brief Save bad heads separated in another struct
     * @param samplingResults - matrix where stored sample results.
     * @param statistics head - struct where stored statistics values.
     * @param damageHeads *badHeads - struct where will stored bad heads values.
     * @param  heads - type of head to test(example: 3, 8, 12, 16).
     * @param  *sizeStructBadheads - variable pass by reference to dinamic size of struct.
     * @return - none.
     */
void errorHeadsRegister(float samplingResults [][25], struct statistics head,
                         struct damageHeads *ptr, int heads){
    memset(&badHeads, 0, sizeof badHeads);         // Clear all struct values
     Serial.println("-------------------------- INSIDE HEAD REGISTER----------------------------------------");
    counter = 0;                                           // for iterate struct later
    int row,col;
    for(col = 0; col < 25; col ++){
      for(row = 0; row < heads; row ++){
        if(samplingResults[row][col] > head.mean * 1.05 ){     // choose others conditions for testing
          Serial.println("-------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>> I'M A BAD HEAD!...");
          Serial.println(samplingResults[row][col]);
                ptr->headNumber = row + 1 ;                // pointer to struct member nº of Head
                ptr->value = samplingResults[row][col];    // pointer to struct member nº of value vacuum/blow
                ptr->sample = col + 1;                     // pointer to struct member nº of sample
                ptr ++;                                    // index structure
                counter ++;                                // for iterate struct later
              delay(400);
              Serial.println("Number of counter  ---->>> " + String(counter));
         }
      }
      Serial.print("\n");
    }
 }
    /**
     * @brief Print struct Bad Heads
     * @param damageHeads *ptr - struct where stored damage heads reference by pointer.
     * @return - none.
     */
void printHeadsRegisters(struct damageHeads *ptr){   // pointer for struture & structure size
  Serial.println("-------------------------- INSIDE PRINT HEAD REGISTER");
  int i = 0;
  for(i = 0; i < counter; i++){
      Serial.println("\n\n\nDetails of bad samples \n" + String(i + 1));              // number of struture register
      // Show the results by ptr variable (pointer)
      Serial.print("\t Number of Bad Head -> " + String(ptr->headNumber));    // show head number by pointer
      Serial.print("\t Abnormal value  -> " + String(ptr->value));           // show the bad value by pointer
      Serial.print("\t Sample Number  -> " + String(ptr->sample));          // show number of bad sample by pointer
      ptr ++;                                                                // Index structure for continuous shows
      Serial.print("\n");
  }
}

    /**
     * @brief Clean the vacuum statistiscs textbox.
     * @param - none.
     * @return - none.
     */
void clearVacuumStatistics(void){
     char clean[] = {'-'};

  rngVac.setText(clean);
  MAX_Vac.setText(clean);
  MIN_Vac.setText(clean);
  meanVac.setText(clean);
  stdVac.setText(clean);
  std_infVac.setText(clean);
  std_supVac.setText(clean);
  CP_Vac.setText(clean);
  CPK_Vac.setText(clean);
  CP_Vac.Set_background_color_bco(65535);
}
    /**
     * @brief Clean the vacuum statistiscs textbox.
     * @param - none.
     * @return - none.
     */
void clearBlowStatistics(void){
     char clean[] = {'-'};

  rngBLW.setText(clean);
  MAX_BLW.setText(clean);
  MIN_BLW.setText(clean);
  meanBLW.setText(clean);
  stdBLW.setText(clean);
  std_infBLW.setText(clean);
  std_supBLW.setText(clean);
  CP_BLW.setText(clean);
  CPK_BLW.setText(clean);
  CP_BLW.Set_background_color_bco(65535);
}
    /**
     * @brief Send the vacuum statistiscs to HMI.
     * @param struck with statistics results.
     * @return false if CP value > 1.33 else return true.
     */
bool sendVacuumStatistics(struct statistics head){
  uint32_t measureButton;
  measure_ON.getValue(&measureButton);
 if(measureButton != 0){
             float range = head.limSE - head.limIE;
             char conv_MAX [3] = {};
             char conv_MIN [3] = {};
             char conv_mean [3] = {};
             char conv_stDes [3] = {};
             char conv_rng [3] = {};
             char conv_CPS [3] = {};
             char conv_CPI [3] = {};
             char conv_CP [3] = {};
             char conv_CPK [3] = {};
             
             char buf_MAX [5] = {};
             char buf_MIN [5] = {};
             char buf_CP [5] = {};
             char buf_CPK [5] = {};
             char buf_CPI [5] = {};
             char buf_CPS [5] = {};
             char buf_std [5] = {};
             char buf_mean [5] = {};
             char buf_rng [5] = {};
                  
            /*************** Convert float to string -->> string to char *****************/
            String aux_Rng = dtostrf(range,3, 2, conv_rng);
            aux_Rng.toCharArray(buf_rng,sizeof(buf_rng));
             
            String aux_MAX = dtostrf(head.MAX,3, 2, conv_MAX);
            aux_MAX.toCharArray(buf_MAX,sizeof(buf_MAX));

            String aux_MIN = dtostrf(head.MIN,3, 2, conv_MIN);
            aux_MIN.toCharArray(buf_MIN,sizeof(buf_MIN));

            String aux_CP = dtostrf(head.CP,3, 2, conv_CP);
            aux_CP.toCharArray(buf_CP,sizeof(buf_CP));

            String aux_CPK = dtostrf(head.CPK,3, 2, conv_CPK);
            aux_CPK.toCharArray(buf_CPK,sizeof(buf_CPK));

            String aux_CPI = dtostrf(head.CPI,3, 2, conv_CPI);
            aux_CPI.toCharArray(buf_CPI,sizeof(buf_CPI));

            String aux_CPS = dtostrf(head.CPS,3, 2, conv_CPS);
            aux_CPS.toCharArray(buf_CPS,sizeof(buf_CPS));

            String aux_std = dtostrf(head.stDes,3, 2, conv_stDes);
            aux_std.toCharArray(buf_std,sizeof(buf_std));

            String aux_mean = dtostrf(head.mean,3, 2, conv_mean);
            aux_mean.toCharArray(buf_mean,sizeof(buf_mean));
            /*************** Send to HMI *****************/
            rngVac.setText(buf_rng);
            MAX_Vac.setText(buf_MAX);
            MIN_Vac.setText(buf_MIN);
            meanVac.setText(buf_mean);
            stdVac.setText(buf_std);
            std_infVac.setText(buf_CPI);
            std_supVac.setText(buf_CPS);
            CP_Vac.setText(buf_CP);
            CPK_Vac.setText(buf_CPK);
            infoText.setText("Vacuum capability test is finish!...");
            if(head.CP > 1.33){
                CP_Vac.Set_background_color_bco(2016);
                return false;
            }else{
                CP_Vac.Set_background_color_bco(65504);
                return true;
            }
    }
}
    /**
     * @brief Send the Blow statistiscs to HMI.
     * @param struck with statistics results.
       * @return false if CP value > 1.33 else return true.
     */
bool sendBlowStatistics(struct statistics head){
 uint32_t measureButton;
 measure_ON.getValue(&measureButton);
        if(measureButton != 0){
             float range = head.limSE - head.limIE;
             char conv_limIE [3] = {};
             char conv_limSE [3] = {};
             char conv_MAX [3] = {};
             char conv_MIN [3] = {};
             char conv_mean [3] = {};
             char conv_stDes [3] = {};
             char conv_rng [3] = {};
             char conv_CPS [3] = {};
             char conv_CPI [3] = {};
             char conv_CP [3] = {};
             char conv_CPK [3] = {};

             char buf_MAX [5] = {};
             char buf_MIN [5] = {};
             char buf_CP [5] = {};
             char buf_CPK [5] = {};
             char buf_CPI [5] = {};
             char buf_CPS [5] = {};
             char buf_std [5] = {};
             char buf_mean [5] = {};
             char buf_rng [5] = {};
                  
             /*************** Convert float to string -->> string to char *****************/
            String aux_Rng = dtostrf(range,3, 2, conv_rng);            //float to string
            aux_Rng.toCharArray(buf_rng,sizeof(buf_rng));            //string to char

            String aux_MAX = dtostrf(head.MAX,3, 2, conv_MAX);
            aux_MAX.toCharArray(buf_MAX,sizeof(buf_MAX));

            String aux_MIN = dtostrf(head.MIN,3, 2, conv_MIN);
            aux_MIN.toCharArray(buf_MIN,sizeof(buf_MIN));

            String aux_CP = dtostrf(head.CP,3, 2, conv_CP);
            aux_CP.toCharArray(buf_CP,sizeof(buf_CP));

            String aux_CPK = dtostrf(head.CPK,3, 2, conv_CPK);
            aux_CPK.toCharArray(buf_CPK,sizeof(buf_CPK));

            String aux_CPI = dtostrf(head.CPI,3, 2, conv_CPI);
            aux_CPI.toCharArray(buf_CPI,sizeof(buf_CPI));

            String aux_CPS = dtostrf(head.CPS,3, 2, conv_CPS);
            aux_CPS.toCharArray(buf_CPS,sizeof(buf_CPS));

            String aux_std = dtostrf(head.stDes,3, 2, conv_stDes);
            aux_std.toCharArray(buf_std,sizeof(buf_std));

            String aux_mean = dtostrf(head.mean,3, 2, conv_mean);
            aux_mean.toCharArray(buf_mean,sizeof(buf_mean));
 
            /*************** Send to HMI *****************/
            rngBLW.setText(buf_rng);
            MAX_BLW.setText(buf_MAX);
            MIN_BLW.setText(buf_MIN);
            meanBLW.setText(buf_mean);
            stdBLW.setText(buf_std);
            std_infBLW.setText(buf_CPI);
            std_supBLW.setText(buf_CPS);
            CP_BLW.setText(buf_CP);
            CPK_BLW.setText(buf_CPK);
            infoText.setText("Blow capability test is finish!...");
            if(head.CP > 1.33){
                CP_BLW.Set_background_color_bco(2016);
                return false;
            }else{
                CP_BLW.Set_background_color_bco(65504);
                return true;
            }
      }
}

    /**
     * @brief Test heads in manual mode
     *
     * @param manualHead_state - vector to store the value of output valves, sended by callback funtions from buttons events.
     * @return - none.
     */
void switchHeads_manual(bool manualHead_state[],int vectorVacuumPins[],int vectorBlowPins[],int vectorSensorPins[]){
      
        //delay(1000);
  if( actualPage == 1){
          Serial.println(" //////////////////// I'm Manual && atual page 1 ////////////////////// ");
          Serial.println("valor do btn vacuum -------------> " + String(vacuumAtive));
          Serial.println("valor do oneTime -------------> " + String(oneTime));
   }
 }
     /**
     * @brief Print all samples results for statistics (CSV file).
     *
     * @param samplingResults - matrix where stored samples values.
     * @param  heads - type of head to test(example: 3, 8, 12, 16).
     * @return - none.
     */
void printCSVfile(float samplingResults [][25], int heads){

   Serial.print("\n\n\n");
    int row,col,nSamples = 0;
    for(col = 0; col < 25; col ++){
      for(row = 0; row < heads; row ++){
        Serial.print("\t" + String(samplingResults[row][col]));
      }
        Serial.print("\n");
        nSamples ++;
    }
     Serial.print("Total of samples -----> "  + String(nSamples));
  }
void valveIniVacuumBlow(int vectorVacuumPins[],int vectorBlowPins[]){
            // Vacuum valves inicialization
          for(int i = 0; i < 16; i++)
            digitalWrite(vectorVacuumPins[i],HIGH);
          // Inicialização das válvulas de blow
          for(int i = 0; i < 16; i++)
            digitalWrite(vectorBlowPins[i],LOW);
}
void resetValves(int vectorVacuumPins[],int vectorBlowPins[]){
            // Vacuum valves inicialization
          for(int i = 0; i < 16; i++)
            digitalWrite(vectorVacuumPins[i],LOW);
          // Inicialização das válvulas de blow
          for(int i = 0; i < 16; i++)
            digitalWrite(vectorBlowPins[i],LOW);
}
     
