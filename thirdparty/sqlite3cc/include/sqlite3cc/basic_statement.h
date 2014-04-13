/*
 * basic_statement.h
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

#ifndef SQLITE3CC_BASIC_STATEMENT_H_
#define SQLITE3CC_BASIC_STATEMENT_H_


#include <sqlite3.h>
#include <boost/utility.hpp>
#include <boost/lexical_cast.hpp>
#include <sqlite3cc/exception.h>


namespace sqlite
{


class connection;
class row;
namespace detail {
	struct null_t;
	struct exec_t;
	struct set_index_t;
}


namespace detail
{


/**
 * The statement class represents an SQL statement.  It is the base class for
 * both the command and the query classes, which should be used for those
 * purposes.  The basic_statement class its self has protected instantiation.
 */
class basic_statement
{
//______________________________________________________________________________
//                                                                 instantiation
protected:

	/**
	 * Constructor that provides a connection upon which to act and the SQL
	 * statement.
	 *
	 * @param connection a reference to a connection
	 * @param sql an SQL statement in UTF-8
	 */
	explicit basic_statement(
		connection &connection,
		const std::string &sql );

	/**
	 * Constructor that provides a connection upon which to act.
	 *
	 * @param connection a reference to a connection
	 */
	explicit basic_statement(
		connection &connection );

	virtual ~basic_statement();

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
	 * Reset the statement, ready to re-execute it.  This does not clear any of
	 * the values bound to the statement.
	 *
	 * @returns an sqlite error code
	 * @see sqlite3_reset()
	 */
	virtual int reset();

	/**
	 * Clears the values bound to a statement to NULL.
	 *
	 * @returns an sqlite error code
	 * @see sqlite3_clear_bindings()
	 */
	int clear_bindings();

	/**
	 * Bind a value to the SQL statement via it's index.  This template will
	 * take a variety of data types and bind them as text.  This is how sqlite
	 * internally stores the data anyway, so always binding as text just means
	 * we do the conversion instead of sqlite and is no less efficient.
	 *
	 * @param index the index of the parameter to bind to
	 * @param value the value to bind
	 * @returns an sqlite error code
	 * @see sqlite3_bind_text()
	 */
	template< class T >
	int bind(
		unsigned int index,
		T value )
	{
		std::string string_value = boost::lexical_cast< std::string >( value );
		return sqlite3_bind_text( _handle, index, string_value.c_str(),
			string_value.length(), SQLITE_TRANSIENT );
	}

	/**
	 * Bind a string value to the SQL statement via it's index where the value
	 * of that string will not change for the duration of the statement.  This
	 * is more optimal because sqlite will not have to take it's own copy of the
	 * data.
	 *
	 * @param index the index of the parameter to bind to
	 * @param value the invariant string value
	 * @param value_length the length of the string including zero-terminator
	 * @returns an sqlite error code
	 * @see sqlite3_bind_text()
	 */
	int bind_static(
		unsigned int index,
		const char *value,
		unsigned int value_length );

	/**
	 * Bind a string value to the SQL statement via it's index where the value
	 * of that string will not change for the duration of the statement.  This
	 * is more optimal because sqlite will not have to take it's own copy of the
	 * data.
	 *
	 * @param index the index of the parameter to bind to
	 * @param value the invariant string value
	 * @returns an sqlite error code
	 * @see sqlite3_bind_text()
	 */
	int bind_static(
		unsigned int index,
		const char *value );

	/**
	 * Bind a string value to the SQL statement via it's index where the value
	 * of that string will not change for the duration of the statement.  This
	 * is more optimal because sqlite will not have to take it's own copy of the
	 * data.
	 *
	 * @param index the index of the parameter to bind to
	 * @param value the invariant string value
	 * @returns an sqlite error code
	 * @see sqlite3_bind_text()
	 */
	int bind_static(
		unsigned int index,
		const std::string &value );

