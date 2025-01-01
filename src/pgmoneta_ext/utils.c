/*
 * Copyright (C) 2025 The pgmoneta community
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
#include <utils.h>

/* PostgreSQL */
#include <access/genam.h>
#include <access/heapam.h>
#include <access/htup_details.h>
#include <catalog/indexing.h>
#include <catalog/pg_authid.h>
#include <catalog/pg_database.h>
#include <miscadmin.h>
#include <utils/acl.h>
#include <utils/builtins.h>
#include <utils/elog.h>
#include <utils/errcodes.h>
#include <utils/memutils.h>
#include <utils/rel.h>
#include <utils/relcache.h>
#include <utils/syscache.h>

/* system */

bool
pgmoneta_ext_check_superuser(Oid roleid)
{
   bool is_superuser;
   HeapTuple roletuple;
   Form_pg_authid roleform;

   is_superuser = false;

   // Fetch the role's tuple from pg_authid
   roletuple = SearchSysCache1(AUTHOID, ObjectIdGetDatum(roleid));
   if (!HeapTupleIsValid(roletuple))
   {
      ereport(ERROR, errmsg_internal("pgmoneta_ext_check_superuser: Role with OID %u does not exist", roleid));
      return false;
   }

   // Get the role's superuser attribute
   roleform = (Form_pg_authid) GETSTRUCT(roletuple);

   is_superuser = roleform->rolsuper;

   // Release the role tuple
   ReleaseSysCache(roletuple);

   return is_superuser;
}

bool
pgmoneta_ext_check_role(Oid roleid, const char* rolename)
{
   bool is_success;
   HeapTuple roletuple;
   List* role_oids;
   ListCell* cell;

   is_success = false;

   roletuple = SearchSysCache1(AUTHOID, ObjectIdGetDatum(roleid));
   if (!HeapTupleIsValid(roletuple))
   {
      ereport(ERROR, errmsg_internal("pgmoneta_ext_check_role: Role with OID %u does not exist", roleid));
      return false;
   }

   // Get role OIDs of a given role name
   Oid role_oid = get_role_oid(rolename, false);
   role_oids = list_make1_oid(role_oid);

   // Check if the role has the specific privilege
   foreach (cell, role_oids)
   {
      Oid checkpoint_role_oid = lfirst_oid(cell);
      if (is_member_of_role(roleid, checkpoint_role_oid))
      {
         is_success = true;
         break;
      }
   }

   ReleaseSysCache(roletuple);
   list_free(role_oids);

   return is_success;
}

int
pgmoneta_ext_check_privilege(Oid roleid)
{
   int privileges = 0;

   if (pgmoneta_ext_check_superuser(roleid))
   {
      privileges |= PRIVILEGE_SUPERUSER;
   }

#if PG_VERSION_NUM >= 150000
   bool is_pg_checkpoint = pgmoneta_ext_check_role(roleid, "pg_checkpoint");

   if (is_pg_checkpoint)
   {
      privileges |= PRIVILEGE_PG_CHECKPOINT;
   }
#endif

   privileges |= PRIVILEGE_DEFAULT;

   return privileges;
}

Oid
pgmoneta_ext_get_oid_by_dbname(char* dbname)
{
   List* db_list;
   ListCell* cell;
   Oid db_oid;

   db_list = pgmoneta_ext_get_all_oids();
   db_oid = InvalidOid;

   foreach(cell, db_list)
   {
      struct db_info* db_info = (struct db_info*) lfirst(cell);

      if (strncmp(db_info->dbname, dbname, MAX_DBNAME_LENGTH) == 0)
      {
         db_oid = db_info->oid;
         break;
      }
   }

   foreach(cell, db_list)
   {
      struct db_info* db_info = (struct db_info*) lfirst(cell);
      pfree(db_info);
   }
   list_free(db_list);

   return db_oid;
}

List*
pgmoneta_ext_get_all_oids(void)
{
   List* db_list = NIL;
   Relation rel;
   TableScanDesc scan;
   HeapTuple tuple;

   rel = table_open(DatabaseRelationId, AccessShareLock);

   scan = table_beginscan_catalog(rel, 0, NULL);

   while ((tuple = heap_getnext(scan, ForwardScanDirection)) != NULL)
   {
      Form_pg_database db_form = (Form_pg_database) GETSTRUCT(tuple);

      struct db_info* db_info = (struct db_info*) palloc(sizeof(struct db_info));

      db_info->oid = db_form->oid;
      strncpy(db_info->dbname, NameStr(db_form->datname), MAX_DBNAME_LENGTH);

      // elog(INFO, "Database OID: %u, Database Name: %s", db_info->oid, db_info->dbname);

      db_list = lappend(db_list, db_info);
   }

   table_endscan(scan);
   table_close(rel, AccessShareLock);

   return db_list;
}
