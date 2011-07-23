Estándares de codificación
==========================

Cuando contribuyas con código a Symfony2, debes seguir sus estándares de codificación. Para no hacer el cuento largo, aquí está la regla de oro: **Imita el código Symfony2 existente**. La mayoría de los Paquetes de código abierto y librerías utilizadas por Symfony2 también siguen las mismas pautas, y también deberías hacerlo.

Recuerda que la principal ventaja de los estándares es que cada pieza de código se ve y se siente familiar, no se trata de tal o cual sea más legible.

Ya que una imagen - o algún código - vale más que mil palabras, he aquí un pequeño ejemplo que contiene la mayoría de las funciones que se describen a continuación:

.. code-block:: php

    <?php

    /*
     * Este archivo es parte del paquete Symfony.
     *
     * (c) Fabien Potencier <fabien@symfony.com>
     *
     * Para obtener la información completa de derechos de autor y licencia,
     * consulta el archivo LICENSE que se distribuye con el código fuente.
     */

    namespace Acme;

    class Foo
    {
        const ALGUNA_CONST = 42;

        private $foo;

        /**
         * @param string $dummy Some argument descripcion
         */
        public function __construct($dummy)
        {
            $this->foo = $this->transform($dummy);
        }

        /**
         * @param string $ficticio Alguna descripción del argumento
         * @return string|null Entrada transformada
         */
        private function transform($ficticio)
        {
            if (true === $ficticio) {
                return;
            } elseif ('string' === $ficticio) {
                $dummy = substr($ficticio, 0, 5);
            }

            return $ficticio;
        }
    }

Estructura
----------

* Nunca utilices las etiquetas cortas (`<?`);

* No termines los archivos de clase con la etiqueta de cierre habitual `?>`;

* La sangría se hace por pasos de cuatro espacios (las tabulaciones no están permitidas);

* Utiliza el carácter de salto de línea (`0x0A`) para terminar las líneas;

* Añade un solo espacio después de cada delimitador coma;

* No pongas espacios después de un paréntesis de apertura ni antes de uno de cierre;

* Añade un solo espacio alrededor de los operadores (`==`, `&&`, ...);

* Añade un solo espacio antes del paréntesis de apertura de una palabra clave de control (`if`, `else`, `for`, `while`, ...);

* Añade una línea en blanco antes de las declaraciones `return`;

* No agregues espacios en blanco al final de las líneas;

* Usa llaves para indicar la estructura del cuerpo de control, independientemente del número de declaraciones que contenga;

* Coloca las llaves en su propia línea para clases, métodos y en la declaración de funciones;

* Separa las declaraciones condicionales (``if``, ``else``, ...) y la llave de apertura con un solo espacio y sin ninguna línea en blanco;

* Declara expresamente la visibilidad para clases, métodos y propiedades (el uso de `var` está prohibido);

* Usa minúsculas para escribir las constantes nativas de PHP: `false`, `true` y `null`. Lo mismo ocurre con `array()`;

* Usa cadenas en mayúsculas para constantes con palabras separadas por subrayas;

* Define una clase por archivo;

* Declara las propiedades de clase antes que los métodos;

* Declara los métodos públicos en primer lugar, a continuación, los protegidos y finalmente los privados.

Convenciones de nomenclatura
----------------------------

* Usa mayúsculas intercaladas, no subrayados, para variables, funciones y nombres de métodos;

* Usa subrayas para opciones, argumentos, nombre de parámetros;

* Utiliza espacios de nombres (namespaces) para todas las clases;

* Utiliza `Symfony` como primer nivel del espacio de nombres;

* Sufija interfaces con `Interface`;

* Utiliza caracteres alfanuméricos y subrayados para los nombres de archivo;

* No olvides consultar en el documento más detallado :doc:`conventions` para más consideraciones subjetivas de nomenclatura.

Documentación
-------------

* Añade bloques PHPDoc a todas las clases, métodos y funciones;

* Las anotaciones `@package` y `@subpackage` no se utilizan.

Licencia
--------

* Symfony se distribuye bajo la licencia MIT, y el bloque de la licencia
  tiene que estar presente en la parte superior de todos los archivos PHP,
  antes del espacio de nombres.
