/* 
 * File:   data.h
 * Author: julio
 *
 * Created on 17. August 2016, 19:08
 */

#ifndef DATA_H
#define	DATA_H

#include "Globals.h"
    
/* -------------*
 * Declarations 
 * -------------*/
    
/**
 * DiscreteSigs structure, all packed in a single byte
 */
typedef struct {
    unsigned vts : 1;
    unsigned other1 : 1;
    unsigned other2 : 1;
    unsigned other3 : 1;
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
    word other1;
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
discrete_sigs DiscreteSigs;
analog_sigs AnalogSigs;
ccp_sigs CcpSigs;


#endif	/* DATA_H */

