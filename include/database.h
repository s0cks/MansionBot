#ifndef KATARINA2_DATABASE_H
#define KATARINA2_DATABASE_H

#include <sqlite3.h>
#include <string>
#include <sstream>
#include <vector>
#include <string.h>

class TwitchUser{
private:
    std::string name;
    int points;
public:
    TwitchUser(std::string name, int points){
        this->name = name;
        this->points = points;
    }
    ~TwitchUser(){}

    int
    getPoints(){
        return this->points;
    }

    std::string
    getName(){
        return this->name;
    }

    std::string
    toString(){
        std::stringstream stream;
        stream << this->name << " has " << this->points << " points";
        return stream.str();
    }
};

typedef std::vector<TwitchUser> user_vec_t;

class Database{
private:
    sqlite3* db;
    int code;
    char* sql;
    char* errMsg;
    sqlite3_stmt *res;

    void
    createPointsTable(){
        std::stringstream stream;
        stream << "CREATE TABLE points(user TEXT, total INT);";
        this->sql = (char *) stream.str().c_str();
        this->code = sqlite3_exec(this->db, this->sql, 0, 0, &errMsg);

        if(this->code != SQLITE_OK){
            fprintf(stderr, "[createPointsTable] SQL Error: %s\n", errMsg);
            sqlite3_free(errMsg);
            sqlite3_close(this->db);
            exit(-1);
        }
    }
public:
    Database(const std::string& path){
        this->code = sqlite3_open(path.c_str(), &db);
        if(this->code){
            fprintf(stderr, "Can't open database %s\n", sqlite3_errmsg(this->db));
        }
        // this->createPointsTable();
    }
    ~Database(){
        sqlite3_close(this->db);
    }

    TwitchUser
    getUser(const std::string name){
        std::stringstream stream;
        stream << "SELECT * FROM points WHERE user = '" << name << "';";
        this->sql = (char*) stream.str().c_str();
        this->code = sqlite3_prepare_v2(this->db, this->sql, -1, &res, 0);

        if(this->code != SQLITE_OK){
            fprintf(stderr, sqlite3_errmsg(this->db));
        }

        int tPoints = -1;
        std::string tName = "<null>";
        for(;;){
            this->code = sqlite3_step(this->res);
            if(this->code == SQLITE_DONE){
                break;
            } else if(this->code != SQLITE_ROW){
                fprintf(stderr, sqlite3_errmsg(this->db));
                break;
            }
            tPoints = sqlite3_column_int(this->res, 1);
            tName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->res, 0)));
        }
        return TwitchUser(tName, tPoints);
    }

    user_vec_t
    getTop5(){
        user_vec_t ret;

        std::stringstream stream;
        stream << "SELECT * FROM points ORDER BY total DESC LIMIT 5";
        this->sql = (char*) stream.str().c_str();
        this->code = sqlite3_prepare_v2(this->db, this->sql, -1, &res, 0);

        if(this->code != SQLITE_OK){
            fprintf(stderr, sqlite3_errmsg(this->db));
        }

        int tPoints = -1;
        std::string tName = "<null>";
        for(;;){
            this->code = sqlite3_step(this->res);
            if(this->code == SQLITE_DONE){
                break;
            } else if(this->code != SQLITE_ROW){
                fprintf(stderr, sqlite3_errmsg(this->db));
                break;
            }

            tPoints = sqlite3_column_int(this->res, 1);
            tName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->res, 0)));
            ret.push_back(TwitchUser(tName, tPoints));
        }

        return ret;
    }

    user_vec_t
    getAllAboveThreshold(int threshold){
        user_vec_t ret;

        std::stringstream stream;
        stream << "SELECT * FROM points WHERE total >= " << threshold << ";";
        this->sql = (char*) stream.str().c_str();
        this->code = sqlite3_prepare_v2(this->db, this->sql, -1, &res, 0);

        if(this->code != SQLITE_OK){
            fprintf(stderr, sqlite3_errmsg(this->db));
        }

        int tPoints = -1;
        std::string tName = "<null>";
        for(;;){
            this->code = sqlite3_step(this->res);
            if(this->code == SQLITE_DONE){
                break;
            } else if(this->code != SQLITE_ROW){
                fprintf(stderr, sqlite3_errmsg(this->db));
                break;
            }

            tPoints = sqlite3_column_int(this->res, 1);
            tName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->res, 0)));
            ret.push_back(TwitchUser(tName, tPoints));
        }

        return ret;
    }

    user_vec_t
    getAll(){
        user_vec_t ret;

        std::stringstream stream;
        stream << "SELECT * FROM points";
        this->sql = (char*) stream.str().c_str();
        this->code = sqlite3_prepare_v2(this->db, this->sql, -1, &res, 0);

        if(this->code != SQLITE_OK){
            fprintf(stderr, sqlite3_errmsg(this->db));
        }

        int tPoints = -1;
        std::string tName = "<null>";
        for(;;){
            this->code = sqlite3_step(this->res);
            if(this->code == SQLITE_DONE){
                break;
            } else if(this->code != SQLITE_ROW){
                fprintf(stderr, sqlite3_errmsg(this->db));
                break;
            }

            tPoints = sqlite3_column_int(this->res, 1);
            tName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(this->res, 0)));
            ret.push_back(TwitchUser(tName, tPoints));
        }

        return ret;
    }

    bool
    createUser(const std::string name, int points){
        std::stringstream stream;
        stream << "INSERT OR IGNORE INTO points VALUES('" << name << "', " << points << ");";
        this->sql = (char*) stream.str().c_str();
        this->code = sqlite3_exec(this->db, this->sql, 0, 0, &errMsg);

        return this->code == SQLITE_OK;

    }

    void
    updatePoints(const std::string name, int newPoints){
        std::stringstream stream;
        stream << "UPDATE points SET total = " << newPoints << " WHERE user = '" << name << "';";
        this->sql = (char*) stream.str().c_str();
        this->code = sqlite3_exec(this->db, this->sql, 0, 0, &errMsg);
    }
};

#endif