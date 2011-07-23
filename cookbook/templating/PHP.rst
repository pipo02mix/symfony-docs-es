.. index::
   single: Plantillas PHP

Cómo usar plantillas PHP en lugar de Twig
=========================================

No obstante que Symfony2 de manera predeterminada usa Twig como su motor de plantillas, puedes usar código PHP simple si lo deseas. Ambos motores de plantilla son igualmente compatibles en Symfony2. Symfony2 añade algunas características interesantes en lo alto de PHP para hacer más poderosa la escritura de plantillas con PHP.

Reproduciendo plantillas PHP
----------------------------

Si deseas utilizar el motor de plantillas PHP, primero, asegúrate de activarlo en el archivo de configuración de tu aplicación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating:    { engines: ['twig', 'php'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ... >
            <!-- ... -->
            <framework:templating ... >
                <framework:engine id="twig" />
                <framework:engine id="php" />
            </framework:templating>
        </framework:config>

    .. code-block:: php

        $contenedor->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig', 'php'),
            ),
        )); 

Ahora puedes reproducir una plantilla PHP en lugar de una Twig simplemente usando la extensión ``.php`` en el nombre de la plantilla en lugar de ``.twig``. El controlador a continuación reproduce la plantilla ``index.html.php``::

    // src/Acme/HolaBundle/Controller/HolaController.php

    public function indexAction($nombre)
    {
        return $this->render('HolaBundle:Hola:index.html.php', array('nombre' => $nombre));
    }

.. index::
  single: Templating; Layout
  single: Layout

Decorando plantillas
--------------------

Muy a menudo, las plantillas en un proyecto comparten elementos comunes, como los bien conocidos encabezados y pies de página. En Symfony2, nos gusta pensar en este problema de forma diferente: una plantilla se puede decorar con otra.

La plantilla ``index.html.php`` está decorada por ``base.html.php``, gracias a la llamada a ``extend()``:

.. code-block:: html+php

    <!-- src/Acme/HolaBundle/Resources/views/Hola/index.html.php -->
    <?php $view->extend('AcmeHolaBundle::base.html.php') ?>

    Hola <?php echo $nombre ?>!

La notación ``HolaBundle::base.html.php`` te suena familiar, ¿no? Es la misma notación utilizada para referir una plantilla. La parte ``::`` simplemente significa que el elemento controlador está vacío, por lo tanto el archivo correspondiente se almacena directamente bajo ``views/``.

Ahora, echemos un vistazo al archivo ``base.html.php``:

.. code-block:: html+php

    <!-- src/Acme/HolaBundle/Resources/views/base.html.php -->
    <?php $view->extend('::base.html.php') ?>

    <h1>Aplicación ``Hola``</h1>

    <?php $view['slots']->output('_content') ?>

El diseño en sí mismo está decorado por otra (``::base.html.php``). Symfony2 admite varios niveles de decoración: un diseño en sí se puede decorar con otro. Cuando la parte nombre del paquete de la plantilla está vacía, se buscan las vistas en el directorio ``app/Resources/views/``. Este directorio almacena vistas globales de tu proyecto completo:

.. code-block:: html+php

    <!-- app/Resources/views/base.html.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title><?php $view['slots']->output('titulo', 'Aplicación ``Hola``') ?></title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

Para ambos diseños, la expresión ``$view['slots']->output('_content')`` se sustituye por el contenido de la plantilla hija, ``index.html.php`` y ``base.html.php``, respectivamente (más de ranuras en la siguiente sección).

Como puedes ver, Symfony2 proporciona métodos en un misterioso objeto ``$view``. En una plantilla, la variable ``$view`` siempre está disponible y se refiere a un objeto especial que proporciona una serie de métodos que hacen funcionar el motor de plantillas.

.. index::
   single: Plantillas; Ranura
   single: Ranura

Trabajar con ranuras
--------------------

Una ranura es un fragmento de código, definido en una plantilla, y reutilizable en cualquier diseño para decorar una plantilla. En la plantilla ``index.html.php``, define una ranura ``titulo``:

.. code-block:: html+php

    <!-- src/Acme/HolaBundle/Resources/views/Hola/index.html.php -->
    <?php $view->extend('AcmeHolaBundle::base.html.php') ?>

    <?php $view['slots']->set('titulo', 'Aplicación Hola Mundo') ?>

    Hola <?php echo $nombre ?>!

El diseño base ya tiene el código para reproducir el título en el encabezado:

.. code-block:: html+php

    <!-- app/Resources/views/base.html.php -->
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title><?php $view['slots']->output('titulo', 'Aplicación ``Hola``') ?></title>
    </head>

El método ``output()`` inserta el contenido de una ranura y, opcionalmente, toma un valor predeterminado si la ranura no está definida. Y ``_content`` sólo es una ranura especial que contiene la plantilla hija reproducida.

Para las ranuras grandes, también hay una sintaxis extendida:

