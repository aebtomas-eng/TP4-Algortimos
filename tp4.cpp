#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

#include <cmath>
#include "graphClass.h"

using namespace std;

// Vértices: nombre de la ciudad, con su provincia como dato.
// Aristas: distancia aproximada en km. Cada ruta está en los dos sentidos.
using GrafoCiudades = Graph<string, int>;

// ================================================================
//                     CARGA DEL GRAFO (ya dada)
// ================================================================

static string strip(const string& s) {
    const char* ws = " \t\r\n";
    size_t b = s.find_first_not_of(ws);
    if (b == string::npos) return "";
    size_t e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

static vector<string> split(const string& l) {
    vector<string> campos;
    stringstream ss(l);
    string campo;
    while (getline(ss, campo, ',')) campos.push_back(strip(campo));
    return campos;
}

static bool cargarCiudades(GrafoCiudades& g, const string& path) {
    ifstream file(path);
    if (!file) {
        cerr << "No se pudo abrir el archivo: " << path << "\n";
        return false;
    }
    string l;
    getline(file, l);  // encabezado: ciudad,provincia,lat,lon
    while (getline(file, l)) {
        l = strip(l);
        if (l.empty()) continue;
        vector<string> c = split(l);
        g.add_vertex(c[0], c[1]);
    }
    return true;
}

static bool cargarRutas(GrafoCiudades& g, const string& path) {
    ifstream file(path);
    if (!file) {
        cerr << "No se pudo abrir el archivo: " << path << "\n";
        return false;
    }
    string l;
    getline(file, l);  // encabezado: ciudad1,ciudad2,distancia_km
    while (getline(file, l)) {
        l = strip(l);
        if (l.empty()) continue;
        vector<string> c = split(l);
        int distancia = stoi(c[2]);
        g.add_edge(c[0], c[1], distancia);
        g.add_edge(c[1], c[0], distancia);
    }
    return true;
}

// Posición de la ciudad en g.get_vertices() (-1 si no está).
int indiceDe(const GrafoCiudades& g, const string& ciudad) {
    vector<string> ciudades = g.get_vertices();
    for (int i = 0; i < (int)ciudades.size(); i++) {
        if (ciudades[i] == ciudad) return i;
    }
    return -1;
}

// ================================================================
//                  EJERCICIO 1: COMPONENTES CONEXAS
// ================================================================

vector<vector<string>> componentesConexas(const GrafoCiudades& g) {
    // TODO: implementar
    vector<string> vertices = g.get_vertices();
    size_t cant = vertices.size();
    int cant_nodos = static_cast<int>(cant);
    vector<bool> visitados(cant_nodos,false);
    vector<vector<string>> comp_conexas;

    std::unordered_map<std::string, int> indices; //dict para convertir str en el indice
    for (int i = 0; i < cant_nodos; i++)
    {
        indices[vertices[i]] = i; 
    }

    for (int i = 0; i < cant_nodos; i++)
    {
    if (!visitados[i])
    {
    vector<string> conexion;
    // bfs
    queue<size_t> cola;   
    cola.push(i);
    visitados[i]=true;
    while (!cola.empty())
    {
        size_t elem = cola.front();
        int elemento = static_cast<int>(elem);
        cola.pop();
        conexion.push_back(vertices[elemento]);
        vector<string> vecinos = g.get_neighbors(vertices[elemento]);
        for (size_t j = 0; j < vecinos.size(); j++)
        {  
            int indice = indices[vecinos[j]];
            if (!visitados[indice])
            {
                visitados[indice] = true;
                cola.push(indice);
            }
        }
    }
    comp_conexas.push_back(conexion);
    }   
    }
    return comp_conexas;
}

// ================================================================
//        EJERCICIO 2: CAMINO MÍNIMO CON PESOS DE TODOS CON TODOS
// ================================================================

const int INFINITO = 1000000000;

// dist[i][j]: distancia mínima en km entre la ciudad i y la ciudad j, con
// las ciudades numeradas en el orden de g.get_vertices() (INFINITO si no hay
// camino).
vector<vector<int>> distanciasTodosConTodos(const GrafoCiudades& g) {
    // TODO: implementar
    vector<vector<int>> distancias_todos;
    vector<string> vertices = g.get_vertices();
    size_t cant = vertices.size();
    int n = static_cast<int>(cant);
    std::unordered_map<std::string, int> indices; //dict para convertir str en el indice
    for (int i = 0; i < n; i++)
    {
        indices[vertices[i]] = i; 
    }
    
for (int i = 0; i < n; i++)
{    
    vector<int> distancia(n,INFINITO);
    vector<bool> procesados(n,false);
    // pair<int, int> a; asi se hace un par
    // a.first; así se accede
    // a.second;
    priority_queue<pair<int,int>> cola;
    distancia[i] = 0;
    cola.push({0, i});
    while (!cola.empty())
    {
        int d = -cola.top().first; //hago menos pq la cola es max entonces revierto lo invertido
        int indice = cola.top().second;
        cola.pop();
        if (!procesados[indice])
        {
            procesados[indice]=true;
            vector<string> vecinos = g.get_neighbors(vertices[indice]);
            for (size_t j = 0; j < vecinos.size(); j++)
            {
                int p = g.get_edge_data(vertices[indice], vecinos[j]).value();
                int y = indices[vecinos[j]];
                if (d + p < distancia[y])
                {
                    distancia[y] = d + p;
                    cola.push({-distancia[y],y}); //le inserto -distancia pq invierto el orden de prioridad
                }                                 //las distancias mas chicas estan mas cerca del 0 que los mas grandes
            }
        } 
    }
    distancias_todos.push_back(distancia);
}
    return distancias_todos;
}

// ================================================================
//                     EJERCICIO 3: TRIÁNGULOS
// ================================================================

int contarTriangulos(const GrafoCiudades& g) {
    // TODO: implementar
    return 0;
}

// ================================================================
//                EJERCICIO 4: DIÁMETRO CON PESOS
// ================================================================

class Diametro {
public:
    int distancia;
    string ciudad1;
    string ciudad2;
};

Diametro diametro(const GrafoCiudades& g, const vector<vector<int>>& dist) {
    // TODO: implementar
    Diametro d;
    vector<string> vertices = g.get_vertices();
    size_t c1 = 0;
    size_t c2 = 0;
    int distmax = 0;
    for (size_t i = 0; i < dist.size(); i++){
    for (size_t j = i + 1; j < dist[i].size(); j++)
    {
        int distancia = dist[i][j];
        if (distancia>distmax && distancia<INFINITO) //hay que asegurarse que no sea uno que no puede llegar
        {                                            // por eso distancia<inf
            c1 = i;
            c2 = j;
            distmax = dist[i][j];
        }
    }
    }
    d.ciudad1 = vertices[c1];
    d.ciudad2 = vertices[c2];
    d.distancia = distmax;
    return d;
}

// ================================================================
//          EJERCICIO 5: RANDOM WALKS Y LA CIUDAD MÁS CENTRAL
// ================================================================

// visitas[c]: cantidad de veces que los random walks pasaron por c.
using Visitas = unordered_map<string, int>;

Visitas randomWalks(const GrafoCiudades& g, int cantidad, int largo) {
    // TODO: implementar
    vector<string> vertices = g.get_vertices();
    Visitas contador;
    size_t n = vertices.size();
    int cant = static_cast<int>(n);
    random_device ran;
    mt19937 gen(ran());
    uniform_int_distribution<int> opciones(0, cant - 1);
    for (int i = 0; i < cantidad; i++)
    {
    int aleatorio = static_cast<int>(opciones(gen));
    string nodo = vertices[aleatorio];
    contador[nodo]++;
    vector<string> vecinos = g.get_neighbors(nodo);
    for (int j = 0; j < largo; j++)
    {
    size_t cant_veci = vecinos.size();
    int cant_vecinos = static_cast<int>(cant_veci);
    if (cant_veci == 0)
    {
        break;
    }
    uniform_int_distribution<int> opcionesvecinos(0, cant_vecinos - 1);
    aleatorio = static_cast<int>(opcionesvecinos(gen));
    int elegido = aleatorio % cant_vecinos;
    nodo = vecinos[elegido];
    contador[nodo]++;
    vecinos = g.get_neighbors(nodo);
    }
    }
    return contador;
}

Visitas randomWalksPorDistancia(const GrafoCiudades& g, int cantidad, int largo) {
    // TODO: implementar
    vector<string> vertices = g.get_vertices();
    Visitas contador;
    size_t n = vertices.size();
    int cant = static_cast<int>(n);
    random_device ran;
    mt19937 gen(ran());
    uniform_int_distribution<int> opciones(0, cant - 1);
    for (int i = 0; i < cantidad; i++)
    {
    int aleatorio = static_cast<int>(opciones(gen));
    string nodo = vertices[aleatorio];
    contador[nodo]++;
    vector<string> vecinos = g.get_neighbors(nodo);
    for (int j = 0; j < largo; j++)
    {
    size_t cant_veci = vecinos.size();
    int cant_vecinos = static_cast<int>(cant_veci);
    if (cant_veci == 0)
    {
        break;
    }
    vector<pair<float,string>> posibilidades;
    float tope = 0;
    for (int w = 0; w < cant_vecinos; w++)
    {
        int  d = g.get_edge_data(nodo,vecinos[w]).value();
        float distancia = static_cast<float>(d);
        float rango = tope + 100/distancia;
        posibilidades.push_back({rango, vecinos[w]});
        tope = tope + 100/distancia; 
    }
    float aleatorio2 = fabs( static_cast<float>(gen()));
    aleatorio2 = fmodf(aleatorio2, tope); //incluí la librería <cmath> pq es un asco operar con floats en C++ plano
    
    int mas_cerca = 0;
    for (int p = 0; p < cant_vecinos; p++)
    {
        if (aleatorio2<posibilidades[p].first)
        {
            mas_cerca = p;
            break;
        }
    }
    nodo = posibilidades[mas_cerca].second;
    contador[nodo]++;
    vecinos = g.get_neighbors(nodo);
    }
    }
    return contador;
}

// ================================================================
//                   EJERCICIO 6: ANÁLISIS LIBRE
// ================================================================

void analisisLibre(const GrafoCiudades& g) {
    // TODO: implementar
    return;
}

// ================================================================
//                              MAIN
// ================================================================

int main(int argc, char* argv[]) {
    string pathCiudades = argc > 1 ? argv[1] : "ciudades_argentina.csv";
    string pathRutas = argc > 2 ? argv[2] : "grafo_ciudades.csv";

    GrafoCiudades g;
    if (!cargarCiudades(g, pathCiudades)) return 1;
    if (!cargarRutas(g, pathRutas)) return 1;
    cout << "Ciudades cargadas: " << g.get_vertices().size() << "\n\n";

    cout << "=== Ejercicio 1 ===\n";
    vector<vector<string>> componentes = componentesConexas(g);
    cout << "Componentes conexas: " << componentes.size() << "\n";
    // TODO: tamaño de la más grande y ciudades que quedan fuera de ella

    cout << "\n=== Ejercicio 2 ===\n";
    vector<vector<int>> dist = distanciasTodosConTodos(g);
    // TODO: distancias Buenos Aires - La Quiaca y Buenos Aires - Ushuaia

    cout << "\n=== Ejercicio 3 ===\n";
    cout << "Triángulos: " << contarTriangulos(g) << "\n";

    cout << "\n=== Ejercicio 4 ===\n";
    Diametro d = diametro(g, dist);
    cout << "Diámetro: " << d.distancia << " km, entre " << d.ciudad1 << " y "
              << d.ciudad2 << "\n";

    cout << "\n=== Ejercicio 5 ===\n";
    Visitas visitas = randomWalks(g, 10000, 100);
    Visitas visitasPorDistancia = randomWalksPorDistancia(g, 10000, 100);
    // TODO: las 10 ciudades más visitadas en cada caso, y las 10 de mayor grado

    cout << "\n=== Ejercicio 6 ===\n";
    analisisLibre(g);

    return 0;
}
