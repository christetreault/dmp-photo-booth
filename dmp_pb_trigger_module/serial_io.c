#include "serial_io.h"


G_DEFINE_QUARK(DMP_TM_SERIAL_IO_ERROR, dmp_tm_serial_io_error)

static GThread * dmp_tm_serial_io_thread = NULL;
static gboolean dmp_tm_serial_io_thread_should_die = TRUE;
gint serial_descriptor;

static struct termios saved_options;
static struct termios trigger_options;

/**
 * reads 1 byte from the trigger
 * @return the read byte, 0 if no data to read
 */
static guint8 dmp_tm_io_read_byte(void)
{
	char working[1];
	gint len = read(serial_descriptor, working, 1);
	
	if (len == -1)
	{
		dmp_tm_console_write("Failed to read data from the trigger\n");
		//dmp_tm_serial_io_thread_should_die = TRUE;
		dmp_tm_set_status(FALSE);
	}
	else if (len == 0) return 0;
	
	guint8 return_value = working[0];
	return return_value;
}

/**
 * Writes 1 byte to the serial port
 * @param the byte to write
 */
void dmp_tm_io_write_byte(gint8 in)
{	
	gchar to_write = (gchar) in;
	
	if (write(serial_descriptor, &to_write, 1) != 1)
	{
		dmp_tm_console_write("Failed to write data to the trigger\n");
		//dmp_tm_serial_io_thread_should_die = TRUE;
		dmp_tm_set_status(FALSE);
	}
}

/**
 * Thread function for serial IO. Handles inputs and output until
 * dmp_tm_serial_io_thread_should_die == TRUE
 * @param user_data NULL, unused
 * @return NULL
 */
static gpointer dmp_tm_serial_io_thread_function(gpointer user_data)
{	
	guint8 read_value = 0;
	while (dmp_tm_io_thread_running())
	{
		read_value = dmp_tm_io_read_byte();
		
		if (read_value & OUTPUT_BUTTON_PRESS) dmp_tm_call_trigger_handler();
		
	}
	
	
	return NULL;
}

void dmp_tm_io_start_serial(void)
{
	gchar * serial_port = dmp_tm_config_get_device_name();

	if ((serial_descriptor = open(serial_port, O_RDWR | O_NOCTTY)) < 0)
	{
		dmp_tm_console_write("Failed to open serial connection!\n");
		return;
	}
	
	g_free(serial_port);
	
	if (tcgetattr(serial_descriptor, &saved_options) < 0)
	{
		dmp_tm_console_write("Failed to get serial attributes\n");
		close(serial_descriptor);
		return;
	}
	
	if (tcgetattr(serial_descriptor, &trigger_options) < 0)
	{
		dmp_tm_console_write("Failed to get serial attributes\n");
		close(serial_descriptor);
		return;
	}
	
	cfsetispeed(&trigger_options, B9600);
    cfsetospeed(&trigger_options, B9600);
	trigger_options.c_cflag &= ~PARENB;
    trigger_options.c_cflag &= ~CSTOPB;
    trigger_options.c_cflag &= ~CSIZE;
    trigger_options.c_cflag |= CS8;
    trigger_options.c_cflag &= ~CRTSCTS;
    trigger_options.c_cflag |= CREAD | CLOCAL;
    trigger_options.c_iflag &= ~(IXON | IXOFF | IXANY);
    trigger_options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    trigger_options.c_oflag &= ~OPOST;
	trigger_options.c_cc[VMIN]  = 0;
    trigger_options.c_cc[VTIME] = 20;
	
	if (tcsetattr(serial_descriptor, TCSANOW, &trigger_options) < 0)
	{
		dmp_tm_console_write("Failed to set serial attributes\n");
		close(serial_descriptor);
		return;
	}
	
	g_usleep(G_USEC_PER_SEC * 3);
	tcflush(serial_descriptor, TCIOFLUSH);
	dmp_tm_serial_io_thread_should_die = FALSE;
	dmp_tm_set_status(TRUE);
	dmp_tm_serial_io_thread = g_thread_new("serial thread", dmp_tm_serial_io_thread_function, NULL);
}

void dmp_tm_io_stop_serial(void)
{
	dmp_tm_serial_io_thread_should_die = TRUE;
	dmp_tm_set_status(FALSE);
	g_thread_join(dmp_tm_serial_io_thread);
	if (tcsetattr(serial_descriptor, TCSANOW, &saved_options) < 0)
	{
		dmp_tm_console_write("Failed to restore saved serial attributes\n");
	}
	
	close(serial_descriptor);
}

gboolean dmp_tm_io_thread_running(void)
{
	return !dmp_tm_serial_io_thread_should_die;
}
