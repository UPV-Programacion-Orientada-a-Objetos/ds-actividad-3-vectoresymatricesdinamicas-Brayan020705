#include <iostream>
#include <iomanip>
#include <new>
#include <limits>

using namespace std;

class SistemaPlanificacionCOGS {
private:
    static const int NUMERO_PRODUCTOS = 5;
    static const int NUMERO_MATERIAS_PRIMAS = 4;
    static const int SEMANAS_INICIALES = 4;

    float** matrizPlanificacionProduccion;
    int numeroSemanasActuales;

    float* costosUnitariosPorProducto;
    int capacidadVectorCostos;

    float* inventarioMateriaPrima;
    int capacidadVectorInventario;

    int requerimientosMateriaPrimaPorProducto[NUMERO_PRODUCTOS][NUMERO_MATERIAS_PRIMAS];

    float costosAdquisicionMateriaPrima[NUMERO_MATERIAS_PRIMAS];

    float valorInventarioInicial;
    float totalComprasRealizadas;

public:
    SistemaPlanificacionCOGS() : matrizPlanificacionProduccion(nullptr), numeroSemanasActuales(0),
                                 costosUnitariosPorProducto(nullptr), capacidadVectorCostos(0),
                                 inventarioMateriaPrima(nullptr), capacidadVectorInventario(0),
                                 valorInventarioInicial(0.0f), totalComprasRealizadas(0.0f) {
        inicializarSistemaCompleto();
    }

    ~SistemaPlanificacionCOGS() {
        liberarMemoriaAsignada();
    }

