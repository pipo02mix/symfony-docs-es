.. index::
   single: Paquetes; Buenas prácticas

Estructura de un paquete y buenas prácticas
===========================================

Un paquete es un directorio que tiene una estructura bien definida y puede alojar cualquier cosa, desde clases hasta controladores y recursos web. A pesar de que los paquetes son tan flexibles, se deben seguir algunas recomendaciones si deseas distribuirlos.

.. index::
   pair: Paquetes; Convenciones de nomenclatura

.. _bundles-naming-conventions:

Nombre de paquete
-----------------

Un paquete también es un espacio de nombres PHP. El espacio de nombres debe seguir los `estándares`_ de interoperabilidad técnica de los espacios de nombres y nombres de clases de PHP 5.3: comienza con un segmento de proveedor, seguido por cero o más segmentos de categoría, y termina con el nombre corto del espacio de nombres, mismo que debe terminar con un sufijo ``Bundle``.

Un espacio de nombres se convierte en un paquete tan pronto como se agrega una clase ``bundle`` al mismo. El nombre de la clase ``bundle`` debe seguir estas sencillas reglas:

* Solamente usa caracteres alfanuméricos y subraya;
* Utiliza un nombre con mayúsculas intercaladas;
* Usa un nombre corto y descriptivo (no más de 2 palabras);
* Prefija el nombre con la concatenación del proveedor (y opcionalmente la categoría del espacio de nombres);
* El nombre tiene el sufijo ``Bundle``.

Estos son algunos espacios de nombres y nombres de clase ``bundle`` válidos:

+-----------------------------------+----------------------------+
| Espacio de nombres                | Nombre de clase ``Bundle`` |
+===================================+============================+
| ``Acme\Bundle\BlogBundle``        | ``AcmeBlogBundle``         |
+-----------------------------------+----------------------------+
| ``Acme\Bundle\Social\BlogBundle`` | ``AcmeSocialBlogBundle``   |
+-----------------------------------+----------------------------+
| ``Acme\BlogBundle``               | ``AcmeBlogBundle``         |
+-----------------------------------+----------------------------+

Por convención, el método ``getNombre()`` de la clase ``bundle`` debe devolver el nombre de la clase.

.. note::

    Si compartes tu paquete públicamente, debes utilizar el nombre de la clase ``bundle`` como nombre del repositorio (``AcmeBlogBundle`` y no ``BlogBundle`` por ejemplo).

.. note::

    Los paquetes del núcleo de Symfony2 no prefijan la clase ``bundle`` con ``Symfony`` y siempre agregan un subespacio de nombres ``Bundle``, por ejemplo: :class:`Symfony\\Bundle\\FrameworkBundle\\FrameworkBundle`.

Estructura del directorio
-------------------------

La estructura básica del directorio del paquete ``HolaBundle`` se debe leer de la siguiente manera:

.. code-block:: text

    XXX/...
        HolaBundle/
            HolaBundle.php
            Controller/
            Resources/
                meta/
                    LICENSE
                config/
                doc/
                    index.rst
                translations/
                views/
                public/
            Tests/

Los directorios ``XXX`` reflejan la estructura del espacio de nombres del paquete.

Los siguientes archivos son obligatorios:

* ``HolaBundle.php``;
* ``Resources/meta/LICENSE``: La licencia completa del código;
* ``Resources/doc/index.rst``: El archivo raíz de la documentación del paquete.

.. note::

    Estos convenios garantizan que las herramientas automatizadas pueden trabajar confiablemente en esta estructura predeterminada.

La profundidad de los subdirectorios se debe reducir al mínimo en la mayoría de las clases y archivos utilizados (2 niveles como máximo). Puedes definir más niveles para archivos no estratégicos, los menos utilizados.

El directorio del paquete es de sólo lectura. Si necesitas escribir archivos temporales, guárdalos en el directorio ``cache/`` o ``log/`` de la aplicación anfitriona. Las herramientas pueden generar archivos en la estructura de directorios del paquete, pero sólo si los archivos generados van a formar parte del repositorio.

Las siguientes clases y archivos tienen emplazamientos específicos:

+---------------------------+-----------------------------+
| Tipo                      | Directorio                  |
+===========================+=============================+
| Controladores             | ``Controller/``             |
+---------------------------+-----------------------------+
| Archivos de traducción    | ``Resources/translations/`` |
+---------------------------+-----------------------------+
| Plantillas                | ``Resources/views/``        |
+---------------------------+-----------------------------+
| Pruebas unitarias         | ``Tests/``                  |
| y funcionales             |                             |
+---------------------------+-----------------------------+
| Recursos Web              | ``Resources/public/``       |
+---------------------------+-----------------------------+
| Configuración             | ``Resources/config/``       |
+---------------------------+-----------------------------+
| Ordenes                   | ``Command/``                |
+---------------------------+-----------------------------+

