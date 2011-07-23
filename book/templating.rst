.. index::
   single: Plantillas

Creando y usando plantillas
===========================

Como sabes, el :doc:`Controlador </book/controller>` es responsable de manejar cada petición entrante en una aplicación Symfony2. En realidad, el controlador delega la mayor parte del trabajo pesado ​​a otros lugares para que el código se pueda probar y volver a utilizar. Cuando un controlador necesita generar HTML, CSS o cualquier otro contenido, que maneje el trabajo fuera del motor de plantillas.
En este capítulo, aprenderás cómo escribir potentes plantillas que puedes utilizar para devolver contenido al usuario, rellenar el cuerpo de correo electrónico y mucho más. Aprenderás métodos abreviados, formas inteligentes para extender las plantillas y cómo reutilizar código de plantilla.

.. index::
   single: Plantillas; ¿Qué es una plantilla?

Plantillas
----------

Una plantilla simplemente es un archivo de texto que puede generar cualquier formato basado en texto (HTML, XML, CSV, LaTeX...). El tipo de plantilla más familiar es una plantilla *PHP* - un archivo de texto interpretado por PHP que contiene una mezcla de texto y código PHP::

    <!DOCTYPE html>
    <html>
        <head>
            <title>¡Bienvenido a Symfony!</title>
        </head>
        <body>
            <h1><?php echo $titulo_pag ?></h1>

            <ul id="navegacion">
                <?php foreach ($navegacion as $elemento): ?>
                    <li>
                        <a href="<?php echo $elemento->getHref() ?>">
                            <?php echo $elemento->getCaption() ?>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

.. index:: Twig; Introducción

Pero Symfony2 contiene un lenguaje de plantillas aún más potente llamado `Twig`_.
Twig te permite escribir plantillas concisas y fáciles de leer que son más amigables para los diseñadores web y, de varias maneras, más poderosas que las plantillas PHP:

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
        <head>
            <title>¡Bienvenido a Symfony!</title>
        </head>
        <body>
            <h1>{{ titulo_pag }}</h1>

            <ul id="navegacion">
                {% for elemento in navegacion %}
                    <li><a href="{{ elemento.href }}">{{ elemento.caption }}</a></li>
                {% endfor %}
            </ul>
        </body>
    </html>

Twig define dos tipos de sintaxis especial:

* ``{{ ... }}``: "Dice algo": imprime una variable o el resultado de una expresión a la plantilla;

* ``{% ... %}``: "Hace algo": una **etiqueta** que controla la lógica de la plantilla; se utiliza para ejecutar declaraciones como bucles ``for``, por ejemplo.