    void inicializarSistemaCompleto() {
        cout << "\nSimulacion de Interaccion" << endl;
        cout << "Sistema de Planificacion y Costos (COGS)\n" << endl;

        int requerimientosTemporales[NUMERO_PRODUCTOS][NUMERO_MATERIAS_PRIMAS] = {
            {10, 5, 3, 2},
            {8, 7, 4, 3},
            {12, 6, 2, 4},
            {9, 8, 5, 1},
            {11, 4, 6, 3}
        };

        for (int indiceFila = 0; indiceFila < NUMERO_PRODUCTOS; ++indiceFila) {
            for (int indiceColumna = 0; indiceColumna < NUMERO_MATERIAS_PRIMAS; ++indiceColumna) {
                requerimientosMateriaPrimaPorProducto[indiceFila][indiceColumna] = requerimientosTemporales[indiceFila][indiceColumna];
            }
        }

        costosAdquisicionMateriaPrima[0] = 15.50f;
        costosAdquisicionMateriaPrima[1] = 22.75f;
        costosAdquisicionMateriaPrima[2] = 18.20f;
        costosAdquisicionMateriaPrima[3] = 12.90f;

        try {
            capacidadVectorCostos = NUMERO_PRODUCTOS;
            costosUnitariosPorProducto = new float[capacidadVectorCostos];

            costosUnitariosPorProducto[0] = 450.00f;
            costosUnitariosPorProducto[1] = 380.00f;
            costosUnitariosPorProducto[2] = 520.00f;
            costosUnitariosPorProducto[3] = 410.00f;
            costosUnitariosPorProducto[4] = 490.00f;

        } catch (const bad_alloc& excepcion) {
            cerr << "ERROR: No se pudo asignar memoria para Costos Unitarios" << endl;
            cerr << "Detalles: " << excepcion.what() << endl;
            throw;
        }

        try {
            capacidadVectorInventario = NUMERO_MATERIAS_PRIMAS;
            inventarioMateriaPrima = new float[capacidadVectorInventario];

            inventarioMateriaPrima[0] = 5000.0f;
            inventarioMateriaPrima[1] = 3500.0f;
            inventarioMateriaPrima[2] = 4200.0f;
            inventarioMateriaPrima[3] = 2800.0f;

            valorInventarioInicial = 0.0f;
            for (int indice = 0; indice < NUMERO_MATERIAS_PRIMAS; ++indice) {
                valorInventarioInicial += inventarioMateriaPrima[indice] * costosAdquisicionMateriaPrima[indice];
            }

        } catch (const bad_alloc& excepcion) {
            cerr << "ERROR: No se pudo asignar memoria para Inventario MP" << endl;
            cerr << "Detalles: " << excepcion.what() << endl;
            delete[] costosUnitariosPorProducto;
            throw;
        }

        try {
            numeroSemanasActuales = SEMANAS_INICIALES;
            matrizPlanificacionProduccion = new float*[NUMERO_PRODUCTOS];

            for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
                matrizPlanificacionProduccion[indiceProducto] = new float[numeroSemanasActuales];

                for (int indiceSemana = 0; indiceSemana < numeroSemanasActuales; ++indiceSemana) {
                    matrizPlanificacionProduccion[indiceProducto][indiceSemana] = 0.0f;
                }
            }

            cargarDatosInicialesPlanificacion();

            cout << "Inicializacion exitosa. Matriz de Planificacion: " << NUMERO_PRODUCTOS
                 << " Productos x " << numeroSemanasActuales << " Semanas.\n" << endl;

        } catch (const bad_alloc& excepcion) {
            cerr << "ERROR: No se pudo asignar memoria para Matriz de Produccion" << endl;
            cerr << "Detalles: " << excepcion.what() << endl;
            delete[] costosUnitariosPorProducto;
            delete[] inventarioMateriaPrima;
            throw;
        }
    }

    void cargarDatosInicialesPlanificacion() {
        float datosProduccionInicial[NUMERO_PRODUCTOS][SEMANAS_INICIALES] = {
            {100, 120, 110, 130},
            {80, 90, 85, 95},
            {60, 70, 65, 75},
            {90, 100, 95, 105},
            {70, 80, 75, 85}
        };

        for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
            for (int indiceSemana = 0; indiceSemana < SEMANAS_INICIALES; ++indiceSemana) {
                matrizPlanificacionProduccion[indiceProducto][indiceSemana] = datosProduccionInicial[indiceProducto][indiceSemana];
            }
        }
    }

    void liberarMemoriaAsignada() {
        if (matrizPlanificacionProduccion != nullptr) {
            for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
                delete[] matrizPlanificacionProduccion[indiceProducto];
            }
            delete[] matrizPlanificacionProduccion;
            matrizPlanificacionProduccion = nullptr;
        }

        if (costosUnitariosPorProducto != nullptr) {
            delete[] costosUnitariosPorProducto;
            costosUnitariosPorProducto = nullptr;
        }

        if (inventarioMateriaPrima != nullptr) {
            delete[] inventarioMateriaPrima;
            inventarioMateriaPrima = nullptr;
        }
    }

    void agregarNuevaSemanaPlanificacion() {
        cout << "\nOpcion seleccionada: 2" << endl;
        cout << "Agregando Semana " << (numeroSemanasActuales + 1) << endl;

        float** matrizNuevaExpandida = nullptr;

        try {
            int numeroColumnasNuevas = numeroSemanasActuales + 1;
            matrizNuevaExpandida = new float*[NUMERO_PRODUCTOS];

            for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
                matrizNuevaExpandida[indiceProducto] = new float[numeroColumnasNuevas];

                for (int indiceSemana = 0; indiceSemana < numeroSemanasActuales; ++indiceSemana) {
                    matrizNuevaExpandida[indiceProducto][indiceSemana] = matrizPlanificacionProduccion[indiceProducto][indiceSemana];
                }

                matrizNuevaExpandida[indiceProducto][numeroSemanasActuales] = 0.0f;
            }

            for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
                delete[] matrizPlanificacionProduccion[indiceProducto];
            }
            delete[] matrizPlanificacionProduccion;

            matrizPlanificacionProduccion = matrizNuevaExpandida;
            numeroSemanasActuales = numeroColumnasNuevas;

            cout << "Matriz redimensionada a " << NUMERO_PRODUCTOS << "x" << numeroSemanasActuales
                 << ". Memoria gestionada con exito!" << endl;

        } catch (const bad_alloc& excepcion) {
            cerr << "ERROR: No se pudo expandir la matriz de planificacion" << endl;
            cerr << "Detalles: " << excepcion.what() << endl;

            if (matrizNuevaExpandida != nullptr) {
                for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
                    if (matrizNuevaExpandida[indiceProducto] != nullptr) {
                        delete[] matrizNuevaExpandida[indiceProducto];
                    }
                }
                delete[] matrizNuevaExpandida;
            }

            throw;
        }
    }

    void modificarPlanificacionProduccion() {
        int numeroProductoSeleccionado, numeroSemanaSeleccionada;
        float cantidadProducir;

        cout << "\nOpcion seleccionada: 3" << endl;
        cout << "Ingrese Producto (0-4): ";
        while (!(cin >> numeroProductoSeleccionado) || numeroProductoSeleccionado < 0 || numeroProductoSeleccionado >= NUMERO_PRODUCTOS) {
            cout << "Entrada invalida. Ingrese un numero entre 0 y " << (NUMERO_PRODUCTOS - 1) << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Ingrese Semana (0-" << (numeroSemanasActuales - 1) << "): ";
        while (!(cin >> numeroSemanaSeleccionada) || numeroSemanaSeleccionada < 0 || numeroSemanaSeleccionada >= numeroSemanasActuales) {
            cout << "Entrada invalida. Ingrese un numero entre 0 y " << (numeroSemanasActuales - 1) << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Cantidad a producir: ";
        while (!(cin >> cantidadProducir) || cantidadProducir < 0) {
            cout << "Entrada invalida. Ingrese una cantidad >= 0: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        matrizPlanificacionProduccion[numeroProductoSeleccionado][numeroSemanaSeleccionada] = cantidadProducir;

        cout << "Celda actualizada exitosamente!" << endl;
    }

    void calcularConsumoMateriaPrima(int numeroSemana) {
        if (numeroSemana < 1 || numeroSemana > numeroSemanasActuales) {
            cout << "Semana invalida" << endl;
            return;
        }

        cout << "\nOpcion seleccionada: 4" << endl;
        cout << "Ingrese Semana para el calculo de costos: " << numeroSemana << endl;

        float consumoTotalPorMateriaPrima[NUMERO_MATERIAS_PRIMAS] = {0.0f};

        int indiceSemanaArray = numeroSemana - 1;
        for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
            float unidadesAProducir = matrizPlanificacionProduccion[indiceProducto][indiceSemanaArray];

            for (int indiceMateriaPrima = 0; indiceMateriaPrima < NUMERO_MATERIAS_PRIMAS; ++indiceMateriaPrima) {
                consumoTotalPorMateriaPrima[indiceMateriaPrima] += unidadesAProducir * requerimientosMateriaPrimaPorProducto[indiceProducto][indiceMateriaPrima];
            }
        }

        float produccionTotal = 0.0f;
        for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
            produccionTotal += matrizPlanificacionProduccion[indiceProducto][indiceSemanaArray];
        }

        cout << "\nReporte de Costos (Semana " << numeroSemana << ")" << endl;
        cout << "Produccion Total (Semana " << numeroSemana << "): " << (int)produccionTotal << " unidades." << endl;

        float costoTotalProduccion = 0.0f;
        for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
            costoTotalProduccion += matrizPlanificacionProduccion[indiceProducto][indiceSemanaArray] * costosUnitariosPorProducto[indiceProducto];
        }

        cout << "Costo Total de Produccion (COGS): $" << fixed << setprecision(2) << costoTotalProduccion << endl;

        cout << "Consumo de Materia Prima: ";
        for (int i = 0; i < NUMERO_MATERIAS_PRIMAS; ++i) {
            cout << "MP0" << (i + 1) << " (" << (int)consumoTotalPorMateriaPrima[i] << " kg)";
            if (i < NUMERO_MATERIAS_PRIMAS - 1) cout << ", ";
        }
        cout << "." << endl;

        float valorInventarioFinal = 0.0f;
        for (int i = 0; i < NUMERO_MATERIAS_PRIMAS; ++i) {
            float restante = inventarioMateriaPrima[i] - consumoTotalPorMateriaPrima[i];
            if (restante < 0) restante = 0;
            valorInventarioFinal += restante * costosAdquisicionMateriaPrima[i];
        }

        cout << "Valor del Inventario Final (M.P. restante): $" << fixed << setprecision(2) << valorInventarioFinal << endl;
    }

    void calcularCostoVentasMercaderiasVendidas(int numeroSemana) {
        calcularConsumoMateriaPrima(numeroSemana);
    }

    void mostrarMatrizPlanificacionProduccion() {
        cout << "\nOpcion seleccionada: 1" << endl;
        cout << "Menu Principal" << endl;
        cout << "1. Ver Plan de Produccion" << endl;
        cout << "2. Agregar Nueva Semana (Redimensionar Matriz)" << endl;
        cout << "3. Modificar Produccion" << endl;
        cout << "4. Calcular COGS y Final Inventory" << endl;
        cout << "5. Salir\n" << endl;

        cout << "Plan de Produccion (Unidades por Semana):" << endl;
        cout << "Producto  ";
        for (int indiceSemana = 0; indiceSemana < numeroSemanasActuales; ++indiceSemana) {
            cout << "S" << (indiceSemana + 1) << "    ";
        }
        cout << endl;

        for (int indiceProducto = 0; indiceProducto < NUMERO_PRODUCTOS; ++indiceProducto) {
            cout << "P" << (indiceProducto + 1) << "        ";
            for (int indiceSemana = 0; indiceSemana < numeroSemanasActuales; ++indiceSemana) {
                cout << fixed << setprecision(0) << matrizPlanificacionProduccion[indiceProducto][indiceSemana] << "   ";
            }
            cout << endl;
        }
    }

    void mostrarRequerimientosMateriaPrima() {
        cout << "\nRequerimientos de Materia Prima por Producto" << endl;
    }

    void mostrarInventarioActualMateriaPrima() {
        cout << "\nInventario Actual de Materia Prima" << endl;
    }

    void ejecutarSistema() {
        int opcionSeleccionada;

        do {
            cout << "\nMenu Principal" << endl;
            cout << "1. Ver Plan de Produccion" << endl;
            cout << "2. Agregar Nueva Semana (Redimensionar Matriz)" << endl;
            cout << "3. Modificar Produccion" << endl;
            cout << "4. Calcular COGS y Final Inventory" << endl;
            cout << "5. Salir" << endl;
            cout << "\nSeleccione una opcion: ";

            while (!(cin >> opcionSeleccionada)) {
                cout << "Entrada invalida. Ingrese un numero: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            try {
                switch (opcionSeleccionada) {
                    case 1:
                        mostrarMatrizPlanificacionProduccion();
                        break;

                    case 2:
                        agregarNuevaSemanaPlanificacion();
                        break;

                    case 3:
                        modificarPlanificacionProduccion();
                        break;

                    case 4: {
                        int numeroSemanaCalcularCOGS;
                        cout << "\nOpcion seleccionada: 4" << endl;
                        cout << "Ingrese Semana para el calculo de costos: ";
                        cin >> numeroSemanaCalcularCOGS;
                        calcularCostoVentasMercaderiasVendidas(numeroSemanaCalcularCOGS);
                        break;
                    }

                    case 5:
                        cout << "\nOpcion seleccionada: 5" << endl;
                        cout << "Liberando memoria de Matriz y Vectores Dinamicos" << endl;
                        cout << "Sistema cerrado." << endl;
                        break;

                    default:
                        cout << "\nOpcion invalida. Intente nuevamente." << endl;
                }

            } catch (const bad_alloc& excepcion) {
                cerr << "\nERROR CRITICO DE MEMORIA: " << excepcion.what() << endl;
                opcionSeleccionada = 5;
            } catch (const exception& excepcion) {
                cerr << "\nERROR: " << excepcion.what() << endl;
            }

        } while (opcionSeleccionada != 5);
    }
};

int main() {
    try {
        SistemaPlanificacionCOGS sistemaProduccion;
        sistemaProduccion.ejecutarSistema();

    } catch (const bad_alloc& excepcion) {
        cerr << "\nERROR FATAL: No se pudo inicializar el sistema" << endl;
        cerr << "Detalles: " << excepcion.what() << endl;
        return 1;

    } catch (const exception& excepcion) {
        cerr << "\nERROR FATAL: " << excepcion.what() << endl;
        return 1;
    }

    return 0;
}
