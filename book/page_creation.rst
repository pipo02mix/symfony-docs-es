.. index::
   single: Creando páginas

Creando páginas en Symfony2
===========================

Crear una nueva página en Symfony2 es un sencillo proceso de dos pasos:

* *Crea una ruta*: Una ruta define la URL de tu página (por ejemplo ``/sobre``) y especifica un controlador (el cual es una función PHP) que Symfony2 debe ejecutar cuando la URL de una petición entrante coincida con el patrón de la ruta;

* *Crea un controlador*: Un controlador es una función PHP que toma la petición entrante y la transforma en el objeto ``Respuesta`` de Symfony2 que es devuelto al usuario.

Nos encanta este enfoque simple porque coincide con la forma en que funciona la Web.
Cada interacción en la Web se inicia con una petición HTTP. El trabajo de la aplicación simplemente es interpretar la petición y devolver la respuesta HTTP adecuada.

Symfony2 sigue esta filosofía y te proporciona las herramientas y convenios para mantener organizada tu aplicación a medida que crece en usuarios y complejidad.

¿Suena bastante simple? ¡Démonos una zambullida!

.. index::
   single: Creando páginas; Ejemplo

La página "¡Hola Symfony!"
--------------------------

Vamos a empezar con una aplicación derivada del clásico "¡Hola Mundo!". Cuando hayamos terminado, el usuario podrá recibir un saludo personal (por ejemplo, "Hola Symfony") al ir a la siguiente URL:

.. code-block:: text

    http://localhost/app_dev.php/hola/Symfony

En realidad, serás capaz de sustituir ``Symfony`` con cualquier otro nombre al cual darle la bienvenida. Para crear la página, sigue el simple proceso de dos pasos.

.. note::

    La guía asume que ya haz descargado Symfony2 y configurado tu servidor web. En la URL anterior se supone que ``localhost`` apunta al directorio ``web``, de tu nuevo proyecto Symfony2. Para información más detallada sobre este proceso, consulta :doc:`Instalando Symfony2 </book/installation>`.

Antes de empezar: Crea el paquete
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Antes de empezar, tendrás que crear un ``bundle`` (*paquete* en adelante). En Symfony2, un :term:`paquete` es como un complemento (o plugin, para los puristas), salvo que todo el código de tu aplicación debe vivir dentro de un paquete.

Un paquete no es más que un directorio que alberga todo lo relacionado con una función específica, incluyendo clases PHP, configuración, e incluso hojas de estilo y archivos de Javascript (consulta :ref:`page-creation-bundles`).

Para crear un paquete llamado ``AcmeHolaBundle`` (el paquete de ejemplo que vamos a construir en este capítulo), ejecuta la siguiente orden y sigue las instrucciones en pantalla (usa todas las opciones predeterminadas):

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/HolaBundle --format=yml

Detrás del escenario, se crea un directorio para el paquete en ``src/Acme/HolaBundle``.
Además agrega automáticamente una línea al archivo ``app/AppKernel.php`` para registrar el paquete en el núcleo::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Acme\HolaBundle\AcmeHolaBundle(),
        );
        // ...

        return $bundles;
    }

Ahora que ya está configurado el paquete, puedes comenzar a construir tu aplicación dentro del paquete.

Paso 1: Creando la ruta
~~~~~~~~~~~~~~~~~~~~~~~

De manera predeterminada, el archivo de configuración de enrutado en una aplicación Symfony2 se encuentra en ``app/config/routing.yml``. Al igual que toda la configuración en Symfony2, fuera de la caja también puedes optar por utilizar XML o PHP para configurar tus rutas.

Si te fijas en el archivo de enrutado principal, verás que Symfony ya ha agregado una entrada al generar el ``AcmeHolaBundle``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        AcmeHolaBundle:
            resource: "@AcmeHolaBundle/Resources/config/routing.yml"
            prefix:   /

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHolaBundle/Resources/config/routing.xml" prefix="/" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->addCollection(
            $loader->import('@AcmeHolaBundle/Resources/config/routing.php'),
            '/',
        );

        return $coleccion;

