/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* NetworkManager system settings service
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * (C) Copyright 2008 - 2012 Red Hat, Inc.
 */

#ifndef __NMS_IFCFG_RH_UTILS_H__
#define __NMS_IFCFG_RH_UTILS_H__

#include "nm-connection.h"

#include "shvar.h"

/*****************************************************************************/

#define NM_IFCFG_CONNECTION_LOG_PATH(path)  ((path) ?: "in-memory")
#define NM_IFCFG_CONNECTION_LOG_FMT         "%s (%s,\"%s\")"
#define NM_IFCFG_CONNECTION_LOG_ARG(con)    NM_IFCFG_CONNECTION_LOG_PATH (nm_settings_connection_get_filename ((NMSettingsConnection *) (con))), nm_connection_get_uuid ((NMConnection *) (con)), nm_connection_get_id ((NMConnection *) (con))
#define NM_IFCFG_CONNECTION_LOG_FMTD        "%s (%s,\"%s\",%p)"
#define NM_IFCFG_CONNECTION_LOG_ARGD(con)   NM_IFCFG_CONNECTION_LOG_PATH (nm_settings_connection_get_filename ((NMSettingsConnection *) (con))), nm_connection_get_uuid ((NMConnection *) (con)), nm_connection_get_id ((NMConnection *) (con)), (con)

char *utils_cert_path (const char *parent, const char *suffix, const char *extension);

const char *utils_get_ifcfg_name (const char *file, gboolean only_ifcfg);

gboolean utils_should_ignore_file (const char *filename, gboolean only_ifcfg);

char *utils_get_ifcfg_path (const char *parent);
char *utils_get_keys_path (const char *parent);
char *utils_get_route_path (const char *parent);
char *utils_get_route6_path (const char *parent);

shvarFile *utils_get_extra_ifcfg (const char *parent, const char *tag, gboolean should_create);
shvarFile *utils_get_keys_ifcfg (const char *parent, gboolean should_create);
shvarFile *utils_get_route_ifcfg (const char *parent, gboolean should_create);
shvarFile *utils_get_route6_ifcfg (const char *parent, gboolean should_create);

gboolean utils_has_route_file_new_syntax (const char *filename);
gboolean utils_has_complex_routes (const char *filename);

gboolean utils_is_ifcfg_alias_file (const char *alias, const char *ifcfg);

char *utils_detect_ifcfg_path (const char *path, gboolean only_ifcfg);

/******************************************************************************/

typedef struct _NMSIfcfgRhPack NMSIfcfgRhPack;

NMSIfcfgRhPack *nms_ifcfg_rh_pack_new_open_file (const char *filename, GError **error);
NMSIfcfgRhPack *nms_ifcfg_rh_pack_new_create_file (const char *filename);
void nms_ifcfg_rh_pack_unref (NMSIfcfgRhPack *self);

shvarFile *nms_ifcfg_rh_pack_get_main (NMSIfcfgRhPack *self);
const char *nms_ifcfg_rh_pack_get_filename (NMSIfcfgRhPack *self);

void nms_ifcfg_rh_pack_subfile_prune (NMSIfcfgRhPack *self, const char *filename);
shvarFile *nms_ifcfg_rh_pack_subfile_create (NMSIfcfgRhPack *self, const char *filename);

gboolean nms_ifcfg_rh_pack_write_file (NMSIfcfgRhPack *self, GError **error);

static inline void
_nm_auto_ifcfg_rh_pack_unref (NMSIfcfgRhPack **p_s)
{
	if (*p_s) {
		int errsv = errno;

		nms_ifcfg_rh_pack_unref (*p_s);
		errno = errsv;
	}
}
#define nm_auto_ifcfg_rh_pack_unref nm_auto(_nm_auto_ifcfg_rh_pack_unref)

/******************************************************************************/

#endif /* __NMS_IFCFG_RH_UTILS_H__ */
