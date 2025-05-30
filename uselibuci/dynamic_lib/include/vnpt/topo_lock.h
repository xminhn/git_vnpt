/*
 *================================================================================
 *	Author      	: VNPT Technology - HEC
 *  Created on      : Jan 21 2022
 *	Description 	: Unify Mesh Stack
 *================================================================================
*/
#ifndef __topo_LOCK_H__
#define __topo_LOCK_H__

#include <semaphore.h>
#include "basic_defs.h"
#include "log.h"

#define		SEM_NAMED_TOPO_DATA		"/topo-data"
#define		FULL_R_W_PERM				    0666

enum 
{
    LV_ERROR   = 0,
    LV_WARNING = 1,
    LV_INFO    = 2,
    LV_DEBUG   = 3,
    LV_ALL     = 4
};

sem_t* topo_lock_open (char* name);

int topo_aquire_lock (sem_t *sem, int second);

int topo_release_lock (sem_t *sem);

void topo_close_lock (sem_t *sem);


#endif /* __topo_LOCK_H__ */