Esta entrada es bastante básica: le dice a Symfony que cargue la configuración de enrutado del archivo ``Resources/config/routing.yml`` que reside en el interior del ``AcmeHolaBundle``.
Esto significa que colocas la configuración de enrutado directamente en ``app/config/routing.yml`` u organizas tus rutas a través de tu aplicación, y las importas desde aquí.

Ahora que el archivo ``routing.yml`` es importado desde el paquete, añade la nueva ruta que define la URL de la página que estás a punto de crear:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/routing.yml
        hola:
            pattern:  /hola/{nombre}
            defaults: { _controller: AcmeHolaBundle:Hola:index }

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hola" pattern="/hola/{nombre}">
                <default key="_controller">AcmeHolaBundle:Hola:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('hola', new Route('/hola/{nombre}', array(
            '_controller' => 'AcmeHolaBundle:Hola:index',
        )));

        return $coleccion;

La ruta se compone de dos piezas básicas: el ``patrón``, que es la URL con la que esta ruta debe coincidir, y un arreglo ``defaults``, que especifica el controlador que se debe ejecutar. La sintaxis del marcador de posición en el patrón (``{nombre}``) es un comodín. Significa que ``/hola/Ryan``, ``/hola/Fabien`` o cualquier otra URI similar coincidirá con esta ruta. El parámetro marcador de posición ``{nombre}`` también se pasará al controlador, de manera que podemos utilizar su valor para saludar personalmente al usuario.

.. note::

  El sistema de enrutado tiene muchas más características para crear estructuras URI flexibles y potentes en tu aplicación. Para más detalles, consulta el capítulo :doc:`Enrutando </book/routing>`.

Paso 2: Creando el controlador
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cuando una URL como ``/hola/Ryan`` es manejada por la aplicación, la ruta ``hola`` corresponde con el controlador ``AcmeHolaBundle:Hola:index`` el cual es ejecutado por la plataforma. El segundo paso del proceso de creación de páginas es precisamente la creación de ese controlador.

El controlador - ``AcmeHolaBundle:Hola:index`` es el *nombre lógico* del controlador, el cual se asigna al método ``indexAction`` de una clase PHP llamada ``Acme\HolaBundle\Controller\Hola``. Empieza creando este archivo dentro de tu ``AcmeHolaBundle``::

    // src/Acme/HolaBundle/Controller/HolaController.php
    namespace Acme\HolaBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HolaController
    {
    }

En realidad, el controlador no es más que un método PHP que tú creas y Symfony ejecuta. Aquí es donde el código utiliza la información de la petición para construir y preparar el recurso solicitado. Salvo en algunos casos avanzados, el producto final de un controlador siempre es el mismo: un objeto ``Respuesta`` de Symfony2.

Crea el método ``indexAction`` que Symfony ejecutará cuando concuerde la ruta ``hola``::

    // src/Acme/HolaBundle/Controller/HolaController.php

    // ...
    class HolaController
    {
        public function indexAction($nombre)
        {
            return new Response('<html><body>Hola '.$nombre.'!</body></html>');
        }
    }

El controlador es simple: este crea un nuevo objeto ``Respuesta``, cuyo primer argumento es el contenido que se debe utilizar para la respuesta (una pequeña página HTML en este ejemplo).

¡Enhorabuena! Después de crear solamente una ruta y un controlador ¡ya tienes una página completamente funcional! Si todo lo haz configurado correctamente, la aplicación debe darte la bienvenida:

.. code-block:: text

    http://localhost/app_dev.php/hola/Ryan

Un opcional, pero frecuente, tercer paso en el proceso es crear una plantilla.

.. note::

   Los controladores son el punto de entrada principal a tu código y un ingrediente clave en la creación de páginas. Puedes encontrar mucho más información en el capítulo :doc:`Controlador </book/controller>`.

Paso opcional 3: Creando la plantilla
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Las plantillas te permiten mover toda la presentación (por ejemplo, código HTML) a un archivo separado y reutilizar diferentes partes del diseño de la página. En vez de escribir el código HTML dentro del controlador, en su lugar reproduce una plantilla:

