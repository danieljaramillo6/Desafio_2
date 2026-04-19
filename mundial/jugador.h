#ifndef JUGADOR_H
#define JUGADOR_H

class Jugador {
private:
    char* nombre;
    char* apellido;
    int dorsal;

    int goles;
    int minutosJugados;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

public:
    // Constructor por defecto
    Jugador();

    // Constructor con parámetros
    Jugador(const char* nombre, const char* apellido, int dorsal);

    // Constructor de copia
    Jugador(const Jugador& otro);

    // Destructor
    ~Jugador();

    // Métodos (acciones)
    Jugador& operator++();
    Jugador& operator++(int);
    void recibirAmarilla();
    void recibirRoja();
    void cometerFalta();
    void agregarMinutos(int minutos);

    // Getters
    int getGoles() const;
    int getMinutos() const;
    int getDorsal() const;
    const char* getNombre() const;
    const char* getApellido() const;

    Jugador& operator=(const Jugador& otra);
};
#endif // JUGADOR_H
