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
#include <postgres.h>
#include <access/htup_details.h>
#include <access/xlog.h>
#include <access/xlog_internal.h>
#include <catalog/pg_authid.h>
#include <catalog/pg_type.h>
#include <common/base64.h>
#include <commands/dbcommands.h>
#include <fmgr.h>
#include <funcapi.h>
#include <lib/stringinfo.h>
#include <mb/pg_wchar.h>
#include <miscadmin.h>
#include <utils/acl.h>
#include <utils/array.h>
#include <utils/builtins.h>
#include <utils/bytea.h>
#include <utils/elog.h>
#include <utils/errcodes.h>
#include <utils/lsyscache.h>
#include <utils/memutils.h>
#include <utils/syscache.h>

/* system */
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>

PG_MODULE_MAGIC;

#define PGMONETA_EXT_CHUNK_SIZE 8192

static void list_files(const char* name, ArrayBuildState* astate);
static text* base64_encode(bytea* data);
static bytea* base64_decode(text* base64, size_t* decoded_len);

PG_FUNCTION_INFO_V1(pgmoneta_ext_version);
PG_FUNCTION_INFO_V1(pgmoneta_ext_switch_wal);
PG_FUNCTION_INFO_V1(pgmoneta_ext_checkpoint);
PG_FUNCTION_INFO_V1(pgmoneta_ext_get_oid);
PG_FUNCTION_INFO_V1(pgmoneta_ext_get_oids);
PG_FUNCTION_INFO_V1(pgmoneta_ext_get_file);
PG_FUNCTION_INFO_V1(pgmoneta_ext_get_files);
PG_FUNCTION_INFO_V1(pgmoneta_ext_receive_file_chunk);
PG_FUNCTION_INFO_V1(pgmoneta_ext_promote);

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
   int privileges;

   memset(nulls, 0, sizeof(nulls));

   roleid = GetUserId();

   privileges = pgmoneta_ext_check_privilege(roleid);

   if (privileges & PRIVILEGE_SUPERUSER)
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
   char cp[1024];
   int privileges;

   memset(nulls, 0, sizeof(nulls));
   memset(&cp, 0, sizeof(cp));

   roleid = GetUserId();
   privileges = pgmoneta_ext_check_privilege(roleid);

   if (privileges & (PRIVILEGE_PG_CHECKPOINT | PRIVILEGE_SUPERUSER))
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

Datum
pgmoneta_ext_get_oid(PG_FUNCTION_ARGS)
{
   text* dbname_text;
   char* dbname;
   Oid result_oid;

   dbname_text = PG_GETARG_TEXT_PP(0);
   dbname = text_to_cstring(dbname_text);
   result_oid = InvalidOid;

   result_oid = pgmoneta_ext_get_oid_by_dbname(dbname);

   if (OidIsValid(result_oid))
   {
      PG_RETURN_TEXT_P(cstring_to_text(psprintf("%u", result_oid)));
   }
   else
   {
      PG_RETURN_NULL();
   }
}

Datum
pgmoneta_ext_get_oids(PG_FUNCTION_ARGS)
{
   FuncCallContext* funcctx;
   List* db_list;
   ListCell* cell;
   MemoryContext oldcontext;

   if (SRF_IS_FIRSTCALL())
   {
      TupleDesc tupdesc;

      funcctx = SRF_FIRSTCALL_INIT();

      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      db_list = pgmoneta_ext_get_all_oids();

      funcctx->user_fctx = db_list;

      tupdesc = CreateTemplateTupleDesc(2);
      TupleDescInitEntry(tupdesc, (AttrNumber) 1, "oid", OIDOID, -1, 0);
      TupleDescInitEntry(tupdesc, (AttrNumber) 2, "dbname", TEXTOID, -1, 0);

      funcctx->attinmeta = TupleDescGetAttInMetadata(tupdesc);

      MemoryContextSwitchTo(oldcontext);
   }

   funcctx = SRF_PERCALL_SETUP();

   db_list = (List*) funcctx->user_fctx;
   cell = list_head(db_list);

   if (cell != NULL)
   {
      struct db_info* db_info = (struct db_info*) lfirst(cell);
      Datum result;
      Datum values[2];
      bool nulls[2] = {false, false};
      HeapTuple tuple;
      List* rest;

      values[0] = ObjectIdGetDatum(db_info->oid);
      values[1] = CStringGetTextDatum(db_info->dbname);

      tuple = heap_form_tuple(funcctx->attinmeta->tupdesc, values, nulls);

      result = HeapTupleGetDatum(tuple);

      rest = list_delete_first(db_list);
      funcctx->user_fctx = rest;

      SRF_RETURN_NEXT(funcctx, result);
   }
   else
   {
      SRF_RETURN_DONE(funcctx);
   }
}

