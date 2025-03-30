#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Importacion de librerias. Los archivos correspondientes deben estar en el mismo directorio que el .cpp.
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

using namespace std;

// Struct que almacena la data de una imagen
struct Imagen{
    unsigned char* data; // Apunta los datos de la imagen
    int width, height, channels; // Como imagen png, cada pixel tiene 3 canales RGB
};

// Abrir imagen desde memoria local
//img->data, es acceder a los atributos. Puntero img, accede a los atributos data.
Imagen* load(const char* filename) {
    Imagen* img = new Imagen(); //se crea "nueva" Imagen con el puntero img
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0); //stbi load carga la info de la imagen en la direccion  correspondiente de cada uno de ancho, alto y canal
    return img; //devuelve el puntero
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}


void operacion_1(Imagen* img){ //reflección horizontal
    for (int y = 0; y < img->height; y++) { //para recorrer fila
        for (int x = 0; x < img->width/2; x++){ //recorrer la mitad de las columnas porque asi el intercambio de pixeles se realizara 1 sola vez, si se recorriera toda la fila se haria 2 veces el cambio lo cual es innecesario
            //La fórmula para obtener la posición de un píxel en un arreglo unidimensional
            //index = (fila * ancho + columna) * canales
            int pixel_izquierdo = (y * img->width + x) * img->channels; //pos 0, al *channels nos aseguramos de que tambien estamos considerando los canales RGB
            int pixel_derecho = (y * img->width + (img->width - x - 1)) * img->channels; //pos -1

            //para recorrer los canales
            //se suma +c porque asi nos permite acceder a cada canal de un pixel y realizar el intercambio de valores
            for (int c = 0; c < img->channels; c++) { 
                unsigned char tmp = img->data[pixel_izquierdo + c]; //Se guarda el valor del canal
                img->data[pixel_izquierdo + c] = img->data[pixel_derecho + c]; //Se asigna el canal del pixel derecho al izq
                img->data[pixel_derecho + c] = tmp; //Se asigna el valor del pixel izq al derecho
            }
        }
    }
}

void operacion_3(Imagen* img, float atenuacion){ //Creamos una función void que no devuelve ningun valor solo modifica, y agregamos nuestro puntero y la funcion float.
    if (atenuacion < 0.0f || atenuacion > 1.0f){ //Revisamos si el valor dado esta dentro del rango [0,1] además usamos ese f que se refiere a un núm float.
        cout <<"EL valor ingresado debe estar entre 0.0 y 1.0.\n"; //Si el valor dado cumple la condición de arriba arroja este error.
        return;
    }

    for(int i = 0; i < img->width * img->height * img->channels; i++){ //Si el valor dado esta dentro del rango,  recorremos  el arreglo. además cada pixel lo guardamos en la posición i.
        img->data[i] = static_cast<unsigned char>(img->data[i] * atenuacion); //Acá multiplicamos cada pixel por el grado de atenuación. Esta operación como que "oscurese o hace mas transparente" y luego lo convertimos a usigned char para asegurar que esta en el rango valido para img.
    }
}

void operacion_4(Imagen* img, int limite){
    for (int i = 0; i < img->width * img->height * img->channels; i++){//Este for es igual que el anterior op3.
        if (img->data[i]> limite){//la posicion i pixel es mayor que el limite 
            img->data[i] = 255;//transformamos ese pixel al blanco. en este caso 255 que es el maximo para un pixel
        } else {//si i es menor al limite
            img->data[i] = 0;//transformamos ese pixel a negro. en este caso 0 que es el min para un pixel.
        }
    }

}

int main() {
    
    int numero;
    cout << "Ingrese un número: "<< endl;
    cin >> numero;
    
        
    

    //Cargamos la imagen
    Imagen* img = load("Pikachu.png");

    //Modificamos un par de pixeles en el borde superior de la imagen como ejemplo
    //img->data[101] = 0;

    if (numero == 0 ){
        operacion_1(img);
    }

    if (numero == 3){ //si el numero es 3 entonces entramos a esta función. #Leer README C:.
        float atenuacion; //Declaramos la variable atenuacion que es de tipo float.
        cout<<"Ingrese un grado de atenuación entre 0.0 y 1.0: "; //El usario nos otorga un valor que hará que la imagen se atenue.
        cin >> atenuacion;//El valor asignado por el usario se guarda en la variable atenuacion.
        operacion_3(img, atenuacion);//Llamamos a la función que es la que modifica el grado de atenuacion.
    }

    if (numero == 4) {
        int limite;
        cout << "Ingrese el límite para la operación (0 a 255): ";
        cin >> limite;
    
        if (limite < 0 || limite > 255) {
            cout << "Por favor, ingrese un límite válido entre 0 y 255.\n";
        } else {
            operacion_4(img, limite);
        }
    }
    // Almacenamos el resultado
    save(img, "out.png");
}
