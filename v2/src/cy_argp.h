/*
 * File: cy_argp.h
 * Edited: 04 Dec 2015
 * Author: Matthew Bauer
 */
#ifndef CYHG_ARGP
#define CYHG_ARGP

#include <string>
#include <stdint.h>

namespace cy
{
	// version
	const uint8_t ARGP_VER_MAJ = 2;
	const uint8_t ARGP_VER_MIN = 0;
	const uint8_t ARGP_VER_PAT = 0;
	
	// structures
	enum argp_param_type
	{
		ARGP_PT_NONE,
		ARGP_PT_STRING,
		ARGP_PT_INTEGER,
		ARGP_PT_DOUBLE
	};
	struct argp_arg_extra_info
	{
		std::string cmd;
		argp_cmd_type cmd_type;
		argp_param_type param_type;
	};
	
	// reserved UIDs
	const int32_t ARGP_UID_NO_CMD = -1; // for lone params 
	const int32_t ARGP_UID_NOT_FOUND = -2; // for unregistered options
	
	// arg struct
	struct argp_arg
	{
		// command id
		int32_t uid;
		
		// parameter (if any)
		struct
		{
			std::string vstr;
			int64_t vint;
			double vdbl;
		} param;
		
		// more info about the param. don't really know why this would be used.
		argp_arg_extra_info info;
	}
	enum argp_cmd_type
	{
		ARGP_CT_SINGLE,
		ARGP_CT_MULTI,
		ARGP_CT_NONE // DO NOT REGISTER COMMANDS WITH THIS! IT IS USED INTERNALLY
	};
	
	// settings commands (ss = set state)
	// they set the state of the setting in question to the argument.
	void argp_ss_allow_lone_params(bool); // default: true
	void argp_ss_allow_unregistered_commands(bool); // default: false

	// registering commands
	// cmd = the command. For "single" type commands, only the first letter is used. if there is more than one letter, an exception is thrown.
	// uid = the command identifier. if this is the same as any other registered commands, an exception will be thrown. see above list for reserved UIDs.
	// cmd_type = the type of the command (single, triggered by a "-..." string or double, triggered by a "--..." string).
	// param_type = the type of the parameter (string, integer, or double)
	void argp_register(const std::string& cmd, int32_t uid, argp_cmd_type cmd_type=ARGP_CT_SINGLE, argp_param_type param_type=ARGP_PT_NONE);
	void argp_clear_registry();
	
	// processing args
	bool argp_begin(int argc, char **argv); // argc and argv straight from main! returns false if not all arguments were good.
	bool argp_next(argp_arg& next); // returns false when no args are present
	void argp_end();
	
	// error reporting
	typedef argp_arg argp_misused_command_info; // struct.param.vint/vdbl are never used.
	std::vector<argp_misused_command_info> argp_get_misused_commands();
	std::vector<std::string> argp_get_unregistered_commands();
	void argp_write_misused_commands_rundown(std::ostream& os, const std::vector<argp_misused_command_info>& vec); // if empty, nothing is written
	void argp_write_unregistered_commands_rundown(std::ostream& os, const std::vector<std::string>& vec); // if empty, nothing is written.
};

#endif
