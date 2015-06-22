#ifndef __srv_h__
#define __srv_h__

#include "tp3.h"

#define TAG_REQUEST 60 /* srv0 -> srv1 */
#define TAG_REPLY	70 /* srv1 -> srv0 */

void servidor(int mi_cliente);

#endif
