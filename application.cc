#include <iostream>
#include <sqlite3.h>

void crearTabla(sqlite3* db) {
    const char* sql = "CREATE TABLE IF NOT EXISTS Libros ("
                      "ISBN TEXT PRIMARY KEY, "
                      "Titulo TEXT NOT NULL, "
                      "Autor TEXT NOT NULL, "
                      "Disponible INTEGER NOT NULL);";
    char* errorMessage;
    if (sqlite3_exec(db, sql, nullptr, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error al crear la tabla: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}
void agregarLibro(sqlite3* db, const std::string& isbn, const std::string& titulo, const std::string& autor) {
    std::string sql = "INSERT INTO Libros (ISBN, Titulo, Autor, Disponible) VALUES ('" + isbn + "', '" + titulo + "', '" + autor + "', 1);";
    char* errorMessage;
    if (sqlite3_exec(db, sql.c_str(), nullptr, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error al agregar libro: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    } else {
        std::cout << "Libro agregado con exito." << std::endl;
    }
}


void mostrarLibros(sqlite3* db) {
    const char* sql = "SELECT * FROM Libros;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "ISBN: " << sqlite3_column_text(stmt, 0) 
                      << ", Titulo: " << sqlite3_column_text(stmt, 1) 
                      << ", Autor: " << sqlite3_column_text(stmt, 2) 
                      << ", Disponible: " << (sqlite3_column_int(stmt, 3) ? "Si" : "No") 
                      << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error al mostrar libros." << std::endl;
    }
}



int main () {
    sqlite3* db;
    if (sqlite3_open("./db/a.db", &db)) {
        std::cerr << "Error al abrir la base de datos." << std::endl; // <-- detect database.
        return 1;
    }

    crearTabla(db);
    agregarLibro(db, "123456789", "El Gran Gatsby", "F. Scott Fitzgerald");
    mostrarLibros(db);

    sqlite3_close(db);
    return 0;
}