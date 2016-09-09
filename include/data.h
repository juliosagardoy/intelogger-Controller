/* 
 * File:   data.h
 * Author: julio
 *
 * Created on 17. August 2016, 19:08
 */

#ifndef DATA_H
#define	DATA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Globals.h"
    
/* -------------*
 * Declarations 
 * -------------*/
    
/**
 * DiscreteSigs structure
 */
typedef struct {
    byte vts;
    byte pcs;
    byte alt;
    byte fan;
} DiscreteSigs_t;

typedef DiscreteSigs_t discrete_sigs;

/**
 * AnalogSigs structure
 */
typedef struct {
    word tps;
    word map;
    word ect;
    word ho2;
    word iat;
    word fle;
    word ext;
    word bat;
    word tin;
} AnalogSigs_t;

/**
 * Defines type "Analog Signals" structure as analog_sigs
 */
typedef AnalogSigs_t analog_sigs;

/**
 * CcpSigs structure. Contains info from CCP modules
 */
typedef struct {
    word rpm;
    word vss;
} CcpSigs_t;

typedef CcpSigs_t ccp_sigs;



/*
 * Function declarations 
 */
void init_data(void);
discrete_sigs discrete_read(void);
analog_sigs analog_read(void);
ccp_sigs ccp_read(void);

/* Struct declaration */
static discrete_sigs DiscreteSigs;
static analog_sigs AnalogSigs;
static ccp_sigs CcpSigs;

#ifdef	__cplusplus
}
#endif

#endif	/* DATA_H */

