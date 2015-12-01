/*
 * File: cy_argp.h
 * Edited: 01 Dec 2015
 * Author: Matthew Bauer
 */
#ifndef CYHG_ARGP
#define CYHG_ARGP

#include <string>
#include <stdint.h>

namespace cy
{
	// version
	const uint8_t ARGP_VER_MAJ = 1;
	const uint8_t ARGP_VER_MIN = 0;
	const uint8_t ARGP_VER_PAT = 0;
	const char *ARGP_VER_STR = "1.0.0";
	
	// structures
	enum argp_type
	{
		ARGP_TYPE_NONE,
		ARGP_TYPE_STRING,
		ARGP_TYPE_INTEGER,
		ARGP_TYPE_DOUBLE
	};
	struct argp_arg
	{
		// command
		bool cmd_is_single;
		struct
		{
			std::string multi;
			char single;
		} cmd;
		
		// parameter (if any)
		argp_type param_type;
		struct
		{
			std::string p_string;
			int64_t p_integer;
			double p_double;
		} param;
	};
	
	// registering commands
	void argp_register_single(char cmd, argp_type type); // DO NOT WRITE - IN CMD
	void argp_register_multi(const std::string& cmd, argp_type type); // CMD IS AUTO PREFACED WITH --
	void argp_clear_registry();
	
	// processing args
	bool argp_begin(int argc, const char **argv); // argc and argv straight from main! If returns false, bad usage
	bool argp_next(argp_arg& next); // returns false when no args are present
	void argp_end();
};

#endif
