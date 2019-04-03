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

static C_BUFFER*		C_BUFFER_new(uint32_t size);
static void 			C_BUFFER_del(C_BUFFER* this);

static uint32_t			C_BUFFER_length(C_BUFFER* this);
static C_BUF_ERROR		C_BUFFER_write(C_BUFFER* this, MESSAGE data);
static C_BUF_ERROR		C_BUFFER_read(C_BUFFER* this, MESSAGE* data);
static bool				C_BUFFER_empty(C_BUFFER* this);

/****************************   Class Struct   *****************************/

const struct C_BUFFER_CLASS cbuf =
{
	.new			= &C_BUFFER_new,
	.del			= &C_BUFFER_del,
	.read			= &C_BUFFER_read,
	.write			= &C_BUFFER_write,
	.length			= &C_BUFFER_length,
	.empty			= &C_BUFFER_empty
};

static C_BUFFER * C_BUFFER_new(uint32_t size)
/*******************************************************************************
 * Delete New
 ******************************************************************************/
{
	MESSAGE * buffer	= malloc(size * sizeof(MESSAGE));
	C_BUFFER * this		= malloc(sizeof(C_BUFFER));

	this->size			= size;
	this->buffer		= buffer;
	this->head			= 0;
	this->tail			= 0;

	return this;
}

static void C_BUFFER_del(C_BUFFER * this)
/*******************************************************************************
 * Delete
 ******************************************************************************/
{
	free( this->buffer );
	free( this );
}

static uint32_t C_BUFFER_length(C_BUFFER * this)
/*******************************************************************************
 * Length Data
 ******************************************************************************/
{
	return ( ( this->head - this->tail ) & ( this->size - 1 ) );
}

static C_BUF_ERROR C_BUFFER_write(C_BUFFER * this, message data)
/*******************************************************************************
 * Write Data
 ******************************************************************************/
{
	if ( C_BUFFER_lendata( this ) == ( this->size-1 ) ) return C_BUF_FULL;

	this->buffer[ this->head ] = data;
	// disable scheduler
	this->head = ( this->head + 1 ) & ( this->size-1 );
	// enable scheduler
	return C_BUF_DONE;
}

static C_BUF_ERROR C_BUFFER_read(C_BUFFER * this, message *data)
/*******************************************************************************
 * Read Data
 ******************************************************************************/
{
	if ( C_BUFFER_lendata( this ) == 0 ) return C_BUF_EMPTY;

	*data = this->buffer[ this->tail ];
	// disable scheduler
	this->tail = ( this->tail + 1 ) & ( this->size-1 );
	// enable scheduler
	return C_BUF_DONE;
}

static bool C_BUFFER_empty(C_BUFFER * this)
/*******************************************************************************
 * Write Data
 ******************************************************************************/
 {
	 return (C_BUFFER_lendata(this) == 0);
 }
