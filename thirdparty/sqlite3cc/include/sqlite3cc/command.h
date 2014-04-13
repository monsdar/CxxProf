/*
 * command.h
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

#ifndef SQLITE3CC_COMMAND_H_
#define SQLITE3CC_COMMAND_H_


#include <sqlite3cc/basic_statement.h>


namespace sqlite
{


/**
 * The command class represents an SQL command.  Since there is very little
 * difference between a command an a statement, it is basically a shim around
 * the statement class with the addition of an exec() method to execute the
 * command.
 */
class command
	:
	public detail::basic_statement
{
//______________________________________________________________________________
//                                                                 instantiation
public:

	/**
	 * Constructor that provides a connection upon which to act and the SQL
	 * command to execute.
	 *
	 * @param connection a reference to a connection
	 * @param sql an SQL statement in UTF-8
	 */
	explicit command(
		connection &connection,
		const std::string &sql );

	/**
	 * Constructor that provides a connection upon which to act.
	 *
	 * @param connection a reference to a connection
	 */
	explicit command(
		connection &connection );

//______________________________________________________________________________
//                                                              public interface
public:

	/**
	 * Prepare an SQL statement.
	 *
	 * @param sql an SQL statement in UTF-8
	 * @returns an sqlite error code
	 * @see sqlite3_prepare_v2()
	 */
	virtual int prepare(
		const std::string &sql );

	/**
	 * Step through one execution cycle of the SQL statement.  If this is an SQL
	 * statement that doesn't return any rows, only one cycle is required,
	 * otherwise, each cycle will return another row
	 *
	 * @return an sqlite error code
	 * @see sqlite3_step()
	 */
	int step();

	/**
	 * Execute the command.  This is the same as doing a step().
	 *
	 * @return an sqlite error code
	 * @see sqlite3_step()
	 */
	inline int exec()
	{
		return step();
	}

	/**
	 * Get the number of changes made by the last successful execution of this
	 * command.  This doesn't include changes made in trigger subcontexts.
	 *
	 * @return the number of changed rows
	 * @see sqlite3_changes()
	 */
	inline int changes()
	{
		return _changes;
	}

	/**
	 * Get the number of changes made by the last successful execution of this
	 * command.  This includes changes made in all trigger subcontexts.
	 *
	 * @return the number of changed rows
	 * @see sqlite3_total_changes()
	 */
	inline int total_changes()
	{
		return _total_changes;
	}

	/**
	 * Get the rowid of the last row inserted via a successful INSERT command
	 * (regardless of whether that is this command or not).
	 *
	 * @return the last inserted rowid
	 */
	inline long long last_insert_rowid()
	{
		return _last_insert_rowid;
	}

//______________________________________________________________________________
//                                                                implementation
private:

	/** non-recursive number of changes made by the last execution */
	int _changes;

	/** recursive number of changes made by the last execution */
	int _total_changes;

	/** the rowid of the last successful insert command */
	long long _last_insert_rowid;

};


namespace detail
{


// template specialisations for detail::basic_statement::operator <<()
template< >
basic_statement &basic_statement::operator << < detail::exec_t >(
	const detail::exec_t & );


} // namespace detail


} // namespace sqlite


#endif /* SQLITE3CC_COMMAND_H_ */
