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

#ifndef PGMONETA_EXT_UTILS_H
#define PGMONETA_EXT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* PostgreSQL */
#include "postgres.h"
#include "fmgr.h"

/* system */
#include <stdbool.h>

#define PRIVILEDGE_DEFAULT            1 << 0  // 001
#define PRIVILEDGE_PG_CHECKPOINT      1 << 1  // 010
#define PRIVILEDGE_SUPERUSER          1 << 2  // 100

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

#ifdef __cplusplus
}
#endif

#endif