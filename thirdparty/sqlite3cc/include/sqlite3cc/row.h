/*
 * row.h
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

#ifndef SQLITE3CC_ROW_H_
#define SQLITE3CC_ROW_H_


#include <sqlite3.h>
#include <boost/utility.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/utility/value_init.hpp>
#include <cassert>
#include <iostream>


namespace sqlite
{


class query;
namespace detail {
	struct null_t;
	struct set_index_t;
}


/**
 * A result row from a query.
 *
 * The row is only valid until the next call to step() or reset() on the parent
 * query object, or until the parent query object is destructed.  This may
 * change in future versions.
 */
class row
{
//______________________________________________________________________________
//                                                                 instantiation
protected:

	friend class query;

	/**
	 * Constructor that produces a valid row.
	 *
	 * @param handle of the statement (query) that created this row
	 * @oaram row_number the index of this row
	 */
	explicit row(
		sqlite3_stmt *handle,
		unsigned long long row_number );

	/**
	 * Constructor that produces an invalid row.
	 */
	explicit row();

//______________________________________________________________________________
//                                                              public interface
public:

	/**
	 * Determine if this row is valid or not.  If it is not valid, there are no
	 * more rows in the results of the query.
	 */
	operator bool() const;

	/**
	 * Get the index in to the results that is this row.
	 *
	 * @return index
	 */
	inline unsigned long long row_number()
	{
		return _row_number;
	}

	/**
	 * Ascertain a column's type.
	 *
	 * @param index column index
	 * @return sqlite datatype code
	 * @see sqlite3_column_type()
	 */
	int column_type(
		unsigned int index );

	/**
	 * Get the number of bytes in the result for a given column.
	 *
	 * @param index column index
	 * @return number of bytes in result
	 * @see sqlite3_column_bytes()
	 */
	unsigned int column_bytes(
		unsigned int index );

	/**
	 * Get a value from the row.
	 *
	 * @param index column index
	 * @param value reference to object to set with the value
	 * @see sqlite3_column_*()
	 */
	template< class T >
	void column(
		unsigned int index,
		T &value )
	{
		assert( index <
			static_cast< unsigned int >( sqlite3_column_count( _handle ) ) );
		const char *text = reinterpret_cast< const char * >(
			sqlite3_column_text( _handle, index ) );
		if( text )
			value = boost::lexical_cast< T >( text );
		else
			value = boost::get( boost::value_initialized< T >() );
	}

	/**
	 * Get a value from the row and return it.
	 *
	 * @param index column index
	 * @return the value
	 * @see sqlite3_column_*()
	 */
	template< class T >
	T column(
		unsigned int index )
	{
		T value;
		column( index, value );
		return value;
	}

	/**
	 * Stream operator is used to obtain values from a result row, fetching from
	 * each column in turn.  In addition, the null and set_index() auto-column-
	 * getting manipulators can be used.
	 *
	 * @param value is a variable to store the retrieved data in
	 */
	template< class T >
	row &operator >>(
		T &value )
	{
		column( _column_index, value );
		_column_index++;
		return *this;
	}

	/**
	 * Stream operator for use with set_index().
	 */
	row &operator >>(
		detail::set_index_t t );

	/**
	 * Test to see if two rows are the same.
	 *
	 * @param other the row to compare this one to
	 * @return true if they are
	 */
	bool operator ==(
		const row &other )
		const;

//______________________________________________________________________________
//                                                                implementation
protected:

	/** the query's handle, or NULL */
	sqlite3_stmt *_handle;

private:

	/** index used when auto-column-getting */
	unsigned int _column_index;

	/** the index of this row */
	unsigned long long _row_number;

};


// template specialisations
template< >
row &row::operator >> < detail::null_t >(
	detail::null_t & );


} // namespace sqlite


#endif /* SQLITE3CC_ROW_H_ */
