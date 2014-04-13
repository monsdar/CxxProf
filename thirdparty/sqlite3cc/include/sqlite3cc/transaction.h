/*
 * transaction.h
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

#ifndef SQLITE3CC_TRANSACTION_H_
#define SQLITE3CC_TRANSACTION_H_


#include <boost/utility.hpp>
#include <string>


namespace sqlite
{


class connection;


namespace detail
{

	/**
	 * A basic (default, deferred) transaction.
	 */
	class basic_transaction
		:
		private boost::noncopyable
	{
	//__________________________________________________________________________
	//                                                             instantiation
	public:

		/**
		 * Constructor that provides a connection upon which to act.
		 *
		 * @param connection a connection
		 */
		explicit basic_transaction(
			connection &connection );

	//__________________________________________________________________________
	//                                                          public interface
	public:

		/**
		 * Begin the transaction
		 */
		virtual void begin();

		/**
		 * Commit the transaction
		 */
		virtual void commit();

		/**
		 * Rollback the transaction
		 */
		virtual void rollback();

	//__________________________________________________________________________
	//                                                            implementation
	protected:

		/** reset any in-progress statements */
		void reset_active_queries();

		/** the connection on which to act */
		connection &_connection;

	};

} // namespace detail


////////////////////////////////////////////////////////////////////////////////


/**
 * A deferred transaction.
 */
typedef detail::basic_transaction deferred_transaction;


////////////////////////////////////////////////////////////////////////////////


/**
 * An immediate transaction.
 */
class immediate_transaction
	:
	public detail::basic_transaction
{
//______________________________________________________________________________
//                                                                 instantiation
public:

	/**
	 * Constructor that provides a connection upon which to act.
	 *
	 * @param connection a connection
	 */
	explicit immediate_transaction(
		connection &connection );

//______________________________________________________________________________
//                                                              public interface
public:

	/**
	 * Begin the transaction.
	 */
	virtual void begin();

};


////////////////////////////////////////////////////////////////////////////////


/**
 * An exclusive transaction.
 */
class exclusive_transaction
	:
	public detail::basic_transaction
{
//______________________________________________________________________________
//                                                                 instantiation
public:

	/**
	 * Constructor that provides a connection upon which to act.
	 *
	 * @param connection a connection
	 */
	explicit exclusive_transaction(
		connection &connection );

//______________________________________________________________________________
//                                                              public interface
public:

	/**
	 * Begin the transaction
	 */
	virtual void begin();

};


////////////////////////////////////////////////////////////////////////////////


/**
 * A recursive transaction, allowing transactions to be nested.
 */
class recursive_transaction
	:
	public detail::basic_transaction
{
//______________________________________________________________________________
//                                                                 instantiation
public:

	/**
	 * Constructor that provides a connection upon which to act.
	 *
	 * @param connection a connection
	 */
	explicit recursive_transaction(
		connection &connection );

//______________________________________________________________________________
//                                                              public interface
public:

	/**
	 * Begin the transaction.
	 */
	virtual void begin();

	/**
	 * Commit the transaction.
	 */
	virtual void commit();

	/**
	 * Rollback the transaction.
	 */
	virtual void rollback();

//______________________________________________________________________________
//                                                                implementation
protected:

	/* this transaction's savepoint name */
	std::string _sp_name;

};


////////////////////////////////////////////////////////////////////////////////


/**
 * A scope guard (sentinel) for use with one of the transaction classes to
 * provide RAII-style transactions.  It defaults to using deferred transactions.
 */
template< class T = deferred_transaction >
class transaction_guard
	:
	private boost::noncopyable
{
//______________________________________________________________________________
//                                                                 instantiation
public:

	/**
	 * Constructor that provides a connection upon which to act.
	 *
	 * @param connection a connection
	 */
	explicit transaction_guard(
		connection &connection )
		:
		_transaction( connection ),
		_released( true )
	{
		_transaction.begin();
		_released = false;
	}

	~transaction_guard()
	{
		if( !_released )
			_transaction.rollback();
	}

//______________________________________________________________________________
//                                                              public interface
public:

	/**
	 * Begin the transaction. Note that this is done automatically in the
	 * constructor.
	 */
	void begin()
	{
		if( _released ) {
			_transaction.begin();
			_released = false;
		}
	}

	/**
	 * Commit the transaction.
	 */
	void commit()
	{
		if( !_released ) {
			_transaction.commit();
			_released = true;
		}
	}

	/**
	 * Rollback the transaction. Note that this is done automatically in the
	 * destructor if the transaction hasn't otherwise been completed.
	 */
	void rollback()
	{
		if( !_released ) {
			_transaction.rollback();
			_released = true;
		}
	}

//______________________________________________________________________________
//                                                                implementation
protected:

	/** the transaction */
	T _transaction;

	/** have we released the transaction yet? */
	bool _released;

};


} // namespace sqlite


#endif /* SQLITE3CC_TRANSACTION_H_ */
