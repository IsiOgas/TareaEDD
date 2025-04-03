#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Importacion de librerias. Los archivos correspondientes deben estar en el mismo directorio que el .cpp.
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
// Importamos <fstream> para poder usar ofstream en la Operacion de ASCII ART
#include <fstream>

using namespace std;

// Struct que almacena la data de una imagen.
struct Imagen{
    unsigned char* data; // Apunta los datos de la imagen.
    int width, height, channels; // Dimesiones de la imagen. Como es una imagen png, cada pixel tiene 3 canales RGB.
};

// Abrir imagen desde memoria local.
// img->data, es acceder a los atributos. Puntero img, accede a los atributos data como el ancho.
Imagen* load(const char* filename) {
    Imagen* img = new Imagen(); // Se crea una nueva estructura Imagen con el puntero img
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0); // stbi load carga la info de la imagen en la dirección correspondiente de cada uno de ancho, alto y canal.
    return img; // Devuelve el puntero.
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}
// Funcíon reflección horizontal
void operacion_1(Imagen* img){ 
    for (int y = 0; y < img->height; y++) { // Para recorrer fila
        for (int x = 0; x < img->width/2; x++){ // Se recorre la mitad de las columnas porque asi el intercambio 
            // de pixeles se realizara 1 sola vez, si se recorriera toda la fila se haria 2 veces el cambio lo cual es innecesario.

            // Fórmula para obtener la posición de un píxel en un arreglo unidimensional.
            // index = (fila * ancho + columna) * canales.
            int pixel_izquierdo = (y * img->width + x) * img->channels; // pos 0, al *channels nos aseguramos de que tambien estamos considerando los canales RGB.
            int pixel_derecho = (y * img->width + (img->width - x - 1)) * img->channels; // pos -1.

            // Para recorrer los canales.
            // c representa cada rgb. 
            for (int c = 0; c < img->channels; c++) { 
                unsigned char temporal = img->data[pixel_izquierdo + c]; // Se guarda el valor del canal + el pixel correspondiente.
                img->data[pixel_izquierdo + c] = img->data[pixel_derecho + c]; // Se asigna el canal del pixel derecho al izq.
                img->data[pixel_derecho + c] = temporal; // Se asigna el valor del pixel izq al derecho.
            }
        }
    }
    // No es necesario delete[] porque no se crea un nuevo arreglo, solo se modifica el arreglo existente.
}
// Función giro 90°
void operacion_2(Imagen* img){ 
    // Al girar una imagen el ancho se vuelve la altura y viceversa.
    int nuevo_ancho = img->height; // El nuevo ancho es igual a la altura.
    int nueva_altura = img->width;// La nueva altura es igual al ancho.
    // Se crea un nuevo arreglo para asi poder almacenar la imagen rotada.
    unsigned char* nuevo_data = new unsigned char[nuevo_ancho * nueva_altura * img->channels];
    // Recorremos todas las filas en este caso porque vamos a estar cambiando las posiciones de los pixeles (x,y) --> (-y,x). 
    for(int y=0; y < img->height; y++){
        for(int x=0; x < img->width; x++){
            // Posición del pixel original.
            int pixel_original = (y * img->width + x) *img->channels;
            // Para girar una coordenada 90°= -y,x.
            int nuevo_x = img->height - y - 1;
            int nuevo_y= x;
            // Posición del pixel al aplicarse los 90°.
            int pixel_rotado = (nuevo_y * nuevo_ancho + nuevo_x) * img->channels;
            // Se recorren los rgb
            for (int c = 0; c < img->channels; c++) {
                nuevo_data[pixel_rotado + c] = img->data[pixel_original + c];
                // En nuevo_data se guardan los valores de los pixeles rotados del nuevo arreglo.
                // En img->data se lee el rgb de la imagen original.
                //Por lo tanto, se copia el valor del canal y se guarda en nueva_data.
            } 
        }
    }
    // Una vez ya creada la imagen debemos reemplazar los datos de la imagen original por los nuevos datos rotados.
    // Liberamos la memoria original para poder reemplazar con los datos de la imagen rotada.
    delete[] img->data;
    img->data = nuevo_data;
    img->width = nuevo_ancho;
    img->height = nueva_altura;
    //Es necesario usar delete[] porque estamos creando un nuevo arreglo.
}

