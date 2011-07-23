.. index::
   single: YAML
   single: Configuración; YAML

YAML
====

La página web de `YAML`_ dice que es "un estándar para la serialización de datos humanamente legible para todos los lenguajes de programación". YAML es un lenguaje simple que describe datos. Como PHP, que tiene una sintaxis para tipos simples, como cadenas, booleanos, reales, o enteros.
Pero a diferencia de PHP, que distingue entre matrices (secuencias) y 'hashes' (mapeos).

El :namespace:`Symfony\\Component\\Yaml` Componente Symfony2 sabe cómo analizar YAML y volcar una matriz PHP a YAML.

.. note::

    Si bien el formato YAML puede describir complejas estructuras anidadas de datos, este capítulo sólo describe el conjunto mínimo de características necesarias para usar YAML como un formato de archivo de configuración.

Leyendo archivos YAML
---------------------

El método :method:`Symfony\\Component\\Yaml\\Parser::parse` analiza una cadena YAML y la convierte en una matriz de PHP::

    use Symfony\Component\Yaml\Parser;

    $yaml = new Parser();
    $value = $yaml->parse(file_get_contents('/ruta/a/file.yaml'));

Si se produce un error durante el análisis, el analizador emite una excepción que indica el tipo de error y la línea en la cadena original YAML donde ocurrió el error::

    try {
        $value = $yaml->parse(file_get_contents('/ruta/a/file.yaml'));
    } catch (\InvalidArgumentException $e) {
        // ocurrió un error durante el análisis
        echo "Unable to parse the YAML string: ".$e->getMessage();
    }

.. tip::

    Debido a que el analizador es reentrante, puedes utilizar el mismo objeto analizador para cargar diferentes cadenas YAML.

Al cargar un archivo YAML, a veces es mejor usar el contenedor del método :method:`Symfony\\Component\\Yaml\\Yaml::load`::

    use Symfony\Component\Yaml\Yaml;

    $loader = Yaml::parse('/ruta/al/archivo.yml');

El método estático ``Yaml::parse()`` toma una cadena YAML o un archivo que contiene YAML. Internamente, este llama al método ``Parser::parse()``, pero con algunas cosas adicionales como:

* Ejecuta el archivo YAML como si fuera un archivo PHP, por lo tanto puedes incrustar ordenes PHP en los archivos YAML;

* Cuando un archivo no se puede analizar, este agrega automáticamente el nombre del archivo al mensaje de error, lo cual simplifica la depuración cuando tu aplicación está cargando varios archivos YAML.

Escribiendo archivos YAML
-------------------------

El método :method:`Symfony\\Component\\Yaml\\Dumper::dump` vierte cualquier matriz PHP en su representación de YAML::

    use Symfony\Component\Yaml\Dumper;

    $array = array('foo' => 'bar', 'bar' => array('foo' => 'bar', 'bar' => 'baz'));

    $dumper = new Dumper();
    $yaml = $dumper->dump($array);
    file_put_contents('/ruta/a/file.yaml', $yaml);

.. note::

    Hay algunas limitaciones: el vertedor no es capaz de volcar los recursos y verter objetos PHP se considera una característica alfa.

Si sólo necesitas volcar una matriz, puedes utilizar el método estático :method:`Symfony\\Component\\Yaml\\Yaml::dump`::

    $yaml = Yaml::dump($array, $inline);

El formato YAML apoya las dos representaciones de matriz YAML. De forma predeterminada, el vertedor utiliza la representación en línea:

.. code-block:: yaml

    { foo: bar, bar: { foo: bar, bar: baz } }

Pero el segundo argumento del método ``dump()`` personaliza el nivel en el cual la salida cambia de la representación expandida a la de en línea::

    echo $dumper->dump($array, 1);

.. code-block:: yaml

    foo: bar
    bar: { foo: bar, bar: baz }

.. code-block:: php

    echo $dumper->dump($array, 2);

.. code-block:: yaml

    foo: bar
    bar:
        foo: bar
        bar: baz

La sintaxis YAML
----------------

Cadenas
~~~~~~~

.. code-block:: yaml

    Una cadena en YAML

.. code-block:: yaml

    'Una cadena entre comillas simples en YAML'

.. tip::
   En una cadena entre comillas simples, una comilla simple ``'`` debe ser doble:

   .. code-block:: yaml

        'Una comilla simple '' en una cadena entre comillas simples'

.. code-block:: yaml

    "Una cadena entre comillas dobles en YAML\n"

Los estilos de citado son útiles cuando una cadena empieza o termina con uno o más espacios relevantes.

.. tip::

    El estilo entre comillas dobles proporciona una manera de expresar cadenas arbitrarias, utilizando secuencias de escape ``\``. Es muy útil cuando se necesita incrustar un ``\n`` o un carácter Unicode en una cadena.

Cuando una cadena contiene saltos de línea, puedes usar el estilo literal, indicado por la tubería (``|``), para indicar que la cadena tendrá una duración de varias líneas. En literales, se conservan los saltos de línea:

.. code-block:: yaml

    |
      \/ /| |\/| |
      / / | |  | |__

Alternativamente, puedes escribir cadenas con el estilo de plegado, denotado por ``>``, en el cual cada salto de línea es sustituido por un espacio:

