
/*************** EEROM **************/


#define EEPROM_START_LOCATION   100
/* External EEPROM Memory Allocations */
#define ETestWrite           10
#define EEPROM_Check         50	//1
#define EE_Sec_MemoryCheck   52   
#define EResolution         100//EEPROM_START_LOCATION   //Weight calibration    100
#define EMaxCapacity        101//(EResolution + 1)       //101
/* Axle Calibration 1*/
//#define EDecimalPoint       105//(EMaxCapacity + 4)      //105
#define EA1_CalCapacity     106//(EDecimalPoint + 1)     //106
#define EA1_CalZero         110//(ECalCapacity + 4)      //110
#define EA1_CalZero1        114//(ECalZero + 4)          //114
#define EA1_CalSpan         118//(ECalZero1 + 4)         //118
/* Axle Calibration 2*/
#define EA2_Resolution      122//(ECalSpan + 4)   //Weight calibration   122
#define EA2_MaxCapacity     123//(E2Resolution + 1)      //123
#define EA2_DecimalPoint    127//(E2MaxCapacity + 4)     //127
#define EA2_CalCapacity     128//(E2DecimalPoint + 1)    //128
#define EA2_CalZero         132//(E2CalCapacity + 4)     //132
#define EA2_CalZero1        136//(E2CalZero + 4)         //136
#define EA2_CalSpan         140//(E2CalZero1 + 4)        //140

#define EBResolution        144//(E2CalSpan + 4)   //Weight calibration Backup   144
#define EBMaxCapacity       145//(EBResolution + 1)      //145
#define EBDecimalPoint      149//(EBMaxCapacity + 4)     //149
#define EBCalCapacity       150//(EBDecimalPoint + 1)    //150
#define EBCalZero           154//(EBCalCapacity + 4)     //154
#define EBCalZero1          158//(EBCalZero + 4)         //158
#define EBCalSpan           162//(EBCalZero1 + 4)        //162

#define EB2Resolution       166//(EBCalSpan + 4)   //Weight calibration Backup   166
#define EB2MaxCapacity      167//(EB2Resolution + 1)     //167
#define EB2DecimalPoint     171//(EB2MaxCapacity + 4)    //171
#define EB2CalCapacity      172//(EB2DecimalPoint + 1)   //172
#define EB2CalZero          176//(EB2CalCapacity + 4)    //176
#define EB2CalZero1         180//(EB2CalZero + 4)        //180
#define EB2CalSpan          184//(EB2CalZero1 + 4)       //184
        
#define ELC1Zero            188//(EB2CalSpan + 4)        //188
#define ELC2Zero            192//(ELC1CalCapacity + 4)   //204
#define ELC3Zero            196//(ELC2CalCapacity + 4)   //220
#define ELC4Zero            200//(ELC3CalCapacity + 4)   //236
#define ELC5Zero            204//(ELC3CalCapacity + 4)   //236

#define ELC1Zero1           208//(ELC1Zero + 4)          //192
#define ELC2Zero1           212//(ELC2Zero + 4)          //208
#define ELC3Zero1           216//(ELC3Zero + 4)          //224
#define ELC4Zero1           220//(ELC4Zero + 4)          //240
#define ELC5Zero1           224//(ELC4Zero + 4)          //240

#define ELC1Span            228//(ELC1Zero1 + 4)         //196
#define ELC2Span            232//(ELC2Zero1 + 4)         //212
#define ELC3Span            236//(ELC3Zero1 + 4)         //228
#define ELC4Span            240//(ELC4Zero1 + 4)         //244
#define ELC5Span            244//(ELC4Zero1 + 4)         //244

#define ELC1CalCapacity     248//(ELC1Span + 4)          //200
#define ELC2CalCapacity     252//(ELC2Span + 4)          //216
#define ELC3CalCapacity     256//(ELC3Span + 4)          //232
#define ELC4CalCapacity     260//(ELC4Span + 4)          //248
#define ELC5CalCapacity     264//(ELC4Span + 4)          //248

#define EBLC1Zero           268//(ELC4CalCapacity + 4)   //252
#define EBLC2Zero           272//(EBLC1CalCapacity + 4)  //268
#define EBLC3Zero           276//(EBLC2CalCapacity + 4)  //284
#define EBLC4Zero           280//(EBLC3CalCapacity + 4)  //300
#define EBLC5Zero           284//(EBLC3CalCapacity + 4)  //300