.. note::

   Hay una tercer sintaxis utilizada para crear comentarios: ``{# esto es un comentario #}``.
   Esta sintaxis se puede utilizar en múltiples líneas como la sintaxis ``/* comentario */`` equivalente de PHP.

Twig también contiene **filtros**, los cuales modifican el contenido antes de reproducirlo.
El siguiente fragmento convierte a mayúsculas la variable ``titulo`` antes de reproducirla:

.. code-block:: jinja

    {{ titulo | upper }}

Twig viene con una larga lista de `etiquetas`_ y `filtros`_ que están disponibles de forma predeterminada. Incluso puedes `agregar tus propias extensiones`_ a Twig, según sea necesario.

.. tip::

    Registrar una extensión de Twig es tan fácil como crear un nuevo servicio y etiquetarlo con las :ref:`etiquetas <book-service-container-tags>` ``twig.extension``.

Como verás en toda la documentación, Twig también es compatible con funciones y fácilmente puedes añadir nuevas funciones. Por ejemplo, la siguiente función, utiliza una etiqueta ``for`` estándar y la función ``cycle`` para imprimir diez etiquetas div, alternando entre clases ``par`` e ``impar``:

.. code-block:: html+jinja

    {% for i in 0..10 %}
      <div class="{{ cycle(['par', 'impar'], i) }}">
        <!-- algún HTML aquí -->
      </div>
    {% endfor %}

A lo largo de este capítulo, mostraremos las plantillas de ejemplo en ambos formatos Twig y PHP.

.. sidebar:: ¿Porqué Twig?

    Las plantillas Twig están destinadas a ser simples y no procesar etiquetas PHP. Esto es por diseño: el sistema de plantillas Twig está destinado a expresar la presentación, no la lógica del programa. Cuanto más utilices Twig, más apreciarás y te beneficiarás de esta distinción. Y, por supuesto, todos los diseñadores web las amarán.
    
    Twig también puede hacer cosas que PHP no puede, como heredar verdaderas plantillas (las plantillas Twig se compilan hasta clases PHP que se heredan unas a otras), controlar los espacios en blanco, restringir un ambiente para prácticas, e incluir funciones y filtros personalizados que sólo afectan a las plantillas. Twig contiene características que facilitan la escritura de plantillas y estas son más concisas. Tomemos el siguiente ejemplo, el cual combina un bucle con una declaración ``if`` lógica:

    .. code-block:: html+jinja

        <ul>
            {% for usuario in usuarios %}
                <li>{{ usuario.nombreusuario }}</li>
            {% else %}
                <li>¡No hay usuarios!</li>
            {% endfor %}
        </ul>

.. index::
   pair: Twig; Caché

Guardando plantillas Twig en caché
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Twig es rápido. Cada plantilla Twig se compila hasta una clase PHP nativa que se reproduce en tiempo de ejecución. Las clases compiladas se encuentran en el directorio ``app/cache/{entorno}/twig`` (donde ``{entorno}`` es el entorno, tal como ``dev`` o ``prod``) y, en algunos casos, pueden ser útiles mientras depuras. Consulta la sección :ref:`environments-summary` para más información sobre los entornos.

Cuando está habilitado el modo ``debug`` (comúnmente en el entorno ``dev``) al realizar cambios a una plantilla Twig, esta se vuelve a compilar automáticamente. Esto significa que durante el desarrollo, felizmente, puedes realizar cambios en una plantilla Twig e inmediatamente ver las modificaciones sin tener que preocuparte de limpiar ninguna caché.

Cuando el modo ``debug`` está desactivado (comúnmente en el entorno ``prod``), sin embargo, debes borrar el directorio de caché para regenerar las plantillas. Recuerda hacer esto al desplegar tu aplicación.

.. index::
   single: Plantillas; Herencia

Plantillas, herencia y diseño
-----------------------------

A menudo, las plantillas en un proyecto comparten elementos comunes, como el encabezado, pie de página, barra lateral o más. En Symfony2, nos gusta pensar en este problema de forma diferente: una plantilla se puede decorar con otra. Esto funciona exactamente igual que las clases PHP: la herencia de plantillas nos permite crear un "diseño" de plantilla base que contiene todos los elementos comunes de tu sitio definidos como **bloques** (piensa en "clases PHP con métodos base"). Una plantilla hija puede extender el diseño base y reemplazar cualquiera de sus bloques (piensa en las "subclases PHP que sustituyen determinados métodos de su clase padre").

En primer lugar, crea un archivo con tu diseño base:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block titulo %}Aplicación de prueba{% endblock %}</title>
            </head>
            <body>
                <div id="barralateral">
                    {% block barralateral %}
                    <ul>
                        <li><a href="/">Portada</a></li>
                        <li><a href="/blog">Blog</a></li>
                    </ul>
                    {% endblock %}
                </div>

                <div id="contenido">
                    {% block body %}{% endblock %}
                </div>
            </body>
        </html>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('titulo', 'Aplicación de prueba') ?></title>
            </head>
            <body>
                <div id="barralateral">
                    <?php if ($view['slots']->has('barralateral'): ?>
                        <?php $view['slots']->output('barralateral') ?>
                    <?php else: ?>
                        <ul>
                            <li><a href="/">Portada</a></li>
                            <li><a href="/blog">Blog</a></li>
                        </ul>
                    <?php endif; ?>
                </div>

                <div id="contenido">
                    <?php $view['slots']->output('body') ?>
                </div>
            </body>
        </html>

.. note::

    Aunque la explicación sobre la herencia de plantillas será en términos de Twig, la filosofía es la misma entre plantillas Twig y PHP.

Esta plantilla define el esqueleto del documento HTML base de una simple página de dos columnas. En este ejemplo, se definen tres áreas ``{% block %}`` (``titulo``, ``barralateral`` y ``body``). Una plantilla hija puede sustituir cada uno de los bloques o dejarlos con su implementación predeterminada. Esta plantilla también se podría reproducir directamente. En este caso, los bloques ``titulo``, ``barralateral`` y ``body`` simplemente mantienen los valores predeterminados usados en esta plantilla.

Una plantilla hija podría tener este aspecto:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block titulo %}Mis interesantes entradas del blog{% endblock %}

        {% block body %}
            {% for entrada in entradas_blog %}
                <h2>{{ entrada.titulo }}</h2>
                <p>{{ entrada.cuerpo }}</p>
            {% endfor %}
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/BlogBundle/Resources/views/Blog/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        <?php $view['slots']->set('titulo', 'Mis interesantes entradas del blog') ?>

        <?php $view['slots']->start('body') ?>
            <?php foreach ($entradas_blog as $entrada): ?>
                <h2><?php echo $entrada->getTitulo() ?></h2>
                <p><?php echo $entrada->getCuerpo() ?></p>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

.. note::

   La plantilla padre se identifica mediante una sintaxis de cadena especial (``::base.html.twig``) la cual indica que la plantilla vive en el directorio ``app/Resources/views`` del proyecto. Esta convención de nomenclatura se explica completamente en :ref:`template-naming-locations`.

La clave para la herencia de plantillas es la etiqueta ``{% extends %}``. Esto le indica al motor de plantillas que primero evalúe la plantilla base, la cual establece el diseño y define varios bloques. Luego reproduce la plantilla hija, en ese momento, los bloques ``titulo`` y ``body`` del padre son reemplazados por los de la hija. Dependiendo del valor de ``entradas_blog``, el resultado sería algo como esto::

    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>Mis interesantes entradas del blog</title>
        </head>
        <body>
            <div id="barralateral">
                <ul>
                    <li><a href="/">Portada</a></li>
                    <li><a href="/blog">Blog</a></li>
                </ul>
            </div>

            <div id="contenido">
                <h2>Mi primer mensaje</h2>
                <p>El cuerpo del primer mensaje.</p>

                <h2>Otro mensaje</h2>
                <p>El cuerpo del segundo mensaje.</p>
            </div>
        </body>
    </html>

Ten en cuenta que como en la plantilla hija no haz definido un bloque ``barralateral``, en su lugar, se utiliza el valor de la plantilla padre. Una plantilla padre, de forma predeterminada, siempre utiliza una etiqueta ``{% block %}`` para el contenido.

Puedes utilizar tantos niveles de herencia como quieras. En la siguiente sección, explicaremos un modelo común de tres niveles de herencia junto con la forma en que se organizan las plantillas dentro de un proyecto Symfony2.

Cuando trabajes con la herencia de plantillas, ten en cuenta los siguientes consejos:

* Si utilizas ``{% extends %}`` en una plantilla, esta debe ser la primer etiqueta en esa plantilla.

