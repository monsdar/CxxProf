/*
 * connection.h
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

#ifndef SQLITE3CC_CONNECTION_H_
#define SQLITE3CC_CONNECTION_H_


#include <sqlite3.h>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <string>


namespace sqlite
{


namespace detail {
	class basic_statement;
	class basic_transaction;
}
class command;
class query;


class connection
	:
	private boost::noncopyable
{
//______________________________________________________________________________
//                                                                         types
public:

	/**
	 * Class to provide a scope lock for the connection mutex
	 */
	class mutex_guard
	{
	public:

		/**
		 * Constructor that provides a connection upon which to act.
		 *
		 * @param connection reference to a connection
		 * @see sqlite3_db_mutex()
		 * @see sqlite3_mutex_enter()
		 */
		mutex_guard(
			connection &connection );

		~mutex_guard();

		/**
		 * Release the mutex early.
		 *
		 * @see sqlite3_mutex_leave()
		 */
		void leave();

	private:
		sqlite3_mutex *_mutex;
	};

//______________________________________________________________________________
//                                                                 instantiation
public:

	/**
	 * Constructor that specifies the filename of a database to open.
	 *
	 * @param filename the filename of the database file to open
	 * @throws database_error on error
	 */
	explicit connection(
		const std::string &filename );

	explicit connection();
	virtual ~connection();

//______________________________________________________________________________
//                                                              public interface
public:

	/**
	 * Open a database.
	 *
	 * @param filename the filename of the database file to open
	 * @param flags flags appropriate for sqlite3_open_v2()
	 * @returns an sqlite error code
	 * @see sqlite3_open_v2()
	 */
	int open(
		const std::string &filename,
		int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE );

	/**
	 * Close an open database.
	 *
	 * @throws database_error on error
	 * @see sqlite3_close()
	 */
	void close();

	/**
	 * Execute an SQL statement.  An exception is thrown on error.
	 *
	 * @param sql an SQL statement in UTF-8
	 * @see sqlite3_exc()
	 */
	void exec(
		const std::string &sql );

	/**
	 * Sets a default busy handler which will wait for the specified number of
	 * milliseconds.
	 *
	 * @param duration number of milliseconds to wait
	 * @returns an sqlite error code
	 * @see sqlite3_busy_timeout()
	 */
	int busy_timeout(
		int duration );

	/**
	 * Create a command.
	 *
	 * @param sql an SQL statement in UTF-8
	 */
	boost::shared_ptr< command > make_command(
		std::string sql );

	/**
	 * Create a query.
	 *
	 * @param sql an SQL statement in UTF-8
	 */
	boost::shared_ptr< query > make_query(
		std::string sql );

//______________________________________________________________________________
//                                                                implementation
private:

	friend class detail::basic_statement;
	friend class detail::basic_transaction;
	friend class command;
	friend class sqlite_error;

	/** the connection handle */
	sqlite3 *_handle;

};


} // namespace sqlite


#endif /* SQLITE3CC_CONNECTION_H_ */
