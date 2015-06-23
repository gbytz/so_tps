#include "srv.h"

/*
 *  Ejemplo de servidor que tiene el "sí fácil" para con su
 *  cliente y no se lleva bien con los demás servidores.
 *
 */

void debug_server(const char *mje, int origen) {
#ifndef NDEBUG
    printf("\trk%-3d %c%-3d %c    %-50s rk%-3d s%-3d\n",
        mi_rank, mi_rol ? 'c' : 's', mi_nro, mi_char, mje, origen, origen/2);
#endif
}

void servidor(int mi_cliente)
{
    MPI_Status status; int origen, tag;
    MPI_Request request;
    int hay_pedido_local = FALSE;
    int listo_para_salir = FALSE;


    // Arreglo para marcar los pedidos pospuestos.
    int diferidos[cant_ranks/2];

    // Arreglo para marcar que otros servidores siguen activos.
    int activos[cant_ranks/2];
    int servers_activos = cant_ranks/2;

    // Contador de respuestas que me falta recibir para obtener acceso exclusivo
    int respuestas_faltantes = (cant_ranks/2);
    int espero_respuesta[cant_ranks/2];

    int k;
    for(k = 0; k < cant_ranks/2; ++k){
        diferidos[k] = FALSE;
        activos[k] = TRUE;
        espero_respuesta[k] = FALSE;
    }
    activos[mi_nro] = FALSE; // Así evito enviarme/esperar mensajes de mí mismo

    int their_sequence_number;
    int our_sequence_number;
    int highest_sequence_number = 0;

    while( ! listo_para_salir ) {
        MPI_Recv(&their_sequence_number, 1, MPI_INT, ANY_SOURCE, ANY_TAG, COMM_WORLD, &status);
        origen = status.MPI_SOURCE;
        tag = status.MPI_TAG;

        if (tag == TAG_PEDIDO) {
            assert(origen == mi_cliente);
            debug("Mi cliente solicita acceso exclusivo");
            assert(hay_pedido_local == FALSE);
            hay_pedido_local = TRUE;

            if ( cant_ranks == 2 || servers_activos == 1 ) {
                debug("Dejo pasar a mi cliente");
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            } else {
                our_sequence_number = highest_sequence_number + 1;
                respuestas_faltantes = 0;
                int i;
                for(i = 0; i < cant_ranks/2; ++i){
                    if( activos[i] ){
                        debug_server("Enviando Request a", i * 2);
                        MPI_Isend(&our_sequence_number, 1, MPI_INT, i * 2, TAG_REQUEST, COMM_WORLD, &request);
                        espero_respuesta[i] = TRUE;
                        respuestas_faltantes++;
                    }
                }
            }
        }

        else if (tag == TAG_LIBERO) {
            assert(origen == mi_cliente);
            debug("Mi cliente libera su acceso exclusivo");
            assert(hay_pedido_local == TRUE);
            hay_pedido_local = FALSE;
            int i;
            for(i = 0; i < cant_ranks/2; ++i)
            {
                if( diferidos[i] ){
                    diferidos[i] = FALSE;
                    MPI_Isend(NULL, 0, MPI_INT, i * 2, TAG_REPLY, COMM_WORLD, &request);
                }
            }
        }

        else if (tag == TAG_TERMINE) {
            assert(origen == mi_cliente);
            debug("Mi cliente avisa que terminó");
            int i;
            for(i = 0; i < cant_ranks/2; ++i){
                if( activos[i] ){
                    debug_server("Le aviso que me cierro a", i * 2);
                    MPI_Isend(NULL, 0, MPI_INT, i * 2, TAG_ME_CIERRO, COMM_WORLD, &request);
                }
            }
            listo_para_salir = TRUE;
        }

        else if (tag == TAG_REQUEST) {
            highest_sequence_number = highest_sequence_number > their_sequence_number ? highest_sequence_number : their_sequence_number;

            if( hay_pedido_local && (mi_rank < origen || our_sequence_number < their_sequence_number) ){
                debug_server("Pospongo pedido de", origen);
                diferidos[origen/2] = TRUE;
            } else {
                debug_server("Le respondo a", origen);
                MPI_Isend(NULL, 0, MPI_INT, origen, TAG_REPLY, COMM_WORLD, &request);
            }
        }

        else if (tag == TAG_REPLY) {
            if( hay_pedido_local && espero_respuesta[origen/2] ){
                debug_server("Recibí respuesta de", origen);
                espero_respuesta[origen/2] = FALSE;
                respuestas_faltantes--;
                if( respuestas_faltantes <= 0 ){
                    debug("Dejo pasar a mi cliente");
                    MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
                }
            }
        }

        else if (tag == TAG_ME_CIERRO ){
            debug_server("Me avisa que se cierra", origen);
            if( espero_respuesta[origen/2] ){
                espero_respuesta[origen/2] = FALSE;
                respuestas_faltantes--;
            }
            activos[origen/2] = FALSE;
            servers_activos--;
        }
    }

}
