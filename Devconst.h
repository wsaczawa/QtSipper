/* **************************< DEVCONST.h >**************************** *
 * Editor: L.Stone                                      Origin: Jul-99  *
 * -------------------------------------------------------------------- *
 * Purpose:                                                             *
 *  Define the constants for device and driver access in common place.  *
 * -------------------------------------------------------------------- *
 * Edit History:                                                        *
 *   1-Jul-99, LES:  Need MOD_ constants, 'standard' for others.        *
 *                  (Merging files, so two names for some constants.)   *
 * ******************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif


#if defined (MCA_S100)
#else
////////////////////

/* -- see <device.h> --

struct device_S {

    BYTE   bDevice;
    BYTE   bModifier;
    USHORT usDeviceType;
    BYTE   bProtocol;

    };
*/
/* ================================== */
/*
** - Constants for the above --
**
**  DEV_  -- possible kinds of device
**  MCA_  -- possible sorts of MCAs
**           Note: MCA_ forms part! of 'usDeviceType'.
**  MOD_  -- modifiers to 'usDeviceType'
**  PRO_  -- possible values for bProtocol
*/

#define DEV_MCA                0
#define DEV_ADC                1
#define DEV_MXR                2
#define DEV_DigStab            3
#define DEV_SmpChngr           4
#define DEV_Changer            4
#define DEV_Amp                5
#define DEV_Amplify            5
#define DEV_HVPS               6
#define DEV_HiVolts            6
#define DEV_PreAmp             7
#define DEV_LFC                8
#define DEV_MCS                9
#define DEV_Protocol          10
    //  Not a 'device', but queries to the protocol driver
    //  go through the same channels; useful.
#define DEV_PwrMgmt           11
#define DEV_Vacuum            12
#define DEV_GainDSP           13
#define DEV_FilterDSP         14
#define DEV_Filter            14
#define DEV_PISR              15
#define DEV_Pulser            15


#define MCA_S100          0x0100
#define MCA_AccuSpecA     0x0300
#define MCA_AccuSpecB     0x0400
#define MCA_AccuSpecNaI   0x0500
#define MCA_AccuSpecNaIP  0x0600
#define MCA_AccuSpecFMS   0x0800
#define MCA_AIM           0x0900
#define MCA_GeniePort     0x0A00
#define MCA_PORTABLE      0x0A00
#define MCA_AAnalyst      0x0B00
#define MCA_InSpectNaI    0x0C00
#define MCA_NAI_INSP      0x0C00
#define MCA_DesktopInsp   0x0D00
#define MCA_AIMwithDSP    0x0E00
#define MCA_AIMDSP        0x0E00
#define MCA_DesktopDSP    0x0F00
#define MCA_DSA2000       0x0F00
#define MCA_InSpectDSP    0x1000
#define MCA_PcapPCI       0x1100
    // (Called I2K, InSpector-2000, Portable-DSP.)
#define MCA_DSA1000       0x1200
#define MCA_G35_90        0x1400
#define MCA_G35_200       0x1500
// Tennelec boards 1600 - 1c00
#define MCA_PcapNAI       0x1600
#define MCA_Pca3Ext       0x1700
#define MCA_Pca3Int2k     0x1800
#define MCA_Pca3Int8k     0x1900
#define MCA_PcamExt       0x1a00
#define MCA_PcamInt8k     0x1b00    
#define MCA_PcamInt16k    0x1c00
#define MCA_Pca3Int1k     0x1d00
#define MCA_Pca3Int4k     0x1e00
#define MCA_Oasis         0x1f00
//Aptec S5000 family - 2000 thru 2400
#define MCA_Aptec5016     0x2000
#define MCA_Aptec5008	  0x2100
#define MCA_Aptec5004	  0x2200
#define MCA_Aptec5002	  0x2300
#define MCA_Aptec5002BB	  0x2400
#define MCA_I2kSim 	      0x2500


#define MOD_Manual             1
#define MOD_Internal           2
#define MOD_Parallel           3
#define MOD_ICB                4
#define MOD_RPI_ICB            5
#define MOD_EtherPlus          6
#define MOD_GAChanger          7


#define PRO_S100P              1
#define PRO_AccuSpecATP        3
#define PRO_AccuSpecFMSP       8
#define PRO_AIMP               9
#define PRO_GeniPortP         10
#define PRO_AAnalystP         11                                                     // drd 2/17/95
#define PRO_DesktopDSPP       12
#define PRO_USBus             13
#define PRO_GPIBus            14  // (Also called IEEE-488.)
#define PRO_TennPCAP          15
#define PRO_ManualP           16
#define PRO_SerialP           17
#define PRO_ParallelP         18
#define PRO_ICBP              19
#define PRO_RPI_ICBP          20
#define PRO_ParallelX         21  // (?? Alternate; don't use LPTn?)
#define PRO_TennPCAP_PCI      22
#define PRO_AptecS5000        23

////////////////////
#endif

#ifdef __cplusplus
}
#endif

/* ************************* End of File ************************** */
