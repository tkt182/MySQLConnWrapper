#ifndef _MYSQLCONNWRAPPER_H_
#define _MYSQLCONNWRAPPER_H_

#include <vector>
#include <stdexcept>

#include <mysql_connection.h>
#include <mysql_driver.h>

#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>



class MySQLConnWrapper {

	public:
		MySQLConnWrapper(
			const std::string host, const std::string dbName,
			const std::string user, const std::string passwd);
		virtual ~MySQLConnWrapper();


		void manageException(sql::SQLException& e);
		void connectDb();
		void switchDb();
		void closeDb();

		void executeQuery(const std::string& sql);

		void setPrepStmt(const std::string& query);
		void executePrepStmt();

		void setInt(const int& num, const int& data);
		void setDouble(const int& num, const double& data);
		void setString(const int& num, const std::string& data);
		void setDateTime(const int& num, const std::string& data);
		void setNull(const int& num);

		void executeFetch(const std::string key, std::string& result);
		void executeFetch(const std::string key, std::vector<std::string>& results);
		void executeFetch(const std::vector<std::string>& keys, std::vector<std::string>& results);
		void executeFetch(const std::vector<std::string>& keys, std::vector< std::vector<std::string> >& results);


	private:

		std::string             _host;
		std::string             _dbName;
		std::string             _port;
		std::string             _user;
		std::string             _passwd;

		sql::Driver*            _driver;
		sql::Connection*        _con;
		sql::Statement*         _stmt;
		sql::ResultSet*         _res;
		sql::PreparedStatement* _prepStmt;

};


#endif /*_MYSQLCONNWRAPPER_H_ */
