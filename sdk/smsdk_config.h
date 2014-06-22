// vim: set ts=4 :
#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

/**
 * @file smsdk_config.h
 * @brief Contains macros for configuring basic extension information.
 */

/* Basic information exposed publically */
#define SMEXT_CONF_NAME			"Socket"
#define SMEXT_CONF_DESCRIPTION	"Socket extension for SourceMod"
#define SMEXT_CONF_VERSION		"3.0.1"
#define SMEXT_CONF_AUTHOR		"Player"
#define SMEXT_CONF_URL			"http://www.player.to/"
#define SMEXT_CONF_LOGTAG		"SOCKET"
#define SMEXT_CONF_LICENSE		"GPL"
#define SMEXT_CONF_DATESTRING	__DATE__

/**
 * @brief Exposes plugin's main interface.
 */
#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;

/**
 * @brief Sets whether or not this plugin required Metamod.
 * NOTE: Uncomment to enable, comment to disable.
 * NOTE: This is enabled automatically if a Metamod build is chosen in
 *		 the Visual Studio project.
 */
//#define SMEXT_CONF_METAMOD

/** Enable interfaces you want to use here by uncommenting lines */
//#define SMEXT_ENABLE_FORWARDSYS
#define SMEXT_ENABLE_HANDLESYS
//#define SMEXT_ENABLE_PLAYERHELPERS
//#define SMEXT_ENABLE_DBMANAGER
//#define SMEXT_ENABLE_GAMECONF
//#define SMEXT_ENABLE_MEMUTILS
//#define SMEXT_ENABLE_GAMEHELPERS
//#define SMEXT_ENABLE_TIMERSYS
//#define SMEXT_ENABLE_ADTFACTORY
//#define SMEXT_ENABLE_PLUGINSYS
//#define SMEXT_ENABLE_THREADER

#endif //_INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