#define EBLC1Zero1          288//(EBLC1Zero + 4)         //256
#define EBLC2Zero1          292//(EBLC2Zero + 4)         //272
#define EBLC3Zero1          296//(EBLC3Zero + 4)         //288
#define EBLC4Zero1          300//(EBLC4Zero + 4)         //304
#define EBLC5Zero1          304//(EBLC4Zero + 4)         //304

#define EBLC1Span           308//(EBLC1Zero1 + 4)        //260
#define EBLC2Span           312//(EBLC2Zero1 + 4)        //276
#define EBLC3Span           316//(EBLC3Zero1 + 4)        //292
#define EBLC4Span           320//(EBLC4Zero1 + 4)        //308
#define EBLC5Span           324//(EBLC4Zero1 + 4)        //308

#define EBLC1CalCapacity    328//(EBLC1Span + 4)         //264
#define EBLC2CalCapacity    332//(EBLC2Span + 4)         //280
#define EBLC3CalCapacity    336// (EBLC3Span + 4)        //296
#define EBLC4CalCapacity    340// (EBLC4Span + 4)        //312
#define EBLC5CalCapacity    344// (EBLC4Span + 4)        //312

#define ECorr_Factor        348//(EBLC4CalCapacity + 4)  //316
#define EPeriodic_Timer     352//(EMulFactor_1 + 4)      //320
#define ECorFacor_3         356//(EMulFactor_2 + 4)      //324
#define ECorFacor_4         360//(EMulFactor_3 + 4)      //328

#define EBUnit              364//(ECorFacor_4 + 4)       //332
#define EComMode            365//(EBUnit + 1)            //333
#define EPowerOnTare        366//(EComMode + 1)          //334
#define EAngleEnDis         370//(EPowerOnTare + 4)      //338
#define ETareED             371//(EAutoZero + 1)         //339
#define EPassword           372//(ETareED + 1)           //340
#define EBuadRate           376//(EPassword + 4)         //344
#define ELC1ED              380//(EBuadRate + 4)         //348
#define ELC2ED              381//(ELC1ED + 1)            //349
#define ELC3ED              382//(ELC2ED + 1)            //350
#define ELC4ED              383//(ELC3ED + 1)            //351
#define ELC5ED              384//(ELC3ED + 1)            //351
#define E_MinWt             385//(ELC4ED + 1)            //352
#define E_LatchDelay        389//(E_MinWt + 4)           //356
#define E_CaptureTime       393//(E_LatchDelay + 4)      //360
#define E_Red_Glow          397//(E_CaptureTime + 4)     //364
#define E_Amber_Flash       401//(E_Red_Glow + 4)        //368
#define E_Amber_Glow        405//(E_Amber_Flash + 4)     //372
#define EInput1Logic        409//(E_Amber_Glow + 4)      //376
#define E_PLMS_Status		413//(EInputLogic + 1)       //377

#define EE_REFvolt1		    414//(E_PLMS_Status + 1)     //378
#define EE_REFvolt2			418//(EE_REFvolt1 + 4)       //382
#define EE_REF1Cnt			422//(EE_REFvolt2 + 4)       //386
#define EE_REF2Cnt			426//(EE_REF1Cnt + 2)        //388
#define EE_Ch1Ref1			428//(EE_REF2Cnt + 2)        //390
#define EE_Ch1Ref2			430//(EE_Ch1Ref1 + 2)        //392
#define EE_Ch2Ref1			432//(EE_Ch1Ref2 + 2)        //394
#define EE_Ch2Ref2			434//(EE_Ch2Ref1 + 2)        //396
#define EE_Ch3Ref1			436//(EE_Ch2Ref2 + 2)        //398
#define EE_Ch3Ref2			438//(EE_Ch3Ref1 + 2)        //400
#define EE_Ch4Ref1			440//(EE_Ch3Ref2 + 2)        //402
#define EE_Ch4Ref2			442//(EE_Ch4Ref1 + 2)        //404
#define EE_Ch5Ref1			444//(EE_Ch3Ref2 + 2)        //402
#define EE_Ch5Ref2			448//(EE_Ch4Ref1 + 2)        //404
#define EE_ChxRef_Volt		450//(EE_Ch4Ref2 + 2)        //406
#define EE_Offset_Volt		452//(EE_ChxRef_Volt + 2)    //408
    
