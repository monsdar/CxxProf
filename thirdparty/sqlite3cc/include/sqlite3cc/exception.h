/*
 * exception.h
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

#ifndef SQLITE3CC_EXCEPTION_H_
#define SQLITE3CC_EXCEPTION_H_


#include <sqlite3.h>
#include <stdexcept>


namespace sqlite
{


class connection;


/**
 * Main (base) sqlite exception class.
 */
class sqlite_error
	:
	public std::exception
{
//______________________________________________________________________________
//                                                                 instantiation
public:

	/**
	 * Constructor that takes an sqlite result code and a connection from which
	 * to determine it's error message.
	 *
	 * @param connection a reference to a connection
	 * @param code the sqlite result code
	 * @see sqlite_errmsg()
	 */
	explicit sqlite_error(
		connection &connection,
		int code );


	/**
	 * Constructor that takes an sqlite result code and determines it's own
	 * generic error message.
	 *
	 * @param code the sqlite result code
	 */
	explicit sqlite_error(
		int code );

	/**
	 * Constructor that allows the creation of an sqlite result with a custom
	 * message.
	 *
	 * @param message a customer error message string
	 * @param code the sqlite result code
	 */
	explicit sqlite_error(
		const std::string &message,
		int code = SQLITE_ERROR );

	virtual ~sqlite_error() throw( );

//______________________________________________________________________________
//                                                              public interface
public:

	/**
	 * Get the sqlite result code associated with this error.
	 *
	 * @returns the sqlite result code
	 */
	int get_code() const;

	/**
	 * Get the error message
	 * @returns the error message
	 */
	virtual const char* what() const throw( );

//______________________________________________________________________________
//                                                                implementation
private:

	/**
	 * Retrieve an automatic error message for a given sqlite result code.
	 *
	 * @param the sqlite result code
	 * @returns the automatic string
	 */
	const std::string &get_message(
		int code );

	/** the result code */
	int _code;

	/** the message string */
	std::string _message;

};


} // namespace sqlite


#endif /* SQLITE3CC_EXCEPTION_H_ */