.. code-block:: html+php

    <?php $view['slots']->start('titulo') ?>
        Some large amount of HTML
    <?php $view['slots']->stop() ?>

.. index::
   single: Templating; Include

Incluyendo otras plantillas
---------------------------

La mejor manera de compartir un fragmento de código de plantilla es definir una plantilla que se pueda incluir en otras plantillas.

Crea una plantilla ``hola.html.php``:

.. code-block:: html+php

    <!-- src/Acme/HolaBundle/Resources/views/Hola/hola.html.php -->
    Hola <?php echo $nombre ?>!

Y cambia la plantilla ``index.html.php`` para incluirla:

.. code-block:: html+php

    <!-- src/Acme/HolaBundle/Resources/views/Hola/index.html.php -->
    <?php $view->extend('AcmeHolaBundle::base.html.php') ?>

    <?php echo $view->render('AcmeHola:Hola:hola.html.php', array('nombre' => $nombre)) ?>

El método ``render()`` evalúa y devuelve el contenido de otra plantilla (este exactamente es el mismo método que utilizamos en el controlador).

.. index::
   single: Plantillas; Incrustando páginas

Integrando otros controladores
------------------------------

¿Y si deseas incrustar el resultado de otro controlador en una plantilla?
Eso es muy útil cuando se trabaja con Ajax, o cuando la plantilla incrustada necesita alguna variable que no está disponible en la plantilla principal.

Si creas una acción ``maravillosa``, y quieres incluirla en la plantilla ``index.html.php``, basta con utilizar el siguiente código:

.. code-block:: html+php

    <!-- src/Acme/HolaBundle/Resources/views/Hola/index.html.php -->
    <?php echo $view['actions']->render('HolaBundle:Hola:maravillosa', array('nombre' => $nombre, 'color' => 'verde')) ?>

Aquí, la cadena ``HolaBundle:Hola:maravillosa`` se refiere a la acción ``maravillosa`` del controlador ``Hola``::

    // src/Acme/HolaBundle/Controller/HolaController.php

    class HolaController extends Controller
    {
        public function maravillosaAction($nombre, $color)
        {
            // crea algún objeto, basado en la variable $color
            $object = ...;

            return $this->render('HolaBundle:Hola:maravillosa.html.php', array('nombre' => $nombre, 'object' => $object));
        }

        // ...
    }

Pero ¿dónde se define el elemento arreglo ``$view['actions'] de la vista? Al igual que ``$view['slots']``, este invoca a un ayudante de plantilla, y la siguiente sección contiene más información sobre ellos.

.. index::
   single: Plantillas; Ayudantes

Usando ayudantes de plantilla
-----------------------------

El sistema de plantillas de Symfony2 se puede extender fácilmente por medio de los ayudantes. Los ayudantes son objetos PHP que ofrecen funciones útiles en el contexto de la plantilla. ``actions`` y ``slots`` son dos de los ayudantes integrados en Symfony2.

Creando enlaces entre páginas
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Hablando de aplicaciones web, crear enlaces entre páginas es una necesidad. En lugar de codificar las direcciones URL en las plantillas, el ayudante ``router`` sabe cómo generar direcciones URL basándose en la configuración de enrutado. De esta manera, todas tus direcciones URL se pueden actualizar fácilmente cambiando la configuración:

.. code-block:: html+php

    <a href="<?php echo $view['router']->generate('hola', array('nombre' => 'Thomas')) ?>">
        Greet Thomas!
    </a>

El método ``generate()`` toma el nombre de la ruta y un arreglo de parámetros como argumentos. El nombre del route es la clave principal en la cual son referidas las rutas y los parámetros son los valores de los marcadores de posición definidos en el patrón route:

.. code-block:: yaml

    # src/Acme/HolaBundle/Resources/config/routing.yml
    hola: # El nombre de ruta
        pattern:  /hola/{nombre}
        defaults: { _controller: AcmeHolaBundle:Hola:index }

Usando activos: imágenes, archivos JavaScript y hojas de estilo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

¿Qué sería de Internet sin imágenes, JavaScript y hojas de estilo?
Symfony2 proporciona la etiqueta ``assets`` para hacer frente a los activos fácilmente:

.. code-block:: html+php

    <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

    <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" />

El principal objetivo del ayudante ``assets`` es hacer más portátil tu aplicación. Gracias a este ayudante, puedes mover el directorio raíz de tu aplicación a cualquier lugar bajo tu directorio raíz del servidor web sin cambiar nada en el código de tu plantilla.

Mecanismo de escape
-------------------

Al utilizar plantillas PHP, escapa las variables cada vez que se muestren al usuario::

    <?php echo $view->escape($var) ?>

De forma predeterminada, el método ``escape()`` supone que la variable se emite dentro de un contexto HTML. El segundo argumento te permite cambiar el contexto. Por ejemplo, para mostrar algo en un archivo de JavaScript, utiliza el contexto ``js``::

    <?php echo $view->escape($var, 'js') ?>
