#include "MySQLConnWrapper.h"


MySQLConnWrapper::MySQLConnWrapper(
	const std::string host, const std::string dbName,
	const std::string user, const std::string passwd){

	_host   = host;
	_dbName = dbName;
	_user   = user;
	_passwd = passwd;

}


MySQLConnWrapper::~MySQLConnWrapper(){

	if(!_res)      delete _res;
	if(!_stmt)     delete _stmt;
	if(!_prepStmt) delete _prepStmt;
	if(!_con)      delete _con;

}


/**
 * Create an error message related to SQL Exception
 */
void MySQLConnWrapper::manageException(sql::SQLException& e){

	if(e.getErrorCode() != 0){

		std::string msg = "SQL Exception : " + static_cast<std::string>(e.what());
		msg += " (MySQL error code : " + e.getErrorCode();
		msg += ", SQLState : " + e.getSQLState() + " )\n";

		std::cerr << msg << std::endl;

	}

}


/**
 * Connect to mysql server
 */
void MySQLConnWrapper::connectDb(){

	try{

		_driver = sql::mysql::get_driver_instance();
		_con    = _driver->connect(_host, _user, _passwd);

	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}


/**
 * Select a database
 */
void MySQLConnWrapper::switchDb(){

	if(!_con){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "System is not still connected to the database.";
		throw std::runtime_error(msg);
	}

	try{

		_con->setSchema(_dbName);
		_stmt = _con->createStatement();

	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}


/**
 * Disconnect to mysql server
 */
void MySQLConnWrapper::closeDb(){

	if(!_con){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "System is not still connected to the database.";
		throw std::runtime_error(msg);
	}

	_con->close();

}


/**
 * Execute a query directly
 */
void MySQLConnWrapper::executeQuery(const std::string& sql){

	if(!_stmt){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "Statement is not set.";
		throw std::runtime_error(msg);
	}

	try{
		_res = _stmt->executeQuery(sql);
	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}


/**
 * Set a prepared statement
 */
void MySQLConnWrapper::setPrepStmt(const std::string& query){

	if(!_con){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "System is not still connected to the database.";
		throw std::runtime_error(msg);
	}

	try{
		_prepStmt = _con->prepareStatement(query);
	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}


/**
 * Excecute a prepared statement
 */
void MySQLConnWrapper::executePrepStmt(){

	if(!_stmt){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "Prepared Statement is not set.";
		throw std::runtime_error(msg);
	}

	try{
		_res = _prepStmt->executeQuery();
	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}


/**
 * Fetch the result(using single key) and assign to a string variable
 */
void MySQLConnWrapper::executeFetch(const std::string key, std::string& result){

	try{

		if(_res->next()){
			result = _res->getString(key);
		}

	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}


/**
 * Fetch the results(using single key) and assign to a string vector
 */
void MySQLConnWrapper::executeFetch(const std::string key, std::vector<std::string>& results){

	try{

		while(_res->next()){
			results.push_back(_res->getString(key));
		}

	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}


/**
 * Fetch the results(using sevral keys) and assign to a string vector
 */
void MySQLConnWrapper::executeFetch(const std::vector<std::string>& keys,
	std::vector<std::string>& results){

	std::vector<std::string>::const_iterator it;

	try{
		if(_res->next()){

			for(it = keys.begin(); it != keys.end(); it++){
				results.push_back(_res->getString(*it));
			}
		}
	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}


/**
 * Fetch the results(using sevral keys) and assign to string vectors
 */
void MySQLConnWrapper::executeFetch(const std::vector<std::string>& keys,
		std::vector< std::vector<std::string> >& results){

	std::vector<std::string>::const_iterator it;

	try{
		while(_res->next()){

			std::vector<std::string> values;
			for(it = keys.begin(); it != keys.end(); it++){
				values.push_back(_res->getString(*it));
			}

			results.push_back(values);

		}

	}catch(sql::SQLException& e){

		this->manageException(e);
		throw;

	}

}



/**
 * Set a int value to a prepared statement
 */
void MySQLConnWrapper::setInt(const int& num, const int& data){

	if(!_stmt){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "Prepared Statement is not set.";
		throw std::runtime_error(msg);
	}

	_prepStmt->setInt(num, data);

}


/**
 * Set a double value to a prepared statement
 */
void MySQLConnWrapper::setDouble(const int& num, const double& data){

	if(!_stmt){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "Prepared Statement is not set.";
		throw std::runtime_error(msg);
	}

	_prepStmt->setDouble(num, data);

}


/**
 * Set a string value to a prepared statement
 */
void MySQLConnWrapper::setString(const int& num, const std::string& data){

	if(!_stmt){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "Prepared Statement is not set.";
		throw std::runtime_error(msg);
	}

	_prepStmt->setString(num, data);

}


/**
 * Set a date value (data type is string) to a prepared statement
 */
void MySQLConnWrapper::setDateTime(const int& num, const std::string& data){

	if(!_stmt){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "Prepared Statement is not set.";
		throw std::runtime_error(msg);
	}

	_prepStmt->setDateTime(num, data);

}


/**
 * Set a NULL value to a prepared statement
 */
void MySQLConnWrapper::setNull(const int& num){

	if(!_stmt){
		std::string msg = std::string(__func__) + " Failed. ";
		msg += "Prepared Statement is not set.";
		throw std::runtime_error(msg);
	}

	// The second argument of setNull is a int variable.
	// However, since this variable is not used in mysqlcppconn, set a NULL.
	_prepStmt->setNull(num, NULL);

}
