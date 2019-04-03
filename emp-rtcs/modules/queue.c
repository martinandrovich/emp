/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	exm.c
* MODULENAME.:	LCD
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include "queue.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

static CIRC_BUFFER *    CIRC_BUFFER_new(uint32_t size);
static void             CIRC_BUFFER_del(CIRC_BUFFER * cb);
static uint32_t         CIRC_BUFFER_lendata(CIRC_BUFFER * cb);
static Q_ERROR          CIRC_BUFFER_write(CIRC_BUFFER * cb, MESSAGE data);
static Q_ERROR          CIRC_BUFFER_read(CIRC_BUFFER * cb, MESSAGE * data);
static bool             CIRC_BUFFER_empty(CIRC_BUFFER * cb);

/****************************   Class Struct   *****************************/

const struct CIRC_BUFFER_CLASS circ_buffer =
{
    .new            = &CIRC_BUFFER_new,
    .del            = &CIRC_BUFFER_del,
    .lendata		= &CIRC_BUFFER_lendata,
	.read		    = &CIRC_BUFFER_read,
	.write          = &CIRC_BUFFER_write,
    .empty          = &CIRC_BUFFER_empty

};

static CIRC_BUFFER * CIRC_BUFFER_new(uint32_t size)
/*******************************************************************************
 * Delete New
 ******************************************************************************/
{
    MESSAGE * buffer    = malloc( size * sizeof( MESSAGE ) );
    CIRC_BUFFER * this  = malloc( sizeof( CIRC_BUFFER ) );
    this->buffer        = buffer;
    this->head          = 0;
    this->tail          = 0;
    this->size          = size;
    return this;
}

static void CIRC_BUFFER_del(CIRC_BUFFER * this)
/*******************************************************************************
 * Delete
 ******************************************************************************/
{
    free( this->buffer );
    free( this );
}

static uint32_t CIRC_BUFFER_lendata(CIRC_BUFFER * cb)
/*******************************************************************************
 * Length Data
 ******************************************************************************/
{
    return ( ( cb->head - cb->tail ) & ( cb->size - 1 ) );
}

static Q_ERROR CIRC_BUFFER_write(CIRC_BUFFER * cb, message data)
/*******************************************************************************
 * Write Data
 ******************************************************************************/
{
    if ( CIRC_BUFFER_lendata( cb ) == ( cb->size-1 ) ) return BUFFERFULL;
    cb->buffer[ cb->head ] = data;
    // disable scheduler
    cb->head = ( cb->head + 1 ) & ( cb->size-1 );
    // enable scheduler
    return DONE;
}

static Q_ERROR CIRC_BUFFER_read(CIRC_BUFFER * cb, message *data)
/*******************************************************************************
 * Read Data
 ******************************************************************************/
{
    if ( CIRC_BUFFER_lendata( cb ) == 0 ) return BUFFEREMPTY;
    *data = cb->buffer[ cb->tail ];
    // disable scheduler
    cb->tail = ( cb->tail + 1 ) & ( cb->size-1 );
    // enable scheduler
    return DONE;
}

static bool CIRC_BUFFER_empty(CIRC_BUFFER * cb)
/*******************************************************************************
 * Write Data
 ******************************************************************************/
 {
     if ( CIRC_BUFFER_lendata( cb ) == 0 )
     {
         return 1;
     }
     else
     {
         return 0;
     }
 }