Datum
pgmoneta_ext_get_file(PG_FUNCTION_ARGS)
{
   int privileges;
   char buffer[PGMONETA_EXT_CHUNK_SIZE];
   char* file_path;
   size_t bytes_read;
   text* file_path_arg;
   FILE* file;
   StringInfoData result;
   Oid roleid;
   bytea* file_data;
   text* base64_result;

   file_path_arg = PG_GETARG_TEXT_PP(0);
   file_path = text_to_cstring(file_path_arg);

   roleid = GetUserId();
   privileges = pgmoneta_ext_check_privilege(roleid);

   if (privileges & PRIVILEGE_SUPERUSER)
   {
      initStringInfo(&result);

      // Open the file
      file = AllocateFile(file_path, "r");
      if (file == NULL)
      {
         ereport(ERROR, errmsg_internal("pgmoneta_ext_get_file: Could not open file \"%s\": %m", file_path));
         PG_RETURN_NULL();
      }

      PG_TRY();
      {
         // Read the file content in chunks
         while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
         {
            if (ferror(file))
            {
               ereport(ERROR, (errmsg_internal("pgmoneta_ext_get_file: Could not read file \"%s\": %m", file_path)));
            }
            appendBinaryStringInfo(&result, buffer, bytes_read);
         }
      }
      PG_CATCH();
      {
         FreeFile(file);
         PG_RE_THROW();
      }
      PG_END_TRY();

      FreeFile(file);

      file_data = (bytea*)palloc(VARHDRSZ + result.len);
      SET_VARSIZE(file_data, VARHDRSZ + result.len);
      memcpy(VARDATA(file_data), result.data, result.len);

      // Encode as Base64
      base64_result = base64_encode(file_data);

      PG_RETURN_TEXT_P(base64_result);
   }
   else
   {
      ereport(LOG, errmsg_internal("pgmoneta_ext_get_file: Current role is not a superuser"));
      PG_RETURN_NULL();
   }
}

Datum
pgmoneta_ext_get_files(PG_FUNCTION_ARGS)
{
   text* file_path_arg;
   char* file_path;
   struct stat path_stat;
   ArrayBuildState* astate = NULL;
   Oid roleid;
   int privileges;

   file_path_arg = PG_GETARG_TEXT_PP(0);
   file_path = text_to_cstring(file_path_arg);

   roleid = GetUserId();
   privileges = pgmoneta_ext_check_privilege(roleid);

   if (privileges & PRIVILEGE_SUPERUSER)
   {
      if (stat(file_path, &path_stat) != 0)
      {
         ereport(ERROR, errmsg_internal("Could not stat file or directory: %s", file_path));
         PG_RETURN_NULL();
      }

      astate = initArrayResult(TEXTOID, CurrentMemoryContext, false);

      if (S_ISDIR(path_stat.st_mode))
      {
         list_files(file_path, astate);
      }
      else if (S_ISREG(path_stat.st_mode))
      {
         astate = accumArrayResult(astate, CStringGetTextDatum(file_path), false, TEXTOID, CurrentMemoryContext);
      }
      else
      {
         ereport(ERROR, errmsg_internal("Argument is neither a file nor a directory: %s", file_path));
         PG_RETURN_NULL();
      }

      PG_RETURN_DATUM(makeArrayResult(astate, CurrentMemoryContext));
   }
   else
   {
      ereport(LOG, errmsg_internal("pgmoneta_ext_get_files: Current role is not a superuser"));
      PG_RETURN_NULL();
   }
}

