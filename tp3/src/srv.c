#include "srv.h"

/*
 *  Ejemplo de servidor que tiene el "sí fácil" para con su
 *  cliente y no se lleva bien con los demás servidores.
 *
 */

void servidor(int mi_cliente)
{
    MPI_Status status; int origen, tag;
    int hay_pedido_local = FALSE;
    int listo_para_salir = FALSE;
    int diferidos[cant_ranks/2];
    int respuestas_faltantes = (cant_ranks/2)-1;

    while( ! listo_para_salir ) {
        
        MPI_Recv(NULL, 0, MPI_INT, ANY_SOURCE, ANY_TAG, COMM_WORLD, &status);
        origen = status.MPI_SOURCE;
        tag = status.MPI_TAG;
        
        if (tag == TAG_PEDIDO) {
            assert(origen == mi_cliente);
            debug("Mi cliente solicita acceso exclusivo");
            assert(hay_pedido_local == FALSE);
            hay_pedido_local = TRUE;
            // debug("Dándole permiso (frutesco por ahora)");
            // MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            for(int i = 0; i < cant_ranks; i += 2){
                if( i != mi_rank ){
                    MPI_Send(NULL, 0, MPI_INT, i, TAG_REQUEST, COMM_WORLD);
                }
            }
        }
        
        else if (tag == TAG_LIBERO) {
            assert(origen == mi_cliente);
            debug("Mi cliente libera su acceso exclusivo");
            assert(hay_pedido_local == TRUE);
            hay_pedido_local = FALSE;
            for(int i = 0; i < cant_ranks/2; ++i)
            {
                if( diferidos[i] ){
                    diferidos[i] = false;
                    MPI_Send(NULL, 0, MPI_INT, 2*i, TAG_REPLY, COMM_WORLD);   
                }
            }
        }
        
        else if (tag == TAG_TERMINE) {
            assert(origen == mi_cliente);
            debug("Mi cliente avisa que terminó");
            listo_para_salir = TRUE;
        }

        else if (tag == TAG_REQUEST) {
            if( hay_pedido_local && mi_rank < origen){
                diferidos[origen/2] = true;
            } else {
                MPI_Send(NULL, 0, MPI_INT, 2*i, TAG_REPLY, COMM_WORLD);
            }
        }

        else if (tag == TAG_REPLY) {
            respuestas_faltantes--;
            if(respuestas_faltantes == 0){
                respuestas_faltantes = 0;
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            }
        }
    }
    
}

