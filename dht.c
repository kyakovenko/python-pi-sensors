#include <wiringPi.h>
#include <Python.h>

#define MAXTIMINGS	85
#define DHTPIN		7
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

static PyObject * read_dht11(PyObject *dummy, PyObject * args, PyObject *keywds)
{
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;
	int 	dhtpin		= DHTPIN;

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	static char *kwlist[] = {"pin", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, keywds, "|i", kwlist, &dhtpin))
        return NULL;

	/* pull pin down for 18 milliseconds */
	pinMode( dhtpin, OUTPUT );
	digitalWrite( dhtpin, LOW );
	delay( 18 );
	/* then pull it up for 40 microseconds */
	digitalWrite( dhtpin, HIGH );
	delayMicroseconds( 40 );
	/* prepare to read the pin */
	pinMode( dhtpin, INPUT );

	/* detect change and read data */
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead( dhtpin ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( dhtpin );

		if ( counter == 255 )
			break;

		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * return it out if data is good
	 */
	if ( (j < 40) ||
	     (dht11_dat[4] != ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		PyErr_SetString(PyExc_RuntimeError, "ERR_CRC");
		return NULL;
	}

    return PyTuple_Pack( 
    	2,
    	PyString_FromFormat("%d.%d", dht11_dat[0], dht11_dat[1]),
    	PyString_FromFormat("%d.%d", dht11_dat[2], dht11_dat[3])
    );

}

static PyObject * wiringPiSetupWrapper(PyObject * self, PyObject * args) {
	return PyLong_FromLong( wiringPiSetup() );
}

#define READ_DHT11_DESCRIPTION "The function reads information from a DHT11 sensor. \n" \
"\n" \
"Args and Kwargs: \n" \
"    • pin (int):  The number of a pin which the sensor is plugged into. \n" \
"\n" \
"Returns: \n" \
"    Tuple: (Humidity, Temperature) \n" \
"\n" \
"Raises: \n" \
"    • RuntimeError with the ERR_CRC description when data isn't ready. \n" \
"\n" \
"A way you might use me is \n" \
"\n" \
">>> wiringPiSetup() \n" \
">>> read_dht11(pin = 7) \n"

static PyMethodDef DHT11Methods[] = {
	{ "wiringPiSetup", wiringPiSetupWrapper, METH_NOARGS, "test"},
	{ "read_dht11", read_dht11, METH_VARARGS | METH_KEYWORDS, READ_DHT11_DESCRIPTION },
 	{ NULL, NULL, 0, NULL }
};

DL_EXPORT(void) initdht11(void)
{
  Py_InitModule("dht11", DHT11Methods);
}