	/**
	 * Bind a NULL value to the SQL statement via it's index.
	 *
	 * @param index the index of the parameter to bind to
	 * @returns an sqlite error code
	 * @see sqlite3_bind_null()
	 */
	int bind_null(
		unsigned int index );

	/**
	 * Bind a value to the SQL statement via a named parameter.  This template
	 * will take a variety of data types and bind them as text.  This is how
	 * sqlite internally stores the data anyway, so always binding as text just
	 * means we do the conversion instead of sqlite and is no less efficient.
	 *
	 * @param name the named parameter to bind to
	 * @param value the value to bind
	 * @returns an sqlite error code
	 * @see sqlite3_bind_text()
	 */
	template< class T >
	int bind(
		const std::string &name,
		T value )
	{
		return bind( bind_parameter_index( name ), value );
	}

	/**
	 * Bind a string value to the SQL statement via a named parameter where the
	 * string value will not change for the duration of the statement.  This
	 * prevents sqlite from taking its own copy of the string.
	 *
	 * @param name the named parameter to bind to
	 * @param value the invariant string value
	 * @param value_length the length of the string including zero-terminator
	 * @returns an sqlite error code
	 * @see sqlite3_bind_text()
	 */
	int bind_static(
		const std::string &name,
		const char *value,
		unsigned int value_length );

	/**
	 * Bind a string value to the SQL statement via a named parameter where the
	 * string value will not change for the duration of the statement.  This
	 * prevents a copy of the string being taken.
	 *
	 * @param name the named parameter to bind to
	 * @param value the invariant string value
	 * @returns an sqlite error code
	 * @see sqlite3_bind_text()
	 */
	int bind_static(
		const std::string &name,
		const char *value );

	/**
	 * Bind a string value to the SQL statement via a named parameter where the
	 * string value will not change for the duration of the statement.  This
	 * prevents a copy of the string being taken.
	 *
	 * @param name the named parameter to bind to
	 * @param value the invariant string value
	 * @returns an sqlite error code
	 * @see sqlite3_bind_text()
	 */
	int bind_static(
		const std::string &name,
		const std::string &value );

	/**
	 * Bind a NULL value to the SQL statement via a named parameter.
	 *
	 * @param name the named parameter to bind to
	 * @returns an sqlite error code
	 * @see sqlite3_bind_null()
	 */
	int bind_null(
		const std::string &name );

	/**
	 * Stream operator is used to bind values to parameters automatically, in
	 * ascending order.  In addition, the null and set_index() auto-binding
	 * manipulators can be used.
	 *
	 * @param value a value to bind
	 */
	template< class T >
	basic_statement &operator <<(
		const T &value )
	{
		int code = bind( _bind_index, value );
		if( code != SQLITE_OK ) throw sqlite_error( _connection, code );
		_bind_index++;
		return *this;
	}

//______________________________________________________________________________
//                                                                implementation
protected:

	friend class row;

	/**
	 * Finalise an SQL statement.
	 *
	 * @returns an sqlite error code
	 * @see sqlite3_finalize()
	 */
	int finalize();

	/**
	 * Get the index number of a named parameter.
	 *
	 * @param parameter name
	 * @return index of named parameter
	 */
	int bind_parameter_index(
		const std::string &name );

	/**
	 * Perform a step.
	 *
	 * @return sqlite error code
	 * @see sqlite3_step()
	 */
	int step();

	/** the connection upon which to act */
	connection &_connection;

	/** the statement handle */
	sqlite3_stmt *_handle;

	/** index used when auto-binding */
	unsigned int _bind_index;

};


// template specialisations for basic_statement::operator <<()
template< >
basic_statement &basic_statement::operator << < detail::null_t >(
	const detail::null_t & );
template< >
basic_statement &basic_statement::operator << < detail::set_index_t >(
	const detail::set_index_t &t );


} // namespace detail


} // namespace sqlite


#endif /* SQLITE3CC_BASIC_STATEMENT_H_ */
