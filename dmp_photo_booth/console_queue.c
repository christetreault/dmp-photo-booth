#include "console_queue.h"

/**
 * Locks the queue mutex
 */
#define CQ_MUTEX_LOCK (g_mutex_lock(&dmp_pb_cq_mutex))

/**
 * Unlocks the queue mutex
 */
#define CQ_MUTEX_UNLOCK (g_mutex_unlock(&dmp_pb_cq_mutex))



static gboolean cq_is_initialized = FALSE;
static GQueue * dmp_pb_cq;
static GMutex dmp_pb_cq_mutex;

gint dmp_pb_console_queue_init()
{
	CQ_MUTEX_LOCK;
	dmp_pb_cq = g_queue_new();
	cq_is_initialized = TRUE;
	CQ_MUTEX_UNLOCK;

	return DMP_PB_SUCCESS;
}

gboolean dmp_pb_console_queue_is_empty()
{
	CQ_MUTEX_LOCK;
	gboolean return_value = g_queue_is_empty(dmp_pb_cq);
	CQ_MUTEX_UNLOCK;
	return return_value;
}

gint dmp_pb_console_queue_push(GString * message)
{
	if (!cq_is_initialized)
	{
		g_string_free(message, TRUE);
		return dmp_pb_set_error_code_return(DMP_PB_CONSOLE_QUEUE_NOT_INITIALIZED);
	}
	CQ_MUTEX_LOCK;

	g_queue_push_tail(dmp_pb_cq, message);

	CQ_MUTEX_UNLOCK;
	return DMP_PB_SUCCESS;
}

GString * dmp_pb_console_queue_pop()
{
	GString * return_value = NULL;
	if (!cq_is_initialized)
	{
		dmp_pb_set_error_code(DMP_PB_CONSOLE_QUEUE_NOT_INITIALIZED);
		return return_value;
	}
	if (dmp_pb_console_queue_is_empty())
	{
		dmp_pb_set_error_code(DMP_PB_NULL_POINTER);
		return return_value;
	}

	CQ_MUTEX_LOCK;

	return_value = g_queue_pop_head(dmp_pb_cq);

	CQ_MUTEX_UNLOCK;
	return return_value;
}

GString * dmp_pb_console_queue_peek()
{
	GString * return_value = NULL;
	if (!cq_is_initialized)
	{
		dmp_pb_set_error_code(DMP_PB_CONSOLE_QUEUE_NOT_INITIALIZED);
		return return_value;
	}
	if (dmp_pb_console_queue_is_empty())
	{
		dmp_pb_set_error_code(DMP_PB_NULL_POINTER);
		return return_value;
	}

	CQ_MUTEX_LOCK;

	return_value = g_queue_peek_head(dmp_pb_cq);

	CQ_MUTEX_UNLOCK;
	return return_value;
}

gint dmp_pb_console_queue_finalize()
{
	if (!cq_is_initialized) return dmp_pb_set_error_code_return(DMP_PB_CONSOLE_QUEUE_NOT_INITIALIZED);

	GString * working;

	CQ_MUTEX_LOCK;

	while (!g_queue_is_empty(dmp_pb_cq))
	{
		working = g_queue_pop_head(dmp_pb_cq);
		g_string_free(working, TRUE);
	}

	g_queue_free(dmp_pb_cq);
	cq_is_initialized = FALSE;

	CQ_MUTEX_UNLOCK;
	return DMP_PB_SUCCESS;
}

gboolean dmp_pb_console_queue_flush_queue(gpointer user_data)
{
	GtkTextBuffer * text_buffer = GTK_TEXT_BUFFER(user_data);
	GString * working;
	GtkTextIter iter;

	while (!dmp_pb_console_queue_is_empty())
	{
		working = dmp_pb_console_queue_pop();
		gtk_text_buffer_get_iter_at_offset(text_buffer, &iter, 0);
		gtk_text_buffer_insert(text_buffer, &iter, working->str, working->len);
		g_string_free(working, TRUE);
	}

	return G_SOURCE_CONTINUE;
}