.. code-block:: php
    :linenos:

    // src/Acme/HolaBundle/Controller/HolaController.php
    namespace Acme\HolaBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HolaController extends Controller
    {
        public function indexAction($nombre)
        {
            return $this->render('AcmeHolaBundle:Hola:index.html.twig', array('nombre' => $nombre));

            // reproduce una plantilla PHP en su lugar
            // return $this->render('AcmeHolaBundle:Hola:index.html.php', array('nombre' => $nombre));
        }
    }

.. note::

   Para poder usar el método ``render()``, debes extender la clase ``Symfony\Bundle\FrameworkBundle\Controller\Controller`` (documentación de la API:  :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`), la cual añade atajos para tareas comunes dentro de los controladores. Esto se hace en el ejemplo anterior añadiendo la declaración ``use`` en la línea 4 y luego extendiéndo el ``Controlador`` en la línea 6.

El método ``render()`` crea un objeto ``Respuesta`` poblado con el contenido propuesto, y reproduce la plantilla. Como cualquier otro controlador, en última instancia vas a devolver ese objeto ``Respuesta``.

Ten en cuenta que hay dos ejemplos diferentes para procesar la plantilla.
De forma predeterminada, Symfony2 admite dos diferentes lenguajes de plantillas: las clásicas plantillas PHP y las breves pero poderosas plantillas `Twig`_. No te espantes - eres libre de optar por una o, incluso, ambas en el mismo proyecto.

El controlador procesa la plantilla ``AcmeHolaBundle:Hola:index.html.twig``, utilizando la siguiente convención de nomenclatura:

    **NombrePaquete**:**NombreControlador**:**NombrePlantilla**

Este es el nombre *lógico* de la plantilla, el cual se asigna a una ubicación física usando la siguiente convención.

    **/ruta/a/NombrePaquete**/Resources/views/**NombreControlador**/**NombrePlantilla**

En este caso, ``AcmeHolaBundle`` es el nombre del paquete, ``Hola`` es el controlador e ``index.html.twig`` la plantilla:

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# src/Acme/HolaBundle/Resources/views/Hola/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            Hola {{ nombre }}!
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/HolaBundle/Resources/views/Hola/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        Hola <?php echo $view->escape($nombre) ?>!

Veamos la situación a través de la plantilla Twig línea por línea:

* *línea 2*: La etiqueta ``extends`` define una plantilla padre. La plantilla define explícitamente un archivo con el diseño dentro del cual será colocada.

* *línea 4*: La etiqueta ``block`` dice que todo el interior se debe colocar dentro de un bloque llamado ``body``. Como veremos, es responsabilidad de la plantilla padre (``base.html.twig``) reproducir en última instancia, el bloque llamado ``body``.

La plantilla padre, ``::base.html.twig``, omite ambas porciones de los nombres **NombrePaquete** y **NombreControlador** (de ahí los dobles dos puntos (``::``) al principio). Esto significa que la plantilla vive fuera de cualquier paquete, en el directorio ``app``:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block titulo %}Aplicación ``Hola``{% endblock %}</title>
            </head>
            <body>
                {% block body %}{% endblock %}
            </body>
        </html>

    .. code-block:: php

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

El archivo de la plantilla base define el diseño HTML y reproduce el bloque ``body`` que definiste en la plantilla ``index.html.twig``. Además reproduce el bloque ``título``, el cual puedes optar por definirlo en la plantilla ``index.html.twig``.
Dado que no definimos el bloque ``título`` en la plantilla derivada, el valor predeterminado es "Aplicación ``Hola``".

Las plantillas son una poderosa manera de reproducir y organizar el contenido de tu página. Una plantilla puede reproducir cualquier cosa, desde el marcado HTML, al código CSS, o cualquier otra cosa que el controlador posiblemente tenga que devolver.

En el ciclo de vida del manejo de una petición, el motor de plantillas simplemente es una herramienta opcional. Recuerda que el objetivo de cada controlador es devolver un objeto ``Respuesta``. Las plantillas son una poderosa, pero opcional, herramienta para crear el contenido de ese objeto ``Respuesta``.

.. index::
   single: Estructura de directorios

La estructura de directorios
----------------------------

Después de unas cortas secciones, ya entiendes la filosofía detrás de la creación y procesamiento de páginas en Symfony2. También haz comenzado a ver cómo están estructurados y organizados los proyectos Symfony2. Al final de esta sección, sabrás dónde encontrar y colocar diferentes tipos de archivos y por qué.

Aunque totalmente flexible, por omisión, cada :term:`aplicación` Symfony tiene la misma estructura de directorios básica y recomendada:

* ``app/``: Este directorio contiene la configuración de la aplicación;

* ``src/``: Todo el código PHP del proyecto se almacena en este directorio;

* ``vendor/``: Por convención aquí se coloca cualquier biblioteca de terceros;

* ``web/``: Este es el directorio web raíz y contiene todos los archivos de acceso público;

El directorio web
~~~~~~~~~~~~~~~~~

El directorio raíz del servidor web, es el hogar de todos los archivos públicos y estáticos tales como imágenes, hojas de estilo y archivos JavaScript. También es el lugar donde vive cada :term:`controlador frontal`::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $nucleo = new AppKernel('prod', false);
    $nucleo->loadClassCache();
    $nucleo->handle(Request::createFromGlobals())->send();

El archivo del controlador frontal (``app.php`` en este ejemplo) es el archivo PHP que realmente se ejecuta cuando utilizas una aplicación Symfony2 y su trabajo consiste en utilizar una clase del núcleo, ``AppKernel``, para arrancar la aplicación.

.. tip::

    Tener un controlador frontal significa que se utilizan diferentes y más flexibles direcciones URL que en una aplicación PHP típica. Cuando usamos un controlador frontal, las direcciones URL se formatean de la siguiente manera:

    .. code-block:: text

        http://localhost/app.php/hola/Ryan

    El controlador frontal, ``app.php``, se ejecuta y la URL "interna": ``/hola/Ryan`` es ruteada internamente con la configuración de enrutado.
    Al utilizar las reglas ``mod_rewrite`` de Apache, puedes forzar la ejecución del archivo ``app.php`` sin necesidad de especificarlo en la URL:

    .. code-block:: text

        http://localhost/hola/Ryan

Aunque los controladores frontales son esenciales en el manejo de cada petición, rara vez los tendrás que modificar o incluso pensar en ellos. Los vamos a mencionar brevemente de nuevo en la sección de `Entornos`_.

El directorio aplicación (``app``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Como vimos en el controlador frontal, la clase ``AppKernel`` es el punto de entrada principal de la aplicación y es la responsable de toda la configuración. Como tal, se almacena en el directorio ``app/``.

Esta clase debe implementar dos métodos que definen todo lo que Symfony necesita saber acerca de tu aplicación. Ni siquiera tienes que preocuparte de estos métodos durante el arranque - Symfony los llena por ti con parámetros predeterminados.

* ``registerBundles()``: Devuelve una matriz con todos los paquetes necesarios para ejecutar la aplicación (consulta :ref:`page-creation-bundles`);

* ``registerContainerConfiguration()``: Carga el archivo de configuración de recursos principal de la aplicación (consulta la sección `Configurando la aplicación`_);

En el desarrollo del día a día, generalmente vas a utilizar el directorio ``app/`` para modificar la configuración y los archivos de enrutado en el directorio ``app/config/`` (consulta la sección `Configurando la aplicación`_). Este también contiene el directorio caché de la aplicación (``app/cache``), un directorio de registro (``app/logs``) y un directorio para archivos de recursos a nivel de aplicación, tal como plantillas (``app/Resources``).
Aprenderás más sobre cada uno de estos directorios en capítulos posteriores.

.. _autoloading-introduction-sidebar:

.. sidebar:: Cargando automáticamente

    Al arrancar Symfony, un archivo especial - ``app/autoload.php`` - es incluido.
    Este archivo es responsable de configurar el cargador automático, el cual cargará automáticamente los archivos de tu aplicación desde el directorio ``src/`` y librerías de terceros del directorio ``vendor/``.

    Gracias al cargador automático, nunca tendrás que preocuparte de usar declaraciones ``include`` o ``require``. En cambio, Symfony2 utiliza el espacio de nombres de una clase para determinar su ubicación e incluir automáticamente el archivo en el instante en que necesitas una clase.

    El cargador automático ya está configurado para buscar en el directorio ``src/`` cualquiera de tus clases PHP. Para que trabaje la carga automática, el nombre de la clase y la ruta del archivo deben seguir el mismo patrón:

    .. code-block:: text

        Nombre de clase:
            Acme\HolaBundle\Controller\HolaController
        Ruta:
            src/Acme/HolaBundle/Controller/HolaController.php

    Típicamente, la única vez que tendrás que preocuparte por el archivo ``app/autoload.php`` es cuando estás incluyendo una nueva biblioteca de terceros en el directorio ``vendor/``. Para más información sobre la carga automática, consulta :doc:`Cómo cargar clases automáticamente </cookbook/tools/autoloader>`.

El directorio fuente (``src``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En pocas palabras, el directorio ``src/`` contiene todo el código real (código PHP, plantillas, archivos de configuración, estilo, etc.) que impulsa a *tu* aplicación.
De hecho, cuando desarrollas, la gran mayoría de tu trabajo se llevará a cabo dentro de uno o más paquetes creados en este directorio.

Pero, ¿qué es exactamente un :term:`paquete`?

.. _page-creation-bundles:

El sistema de paquetes
----------------------

Un paquete es similar a un complemento en otro software, pero aún mejor. La diferencia clave es que en Symfony2 *todo* es un paquete, incluyendo tanto la funcionalidad básica de la plataforma como el código escrito para tu aplicación.
Los paquetes son ciudadanos de primera clase en Symfony2. Esto te proporciona la flexibilidad para utilizar las características preconstruidas envasadas en `paquetes de terceros`_ o para distribuir tus propios paquetes. Además, facilita la selección y elección de las características por habilitar en tu aplicación y optimizarlas en la forma que desees.

.. note::

   Si bien, aquí vamos a cubrir lo básico, hay un capítulo dedicado completamente al tema de los :doc:`paquetes </cookbook/bundles/best_practices>`.

Un paquete simplemente es un conjunto estructurado de archivos en un directorio que implementa una sola característica. Puedes crear un ``BlogBundle``, un ``ForoBundle`` o un paquete para gestionar usuarios (muchos de ellos ya existen como paquetes de código abierto). Cada directorio contiene todo lo relacionado con esa característica, incluyendo archivos PHP, plantillas, hojas de estilo, archivos Javascript, pruebas y cualquier otra cosa necesaria.
Cada aspecto de una característica existe en un paquete y cada característica vive en un paquete.

Una aplicación se compone de paquetes tal como está definido en el método ``registerBundles()`` de la clase ``AppKernel``::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\SecurityBundle\SecurityBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            new Symfony\Bundle\MonologBundle\MonologBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Symfony\Bundle\DoctrineBundle\DoctrineBundle(),
            new Symfony\Bundle\AsseticBundle\AsseticBundle(),
            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
            new JMS\SecurityExtraBundle\JMSSecurityExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

Con el método ``registerBundles()``, tienes el control total sobre cuales paquetes utiliza tu aplicación (incluyendo los paquetes del núcleo de Symfony).

.. tip::

   Un paquete puede vivir en *cualquier lugar* siempre y cuando Symfony2 lo pueda cargar automáticamente (vía el autocargador configurado en ``app/autoload.php``).

Creando un paquete
~~~~~~~~~~~~~~~~~~

La edición estándar de Symfony viene con una práctica tarea que crea un paquete totalmente funcional para ti. Por supuesto, la creación manual de un paquete también es muy fácil.

Para mostrarte lo sencillo que es el sistema de paquetes, vamos a crear y activar un nuevo paquete llamado ``AcmePruebaBundle``.

.. tip::

    La parte ``Acme`` es sólo un nombre ficticio que debes sustituir por un "proveedor" que represente tu nombre u organización (por ejemplo, ``ABCPruebaBundle`` por alguna empresa llamada ``ABC``).

En primer lugar, crea un directorio ``src/Acme/PruebaBundle/`` y añade un nuevo archivo llamado ``AcmePruebaBundle.php``::

    // src/Acme/PruebaBundle/AcmePruebaBundle.php
    namespace Acme\PruebaBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmePruebaBundle extends Bundle
    {
    }

.. tip::

   El nombre ``AcmePruebaBundle`` sigue las :ref:`convenciones de nomenclatura de paquetes <bundles-naming-conventions>` estándar.
   También puedes optar por acortar el nombre del paquete simplemente a ``PruebaBundle`` al nombrar esta clase ``PruebaBundle`` (y el nombre del archivo ``PruebaBundle.php``).

Esta clase vacía es la única pieza que necesitamos para crear nuestro nuevo paquete. Aunque comúnmente está vacía, esta clase es poderosa y se puede utilizar para personalizar el comportamiento del paquete.

Ahora que hemos creado nuestro paquete, tenemos que activarlo a través de la clase ``AppKernel``::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...

            // registra tus paquetes
            new Acme\PruebaBundle\AcmePruebaBundle(),
        );
        // ...

        return $bundles;
    }

Y si bien aún no hace nada, ``AcmePruebaBundle`` está listo para utilizarlo.

Y aunque esto es bastante fácil, Symfony también proporciona una interfaz de línea de ordenes para generar una estructura de paquete básica:

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/PruebaBundle

Esto genera el esqueleto del paquete con un controlador básico, la plantilla y recursos de enrutado que se pueden personalizar. Aprenderás más sobre la línea de ordenes de las herramientas de Symfony2 más tarde.

.. tip::

   Cuando quieras crear un nuevo paquete o uses un paquete de terceros, siempre asegúrate de habilitar el paquete en ``registerBundles()``. Cuando usas la orden ``generate:bundle``, hace esto para ti.

Estructura de directorios de un paquete
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La estructura de directorios de un paquete es simple y flexible. De forma predeterminada, el sistema de paquetes sigue una serie de convenciones que ayudan a mantener el código consistente entre todos los paquetes Symfony2. Echa un vistazo a ``AcmeHolaBundle``, ya que contiene algunos de los elementos más comunes de un paquete:

* ``Controller/`` Contiene los controladores del paquete (por ejemplo, ``HolaController.php``);

* ``Resources/config/`` Contiene la configuración, incluyendo la configuración de enrutado (por ejemplo, ``routing.yml``);

* ``Resources/views/`` Contiene las plantillas organizadas por nombre de controlador (por ejemplo, ``Hola/index.html.twig``);

* ``Resources/public/`` Contiene recursos web (imágenes, hojas de estilo, etc.) y es copiado o enlazado simbólicamente al directorio ``web/`` del proyecto vía la orden de consola inicial ``assets:install``;

* ``Tests/`` Tiene todas las pruebas para el paquete.

Un paquete puede ser tan pequeño o tan grande como la característica que implementa. Este contiene sólo los archivos que necesita y nada más.

A medida que avances en el libro, aprenderás cómo persistir objetos a una base de datos, crear y validar formularios, crear traducciones para tu aplicación, escribir pruebas y mucho más. Cada uno de estos tiene su propio lugar y rol dentro del paquete.

Configurando la aplicación
--------------------------

La aplicación consiste de una colección de paquetes que representan todas las características y capacidades de tu aplicación. Cada paquete se puede personalizar a través de archivos de configuración escritos en YAML, XML o PHP. De forma predeterminada, el archivo de configuración principal vive en el directorio ``app/config/`` y se llama ``config.yml``, ``config.xml`` o ``config.php`` en función del formato que prefieras:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.ini }
            - { resource: security.yml }

        framework:
            secret:          %secret%
            charset:         UTF-8
            router:          { resource: "%kernel.root_dir%/config/routing.yml" }
            form:            true
            csrf_protection: true
            validation:      { enable_annotations: true }
            templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
            session:
                default_locale: %locale%
                auto_start:     true

        # Configuración Twig
        twig:
            debug:            %kernel.debug%
            strict_variables: %kernel.debug%

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.ini" />
            <import resource="security.yml" />
        </imports>

        <framework:config charset="UTF-8" secret="%secret%">
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <framework:form />
            <framework:csrf-protection />
            <framework:validation annotations="true" />
            <framework:templating assets-version="SomeVersionScheme">
                <framework:engine id="twig" />
            </framework:templating>
            <framework:session default-locale="%locale%" auto-start="true" />
        </framework:config>

        <!-- Configuración Twig -->
        <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" />

        <!-- ... -->

    .. code-block:: php

        $this->import('parameters.ini');
        $this->import('security.yml');

        $contenedor->loadFromExtension('framework', array(
            'secret'          => '%secret%',
            'charset'         => 'UTF-8',
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            'form'            => array(),
            'csrf-protection' => array(),
            'validation'      => array('annotations' => true),
            'templating'      => array(
                'engines' => array('twig'),
                #'assets_version' => "SomeVersionScheme",
            ),
            'session' => array(
                'default_locale' => "%locale%",
                'auto_start'     => true,
            ),
        ));

        // Configuración Twig
        $contenedor->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

        // ...

.. note::

   Aprenderás exactamente cómo cargar cada archivo/formato en la siguiente sección `Entornos`_.

Cada entrada de nivel superior como ``framework`` o ``twig`` define la configuración de un paquete específico. Por ejemplo, la clave ``framework`` define la configuración para el núcleo de Symfony ``FrameworkBundle`` e incluye la configuración de enrutado, plantillas, y otros sistemas del núcleo.

Por ahora, no te preocupes por las opciones de configuración específicas de cada sección.
El archivo de configuración viene con parámetros predeterminados. A medida que leas y explores más cada parte de Symfony2, aprenderás sobre las opciones de configuración específicas de cada característica.

.. sidebar:: Formatos de configuración

    A lo largo de los capítulos, todos los ejemplos de configuración muestran los tres formatos (YAML, XML y PHP). Cada uno tiene sus propias ventajas y desventajas. Tú eliges cual utilizar:

    * *YAML*: Sencillo, limpio y fácil de leer;

    * *XML*: Más poderoso que YAML a veces y es compatible con el autocompletado del IDE;

    * *PHP*: Muy potente, pero menos fácil de leer que los formatos de configuración estándar.

.. index::
   single: Entornos; Introducción

.. _environments-summary:

Entornos
--------

Una aplicación puede funcionar en diversos entornos. Los diferentes entornos comparten el mismo código PHP (aparte del controlador frontal), pero usan diferente configuración. Por ejemplo, un entorno de desarrollo ``dev`` registrará las advertencias y errores, mientras que un entorno de producción ``prod`` sólo registra los errores. Algunos archivos se vuelven a generar en cada petición en el entorno ``dev`` (para mayor comodidad de los desarrolladores), pero se memorizan en caché en el entorno ``prod``. Todos los entornos viven juntos en la misma máquina y ejecutan la misma aplicación.

Un proyecto Symfony2 generalmente comienza con tres entornos (``dev``, ``test`` y ``prod``), aunque la creación de nuevos entornos es fácil. Puedes ver tu aplicación en diferentes entornos con sólo cambiar el controlador frontal en tu navegador. Para ver la aplicación en el entorno ``dev``, accede a la aplicación a través del controlador frontal de desarrollo:

.. code-block:: text

    http://localhost/app_dev.php/hola/Ryan

Si deseas ver cómo se comportará tu aplicación en el entorno de producción, en su lugar, llama al controlador frontal ``prod``:

.. code-block:: text

    http://localhost/app.php/hola/Ryan

.. note::

   Si abres el archivo ``web/app.php``, encontrarás que está configurado explícitamente para usar el entorno ``prod``::

       $kernel = new AppKernel('prod', false);

   Puedes crear un nuevo controlador frontal para un nuevo entorno copiando el archivo y cambiando ``prod`` por algún otro valor.

Puesto que el entorno ``prod`` está optimizado para velocidad, la configuración, el enrutado y las plantillas Twig se compilan en clases PHP simples y se guardan en caché.
Cuando cambies las vistas en el entorno ``prod``, tendrás que borrar estos archivos memorizados en caché y así permitir su reconstrucción::

    php app/console cache:clear --env=prod

.. note::

    El entorno ``test`` se utiliza cuando se ejecutan pruebas automáticas y no se puede acceder directamente a través del navegador. Consulta el capítulo :doc:`Probando </book/testing>` para más detalles.

.. index::
   single: Entornos; Configuración

Configurando entornos
~~~~~~~~~~~~~~~~~~~~~

La clase ``AppKernel`` es responsable de cargar realmente el archivo de configuración de tu elección::

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

Ya sabes que la extensión ``.yml`` se puede cambiar a ``.xml`` o ``.php`` si prefieres usar XML o PHP para escribir tu configuración.
Además, observa que cada entorno carga su propio archivo de configuración. Considera el archivo de configuración para el entorno ``dev``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        framework:
            router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
            profiler: { only_exceptions: false }

        # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <framework:config>
            <framework:router resource="%kernel.root_dir%/config/routing_dev.xml" />
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- ... -->

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $contenedor->loadFromExtension('framework', array(
            'router'   => array('resource' => '%kernel.root_dir%/config/routing_dev.php'),
            'profiler' => array('only-exceptions' => false),
        ));

        // ...

La clave ``imports`` es similar a una declaración ``include`` PHP y garantiza que en primer lugar se carga el archivo de configuración principal (``config.yml``). El resto del archivo de configuración predeterminado aumenta el registro de eventos y otros ajustes conducentes a un entorno de desarrollo.

Ambos entornos ``prod`` y ``test`` siguen el mismo modelo: cada uno importa el archivo de configuración básico y luego modifica sus valores de configuración para adaptarlos a las necesidades específicas del entorno. Esto es sólo una convención, pero te permite reutilizar la mayor parte de tu configuración y personalizar sólo piezas puntuales entre entornos.

Resumen
-------

¡Enhorabuena! Ahora haz visto todos los aspectos fundamentales de Symfony2 y afortunadamente descubriste lo fácil y flexible que puede ser. Y si bien aún *hay muchas* características por venir, asegúrate de tener en cuenta los siguientes puntos básicos:

* La creación de una página es un proceso de tres pasos que involucran una **ruta**, un **controlador** y (opcionalmente) una **plantilla**.

* Cada proyecto contiene sólo unos cuantos directorios principales: ``web/`` (recursos web y controladores frontales), ``app/`` (configuración), ``src/`` (tus paquetes), y ``vendor/`` (código de terceros) (también hay un directorio ``bin/`` que se utiliza para ayudarte a actualizar las bibliotecas de proveedores);

* Cada característica en Symfony2 (incluyendo el núcleo de la plataforma Symfony2) está organizada en un *paquete*, el cual es un conjunto estructurado de archivos para esa característica;

* La **configuración** de cada paquete vive en el directorio ``app/config`` y se puede especificar en YAML, XML o PHP;

* Cada **entorno** es accesible a través de un diferente controlador frontal (por ejemplo, ``app.php`` y ``app_dev.php``) y carga un archivo de configuración diferente.

A partir de aquí, cada capítulo te dará a conocer más y más potentes herramientas y conceptos avanzados. Cuanto más sepas sobre Symfony2, tanto más apreciarás la flexibilidad de su arquitectura y el poder que te proporciona para desarrollar aplicaciones rápidamente.

.. _`Twig`: http://www.twig-project.org
.. _`paquetes de terceros`: http://symfony2bundles.org/
.. _`Edición estándar de Symfony`: http://symfony.com/download