#define EE_BREFvolt1		454//(EE_Offset_Volt + 2)    //410
#define EE_BREFvolt2		456//(EE_BREFvolt1 + 4)      //414
#define EE_BREF1Cnt			460//(EE_BREFvolt2 + 4)      //418
#define EE_BREF2Cnt			462//(EE_BREF1Cnt + 2)       //420
#define EE_BCh1Ref1			464//(EE_BREF2Cnt + 2)       //422
#define EE_BCh1Ref2			466//(EE_BCh1Ref1 + 2)       //424
#define EE_BCh2Ref1			468//(EE_BCh1Ref2 + 2)       //426
#define EE_BCh2Ref2			470//(EE_BCh2Ref1 + 2)       //428
#define EE_BCh3Ref1			472//(EE_BCh2Ref2 + 2)       //430
#define EE_BCh3Ref2			474//(EE_BCh3Ref1 + 2)       //432
#define EE_BCh4Ref1			476//(EE_BCh3Ref2 + 2)       //434
#define EE_BCh4Ref2			478//(EE_BCh4Ref1 + 2)       //436
#define EE_BCh5Ref1			480//(EE_BCh3Ref2 + 2)       //434
#define EE_BCh5Ref2			482//(EE_BCh4Ref1 + 2)       //436
#define EE_BChxRef_Volt		484//(EE_BCh4Ref2 + 2)       //438
#define EE_BOffset_Volt		486//(EE_BChxRef_Volt + 2)   //440
#define EE_CumulativeWeight 488//(EE_BOffset_Volt + 2)   //442
#define EE_TripCount        492//(EE_CumulativeWeight + 4)   //446
#define EE_Minus_multiply_selection       496//(EE_TripCount + 2)   //448

#define EDeviceID           498 //2 byte
#define EAutoZero           500 //2 byte
#define EUnit               502 //1 byte

#define EE_TareDelay        503 //4 byte
#define EE_StableCntr       507 //4 byte
#define EE_StableToll       511 //4 byte
#define EE_OverWeight       515 //4 byte
#define EE_BucketWt         519 //4 byte
#define EDecimalPoint       523 //2 byte
#define E_LatchedWt         525 //4 byte
#define	EES_Mobile_Operator	529	//1 byte

#define EAngleConfig           530	//2 byte
#define ENo_of_Samples         532	//2 byte
#define EAverage_Count         534	//2 byte
#define ECounterOverflowFlg    536	//2 byte
#define ENo_of_SamplesArray    538  // 800 bytes
#define EMinWt_Check_Counter  1338  //1 byte
#define EES_PON_first_Line 	  1339  //  16 byte

#define ELC6Zero            1355    //4 bytes
#define ELC6Zero1           1359    //4 bytes
#define ELC6Span            1363    //4 bytes
#define ELC6CalCapacity     1367    //4 bytes

#define EBLC6Zero           1371    //4 bytes 
#define EBLC6Zero1          1375    //4 bytes 
#define EBLC6Span           1379    //4 bytes
#define EBLC6CalCapacity    1383    //4 bytes
#define ELC6ED              1387    //1 bytes
#define EE_Ch6Ref1          1388    //2 bytes
#define EE_Ch6Ref2          1340    //2 bytes
#define EE_BCh6Ref1         1342    //2 bytes
#define EE_BCh6Ref2         1344    //2 bytes
#define EE_OperationLogic   1346     //1 byte
#define EInput2Logic        1347

#define Nextaddress                 1348   

//#define ECAN_BuadRate               1348
//#define ECAN_Broadcast_Time         1352
//#define Nextaddress                 1356   

#define E_LastTare                  2500    // 7 bytes
#define E_LastCalib                 2507    // 7 bytes


#define EES_POD_first_Line          2515  //  16 byte
#define ECAN_BuadRate               2532
#define ECAN_Broadcast_Time         2536
#define EERS232_Dump_Time           2540

#define ERT_Temp_Date               2544
#define ERT_Count                   2545  //4Byte

#define Nextaddress1                2549
#define EEPROM_STORING_ADD  5000