* Mientras más etiquetas ``{% block %}`` tengas en tu plantilla base, mejor.
  Recuerda, las plantillas hijas no tienen que definir todos los bloques de los padres, por lo tanto crea tantos bloques en tus plantillas base como desees y dale a cada uno un valor predeterminado razonable. Mientras más bloques tengan tus plantillas base, más flexible será tu diseño.

* Si te encuentras duplicando contenido en una serie de plantillas, probablemente significa que debes mover el contenido a un ``{% block %}`` en una plantilla padre.
  En algunos casos, una mejor solución podría ser mover el contenido a una nueva plantilla e incluirla con ``include`` (consulta :ref:`incluyendo-plantillas`).

* Si necesitas conseguir el contenido de un bloque de la plantilla padre, puedes usar la función ``{{ parent() }}``. Esta es útil si deseas añadir contenido a un bloque padre en vez de reemplazarlo por completo:

    .. code-block:: html+jinja

        {% block barralateral %}
            <h3>Tabla de contenido</h3>
            ...
            {{ parent() }}
        {% endblock %}

.. index::
   single: Plantillas; Convenciones de nomenclatura
   single: Plantillas; Ubicación de archivos

.. _template-naming-locations:

Nomenclatura y ubicación de plantillas
--------------------------------------

De forma predeterminada, las plantillas pueden vivir en dos diferentes lugares:

* ``app/Resources/views/`` El directorio de las ``vistas`` de la aplicación puede contener toda las plantillas base de la aplicación (es decir, los diseños de tu aplicación), así como plantillas que sustituyen a plantillas de paquetes (consulta :ref:`overriding-bundle-templates`);

* ``ruta/al/paquete/Resources/views/`` Cada paquete contiene sus plantillas en su directorio ``Resources/views`` (y subdirectorios). La mayoría de las plantillas viven dentro de un paquete.

Symfony2 utiliza una sintaxis de cadena **paquete**:**controlador**:**plantilla** para las plantillas. Esto permite diferentes tipos de plantilla, dónde cada una vive en un lugar específico:

* ``AcmeBlogBundle:Blog:index.html.twig``: Esta sintaxis se utiliza para especificar una plantilla para una página específica. Las tres partes de la cadena, cada una separada por dos puntos (``:``), significan lo siguiente:

    * ``AcmeBlogBundle``: (*paquete*) la plantilla vive dentro de ``AcmeBlogBundle`` (por ejemplo, ``src/Acme/BlogBundle``);

    * ``Blog``: (*controlador*) indica que la plantilla vive dentro del subdirectorio ``Blog`` de ``Resources/views``;

    * ``index.html.twig``: (*plantilla*) el nombre real del archivo es ``index.html.twig``.

  Suponiendo que ``AcmeBlogBundle`` vive en ``src/Acme/BlogBundle``, la ruta final para el diseño debería ser `src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``.

* ``AcmeBlogBundle::base.html.twig``: Esta sintaxis se refiere a una plantilla base que es específica para ``AcmeBlogBundle``. Puesto que falta la porción central, "controlador", (por ejemplo, ``Blog``), la plantilla vive en ``Resources/views/base.html.twig`` dentro de ``AcmeBlogBundle``.

* ``::base.html.twig``: Esta sintaxis se refiere a una plantilla o diseño base de la aplicación. Observa que la cadena comienza con dobles dos puntos (``::``), lo cual significa que faltan ambas porciones *paquete* y *controlador*. Esto significa que la plantilla no se encuentra en ningún paquete, sino en el directorio raíz de la aplicación ``app/Resources/views/``.

En la sección :ref:`overriding-bundle-templates`, encontrarás cómo puedes sustituir cada plantilla que vive dentro de ``AcmeBlogBundle``, por ejemplo, colocando una plantilla del mismo nombre en el directorio ``app/Resources/AcmeBlog/views/``. Esto nos da el poder para sustituir plantillas de cualquier paquete de terceros.

.. tip::

    Esperemos que la sintaxis de nomenclatura de plantilla te resulte familiar - es la misma convención de nomenclatura utilizada para referirse al :ref:`controller-string-syntax`.

Sufijo de plantilla
~~~~~~~~~~~~~~~~~~~

El formato **paquete**:**controlador**:**plantilla** de cada plantilla, especifica *dónde* se encuentra el archivo de plantilla. Cada nombre de plantilla también cuenta con dos extensiones que especifican el *formato* y *motor* de esa plantilla.

* **AcmeBlogBundle:Blog:index.html.twig** - formato HTML, motor Twig

* **AcmeBlogBundle:Blog:index.html.php** - formato HTML, motor PHP

* **AcmeBlogBundle:Blog:index.css.twig** - formato CSS, motor Twig

De forma predeterminada, cualquier plantilla Symfony2 se puede escribir en Twig o PHP, y la última parte de la extensión (por ejemplo ``.twig`` o ``.php``) especifica cuál de los dos *motores* se debe utilizar. La primera parte de la extensión, (por ejemplo ``.html``, ``.css``, etc.) es el formato final que la plantilla debe generar. A diferencia del motor, el cual determina cómo analiza Symfony2 la plantilla, esta simplemente es una táctica de organización utilizada en caso de que el mismo recurso se tenga que reproducir como HTML (``index.html.twig``), XML (``index.xml.twig``), o cualquier otro formato. Para más información, lee la sección :ref:`template-formats`.

.. note::

   Los "motores" disponibles se pueden configurar e incluso agregar nuevos motores.
   Consulta :ref:`Configuración de plantillas <template-configuration>` para más detalles.

.. index::
   single: Plantillas; Etiquetas y ayudantes
   single: Plantillas; Ayudantes

Etiquetas y ayudantes
---------------------

Ya entendiste los conceptos básicos de las plantillas, cómo son denominadas y cómo utilizar la herencia en plantillas. Las partes más difíciles ya quedaron atrás. En esta sección, aprenderás acerca de un amplio grupo de herramientas disponibles para ayudarte a realizar las tareas de plantilla más comunes, como la inclusión de otras plantillas, enlazar páginas e incluir imágenes.

Symfony2 viene con varias etiquetas Twig especializadas y funciones que facilitan la labor del diseñador de la plantilla. En PHP, el sistema de plantillas extensible ofrece un sistema de *ayudantes* que proporciona funciones útiles en el contexto de la plantilla.

Ya hemos visto algunas etiquetas integradas en Twig (``{% block %}`` y ``{% extends %}``), así como un ejemplo de un ayudante PHP (consulta ``$view['slot']``). Aprendamos un poco más...

.. index::
   single: Plantillas; Incluyendo otras plantillas

.. _incluyendo-plantillas:

Incluyendo otras plantillas
~~~~~~~~~~~~~~~~~~~~~~~~~~~

A menudo querrás incluir la misma plantilla o fragmento de código en varias páginas diferentes. Por ejemplo, en una aplicación con "artículos de noticias", el código de la plantilla que muestra un artículo se puede utilizar en la página de detalles del artículo, en una página que muestra los artículos más populares, o en una lista de los últimos artículos.

Cuando necesites volver a utilizar un trozo de código PHP, normalmente mueves el código a una nueva clase o función PHP. Lo mismo es cierto para las plantillas. Al mover el código de la plantilla a su propia plantilla, este se puede incluir en cualquier otra plantilla. En primer lugar, crea la plantilla que tendrás que volver a usar.

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticuloBundle/Resources/views/Articulo/articuloDetalles.html.twig #}
        <h1>{{ articulo.titulo }}</h1>
        <h3 class="lineapor">por {{ articulo.nombreAutor }}</h3>

        <p>
          {{ articulo.cuerpo }}
        </p>

    .. code-block:: php

        <!-- src/Acme/ArticuloBundle/Resources/views/Articulo/articuloDetalles.html.php -->
        <h2><?php echo $articulo->getTitulo() ?></h2>
        <h3 class="lineapor">por <?php echo $articulo->getNombreAutor() ?></h3>

        <p>
          <?php echo $articulo->getCuerpo() ?>
        </p>

Incluir esta plantilla en cualquier otra plantilla es sencillo:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticuloBundle/Resources/Articulo/lista.html.twig #}
        {% extends 'AcmeArticuloBundle::base.html.twig' %}

        {% block body %}
            <h1>Artículos recientes<h1>

            {% for articulo in articulos %}
                {% include 'AcmeArticuloBundle:Articulo:articuloDetalles.html.twig' with {'articulo': articulo} %}
            {% endfor %}
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/ArticuloBundle/Resources/Articulo/lista.html.php -->
        <?php $view->extend('AcmeArticuloBundle::base.html.php') ?>

        <?php $view['slots']->start('body') ?>
            <h1>Artículos recientes</h1>

            <?php foreach ($articulos as $articulo): ?>
                <?php echo $view->render('AcmeArticuloBundle:Articulo:articuloDetalles.html.php', array('articulo' => $articulo)) ?>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

La plantilla se incluye con la etiqueta ``{% include %}``. Observa que el nombre de la plantilla sigue la misma convención típica. La plantilla ``articuloDetalles.html.twig`` utiliza una variable ``articulo``. Esta es proporcionada por la plantilla ``lista.html.twig`` utilizando la orden ``with``.

.. tip::

    La sintaxis ``{'articulo': articulo}`` es la sintaxis estándar de Twig para asignar |hash| (es decir, una matriz con claves nombradas). Si tuviéramos que pasar varios elementos, se vería así: ``{'foo': foo, 'bar': bar}``.

.. index::
   single: Plantillas; Incrustando acción

.. _templating-embedding-controller:

Incrustando controladores
~~~~~~~~~~~~~~~~~~~~~~~~~

En algunos casos, es necesario hacer algo más que incluir una simple plantilla. Supongamos que en tu diseño tienes una barra lateral, la cual contiene los tres artículos más recientes.
Recuperar los tres artículos puede incluir consultar la base de datos o realizar otra pesada lógica ​​que no se puede hacer desde dentro de una plantilla.

La solución es simplemente insertar el resultado de un controlador en tu plantilla entera. En primer lugar, crea un controlador que reproduzca un cierto número de artículos recientes:

.. code-block:: php

    // src/Acme/ArticuloBundle/Controller/ArticuloController.php

    class ArticuloController extends Controller
    {
        public function arcticulosRecientesAction($max = 3)
        {
            // hace una llamada a la base de datos u otra lógica para obtener los "$max" artículos más recientes
            $articulos = ...;

            return $this->render('AcmeArticuloBundle:Articulo:listaRecientes.html.twig', array('articulos' => $articulos));
        }
    }

La plantilla ``listaRecientes`` es perfectamente clara:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticuloBundle/Resources/views/Articulo/listaRecientes.html.twig #}
        {% for articulo in articulos %}
          <a href="/articulo/{{ articulo.ficha }}">
              {{ articulo.titulo }}
          </a>
        {% endfor %}

    .. code-block:: php

        <!-- src/Acme/ArticuloBundle/Resources/views/Articulo/listaRecientes.html.php -->
        <?php foreach ($articulos in $articulo): ?>
            <a href="/articulo/<?php echo $articulo->getFicha() ?>">
                <?php echo $articulo->getTitulo() ?>
            </a>
        <?php endforeach; ?>

.. note::

    Ten en cuenta que en este ejemplo hemos falsificado y codificado la URL del artículo (por ejemplo ``/articulo/ficha``). Esta es una mala práctica. En la siguiente sección, aprenderás cómo hacer esto correctamente.

Para incluir el controlador, tendrás que referirte a él utilizando la sintaxis de cadena estándar para controladores (es decir, **paquete**:**controlador**:**acción**):

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        ...

        <div id="barralateral">
            {% render "AcmeArticuloBundle:Articulo:articulosRecientes" with {'max': 3} %}
        </div>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        ...

        <div id="barralateral">
            <?php echo $view['actions']->render('AcmeArticuloBundle:Articulo:articulosRecientes', array('max' => 3)) ?>
        </div>

Cada vez que te encuentres necesitando una variable o una pieza de información a la que una plantilla no tiene acceso, considera reproducir un controlador.
Los controladores se ejecutan rápidamente y promueven la buena organización y reutilización de código.

.. index::
   single: Plantillas; Enlazando páginas

Enlazando páginas
~~~~~~~~~~~~~~~~~

La creación de enlaces a otras páginas en tu aplicación es uno de los trabajos más comunes de una plantilla. En lugar de codificar las direcciones URL en las plantillas, utiliza la función ``path`` de Twig (o el ayudante ``router`` en PHP) para generar direcciones URL basadas en la configuración de enrutado. Más tarde, si deseas modificar la URL de una página en particular, todo lo que tienes que hacer es cambiar la configuración de enrutado, las plantillas automáticamente generarán la nueva URL.

En primer lugar, crea el enlace a la página "*_bienvenida*", la cual es accesible a través de la siguiente configuración de enrutado:

.. configuration-block::

    .. code-block:: yaml

        _bienvenida:
            pattern:  /
            defaults: { _controller: AcmeDemoBundle:Bienvenida:index }

    .. code-block:: xml

        <route id="_bienvenida" pattern="/">
            <default key="_controller">AcmeDemoBundle:Bienvenida:index</default>
        </route>

    .. code-block:: php

        $coleccion = new RouteCollection();
        $coleccion->add('_bienvenida', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Bienvenida:index',
        )));

        return $coleccion;

Para enlazar a la página, sólo tienes que utilizar la función ``path`` de Twig y referir la ruta:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('_bienvenida') }}">Portada</a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('_bienvenida') ?>">Portada</a>

Como era de esperar, esto genera la URL ``/``. Vamos a ver cómo funciona esto con una ruta más complicada:

.. configuration-block::

    .. code-block:: yaml

        articulo_show:
            pattern:  /articulo/{ficha}
            defaults: { _controller: AcmeArticuloBundle:Articulo:show }

    .. code-block:: xml

        <route id="articulo_show" pattern="/articulo/{ficha}">
            <default key="_controller">AcmeArticuloBundle:Articulo:show</default>
        </route>

    .. code-block:: php

        $coleccion = new RouteCollection();
        $coleccion->add('articulo_show', new Route('/articulo/{ficha}', array(
            '_controller' => 'AcmeArticuloBundle:Articulo:show',
        )));

        return $coleccion;

En este caso, es necesario especificar el nombre de la ruta (``articulo_show``) y un valor para el parámetro ``{ficha}``. Usando esta ruta, vamos a volver a la plantilla ``listaRecientes`` de la sección anterior y enlazar los artículos correctamente:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/ArticuloBundle/Resources/views/Articulo/listaRecientes.html.twig #}
        {% for articulo in articulos %}
          <a href="{{ path('articulo_show', { 'ficha': articulo.ficha }) }}">
              {{ articulo.titulo }}
          </a>
        {% endfor %}

    .. code-block:: php

        <!-- src/Acme/ArticuloBundle/Resources/views/Articulo/listaRecientes.html.php -->
        <?php foreach ($articulos in $articulo): ?>
            <a href="<?php echo $view['router']->generate('articulo_show', array('ficha' => $articulo->getFicha()) ?>">
                <?php echo $articulo->getTitulo() ?>
            </a>
        <?php endforeach; ?>

.. tip::

    También puedes generar una URL absoluta utilizando la función ``url`` de Twig:

    .. code-block:: html+jinja

        <a href="{{ url('_bienvenida') }}">Portada</a>

    Lo mismo se puede hacer en plantillas PHP pasando un tercer argumento al método ``generate()``:

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('_bienvenida', array(), true) ?>">Portada</a>

.. index::
   single: Plantillas; Enlazando activos

Enlazando activos
~~~~~~~~~~~~~~~~~

Las plantillas también se refieren comúnmente a imágenes, JavaScript, hojas de estilo y otros activos. Por supuesto, puedes codificar la ruta de estos activos (por ejemplo ``/images/logo.png``), pero Symfony2 ofrece una opción más dinámica a través de la función ``assets`` de Twig:

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

        <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    .. code-block:: php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

        <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

El propósito principal de la función ``asset`` es hacer más portátil tu aplicación.
Si tu aplicación vive en la raíz de tu anfitrión (por ejemplo, http://ejemplo.com), entonces las rutas reproducidas deben ser ``/images/logo.png``. Pero si tu aplicación vive en un subdirectorio (por ejemplo, http://ejemplo.com/mi_aplic), cada ruta de activo debe reproducir el subdirectorio (por ejemplo ``/mi_aplic/images/logo.png``). La función ``asset`` se encarga de esto determinando cómo se está utilizando tu aplicación y generando las rutas correctas en consecuencia.

.. index::
   single: Plantillas; Incluyendo hojas de estilo y Javascript
   single: Hojas de estilo; Incluyendo hojas de estilo
   single: Javascript; Incluyendo Javascript

Incluyendo hojas de estilo y JavaScript en Twig
-----------------------------------------------

Ningún sitio estaría completo sin incluir archivos de JavaScript y hojas de estilo.
En Symfony, la inclusión de estos activos se maneja elegantemente, aprovechando la herencia de plantillas de Symfony.

.. tip::

    Esta sección te enseñará la filosofía detrás de la inclusión de activos como hojas de estilo y Javascript en Symfony. Symfony también empaca otra biblioteca, llamada ``assetic``, la cual sigue esta filosofía, pero te permite hacer cosas mucho más interesantes con esos activos. Para más información sobre el uso de ``assetic`` consulta :doc:`/cookbook/assetic/asset_management`.


Comienza agregando dos bloques a la plantilla base que mantendrá tus activos: uno llamado ``stylesheet`` dentro de la etiqueta ``head`` y otro llamado ``javascript`` justo por encima de la etiqueta de cierre ``body``. Estos bloques deben contener todas las hojas de estilo y archivos Javascript que necesitas en tu sitio:

.. code-block:: html+jinja

    {# 'app/Resources/views/base.html.twig' #}
    <html>
        <head>
            {# ... #}

            {% block stylesheets %}
                <link href="{{ asset('/css/main.css') }}" type="text/css" rel="stylesheet" />
            {% endblock %}
        </head>
        <body>
            {# ... #}

            {% block javascripts %}
                <script src="{{ asset('/js/main.js') }}" type="text/javascript"></script>
            {% endblock %}
        </body>
    </html>

¡Eso es bastante fácil! Pero ¿y si es necesario incluir una hoja de estilo extra o archivos Javascript desde una plantilla hija? Por ejemplo, supongamos que tienes una página de contacto y necesitas incluir una hoja de estilo ``contacto.css`` *sólo* en esa página. Desde dentro de la plantilla de la página de contacto, haz lo siguiente:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Contacto/contacto.html.twig #}
    {# extends '::base.html.twig' #}

    {% block stylesheets %}
        {{ parent() }}

        <link href="{{ asset('/css/contacto.css') }}" type="text/css" rel="stylesheet" />
    {% endblock %}

    {# ... #}

En la plantilla hija, sólo tienes que reemplazar el bloque ``stylesheet`` y poner tu nueva etiqueta de hoja de estilo dentro de ese bloque. Por supuesto, debido a que deseas añadir al contenido del bloque padre (y no *cambiarlo* en realidad), debes usar la función ``parent()`` de Twig para incluir todo, desde el bloque ``stylesheet`` de la plantilla base.

El resultado final es una página que incluye ambas hojas de estilo ``main.css`` y ``contacto.css``.

.. index::
   single: Plantillas; La plantilla servicio

Configurando y usando el servicio ``plantilla``
-----------------------------------------------

El corazón del sistema de plantillas en Symfony2 es el ``motor`` de plantillas.
Este objeto especial es el encargado de reproducir las plantillas y devolver su contenido. Cuando reproduces una plantilla en un controlador, por ejemplo, en realidad estás usando el motor del servicio de plantillas. Por ejemplo:

.. code-block:: php

    return $this->render('AcmeArticuloBundle:Articulo:index.html.twig');

es equivalente a

.. code-block:: php

    $motor = $this->container->get('templating');
    $contenido = $motor->render('AcmeArticuloBundle:Articulo:index.html.twig');

    return $respuesta = new Response($contenido);

.. _template-configuration:

El motor de plantillas (o "servicio") está configurado para funcionar automáticamente al interior de Symfony2. Por supuesto, puedes configurar más en el archivo de configuración de la aplicación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'] }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating>
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig'),
            ),
        ));

Disponemos de muchas opciones de configuración y están cubiertas en el :doc:`Apéndice Configurando </reference/configuration/framework>`.

.. note::

   En el motor de ``twig`` es obligatorio el uso del ``webprofiler`` (así como muchos otros paquetes de terceros).

.. index::
    single; Plantilla; Sustituyendo plantillas

.. _overriding-bundle-templates:

Sustituyendo plantillas del paquete
-----------------------------------

La comunidad de Symfony2 se enorgullece de crear y mantener paquetes de alta calidad (consulta `Symfony2Bundles.org`_) para una gran cantidad de diferentes características.
Una vez que utilizas un paquete de terceros, probablemente necesites redefinir y personalizar una o más de sus plantillas.

Supongamos que hemos incluido el paquete imaginario ``AcmeBlogBundle`` de código abierto en el proyecto (por ejemplo, en el directorio ``src/Acme/BlogBundle``). Y si bien estás muy contento con todo, deseas sustituir la página "lista" del blog para personalizar el marcado específicamente para tu aplicación. Al excavar en el controlador del ``Blog`` de ``AcmeBlogBundle``, encuentras lo siguiente::

    public function indexAction()
    {
        $blogs = // cierta lógica para recuperar las entradas

        $this->render('AcmeBlogBundle:Blog:index.html.twig', array('blogs' => $blogs));
    }

Al reproducir ``AcmeBlogBundle:Blog:index.html.twig``, en realidad Symfony2 busca la plantilla en dos diferentes lugares:

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

Para sustituir la plantilla del paquete, sólo tienes que copiar la plantilla ``index.html.twig`` del paquete a ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig`` (el directorio ``app/Resources/AcmeBlogBundle`` no existe, por lo tanto tendrás que crearlo). Ahora eres libre de personalizar la plantilla para tu aplicación.

Esta lógica también aplica a las plantillas base del paquete. Supongamos también que cada plantilla en ``AcmeBlogBundle`` hereda de una plantilla base llamada ``AcmeBlogBundle::base.html.twig``. Al igual que antes, Symfony2 buscará la plantilla en los dos siguientes lugares:

#. ``app/Resources/AcmeBlogBundle/views/base.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/base.html.twig``

Una vez más, para sustituir la plantilla, sólo tienes que copiarla desde el paquete a ``app/Resources/AcmeBlogBundle/views/base.html.twig``. Ahora estás en libertad de personalizar esta copia como mejor te parezca.

Si retrocedes un paso, verás que Symfony2 siempre empieza a buscar una plantilla en el directorio ``app/Resources/{NOMBRE_PAQUETE}/views/``. Si la plantilla no existe allí, continúa buscando dentro del directorio ``Resources/views`` del propio paquete. Esto significa que todas las plantillas del paquete se pueden sustituir colocándolas en el subdirectorio ``app/Resources`` correcto.

.. _templating-overriding-core-templates:

.. index::
    single; Plantilla; Sustituyendo plantillas de excepción

Sustituyendo plantillas del núcleo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Puesto que la plataforma Symfony2 en sí misma sólo es un paquete, las plantillas del núcleo se pueden sustituir de la misma manera. Por ejemplo, ``FrameworkBundle`` del núcleo contiene una serie de diferentes plantillas para "excepción" y "error" que se pueden sustituir copiando cada una del directorio ``Resources/views/Exception`` de ``FrameworkBundle`` al directorio... ¡adivinaste! ``app/Resources/FrameworkBundle/views/Exception``.

.. index::
   single: Plantillas; Patrón de herencia de tres niveles

Herencia de tres niveles
------------------------

Una manera común de usar la herencia es utilizar un enfoque de tres niveles. Este método funciona a la perfección con los tres diferentes tipos de plantillas que acabamos de cubrir:

* Crea un archivo ``app/Resources/views/base.html.twig`` que contenga el diseño principal para tu aplicación (como en el ejemplo anterior). Internamente, esta plantilla se llama ``::base.html.twig``;

* Crea una plantilla para cada "sección" de tu sitio. Por ejemplo, un ``AcmeBlogBundle``, tendría una plantilla llamada ``AcmeBlogBundle::base.html.twig`` que sólo contiene los elementos específicos de la sección blog;

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/base.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            <h1>Aplicación Blog</h1>

            {% block contenido %}{% endblock %}
        {% endblock %}

* Crea plantillas individuales para cada página y haz que cada una extienda la plantilla de la sección adecuada. Por ejemplo, la página "index" se llama algo parecido a ``AcmeBlogBundle:Blog:index.html.twig`` y lista las entradas del blog real.

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends 'AcmeBlogBundle::base.html.twig' %}

        {% block contenido %}
            {% for entrada in entradas_blog %}
                <h2>{{ entrada.titulo }}</h2>
                <p>{{ entrada.cuerpo;/p>
            {% endfor %}
        {% endblock %}

Ten en cuenta que esta plantilla extiende la plantilla de la sección - (``AcmeBlogBundle::base.html.twig``), que a su vez, extiende el diseño base de la aplicación (``::base.html.twig``).
Este es el modelo común de la herencia de tres niveles.

Cuando construyas tu aplicación, podrás optar por este método o, simplemente, hacer que cada plantilla de página extienda directamente la plantilla base de tu aplicación (por ejemplo, ``{% extends '::base.html.twig' %}``). El modelo de tres plantillas es un método de las buenas prácticas utilizadas por los paquetes de proveedores a fin de que la plantilla base de un paquete se pueda sustituir fácilmente para extender correctamente el diseño base de tu aplicación.

.. index::
   single: Plantillas; Mecanismo de escape

Mecanismo de escape
-------------------

Cuando generas HTML a partir de una plantilla, siempre existe el riesgo de que una variable de plantilla pueda producir HTML involuntario o código peligroso de lado del cliente. El resultado es que el contenido dinámico puede romper el código HTML de la página resultante o permitir a un usuario malicioso realizar un ataque de `Explotación de vulnerabilidades del sistema`_ (Cross Site Scripting XSS). Considera este ejemplo clásico:

.. configuration-block::

    .. code-block:: jinja

        Hola {{ nombre }}

    .. code-block:: php

        Hola <?php echo $nombre ?>!

Imagina que el usuario introduce el siguiente código como su nombre::

    <script>alert('hola!')</script>

Sin ningún tipo de mecanismo de escape, la plantilla resultante provocaría que aparezca un cuadro de alerta JavaScript::

    Hola <script>alert('hola!')</script>

Y aunque esto parece inofensivo, si un usuario puede llegar hasta aquí, ese mismo usuario también debe ser capaz de escribir código JavaScript malicioso que realice acciones dentro de la zona segura de un usuario legítimo sin saberlo.

La respuesta al problema es el mecanismo de escape. Con el mecanismo de escape, reproduces la misma plantilla sin causar daño alguno, y, literalmente, imprimes en pantalla la etiqueta ``script``::

    Hola &lt;script&gt;alert(&#39;holae&#39;)&lt;/script&gt;

Twig y los sistemas de plantillas PHP abordan el problema de diferentes maneras.
Si estás utilizando Twig, el mecanismo de escape por omisión está activado y tu aplicación está protegida.
En PHP, el mecanismo de escape no es automático, lo cual significa que, de ser necesario, necesitas escapar todo manualmente.

Mecanismo de escape en Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si estás utilizando las plantillas de Twig, entonces el mecanismo de escape está activado por omisión. Esto significa que estás protegido fuera de la caja de las consecuencias no intencionales del código presentado por los usuarios. De forma predeterminada, el mecanismo de escape asume que el contenido se escapó para salida HTML.

En algunos casos, tendrás que desactivar el mecanismo de escape cuando estás reproduciendo una variable de confianza y marcado que no se debe escapar.
Supongamos que los usuarios administrativos están autorizados para escribir artículos que contengan código HTML. De forma predeterminada, Twig debe escapar el cuerpo del artículo. Para reproducirlo normalmente, agrega el filtro ``raw``: ``{{ articulo.cuerpo | raw }}``.

También puedes desactivar el mecanismo de escape dentro de una área ``{% block %}`` o para una plantilla completa. Para más información, consulta la documentación del `Mecanismo de escape`_  Twig.

Mecanismo de escape en PHP
~~~~~~~~~~~~~~~~~~~~~~~~~~

El mecanismo de escape no es automático cuando utilizas plantillas PHP. Esto significa que a menos que escapes una variable expresamente, no estás protegido. Para utilizar el mecanismo de escape, usa el método especial de la vista ``escape()``::

    Hola <?php echo $view->escape($nombre) ?>

De forma predeterminada, el método ``escape()`` asume que la variable se está reproduciendo en un contexto HTML (y por tanto la variable se escapa para que sea HTML seguro).
El segundo argumento te permite cambiar el contexto. Por ejemplo, para mostrar algo en una cadena JavaScript, utiliza el contexto ``js``:

.. code-block:: js

    var miMsj = 'Hola <?php echo $view->escape($nombre, 'js') ?>';

.. index::
   single: Plantillas; Formatos

.. _template-formats:

Formato de plantillas
---------------------

Las plantillas son una manera genérica para reproducir contenido en *cualquier* formato. Y aunque en la mayoría de los casos debes utilizar plantillas para reproducir contenido HTML, una plantilla fácilmente puede generar JavaScript, CSS, XML o cualquier otro formato que puedas soñar.

Por ejemplo, el mismo "recurso" a menudo se reproduce en varios formatos diferentes.
Para reproducir una página índice de artículos en formato XML, basta con incluir el formato en el nombre de la plantilla:

*nombre de plantilla XML*: ``AcmeArticuloBundle:Articulo:index.xml.twig``
*nombre de archivo XML*: ``index.xml.twig``

En realidad, esto no es más que una convención de nomenclatura y la plantilla realmente no se reproduce de manera diferente en función de ese formato.

En muchos casos, posiblemente desees permitir que un solo controlador reproduzca múltiples formatos basándose en el "formato de la petición". Por esa razón, un patrón común es hacer lo siguiente:

.. code-block:: php

    public function indexAction()
    {
        $formato = $this->getRequest()->getRequestFormat();

        return $this->render('AcmeBlogBundle:Blog:index.'.$formato.'.twig');
    }

El ``getRequestFormat`` en el objeto ``Petición`` por omisión es HTML, pero lo puedes devolver en cualquier otro formato basándote en el formato solicitado por el usuario.
El formato de la petición muy frecuentemente es gestionado por el ruteador, donde puedes configurar una ruta para que ``/contacto`` establezca el formato ``html`` de la petición, mientras que ``/contacto.xml`` establezca al formato ``xml``. Para más información, consulta el :ref:`ejemplo avanzado en el capítulo de Enrutado <advanced-routing-example>`.

Para crear enlaces que incluyan el parámetro de formato, agrega una clave ``_format`` en el parámetro |hash|:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('articulo_show', {'id': 123, '_format': 'pdf'}) }}">
	        Versión PDF
	    </a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('articulo_show', array('id' => 123, '_format' => 'pdf')) ?>">
            Versión PDF
        </a>

Consideraciones finales
-----------------------

El motor de plantillas de Symfony es una poderosa herramienta que puedes utilizar cada vez que necesites generar contenido de presentación en HTML, XML o cualquier otro formato.
Y aunque las plantillas son una manera común de generar contenido en un controlador, su uso no es obligatorio. El objeto ``Petición`` devuelto por un controlador se puede crear usando o sin usar una plantilla:

.. code-block:: php

    // crea un objeto Respuesta donde el contenido reproduce la plantilla
    $respuesta = $this->render('AcmeArticuloBundle:Articulo:index.html.twig');

    // crea un objeto Response cuyo contenido es texto simple
    $respuesta = new Response('contenido respuesta');

El motor de plantillas de Symfony es muy flexible y de manera predeterminada hay dos diferentes reproductores de plantilla disponibles: las tradicionales plantillas *PHP* y las elegantes y potentes plantillas *Twig*. Ambas apoyan una jerarquía de plantillas y vienen empacadas con un rico conjunto de funciones ayudantes capaces de realizar las tareas más comunes.

En general, el tema de las plantillas se debe pensar como una poderosa herramienta que está a tu disposición. En algunos casos, posiblemente no necesites reproducir una plantilla, y en Symfony2, eso está absolutamente bien.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`

.. _`Twig`: http://www.twig-project.org
.. _`Symfony2Bundles.org`: http://symfony2bundles.org
.. _`Explotación de vulnerabilidades del sistema`: http://es.wikipedia.org/wiki/Cross-site_scripting
.. _`Mecanismo de escape`:              http://www.twig-project.org
.. _`tags`: http://www.twig-project.org/doc/templates.html#list-of-control-structures
.. _`filtros`:                          http://www.twig-project.org/doc/templates.html#list-of-built-in-filters
.. _`agregar tus propias extensiones`:  http://www.twig-project.org/doc/advanced.html