.. code-block:: yaml

    >
      Esta es una oración muy larga
      que se extiende por varias líneas en el YAML
      pero que se reproduce como una cadena
      sin retornos de carro.

.. note::

    Observa los dos espacios antes de cada línea en los ejemplos anteriores. Ellos no aparecen en las cadenas PHP resultantes.

Números
~~~~~~~

.. code-block:: yaml

    # un entero
    12

.. code-block:: yaml

    # un octal
    014

.. code-block:: yaml

    # un hexadecimal
    0xC

.. code-block:: yaml

    # un float
    13.4

.. code-block:: yaml

    # un número exponencial
    1.2e+34

.. code-block:: yaml

    # infinito
    .inf

Nulos
~~~~~

Los nulos en YAML se pueden expresar con ``null`` o ``~``.

Booleanos
~~~~~~~~~

Los booleanos en YAML se expresan con ``true`` y ``false``.

fechas
~~~~~~

YAML utiliza la norma ISO-8601 para expresar fechas:

.. code-block:: yaml

    2001-12-14t21:59:43.10-05:00

.. code-block:: yaml

    # fecha simple
    2002-12-14

Colecciones
~~~~~~~~~~~

Rara vez se utiliza un archivo YAML para describir un simple escalar. La mayoría de las veces, describe una colección. Una colección puede ser una secuencia o una asignación de elementos. Ambas, secuencias y asignaciones se convierten en matrices PHP.

Las secuencias usan un guión seguido de un espacio (``-``):

.. code-block:: yaml

    - PHP
    - Perl
    - Python

El archivo YAML anterior es equivalente al siguiente código PHP::

    array('PHP', 'Perl', 'Python');

Las asignaciones usan dos puntos (``:``) seguidos de un espacio para marcar cada par clave/valor:

.. code-block:: yaml

    PHP: 5.2
    MySQL: 5.1
    Apache: 2.2.20

el cual es equivalente a este código PHP::

    array('PHP' => 5.2, 'MySQL' => 5.1, 'Apache' => '2.2.20');

.. note::

    En una asignación, una clave puede ser cualquier escalar válido.

El número de espacios entre los dos puntos y el valor no importa:

.. code-block:: yaml

    PHP:    5.2
    MySQL:  5.1
    Apache: 2.2.20

YAML utiliza sangría con uno o más espacios para describir colecciones anidadas:

.. code-block:: yaml

    "symfony 1.4":
        PHP:      5.2
        Doctrine: 1.2
    "Symfony2":
        PHP:      5.3
        Doctrine: 2.0

El YAML anterior es equivalente al siguiente código PHP::

    array(
        'symfony 1.4' => array(
            'PHP'      => 5.2,
            'Doctrine' => 1.2,
        ),
        'Symfony2' => array(
            'PHP'      => 5.3,
            'Doctrine' => 2.0,
        ),
    );

Hay una cosa importante que tienes que recordar cuando utilices sangría en un archivo YAML: *La sangría se debe hacer con uno o más espacios, pero nunca con tabulaciones*.

Puedes anidar secuencias y asignaciones a tu gusto:

.. code-block:: yaml

    'Chapter 1':
        - Introduction
        - Event Types
    'Chapter 2':
        - Introduction
        - Helpers

YAML también puedes utilizar estilos de flujo para colecciones, utilizando indicadores explícitos en lugar de sangría para denotar el alcance.

Puedes escribir una secuencia como una lista separada por comas entre corchetes (``[]``):

.. code-block:: yaml

    [PHP, Perl, Python]

Puedes escribir una asignación como una lista separada por comas de clave/valor dentro de llaves (``{}``):

.. code-block:: yaml

    { PHP: 5.2, MySQL: 5.1, Apache: 2.2.20 }

Puedes mezclar y combinar estilos para conseguir mayor legibilidad:

.. code-block:: yaml

    'Chapter 1': [Introduction, Event Types]
    'Chapter 2': [Introduction, Helpers]

.. code-block:: yaml

    "symfony 1.4": { PHP: 5.2, Doctrine: 1.2 }
    "Symfony2":    { PHP: 5.3, Doctrine: 2.0 }

Comentarios
~~~~~~~~~~~

En YAML puedes añadir comentarios prefijando con una almohadilla (``#``):

.. code-block:: yaml

    # Comentario en una línea
    "Symfony2": { PHP: 5.3, Doctrine: 2.0 } # Comentario al final de la línea

.. note::

    Los comentarios simplemente son ignorados por el analizador de YAML y no necesitan sangría de acuerdo al nivel de anidamiento actual de una colección.

Archivos dinámicos YAML
~~~~~~~~~~~~~~~~~~~~~~~

En Symfony2, un archivo YAML puede contener código PHP que se evalúa justo antes de que ocurra el análisis:

.. code-block:: yaml

    1.0:
        version: <?php echo file_get_contents('1.0/VERSION')."\n" ?>
    1.1:
        version: "<?php echo file_get_contents('1.1/VERSION') ?>"

Ten cuidado de no estropearlo con la sangría. Ten en cuenta los siguientes consejos sencillos al añadir código PHP en un archivo YAML:

* La declaración ``<?php ?>`` siempre debe comenzar la línea o estar integrada en un valor.

* Si una declaración ``<?php ?>`` termina una línea, necesitas producir una nueva línea ("\n") de manera explícita.

.. _YAML: http://yaml.org/