//Creamos una función void que no devuelve ningun valor solo modifica, y agregamos nuestro puntero y la funcion float.
void operacion_3(Imagen* img, float atenuacion){ 
    if (atenuacion < 0.0f || atenuacion > 1.0f){ 
        //Si el valor dado cumple la condición de arriba arroja este error.
        cout <<"EL valor ingresado debe estar entre 0.0 y 1.0.\n";
        return;
    }

    //Si el valor dado esta dentro del rango,  recorremos  el arreglo. además cada pixel lo guardamos en la posición i.
    for(int i = 0; i < img->width * img->height * img->channels; i++){ 
        //Calculamos cuanto falta para que el pixel sea blanco.
        int resta_blanco = 255 - img->data[i]; 
        //Aplicamos la atenuacion a lo anterior para que no se vea tan oscuro.
        int ajuste = resta_blanco * atenuacion; 
        //Le sumamos este ajuste al pixel orginal para obtener el nuevo pixel con su diferente color.
        int NuevoPixel = img->data[i] + ajuste; 

        //Si el nuevo pixel es mayor a 255 lo ajustamos para que se quede dentro del rango permitido para una imagen.
        if (NuevoPixel > 255) NuevoPixel = 255; 
        //Si el neuvo pixel es menor a 0 le ajustamos para que se quede dentro del rango permitido para una imagen.
        if (NuevoPixel < 0) NuevoPixel = 0; 
        //**Estos ajustes los hacemos para que no nos ocurra una imagen "corrupta" como que le salga unos cuadritos extraños*/

        //Convertimos el nuevo pixel a usigned char(tipo de datoq ue solo acepta valores engtre 0 y 255) y guardamos el resultado en el arreglo
        img->data[i] = static_cast<unsigned char>(NuevoPixel); 
    }
}

void operacion_4(Imagen* img, int limite){
    //Bucle que recorre los pixeles de la img, al multiplicar la altura y el ancho obtenemos todos los pixeles, el i actua como el pixel que estamos "manejando".
    for (int i = 0; i < img->width * img->height; i++){ 
        int R = img->data[i * img->channels]; //Color Rojo
        int G = img->data[i * img->channels + 1]; //Color Verde
        int B = img->data[i * img->channels +2]; // Color Azul

        //Verificamos si algunos de los canales del color del pixel es mayor al limite.
        if (R > limite || G > limite || B > limite) { 
            // Convertimos todos los canales (R,G,B) al color blanco.
            img->data[i * img->channels] = 255; 
            img->data[i * img->channels + 1] = 255;
            img->data[i * img->channels +2] = 255;

            //Sino los convertimos negros.
        } else { 
            img->data[i * img->channels] = 0;
            img->data[i * img->channels + 1] = 0;
            img->data[i * img->channels +2] = 0;
        }
    }
    save(img, "Op4.png");
}

void operacion_5(Imagen* img){
    const string caracteres_ASCII = "@%#*+=-:."; //Arreglo N ordenados por luminosidad aparente. Del más "oscuro" al más "blanco" porque
                                                  // asi los valores más altos corresponden al blanco y los menores al oscuro. 

    ofstream Salida_Archivo("Pikachu_ascii.txt");

    for (int y = 0; y < img->height; y++) { //Recorremos la imagen completa. 
        for (int x = 0; x < img->width; x++) {
            int pos_pixel = (y * img->width + x) * img->channels; //posición pixel, pos 0.

            int R = img->data[pos_pixel]; //Canal Rojo.
            int G = img->data[pos_pixel + 1]; //Canal Verde.
            int B = img->data[pos_pixel + 2]; //Canal Azul.
            int escala_gris = 0.3 * R + 0.59 * G + 0.11 * B; //Transformamos a escala de grises.

            // Como estamos usando la formula de escala de grises es mejor poner un rango para que no se pase ningun pixel blanco, ya que
            // a veces por detalle de la foto un pixel no es completamente blanco. 
            if (escala_gris > 230) {  //Para que el fondo blanco sea "transparente".
                Salida_Archivo << ' ';
            } else {
                char Char_a_ASCII = caracteres_ASCII[escala_gris * (caracteres_ASCII.size() - 1) / 255]; // Transformación al caracter ASCII.
                Salida_Archivo << Char_a_ASCII; // Se escribe caracter del arreglo ASCII en archivo.
            }
        }
        Salida_Archivo << endl; // Nueva línea para la siguiente fila
    }
    Salida_Archivo.close();
}

int main() {
    
    int numero;
    cout << "Ingresar número de operación: "<< endl;
    cin >> numero;
    
        
    

    //Cargamos la imagen
    Imagen* img = load("Pikachu.png");

    //Modificamos un par de pixeles en el borde superior de la imagen como ejemplo
    //img->data[101] = 0;

    if (numero == 1 ){
        operacion_1(img);

    }else if (numero == 2){
        operacion_2(img);

    }else if (numero == 3){ //si el numero es 3 entonces entramos a esta función. #Leer README C:.
        float atenuacion; //Declaramos la variable atenuacion que es de tipo float.
        cout<<"Ingrese un grado de atenuación entre 0.0 y 1.0: "; //El usario nos otorga un valor que hará que la imagen se atenue.
        cin >> atenuacion;//El valor asignado por el usario se guarda en la variable atenuacion.
        operacion_3(img, atenuacion);//Llamamos a la función que es la que modifica el grado de atenuacion.

    }else if (numero == 4) {
        int limite;
        cout << "Ingrese el límite para la operación (0 a 255): ";
        cin >> limite;
    
        if (limite < 0 || limite > 255) {
            cout << "Por favor, ingrese un límite válido entre 0 y 255.\n";
        } else {
            operacion_4(img, limite);
        }
    }else{
        operacion_5(img);
    }
    // Almacenamos el resultado
    save(img, "out.png");

    // Liberamos la memoria ocupada por la imagen antes de finalizar.
    delete img;
}
