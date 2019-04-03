#include<stdio.h>
#include<stdint.h>
#include<malloc.h>


typedef enum Q_ERROR Q_ERROR;
typedef enum MESSAGE_TYPE MESSAGE_TYPE;

enum Q_ERROR{
    FAIL,
    DONE,
    BUFFERFULL,
    BUFFEREMPTY
};

enum MESSAGE_TYPE {
    IMPORTANT,
    DATA
};

enum ID {
    INT,
    CHAR
};

typedef struct {
    uint32_t size;
    uint32_t ID;
    MESSAGE_TYPE type;
} message;


typedef struct {
    message * buffer;
    uint32_t head;
    uint32_t tail;
    uint32_t size;
} circ_bbuf;

uint32_t CBlendata(circ_bbuf * cb)
{
    return ((cb->head - cb->tail) & (cb->size - 1));
}

static Q_ERROR CBwrite(circ_bbuf * cb, message data)
{
    if ( CBlendata(cb) == (cb->size-1) ) return BUFFERFULL;
    cb->buffer[cb->head] = data;
    // disable scheduler
    cb->head = (cb->head + 1) & (cb->size-1);
    // enable scheduler
    return DONE;
}

static Q_ERROR CBread(circ_bbuf * cb, message *data)
{
    if ( CBlendata(cb) == 0 ) return BUFFEREMPTY;
    *data = cb->buffer[cb->tail];
    // disable scheduler
    cb->tail = (cb->tail + 1) & (cb->size-1);
    // enable scheduler
    return DONE;
}

static circ_bbuf * CB_new(uint32_t size)
{
    message * buffer = malloc(size * sizeof(message));
    circ_bbuf * this = malloc(sizeof(circ_bbuf));
    this->buffer = buffer;
    this->head = 0;
    this->tail = 0;
    this->size = size;
    return this;
}

static void CB_del(circ_bbuf * this)
{
    free(this->buffer);
    free(this);
}

int main()
{
    message b = {5, IMPORTANT};
    message d = {0};

    circ_bbuf * c = CB_new(32);

    Q_ERROR write = CBwrite( c , b );

    Q_ERROR read = CBread(c , &d );

    CB_del(c);


    return 0;
}
