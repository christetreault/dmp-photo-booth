#include "error_handling.h"

static gint dmp_pb_last_error_code = DMP_PB_SUCCESS;

gint dmp_pb_get_last_error_code()
{
	return dmp_pb_last_error_code;
}

gint dmp_pb_set_error_code(gint to_set)
{
	static GMutex mutex;
	g_mutex_lock(&mutex);
	if (to_set == DMP_PB_SUCCESS)
	{
		dmp_pb_last_error_code = DMP_PB_NO_OP;
		g_mutex_unlock(&mutex);
		return DMP_PB_NO_OP;
	}
	dmp_pb_last_error_code = to_set;
	g_mutex_unlock(&mutex);
	return DMP_PB_SUCCESS;
}

GString * dmp_pb_resolve_error_code(gint to_resolve)
{
	//TODO: implementation
}

gint dmp_pb_set_error_code_return(gint to_set)
{
	g_assert(to_set != DMP_PB_SUCCESS);
	dmp_pb_set_error_code(to_set);
	return to_set;
}