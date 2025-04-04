
Nombre: Isidora Ogas 
Rol: 202473654-K

Nombre: Matilde Vásquez
Rol: 202473652-3


Pregunta 6 (10 pts)
La siguiente implementación de la Operación 4 produce un resultado incorrecto. ¿Cuál
es el error, y como podría resolverse a nivel de código? La respuesta puede justificarse
teóricamente o con código.

R- El error en la implementación de la Operación 4 se encuentra en que no se evalúa los
canales de color RGB de forma independiente. En lugar de procesar cada canal por 
separado, el código trata a los valores RGB como si fuera un "único color combinado".
Esto es lo que genera el resultado incorrecto, ya que al aplicar operaciones como la 
atenuación, se incrementa el brillo (o se vuelve más oscuro, dependiendo del límite)
de los colores en lugar de producir la operación esperada.

Para resolverlo a nivel de código se podría realizar de 2 formas:

   1- Separar los canales de color: El código tendrá que procesar cada componente
   RGB de forma individual, evaluando si es que supera el límite para decidir si ese
   pixel será blanco o negro (Esta es la forma que ocupamos nosotras).

   2- Conversión a escala de grises: Transformar la imagen RGB a escala de grises, lo que
   simplifica un poco la operación al trabajar solo con un canal que representaría el 
   brillo general del pixel; facilitando aplicar las transformaciones sin afectar el brillo
   de los colores individuales.


