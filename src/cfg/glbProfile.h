/*
 *  Copyright 2003, China Union Pay Co., Ltd.  All right reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF CHINA UNION PAY CO.,
 *  LTD.  THE CONTENTS OF THIS FILE MAY NOT BE DISCLOSED TO THIRD
 *  PARTIES, COPIED OR DUPLICATED IN ANY FORM, IN WHOLE OR IN PART,
 *  WITHOUT THE PRIOR WRITTEN PERMISSION OF CHINA UNION PAY CO., LTD.
 *
 *  $Id: glbProfile.h,v 1.3 2012/11/18 05:48:16 huilu Exp $
 *
 *  This is the header file of common defines
 *
 *  Edit History:
 *
 *    2003/08/12 - Created by Derek.
 *
 */

#ifndef __GLBPROFILE_H
#define __GLBPROFILE_H
#include "cupstype.h"

int glbPflGetInt(const char *section,
                 const char *entry,
                 const char *filename,
                 INT32 *value);
int glbPflGetLong(const char *section,
                  const char *entry,
                  const char *filename,
                  INT64 *value);
int glbPflGetString(const char *section,
                    const char *entry,
                    const char *filename,
                    char *value);

#endif
