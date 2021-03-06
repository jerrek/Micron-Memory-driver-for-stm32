extern void ReinitMemsPinsForOuts (void);
extern void ReinitMemsPinsForIns (void);
extern volatile uint8_t MEMReadData (void);
extern void MEMWriteData (volatile uint8_t byte);
extern void MEMWriteAdress (volatile uint8_t byte);
extern void MEMWriteCommand (volatile uint8_t byte);
extern uint8_t MEM_Ready_Busy (void);
extern void MEMRead (uint8_t status);
extern void MEMWrite (uint8_t status);
extern void MEMAddress (uint8_t status);
extern void MEMCommand(uint8_t status);
extern void MEMChipSelect (uint8_t status);
extern void MEMWriteProtect (uint8_t status);
extern uint8_t ReadStatus(void);
extern void ReadManufacturID( uint8_t *data);
extern void MEMReset(void);
extern uint8_t MEMInit(void);
extern void MEMCycleWriteData (volatile uint8_t *byte, uint8_t *adr);
extern void MEMCycleReadData (uint8_t reset, uint8_t *adr,  uint8_t *data);
extern void MEMEraseData (uint8_t reset, uint8_t *adr);
