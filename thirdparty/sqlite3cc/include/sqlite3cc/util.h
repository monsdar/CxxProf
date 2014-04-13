/*
 * util.h
 *
 * Copyright (C) 2009 Tim Marston <tim@ed.am>
 *
 * This file is part of sqlite3cc (hereafter referred to as "this program").
 * See http://ed.am/dev/sqlite3cc for more information.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SQLITE3CC_UTIL_H_
#define SQLITE3CC_UTIL_H_


#include <sqlite3.h>


namespace sqlite
{


/**
 * Check to see if sqlite is threadsafe.  See
 * http://www.sqlite.org/threadsafe.html for more information.
 *
 * @returns sqlite threading mode
 * @see sqlite3_threadsafe()
 */
int threadsafe();


} // namespace sqlite


extern "C" {

	/**
	 * Convenience function, so that the presence of libsqlite3cc can be
	 * detected easily from autoconf's AC_CHECK_LIB() macro.
	 */
	void libsqlite3cc_is_present( void );

} // extern "C"


#endif /* SQLITE3CC_UTIL_H_ */
