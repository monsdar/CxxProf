/*
 * manipulator.h
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

#ifndef SQLITE3CC_MANIPULATOR_H_
#define SQLITE3CC_MANIPULATOR_H_


namespace sqlite
{


namespace detail
{
	// types to represent special manipulators for use with stream operators
	struct null_t { };
	struct exec_t { };
	struct set_index_t { unsigned int _index; };
}


/**
 * Stream manipulator.  When used with a statment's stream operator, this
 * specifies a NULL value to bind to a parameter.  When used with a row's stream
 * operator, no value is fetched for a column.
 */
extern detail::null_t null;

/**
 * Stream manipulator.  When used with a statment's stream operator, this
 * indicates that the statement should be executed immediately.  Unlike a
 * statment's exec() method, this will throw on error.  Also, it will throw if
 * the execution returns any result rows.  When used with a query's stream
 * operator, behaviour is the same except that it will not throw when the
 * execution returns results.
 */
extern detail::exec_t exec;

/**
 * Stream manipulator.  When used with a statment's or a row's stream operator,
 * this manipulator sets the index used to automatically bind values or get
 * column values.
 *
 * @param index the new index
 */
detail::set_index_t set_index(
	unsigned int index );


} // namespace sqlite


#endif /* SQLITE3CC_MANIPULATOR_H_ */
