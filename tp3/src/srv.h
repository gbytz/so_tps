#ifndef __srv_h__
#define __srv_h__

#include "tp3.h"

#define TAG_REQUEST 	60 /* srv0 -> srvN */
#define TAG_REPLY		70 /* srv1 -> srv0 */
#define TAG_ME_CIERRO	80 /* srv0 -> srvN */

void servidor(int mi_cliente);

#endif
