#include "console_queue.h"

static GMutex dmp_pb_cq_mutex;
static gboolean cq_is_initialized = FALSE;
static GAsyncQueue * dmp_pb_cq;

/**
 * Thread safe setter for cq_is_initialized
 * @param to_set the value to set
 */
static void dmp_pb_console_queue_set_init(gboolean to_set)
{
	g_mutex_lock(&dmp_pb_cq_mutex);
	cq_is_initialized = to_set;
	g_mutex_unlock(&dmp_pb_cq_mutex);
}

/**
 * thread safe geter for cq_is_initialized
 * @return the value of cq_is_initialized
 */
static gboolean dmp_pb_console_queue_get_init()
{
	gboolean return_value;
	g_mutex_lock(&dmp_pb_cq_mutex);
	return_value = cq_is_initialized;
	g_mutex_unlock(&dmp_pb_cq_mutex);
	return return_value;
}

/**
 * The GDestroyNotify function for the console queue
 * @param data
 */
static void dmp_pb_console_queue_destroy_notify(gpointer data)
{
	g_string_free((GString *) data, TRUE);
}

void dmp_pb_console_queue_init()
{
	if (dmp_pb_console_queue_get_init()) return;
	
	dmp_pb_cq = g_async_queue_new_full(dmp_pb_console_queue_destroy_notify);
	dmp_pb_console_queue_set_init(TRUE);
}

void dmp_pb_console_queue_push(GString * message)
{
	g_assert(dmp_pb_console_queue_get_init());
	g_assert(message != NULL);

	g_async_queue_push(dmp_pb_cq, message);
}

GString * dmp_pb_console_queue_pop()
{
	g_assert(dmp_pb_console_queue_get_init());
	
	return g_async_queue_try_pop(dmp_pb_cq);
}

void dmp_pb_console_queue_finalize()
{
	if (!dmp_pb_console_queue_get_init()) return;
	g_async_queue_unref(dmp_pb_cq);
	dmp_pb_console_queue_set_init(FALSE);
}

gboolean dmp_pb_console_queue_flush_queue(gpointer user_data)
{
	g_assert(user_data != NULL);
	
	GtkTextBuffer * text_buffer = GTK_TEXT_BUFFER(user_data);
	GString * working;
	GtkTextIter iter;

	while (working = dmp_pb_console_queue_pop())
	{
		gtk_text_buffer_get_iter_at_offset(text_buffer, &iter, 0);
		gtk_text_buffer_insert(text_buffer, &iter, working->str, working->len);
		g_string_free(working, TRUE);
	}

	return G_SOURCE_CONTINUE;
}

void dmp_pb_console_queue_flush_stdout()
{
	GString * working;
	GtkTextIter iter;
	
	while (working = dmp_pb_console_queue_pop())
	{
		g_printf("%s", working->str);
		g_string_free(working, TRUE);
	}
}