Clases
------

La estructura del directorio de un paquete se utiliza como la jerarquía del espacio de nombres. Por ejemplo, un controlador ``HolaController`` se almacena en ``/HolaBundle/Controller/HolaController.php`` y el nombre de clase completamente cualificado es ``Bundle\HolaBundle\Controller\HolaController``.

Todas las clases y archivos deben seguir los :doc:`estándares </contributing/code/standards>` de codificación Symfony2.

Algunas clases se deben ver como fachada y deben ser lo más breves posible, al igual que las ordenes, ayudantes, escuchas y controladores.

Las clases que conectan el Evento al Despachador deben llevar el posfijo ``Listener``.

Las clases de excepciones se deben almacenar en un subespacio de nombres ``Exception``.

Terceros
--------

Un paquete no debe integrar bibliotecas PHP de terceros. Se debe confiar en la carga automática estándar de Symfony2 en su lugar.

Un paquete no debería integrar bibliotecas de terceros escritas en JavaScript, CSS o cualquier otro lenguaje.

Pruebas
-------

Un paquete debe venir con un banco de pruebas escritas con PHPUnit , las cuales se deben almacenar en el directorio ``Test/``. Las pruebas deben seguir los siguientes principios:

* El banco de pruebas se debe ejecutar con una simple orden ``PHPUnit`` desde una aplicación de ejemplo;
* Las pruebas funcionales sólo se deben utilizar para probar la salida de la respuesta y alguna información del perfil si tienes alguno;
* La cobertura de código debe cubrir cuando menos el 95% del código base.

.. note::
    Un banco de pruebas no debe contener archivos ``AllTests.php``, sino que se debe basar en la existencia de un archivo ``phpunit.xml.dist``.

Documentación
-------------

Todas las clases y funciones deben venir con PHPDoc completo.

También deberá proporcionar abundante documentación provista en formato :doc:`reStructuredText </contributing/documentation/format>`, bajo el directorio ``Resources/doc/``, el archivo ``Resources/doc/index.rst`` es el único archivo obligatorio.

Controladores
-------------

Como práctica recomendada, los controladores en un paquete que está destinado a ser distribuido a otros no debe extender la clase base :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller``.
Puede implementar la :class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface` o en su lugar extender la clase :class:`Symfony\\Component\\DependencyInjection\\ContainerAware`.

.. note::

    Si echas un vistazo a los métodos de la clase :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`, podrás ver que sólo son buenos accesos directos para facilitar la curva de aprendizaje.

Plantillas
----------

Si un paquete proporciona plantillas, debe utilizar Twing. Un paquete no debe proporcionar un diseño principal, salvo si ofrece una aplicación completa.

Archivos de traducción
----------------------

Si un paquete proporciona traducción de mensajes, se deben definir en formato XLIFF, el dominio se debe nombrar después del nombre del paquete (``bundle.hola``).

Un paquete no debe reemplazar los mensajes de otro paquete existente.

Configurando
------------

Para proporcionar mayor flexibilidad, un paquete puede proporcionar opciones configurables utilizando los mecanismos integrados de Symfony2.

Para ajustes de configuración simples, confía en los ``parámetros`` predeterminados de la configuración de Symfony2. Los parámetros de Symfony2 simplemente son pares clave/valor; un valor es cualquier valor PHP válido. Cada nombre de parámetro debe comenzar con una versión corta en minúscula del nombre del paquete usando guiones bajos (``acme_hola`` para ``AcmeHolaBundle`` o ``acme_social_blog`` para ``Acme\Social\BlogBundle`` por ejemplo) , aunque esto sólo es una sugerencia de buenas prácticas. El resto del nombre del parámetro utiliza un punto (``.``) para separar las diferentes partes (por ejemplo, ``acme_hola.correo.from``).

El usuario final puede proporcionar valores en cualquier archivo de configuración:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            acme_hola.correo.from: fabien@ejemplo.com

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="acme_hola.correo.from">fabien@ejemplo.com</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $contenedor->setParameter('acme_hola.correo.from', 'fabien@ejemplo.com');

    .. code-block:: ini

        [parameters]
        acme_hola.correo.from = fabien@ejemplo.com

Recupera los parámetros de configuración en tu código desde el contenedor::

    $contenedor->getParameter('acme_hola.correo.from');

Incluso si este mecanismo es bastante simple, te animamos a usar la configuración semántica descrita en el recetario.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/bundles/extension`

.. _estándares:     http://groups.google.com/group/php-standards/web/psr-0-final-proposal
