
#ifndef _DEFAULTS
#define _DEFAULTS

#define UInt unsigned int 
#define UChar unsigned char

#define EEDataAvail     100
#define EEWriteCounter 	102
#define EEReadCounter	104
#define EETrips  		106
#define EEOLTrips		108
#define EELoadedQty		110	 // 4 bytes - long variable

#define EEDataStart 115

#define ArraySize		150
#define RecordsInABank 	934
#define RecordSize 		70


struct Record {
	UInt DateNMonth;
	UInt Year;
	UInt HoursNMinute;
	UInt PayLoadWeight;
	UInt Swings;
	UInt LoadTime;
	UInt HaulTime;
	UInt HaulStopTime;
	UInt DumpTime;
	UInt ReturnTime;
	UInt ReturnStopTime;
	UInt HaulDistance;
	UInt ReturnDistance;
	UInt HaulMaxSpeed;
	UInt HaulAvgSpeed;
	UInt ReturnMaxSpeed;
	UInt ReturnAvgSpeed;
	char Operator[8];
	UInt LastStatus;
	UInt LastDnM;
	UInt LastYear;
	UInt LastHnM;	
	UInt LastSeconds;
	UInt OverLoaded;
	UInt Seconds;
	UInt BucketWeight;
	UInt K1;
	UInt K2;
	UInt ChnlErr;
	char FreeBuffer[6];
};
typedef struct tagFBITS {
        unsigned SendData1      :1;     
        unsigned SendData2      :1;   
        unsigned Stx1Received   :1;     
        unsigned Stx2Received   :1;     
        unsigned Etx1Received   :1;     
        unsigned Etx2Received   :1;   
        unsigned ADDATAREADY  	:1;
        unsigned ProcessPack1	:1;
        unsigned ProcessPack2	:1;
        unsigned Read1HR		:1;
        unsigned Write1HR		:1;
        unsigned Read2HR		:1;
        unsigned Write2HR		:1;
        unsigned NewSetting		:1;
        unsigned mACalAvail		:1;
		unsigned Calibrated 	:1;
		unsigned CalculateSpeed	:1;
		unsigned ReadRTC		:1;
		unsigned DataAvail		:1;
		unsigned FirstReading 	:1;
		unsigned FirstSpeed		:1;
		unsigned UpdateRecord	:1;
		unsigned AnyChange		:1;
		unsigned SendWeight		:1;
		unsigned PowerOnDelay	:1;
		unsigned WeightUpdated  :1;
		unsigned GreenSet		:1;
		unsigned AmberSet		:1;
		unsigned RedSet			:1;
		unsigned OLSet			:1;
		unsigned Force1Coil		:1;
		unsigned Force2Coil		:1;
		unsigned InitPort1		:1;
		unsigned Port1InitOver	:1;
		unsigned CodeAvail		:1;
		unsigned AngleCalibrated:1;
		unsigned ReadStatus		:1;
		unsigned AllowZero		:1;
		unsigned Zeroed			:1;
		unsigned InitPort2		:1;
		unsigned Port2InitOver	:1;
		unsigned ParkingSet		:1;
		unsigned DoAverage		:1;
		unsigned UpdateTime		:1;
		unsigned DataQueried	:1;
		unsigned FirstTime		:1;
		unsigned FirstTimeRun	:1;
		unsigned PowerOnCond1	:1;
		unsigned CheckTime		:1;
		unsigned FloatRcvd 		:1;
		unsigned FaultAvail		:1;
		unsigned UpdateNewWt	:1;
		unsigned StoreInitial	:1;
		unsigned InitCAN		:1;
		unsigned FromCAN		:1;
		unsigned SendLastRec	:1;
		unsigned SendSetup		:1;
} FLAGBITS;

void InerruptCheck_CAN_ID(void);
void RequestInfo(unsigned char TxCANPGN);
void UpdatePack2();
void Report_data_retrival(void);
void UpdatePack_USB(unsigned char startposition);




#endif