Datum
pgmoneta_ext_receive_file_chunk(PG_FUNCTION_ARGS)
{
   int privileges;
   Oid roleid;
   text* base64_chunk;
   bytea* decoded_data = NULL;
   size_t decoded_len;
   char* file_path;
   FILE* file = NULL;
   int fd;

   base64_chunk = PG_GETARG_TEXT_PP(0);
   file_path = text_to_cstring(PG_GETARG_TEXT_PP(1));

   roleid = GetUserId();
   privileges = pgmoneta_ext_check_privilege(roleid);

   if (privileges & PRIVILEGE_SUPERUSER)
   {
      file = fopen(file_path, "ab");
      if (file == NULL)
      {
         ereport(ERROR, (errmsg_internal("pgmoneta_ext_receive_file_chunk: Could not open file '%s' for writing: %m", file_path)));
         PG_RETURN_INT32(1);
      }

      fd = fileno(file);
      if (fd == -1)
      {
         fclose(file);
         ereport(ERROR, (errmsg_internal("pgmoneta_ext_receive_file_chunk: Could not get file descriptor for '%s'", file_path)));
         PG_RETURN_INT32(1);
      }

      if (flock(fd, LOCK_EX | LOCK_NB) != 0)
      {
         fclose(file);
         ereport(ERROR, (errmsg_internal("pgmoneta_ext_receive_file_chunk: File is already locked by another thread")));
         PG_RETURN_INT32(1);
      }

      decoded_data = base64_decode(base64_chunk, &decoded_len);
      if (decoded_data == NULL)
      {
         flock(fd, LOCK_UN);
         fclose(file);
         ereport(ERROR, (errmsg_internal("pgmoneta_ext_receive_file_chunk: Failed to decode base64 chunk")));
         PG_RETURN_INT32(1);
      }

      if (fwrite(decoded_data, 1, decoded_len, file) != decoded_len)
      {
         flock(fd, LOCK_UN);
         pfree(decoded_data);
         fclose(file);
         ereport(ERROR, (errmsg_internal("pgmoneta_ext_receive_file_chunk: Failed to write chunk to file")));
         PG_RETURN_INT32(1);
      }

      pfree(decoded_data);
      flock(fd, LOCK_UN);
      fclose(file);

      PG_RETURN_INT32(0);
   }
   else
   {
      ereport(LOG, errmsg_internal("pgmoneta_ext_receive_file_chunk: Current role is not a superuser"));
      PG_RETURN_INT32(1);
   }

}

Datum
pgmoneta_ext_promote(PG_FUNCTION_ARGS)
{
   Oid roleid;
   int privileges;
   Datum promoted;

   roleid = GetUserId();
   privileges = pgmoneta_ext_check_privilege(roleid);

   if (privileges & PRIVILEGE_SUPERUSER)
   {

      promoted = DirectFunctionCall2(pg_promote,
                                     BoolGetDatum(false),
                                     Int32GetDatum(0));

      PG_RETURN_BOOL(DatumGetBool(promoted));
   }
   else
   {
      ereport(LOG, errmsg_internal("pgmoneta_ext_promote: Current role is not a superuser"));
      PG_RETURN_BOOL(false);
   }
}

static bytea*
base64_decode(text* base64, size_t* decoded_len)
{
   size_t base64_len;
   char* base64_str;
   int actual_decoded_len;
   bytea* decoded_bytea = NULL;

   if (base64 == NULL)
   {
      ereport(ERROR, errmsg_internal("Invalid input to base64_decode"));
      return NULL;
   }

   base64_len = VARSIZE_ANY_EXHDR(base64);
   base64_str = VARDATA_ANY(base64);

   *decoded_len = pg_b64_dec_len(base64_len);

   decoded_bytea = (bytea*) palloc(VARHDRSZ + *decoded_len);

   actual_decoded_len = pg_b64_decode(base64_str, base64_len, VARDATA(decoded_bytea), *decoded_len);

   if (actual_decoded_len < 0)
   {
      ereport(ERROR, errmsg_internal("Failed to decode Base64 data"));
      pfree(decoded_bytea);
      return NULL;
   }

   SET_VARSIZE(decoded_bytea, VARHDRSZ + actual_decoded_len);
   *decoded_len = actual_decoded_len;

   return decoded_bytea;
}

static text*
base64_encode(bytea* data)
{
   size_t data_len = VARSIZE_ANY_EXHDR(data);
   size_t encoded_len = pg_b64_enc_len(data_len);
   char* encoded = palloc(encoded_len + 1);
   int actual_encoded_len = pg_b64_encode(VARDATA_ANY(data), data_len, encoded, encoded_len);

   text* result = (text*)palloc(VARHDRSZ + actual_encoded_len);
   SET_VARSIZE(result, VARHDRSZ + actual_encoded_len);
   memcpy(VARDATA(result), encoded, actual_encoded_len);

   pfree(encoded);
   return result;
}

static void
list_files(const char* name, ArrayBuildState* astate)
{
   DIR* dir;
   struct dirent* entry;

   if (!(dir = opendir(name)))
   {
      return;
   }

   while ((entry = readdir(dir)) != NULL)
   {
      char path[MAX_PATH];
      snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);

      struct stat path_stat;
      if (stat(path, &path_stat) != 0)
      {
         continue;
      }

      if (S_ISDIR(path_stat.st_mode))
      {
         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
         {
            continue;
         }

         list_files(path, astate);
      }
      else if (S_ISREG(path_stat.st_mode))
      {
         astate = accumArrayResult(astate, CStringGetTextDatum(path), false, TEXTOID, CurrentMemoryContext);
      }
   }
   closedir(dir);
}
