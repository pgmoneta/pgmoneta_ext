/*
 * Copyright (C) 2026 The pgmoneta community
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

#ifndef PGMONETA_EXT_UTILS_H
#define PGMONETA_EXT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* PostgreSQL */
#include <postgres.h>
#include <fmgr.h>
#include <funcapi.h>
#include <nodes/pg_list.h>
#include <utils/array.h>

/* system */
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PRIVILEGE_DEFAULT               1 << 0  // 000001
#define PRIVILEGE_PG_CHECKPOINT         1 << 1  // 000010
#define PRIVILEGE_PG_READ_ALL_DATA      1 << 2  // 000100
#define PRIVILEGE_PG_READ_SERVER_FILES  1 << 3  // 001000
#define PRIVILEGE_PG_WRITE_SERVER_FILES 1 << 4  // 010000
#define PRIVILEGE_SUPERUSER             1 << 5  // 100000

#define MAX_DBNAME_LENGTH              128
#define MAX_PATH                       1024

/** @struct db_info
 * Define a structure to hold both the OID and database name
 */
struct db_info
{
   Oid oid;                         /**< The OID of the database */
   char dbname[MAX_DBNAME_LENGTH];  /**< The name of the database */
} __attribute__ ((aligned (64)));

/** @struct path_info
 * Define a structure to hold the path name and its meta data
 */
struct path_info
{
   char name[MAX_PATH];       /**< The name of the path */
   bool is_dir;               /**< The path is directory/file */
   size_t size;               /**< The size of the path */
} __attribute__ ((aligned (64)));

/**
 * Check if the role has superuser privileges.
 * @param roleid The current role's ID
 * @return The result
 */
bool
pgmoneta_ext_check_superuser(Oid roleid);

/**
 * Check if the role has specific role.
 * @param roleid The current role's ID
 * @param rolename Specific role name
 * @return The result
 */
bool
pgmoneta_ext_check_role(Oid roleid, const char* rolename);

/**
 * Get a bit mask of the privileges of the role accessing.
 * @param roleid The current role's ID
 * @return The result
 */
int
pgmoneta_ext_check_privilege(Oid roleid);

/**
 * Get the specific database OID by database name.
 * @param dbname The name of the database.
 * @return The OID of the database if found, otherwise InvalidOid.
 */
Oid
pgmoneta_ext_get_oid_by_dbname(char* dbname);

/**
 * Get a list of all database OIDs and their names.
 * @param dbname The name of the database.
 * @return A list of db_info structs containing the OID and name of each database.
 */
List*
pgmoneta_ext_get_all_oids(void);

#ifdef __cplusplus
}
#endif

#endif
