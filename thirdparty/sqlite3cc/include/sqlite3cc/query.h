/*
 * query.h
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

#ifndef SQLITE3CC_QUERY_H_
#define SQLITE3CC_QUERY_H_


#include <boost/iterator/iterator_facade.hpp>
#include <sqlite3cc/basic_statement.h>
#include <sqlite3cc/row.h>


namespace sqlite
{


class query
	:
	public detail::basic_statement
{
//______________________________________________________________________________
//                                                                 instantiation
public:

	/**
	 * Constructor that provides a connection upon which to act and the SQL
	 * query to execute.
	 *
	 * @param connection a reference to a connection
	 * @param sql an SQL statement in UTF-8
	 */
	explicit query(
		connection &connection,
		const std::string &sql );

	/**
	 * Constructor that provides a connection upon which to act.
	 *
	 * @param connection a reference to a connection
	 */
	explicit query(
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
	 * Reset the statement, ready to re-execute it.  This does not clear any of
	 * the values bound to the statement.
	 *
	 * @returns an sqlite error code
	 * @see sqlite3_reset()
	 */
	virtual int reset();

	/**
	 * Perform a step() and return row object that can be used to retrieve the
	 * results.
	 *
	 * @return a row object
	 */
	row step();

	/**
	 * Get the number of columns in the results.
	 *
	 * @see sqlite3_column_count()
	 */
	unsigned int column_count();

	/**
	 * Get the name of a column in the results.
	 *
	 * @param index column index
	 * @see sqlite3_column_name()
	 */
	const std::string column_name(
		unsigned int index );

	/**
	 * Gets the number of results.  Be aware that this merely step()s over the
	 * results and counts them, which is something you could do yourself.  This
	 * method is intended to be used for convenience, where you only need know
	 * the number of results and not what they are.  You could also execute some
	 * SQL like this: "SELECT COUNT(*) ...", but this only causes SQLite to
	 * internally do the same counting as this method does.  Also note that this
	 * method reset()s the query.
	 *
	 * @returns the number of results
	 */
	unsigned long long num_results();

	/**
	 * Query iterator which can be used to obtain rows.
	 */
	class iterator
		:
		public boost::iterator_facade< iterator, row,
			boost::single_pass_traversal_tag, row >
	{
	public:
		explicit iterator( query &query, bool step );

	private:
		friend class boost::iterator_core_access;

		row dereference() const;
		void increment();
		bool equal( iterator const &other ) const;

		/** the current row */
		row _row;

		/** the query */
		query &_query;
	};

	/**
	 * Get an iterator to the initial row.  Note that creating an iterator
	 * causes step() to be called, so it should only be called to begin
	 * iterating over the rows and not for comparison.
	 *
	 * @return a query iterator
	 */
	iterator begin();

	/**
	 * Get an iterator to after the last row (i.e., an invalid row).
	 *
	 * @return an invalid query iterator
	 */
	iterator end();

//______________________________________________________________________________
//                                                                implementation
private:

	/** next row number */
	unsigned long long _next_row_number;

};


} // namespace sqlite


#endif /* SQLITE3CC_QUERY_H_ */
