/**
 * data.c
 *
 */

#include "data.h"
#include "adc.h"
#include "ccp.h"

void init_data()
{
    memset(&DiscreteSigs, 0x00, sizeof(DiscreteSigs));
    memset(&AnalogSigs, 0x00, sizeof(AnalogSigs));
    memset(&CcpSigs, 0x00, sizeof(CcpSigs));
}

/**
 * Retrieve current discrete values from ECM
 * @return discrete_sigs structure data
 */
discrete_sigs discrete_read() {
//    *DiscreteSigs |= GPIO_VTS;
//    *DiscreteSigs |= GPIO_PCS << 1;
//    *DiscreteSigs |= GPIO_ALT << 2;
//    *DiscreteSigs |= GPIO_FAN << 3;
    DiscreteSigs.vts = GPIO_VTS;
    DiscreteSigs.pcs = GPIO_PCS;
    DiscreteSigs.alt = GPIO_ALT;
    DiscreteSigs.fan = GPIO_FAN;
#ifdef DEBUG
    DiscreteSigs.vts = 1;
    DiscreteSigs.pcs = 0;
    DiscreteSigs.alt = 1;
    DiscreteSigs.fan = 1;    
#endif
    
    return DiscreteSigs;
}

/**
 * Retrieve current analog data from ADC
 * @return analog_sigs structure data
 */
analog_sigs analog_read(void) {
    adc_enable();
    AnalogSigs.tps = adc_read(AN_TPS_CHAN);
    AnalogSigs.ect = adc_read(AN_ECT_CHAN);
    AnalogSigs.map = adc_read(AN_MAP_CHAN);
    AnalogSigs.ho2 = adc_read(AN_HO2_CHAN);
    AnalogSigs.iat = adc_read(AN_IAT_CHAN);
    AnalogSigs.fle = adc_read(AN_FLV_CHAN);
    AnalogSigs.bat = adc_read(AN_BAT_CHAN);
    AnalogSigs.tin = adc_read(AN_TIM_CHAN);
    AnalogSigs.ext = adc_read(AN_EXT_CHAN);
    adc_disable();
    
    return AnalogSigs;
}

/**
 * Returns current CCPRxH and CCPRxL registers
 * @return ccp_sigs structure data
 */
ccp_sigs ccp_read(void) {
	CcpSigs.rpm = ccp_module_read(CCP_RPM_CHAN) - CcpSigs.rpm;
	CcpSigs.vss = ccp_module_read(CCP_VSS_CHAN) - CcpSigs.vss;
    
    return CcpSigs;
}