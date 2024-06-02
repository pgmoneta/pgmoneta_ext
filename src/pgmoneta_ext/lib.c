/*
 * Copyright (C) 2024 The pgmoneta community
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may
 * be used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* pgmoneta_ext */
#include <pgmoneta_ext.h>
#include <utils.h>

/* PostgreSQL */
#include "postgres.h"
#include "access/htup_details.h"
#include "access/xlog.h"
#include "access/xlog_internal.h"
#include "catalog/pg_authid.h"
#include "catalog/pg_type.h"
#include "fmgr.h"
#include "funcapi.h"
#include "miscadmin.h"
#include "utils/acl.h"
#include "utils/builtins.h"
#include "utils/elog.h"
#include "utils/errcodes.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(pgmoneta_ext_version);
PG_FUNCTION_INFO_V1(pgmoneta_ext_switch_wal);
PG_FUNCTION_INFO_V1(pgmoneta_ext_checkpoint);

Datum
pgmoneta_ext_version(PG_FUNCTION_ARGS)
{
   Datum version;
   char v[1024];

   memset(&v, 0, sizeof(v));
   snprintf(&v[0], sizeof(v), "%s", VERSION);

   version = CStringGetTextDatum(v);

   PG_RETURN_DATUM(version);
}

Datum
pgmoneta_ext_switch_wal(PG_FUNCTION_ARGS)
{
   Datum values[2];
   Datum result;
   HeapTuple tuple;
   Oid roleid;
   TupleDesc tupdesc;
   XLogRecPtr recptr;
   bool nulls[2];
   char str_res[1024];
   bool is_superuser;

   memset(nulls, 0, sizeof(nulls));

   roleid = GetUserId();

   is_superuser = pgmoneta_ext_check_superuser(roleid);

   if (is_superuser)
   {
      // Request to switch WAL with mark_unimportant set to false.
      recptr = RequestXLogSwitch(false);

      values[0] = BoolGetDatum(true);
      // Format the WAL position
      snprintf(str_res, sizeof(str_res), "%X/%X", (uint32) (recptr >> 32), (uint32) recptr);
      values[1] = CStringGetTextDatum(str_res);
   }
   else
   {
      ereport(LOG, errmsg_internal("pgmoneta_ext_switch_wal: Current role is not a superuser"));

      values[0] = BoolGetDatum(false);
      nulls[1] = true;
   }

   // Create a tuple descriptor for our result type
   if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
   {
      ereport(ERROR, errmsg_internal("pgmoneta_ext_switch_wal: Return type must be a row type"));
   }

   // Build the result tuple
   tuple = heap_form_tuple(tupdesc, values, nulls);
   result = HeapTupleGetDatum(tuple);

   PG_RETURN_DATUM(result);
}

#ifndef RequestCheckpoint
extern void RequestCheckpoint(int flags);
#endif

Datum
pgmoneta_ext_checkpoint(PG_FUNCTION_ARGS)
{
   Datum values[2];
   Datum result;
   HeapTuple tuple;
   Oid roleid;
   TupleDesc tupdesc;
   bool nulls[2];
   bool is_superuser;
   bool is_pg_checkpoint;
   char cp[1024];

   memset(nulls, 0, sizeof(nulls));
   memset(&cp, 0, sizeof(cp));
   is_superuser = false;
   is_pg_checkpoint = false;

   roleid = GetUserId();
   is_superuser = pgmoneta_ext_check_superuser(roleid);

#if PG_VERSION_NUM >= 150000
   is_pg_checkpoint = pgmoneta_ext_check_role(roleid, "pg_checkpoint");
#endif

   if (is_superuser || is_pg_checkpoint)
   {
      // Perform the checkpoint
      RequestCheckpoint(CHECKPOINT_IMMEDIATE | CHECKPOINT_WAIT | CHECKPOINT_FORCE);

      values[0] = BoolGetDatum(true);
      snprintf(&cp[0], sizeof(cp), "%s", "CHECKPOINT");
      values[1] = CStringGetTextDatum(cp);
   }
   else
   {
      ereport(LOG, errmsg_internal("pgmoneta_ext_checkpoint: Current role is not a superuser"));

      values[0] = BoolGetDatum(false);
      nulls[1] = true;
   }

   // Create a tuple descriptor for our result type
   if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
   {
      ereport(ERROR, errmsg_internal("pgmoneta_ext_checkpoint: Return type must be a row type"));
   }

   // Build the result tuple
   tuple = heap_form_tuple(tupdesc, values, nulls);
   result = HeapTupleGetDatum(tuple);

   PG_RETURN_DATUM(result);
}