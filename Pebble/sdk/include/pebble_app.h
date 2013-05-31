#pragma once

#include <stdint.h>

#include "pebble_app_info.h"

#include "src/resource_ids.auto.h"

//! @addtogroup Foundation
//! @{
//!   @addtogroup App
//!   @{

#define PBL_APP_INFO_NOPAREN(...) __VA_ARGS__

//! Macro to set the minimal metadata of the application
//! @param uuid The universally unique identifier of a watchapp
//! @param name The name of your app (max. 32 bytes long)
//! @param company The name of your company (max. 32 bytes long)
//! @param version The app's version as 16-bit value. The lower 8 bits will
//! be used as minor version, the higher 8 bits will be used as major version.
//! @note The metadata will not contain an icon resource. Use \ref PBL_APP_INFO
//! instead to specify the icon resource.
#define PBL_APP_INFO_SIMPLE(_uuid, name, company, version) PBL_APP_INFO(PBL_APP_INFO_NOPAREN(_uuid), (name), (company), ((version) & 0xff00) >> 8, ((version) & 0xff), INVALID_RESOURCE, 0)

//! Macro to set the metadata of the application
//! @param _uuid The universally unique identifier of your app
//! @param _name The name of your app (max. 32 bytes long)
//! @param _company The name of your company (max. 32 bytes long)
//! @param _version_major The app's major version component (e.g. the '1' in v1.0)
//! @param _version_minor The app's minor version component (e.g. the '0' in v1.0)
//! @param _icon_resource_id The resource ID of the app's icon. See \htmlinclude UsingResources.html .
//! @param _flags Bitmask of \ref PebbleAppFlags values.
#define PBL_APP_INFO(_uuid, _name, _company, _version_major, _version_minor, _icon_resource_id, _flags)    \
  const PebbleAppInfo __pbl_app_info __attribute__ ((section (".pbl_header"))) = { \
    .header = "PBLAPP", \
    .struct_version = { APP_INFO_CURRENT_STRUCT_VERSION_MAJOR, APP_INFO_CURRENT_STRUCT_VERSION_MINOR }, \
    .sdk_version = { APP_INFO_CURRENT_SDK_VERSION_MAJOR, APP_INFO_CURRENT_SDK_VERSION_MINOR }, \
    .app_version = { _version_major, _version_minor }, \
    .size = 0xb6b6, \
    .offset = 0xb6b6b6b6, \
    .crc = 0xb6b6b6b6, \
    .name = (_name), \
    .company = (_company), \
    .icon_resource_id = (_icon_resource_id), \
    .sym_table_addr = 0xA7A7A7A7, \
    .flags = (_flags), \
    .reloc_list_start = 0xdeafbeef, \
    .num_reloc_entries = 0xdeadcafe, \
    .uuid = _uuid \
  }

//!   @} // group App
//! @} // group Foundation
