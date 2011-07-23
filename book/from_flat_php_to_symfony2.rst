Symfony2 frente a PHP simple
============================

**¿Por qué Symfony2 es mejor que sólo abrir un archivo y escribir PHP simple?**

Si nunca haz usado una plataforma PHP, no estás familiarizado con la filosofía MVC, o simplemente te preguntas qué es todo ese *alboroto* en torno a Symfony2, este capítulo es para ti. En vez de *decirte* que Symfony2 te permite desarrollar software más rápido y mejor que con PHP simple, debes verlo tú mismo.

En este capítulo, vamos a escribir una aplicación sencilla en PHP simple, y luego la reconstruiremos para que esté mejor organizada. Podrás viajar a través del tiempo, viendo las decisiones de por qué el desarrollo web ha evolucionado en los últimos años hasta donde está ahora. 

Al final, verás cómo Symfony2 te puede rescatar de las tareas cotidianas y te permite recuperar el control de tu código.

Un sencillo blog en PHP simple
------------------------------

En este capítulo, crearemos una simbólica aplicación de blog utilizando sólo PHP simple.
Para empezar, crea una página que muestra las entradas del blog que se han persistido en la base de datos. Escribirla en PHP simple es rápido y sucio:

.. code-block:: html+php

    <?php
    // index.php

    $enlace = mysql_connect('localhost', 'miusuario', 'mipase');
    mysql_select_db('bd_blog', $enlace);

    $resultado = mysql_query('SELECT id, titulo FROM post', $enlace);
    ?>

    <html>
        <head>
            <title>Lista de mensajes</title>
        </head>
        <body>
            <h1>Lista de mensajes</h1>
            <ul>
                <?php while ($fila = mysql_fetch_assoc($resultado)): ?>
                <li>
                    <a href="/show.php?id=<?php echo $fila['id'] ?>">
                        <?php echo $fila['titulo'] ?>
                    </a>
                </li>
                <?php endwhile; ?>
            </ul>
        </body>
    </html>

    <?php
    mysql_close($enlace);

Eso es fácil de escribir, se ejecuta rápido, y, cuando tu aplicación crece, imposible de mantener. Hay varios problemas que es necesario abordar:

* **No hay comprobación de errores**: ¿Qué sucede si falla la conexión a la base de datos?

* **Deficiente organización**: Si la aplicación crece, este único archivo cada vez será más difícil de mantener, hasta que finalmente sea imposible. ¿Dónde se debe colocar el código para manejar un formulario enviado? ¿Cómo se pueden validar los datos? ¿Dónde debe ir el código para enviar mensajes de correo electrónico?

* **Es difícil reutilizar el código**: Ya que todo está en un archivo, no hay manera de volver a utilizar alguna parte de la aplicación en otras "páginas" del blog.

.. note::
    Otro problema no mencionado aquí es el hecho de que la base de datos está vinculada a MySQL. Aunque no se ha tratado aquí, Symfony2 integra `Doctrine`_ plenamente, una biblioteca dedicada a la abstracción y asignación de bases de datos.

Vamos a trabajar en la solución de estos y muchos problemas más.

Aislando la presentación
~~~~~~~~~~~~~~~~~~~~~~~~

El código inmediatamente se puede beneficiar de la separación entre la "lógica" de la aplicación y el código que prepara la "presentación" HTML:

.. code-block:: html+php

    <?php
    // index.php

    $enlace = mysql_connect('localhost', 'miusuario', 'mipase');
    mysql_select_db('bd_blog', $enlace);

    $resultado = mysql_query('SELECT id, titulo FROM post', $enlace);

    $mensajes = array();
    while ($fila = mysql_fetch_assoc($resultado)) {
        $mensajes[] = $fila;
    }

    mysql_close($enlace);

    // incluye el código HTML de la presentación
    require 'plantillas/lista.php';

Ahora el código HTML está guardado en un archivo separado (``plantillas/lista.php``), el cual principalmente es un archivo HTML que utiliza una sintaxis de plantilla tipo PHP:

.. code-block:: html+php

    <html>
        <head>
            <title>Lista de mensajes</title>
        </head>
        <body>
            <h1>Lista de mensajes</h1>
            <ul>
                <?php foreach ($mensajes as $mensaje): ?>
                <li>
                    <a href="/leer?id=<?php echo $mensaje['id'] ?>">
                        <?php echo $mensaje['titulo'] ?>
                    </a>
                </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

Por convención, el archivo que contiene toda la lógica de la aplicación - ``index.php`` - se conoce como *"controlador"*. El término :term:`controlador` es una palabra que se escucha mucho, independientemente del lenguaje o plataforma que utilices. Simplemente se refiere a la zona de *tu código* que procesa la entrada del usuario y prepara la respuesta.

En este caso, nuestro controlador prepara los datos de la base de datos y, luego los incluye en una plantilla para presentarlos. Con el controlador aislado, fácilmente podríamos cambiar *sólo* el archivo de plantilla si es necesario procesar las entradas de blog en algún otro formato (por ejemplo, ``lista.json.php`` para el formato JSON). 

Aislando la lógica de la aplicación (el dominio)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Hasta ahora, la aplicación sólo contiene una página. Pero ¿qué pasa si una segunda página necesita utilizar la misma conexión a la base de datos, e incluso la misma matriz de entradas del blog? Reconstruye el código para que el comportamiento de las funciones básicas de acceso a datos de la aplicación esté aislado en un nuevo archivo llamado ``modelo.php``:

.. code-block:: html+php

    <?php
    // modelo.php

    function abre_conexion_bd()
    {
        $enlace = mysql_connect('localhost', 'miusuario', 'mipase');
        mysql_select_db('bd_blog', $enlace);

        return $enlace;
    }

    function cierra_conexion_bd($enlace)
    {
        mysql_close($enlace);
    }

    function obt_todo_mensaje()
    {
        $enlace = abre_conexion_bd();

        $resultado = mysql_query('SELECT id, titulo FROM post', $enlace);
        $mensajes = array();
        while ($fila = mysql_fetch_assoc($resultado)) {
            $mensajes[] = $fila;
        }
        cierra_conexion_bd($enlace);

        return $mensajes;
    }

.. tip::

   Utilizamos el nombre de archivo ``modelo.php`` debido a que el acceso a la lógica y los datos de una aplicación, tradicionalmente, se conoce como la capa del "modelo". En una aplicación bien organizada, la mayoría del código que representa tu "lógica de negocio" debe vivir en el modelo (en lugar de vivir en un controlador). Y, a diferencia de este ejemplo, sólo una parte (o ninguna) del modelo realmente está interesada en acceder a la base de datos.

El controlador (``index.php``) ahora es muy sencillo:

.. code-block:: html+php

    <?php
    require_once 'modelo.php';

    $mensajes = obt_todo_mensaje();

    require 'plantillas/lista.php';

Ahora, la única tarea del controlador es conseguir los datos de la capa del modelo de la aplicación (el modelo) e invocar a una plantilla que reproduce los datos.
Este es un ejemplo muy simple del patrón modelo-vista-controlador.

Aislando el diseño
~~~~~~~~~~~~~~~~~~

En este punto, hemos reconstruido la aplicación en tres piezas distintas, mismas que nos ofrecen varias ventajas y la oportunidad de volver a utilizar casi todo en diferentes páginas.

La única parte del código que *no* se puede reutilizar es el diseño de la página. Corregiremos esto creando un nuevo archivo ``esbozo.php``:

.. code-block:: html+php

    <!-- plantillas/esbozo.php -->
    <html>
        <head>
            <title><?php echo $titulo ?></title>
        </head>
        <body>
            <?php echo $contenido ?>
        </body>
    </html>

La plantilla (``plantillas/lista.php``) ahora se puede simplificar para "extender" el diseño:

.. code-block:: html+php

    <?php $titulo = 'Lista de mensajes' ?>

    <?php ob_start() ?>
        <h1>Lista de mensajes</h1>
        <ul>
            <?php foreach ($mensajes as $mensaje): ?>
            <li>
                <a href="/leer?id=<?php echo $mensaje['id'] ?>">
                    <?php echo $mensaje['titulo'] ?>
                </a>
            </li>
            <?php endforeach; ?>
        </ul>
    <?php $contenido = ob_get_clean() ?>

    <?php include 'esbozo.php' ?>

Ahora hemos introducido una metodología que nos permite reutilizar el diseño. Desafortunadamente, para lograrlo, estamos obligados a utilizar algunas desagradables funciones de PHP (``ob_start()``, ``ob_get_clean()``) en la plantilla. Symfony2 utiliza un componente ``Templating`` que nos permite realizar esto limpia y fácilmente. En breve lo verás en acción.

Agregando una página "show" blog
--------------------------------

La página "lista" del blog se ha rediseñado para que el código esté mejor organizado y sea reutilizable. Para probarlo, añade una página "show" al blog, que muestre una entrada individual del blog identificada por un parámetro de consulta ``id``.

Para empezar, crea una nueva función en el archivo ``modelo.php`` que recupere un resultado individual del blog basándose en un identificador dado::

    // modelo.php
    function obt_mensaje_por_id($id)
    {
        $enlace = abre_conexion_bd();

        $id = mysql_real_escape_string($id);
        $consulta = 'SELECT date, titulo, body FROM post WHERE id = '.$id;
        $resultado = mysql_query($consulta);
        $fila = mysql_fetch_assoc($resultado);

        cierra_conexion_bd($enlace);

        return $fila;
    }

A continuación, crea un nuevo archivo llamado ``show.php`` - el controlador para esta nueva página:

.. code-block:: html+php

    <?php
    require_once 'modelo.php';

    $mensaje = obt_mensaje_por_id($_GET['id']);

    require 'plantillas/show.php';

Por último, crea el nuevo archivo de plantilla - ``plantillas/show.php`` - para reproducir una entrada individual del blog:

.. code-block:: html+php

    <?php $titulo = $mensaje['titulo'] ?>

    <?php ob_start() ?>
        <h1><?php echo $mensaje['titulo'] ?></h1>

        <div class="date"><?php echo $mensaje['date'] ?></div>
        <div class="body">
            <?php echo $mensaje['body'] ?>
        </div>
    <?php $contenido = ob_get_clean() ?>

    <?php include 'esbozo.php' ?>

Ahora, es muy fácil crear la segunda página y sin duplicar código. Sin embargo, esta página introduce problemas aún más perniciosos que una plataforma puede resolver por ti. Por ejemplo, un parámetro ``id`` ilegal u omitido en la consulta hará que la página se bloquee. Sería mejor si esto reprodujera una página 404, pero sin embargo, en realidad esto no se puede hacer fácilmente. Peor aún, si olvidaras desinfectar el parámetro ``id`` por medio de la función ``mysql_real_escape_string()``, tu base de datos estaría en riesgo de un ataque de inyección SQL.

Otro importante problema es que cada archivo de controlador individual debe incluir al archivo ``modelo.php``. ¿Qué pasaría si cada archivo de controlador de repente tuviera que incluir un archivo adicional o realizar alguna tarea global (por ejemplo, reforzar la seguridad)?
Tal como está ahora, el código tendría que incluir todos los archivos de los controladores.
Si olvidas incluir algo en un solo archivo, esperemos que no sea alguno relacionado con la seguridad...

El "controlador frontal" al rescate
-----------------------------------

La solución es utilizar un :term:`controlador frontal`: un único archivo PHP a través del cual se procesan *todas* las peticiones. Con un controlador frontal, la URI de la aplicación cambia un poco, pero se vuelve más flexible:

.. code-block:: text

    Sin controlador frontal
    /index.php          => (ejecuta index.php) la página lista de mensajes.
    /show.php           => (ejecuta show.php)  la página muestra un mensaje particular.

    Con index.php como controlador frontal
    /index.php          => (ejecuta index.php) la página lista de mensajes.
    /index.php/show     => (ejecuta index.php) la página muestra un mensaje particular.

.. tip::
    La porción ``index.php`` de la URI se puede quitar si se utilizan las reglas de reescritura de Apache (o equivalentes). En ese caso, la URI resultante de la página show del blog simplemente sería ``/show``.

Cuando se usa un controlador frontal, un solo archivo PHP (``index.php`` en este caso) procesa todas las peticiones. Para la página 'show' del blog, ``/index.php/show`` realmente ejecuta el archivo ``index.php``, que ahora es el responsable de dirigir internamente las peticiones basándose en la URI completa. Como puedes ver, un controlador frontal es una herramienta muy poderosa.

Creando el controlador frontal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Estás a punto de dar un **gran** paso en la aplicación. Con un archivo manejando todas las peticiones, puedes centralizar cosas tales como el manejo de la seguridad, la carga de configuración y enrutado. En esta aplicación, ``index.php`` ahora debe ser lo suficientemente inteligente como para reproducir la lista de entradas del blog *o* mostrar la página de una entrada particular basándose en la URI solicitada:

.. code-block:: html+php

    <?php
    // index.php

    // carga e inicia algunas bibliotecas globales
    require_once 'modelo.php';
    require_once 'controladores.php';

    // encamina la petición internamente
    $uri = $_SERVER['REQUEST_URI'];
    if ($uri == '/index.php') {
        lista_action();
    } elseif ($uri == '/index.php/show' && isset($_GET['id'])) {
        show_action($_GET['id']);
    } else {
        header('Status: 404 Not Found');
        echo '<html><body><h1>Página no encontrada</h1></body></html>';
    }

Por organización, ambos controladores (antes ``index.php`` y ``show.php``) son funciones PHP y cada una se ha movido a un archivo separado, ``controladores.php``:

.. code-block:: php

    function lista_action()
    {
        $mensajes = obt_todo_mensaje();
        require 'plantillas/lista.php';
    }

    function show_action($id)
    {
        $mensaje = obt_mensaje_por_id($id);
        require 'plantillas/show.php';
    }

Como controlador frontal, ``index.php`` ha asumido un papel completamente nuevo, el cual incluye la carga de las bibliotecas del núcleo y enrutar la aplicación para invocar a uno de los dos controladores (las funciones ``lista_action()`` y ``show_action()``). En realidad, el controlador frontal está empezando a verse y actuar como el mecanismo Symfony2 para la manipulación y enrutado de peticiones.

.. tip::

   Otra ventaja del controlador frontal es la flexibilidad de las direcciones URL. Ten en cuenta que la URL a la página 'show' del blog se puede cambiar de ``/show`` a ``/leer`` cambiando el código solamente en una única ubicación. Antes, era necesario cambiar todo un archivo para cambiar el nombre. En Symfony2, incluso las direcciones URL son más flexibles.

Por ahora, la aplicación ha evolucionado de un único archivo PHP, a una estructura organizada y permite la reutilización de código. Debes estar feliz, pero aún lejos de estar satisfecho. Por ejemplo, el sistema de "enrutado" es voluble, y no reconoce que la página 'lista' (``/index.php``) también debe ser accesible a través de ``/`` (si se han agregado las reglas de reescritura de Apache). Además, en lugar de desarrollar el blog, una gran cantidad del tiempo se ha gastado trabajando en la "arquitectura" del código (por ejemplo, el enrutado, invocando controladores, plantillas, etc.) Se tendrá que gastar más tiempo para manejar la presentación de formularios, validación de entradas, registro de sucesos y seguridad.
¿Por qué tienes que reinventar soluciones a todos estos problemas rutinarios?

Añadiendo un toque Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 al rescate. Antes de utilizar Symfony2 realmente, debes asegurarte de que PHP sabe cómo encontrar las clases Symfony2. Esto se logra a través de un cargador automático que proporciona Symfony. Un cargador automático es una herramienta que permite empezar a utilizar clases PHP sin incluir explícitamente el archivo que contiene la clase.

Primero, `descarga symfony`_ y colócalo en el directorio ``vendor/symfony/``.
A continuación, crea un archivo ``app/bootstrap.php``. Se usa para ``requerir`` los dos archivos en la aplicación y para configurar el cargador automático:

.. code-block:: html+php

    <?php
    // bootstrap.php
    require_once 'modelo.php';
    require_once 'controladores.php';
    require_once 'vendor/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    $cargador = new Symfony\Component\ClassLoader\UniversalClassLoader();
    $cargador->registerNamespaces(array(
        'Symfony' => __DIR__.'/vendor/symfony/src',
    ));

    $cargador->register();

Esto le dice al cargador automático donde están las clases de ``Symfony``. Con esto, puedes comenzar a utilizar las clases de Symfony sin necesidad de utilizar la declaración ``requiere`` en los archivos que las utilizan.

La esencia de la filosofía Symfony es la idea de que el trabajo principal de una aplicación es interpretar cada petición y devolver una respuesta. Con este fin, Symfony2 proporciona ambas clases :class:`Symfony\\Component\\HttpFoundation\\Request` y :class:`Symfony\\Component\\HttpFoundation\\Response`. Estas clases son representaciones orientadas a objetos de la petición HTTP que se está procesando y la respuesta HTTP que devolverá. Úsalas para mejorar el blog:

.. code-block:: html+php

    <?php
    // index.php
    require_once 'app/bootstrap.php';

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $peticion = Request::createFromGlobals();

    $uri = $peticion->getPathInfo();
    if ($uri == '/') {
        $respuesta = lista_action();
    } elseif ($uri == '/show' && $peticion->query->has('id')) {
        $respuesta = show_action($peticion->query->get('id'));
    } else {
        $html = '<html><body><h1>Página no encontrada</h1></body></html>';
        $respuesta = new Response($html, 404);
    }

    // difunde las cabeceras y envía la respuesta
    $respuesta->send();

Los controladores son responsables de devolver un objeto ``Respuesta``.
Para facilitarnos esto, puedes agregar una nueva función ``reproduce_plantilla()``, la cual, por cierto, actúa un poco como el motor de plantillas de Symfony2:

.. code-block:: php

    // controladores.php
    use Symfony\Component\HttpFoundation\Response;

    function lista_action()
    {
        $mensajes = obt_todo_mensaje();
        $html = reproduce_plantilla('plantillas/lista.php', array('posts' => $mensajes));

        return new Response($html);
    }

    function show_action($id)
    {
        $mensaje = obt_mensaje_por_id($id);
        $html = reproduce_plantilla('plantillas/show.php', array('post' => $mensaje));

        return new Response($html);
    }

    // función ayudante para reproducir plantillas
    function reproduce_plantilla($ruta, array $args)
    {
        extract($args);
        ob_start();
        require $ruta;
        $html = ob_get_clean();

        return $html;
    }

Al reunir una pequeña parte de Symfony2, la aplicación es más flexible y fiable. La ``Petición`` proporciona una manera confiable para acceder a información de la petición HTTP. Especialmente, el método ``getPathInfo()`` devuelve una URI limpia (siempre devolviendo ``/show`` y nunca ``/index.php/show``).
Por lo tanto, incluso si el usuario va a ``/index.php/show``, la aplicación es lo suficientemente inteligente para encaminar la petición hacia ``show_action()``.

El objeto ``Respuesta`` proporciona flexibilidad al construir la respuesta HTTP, permitiendo que las cabeceras HTTP y el contenido se agreguen a través de una interfaz orientada a objetos.
Y aunque las respuestas en esta aplicación son simples, esta flexibilidad pagará dividendos en cuanto tu aplicación crezca.

Aplicación de ejemplo en Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El blog ha *avanzado*, pero todavía contiene una gran cantidad de código para una aplicación tan simple. De paso, también inventamos un sencillo sistema de enrutado y un método que utiliza ``ob_start()`` y ``ob_get_clean()`` para procesar plantillas. Si, por alguna razón, necesitas continuar la construcción de esta "plataforma" desde cero, por lo menos puedes usar los componentes independientes de Symfony `Routing`_ y `Templating`_, que resuelven estos problemas.

En lugar de resolver problemas comunes de nuevo, puedes dejar que Symfony2 se preocupe de ellos por ti. Aquí está la misma aplicación de ejemplo, ahora construida en Symfony2:

.. code-block:: html+php

    <?php
    // src/Acme/BlogBundle/Controller/BlogController.php

    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function listAction()
        {
            $mensajes = $this->get('doctrine')->getEntityManager()
                ->createQuery('SELECT p FROM AcmeBlogBundle:Post p')
                ->execute();

            return $this->render('AcmeBlogBundle:Post:lista.html.php', array('posts' => $mensajes));
        }

        public function showAction($id)
        {
            $mensaje = $this->get('doctrine')
                ->getEntityManager()
                ->getRepository('AcmeBlogBundle:Post')
                ->find($id);

            if (!$mensaje) {
                // provoca que se muestre el error 404 Página no encontrada
                throw $this->createNotFoundException();
            }

            return $this->render('AcmeBlogBundle:Post:show.html.php', array('post' => $mensaje));
        }
    }

Los dos controladores siguen siendo ligeros. Cada uno utiliza la biblioteca ORM de Doctrine para recuperar objetos de la base de datos y el componente ``Templating`` para reproducir una plantilla y devolver un objeto ``Respuesta``. La plantilla lista ahora es un poco más simple:

.. code-block:: html+php

    <!-- src/Acme/BlogBundle/Resources/views/Blog/lista.html.php --> 
    <?php $view->extend('::base.html.php') ?>

    <?php $view['slots']->set('titulo', 'Lista de mensajes') ?>

    <h1>Lista de mensajes</h1>
    <ul>
        <?php foreach ($mensajes as $mensaje): ?>
        <li>
            <a href="<?php echo $view['router']->generate('blog_show', array('id' => $mensaje->getId())) ?>">
                <?php echo $mensaje->getTitulo() ?>
            </a>
        </li>
        <?php endforeach; ?>
    </ul>

El diseño es casi idéntico:

.. code-block:: html+php

    <!-- app/Resources/views/base.html.php -->
    <html>
        <head>
            <title><?php echo $view['slots']->output('titulo', 'Default titulo') ?></title>
        </head>
        <body>
            <?php echo $view['slots']->output('_content') ?>
        </body>
    </html>

.. note::

    Te vamos a dejar como ejercicio la plantilla 'show', porque debería ser trivial crearla basándote en la plantilla lista.

Cuando arranca el motor Symfony2 (llamado ``kernel``), necesita un mapa para saber qué controladores ejecutar basándose en la información solicitada.
Un mapa de configuración de enrutado proporciona esta información en formato legible::

    # app/config/routing.yml
    blog_list:
        pattern:  /blog
        defaults: { _controller: AcmeBlogBundle:Blog:lista }

    blog_show:
        pattern:  /blog/show/{id}
        defaults: { _controller: AcmeBlogBundle:Blog:show }

Ahora que Symfony2 se encarga de todas las tareas rutinarias, el controlador frontal es muy simple. Y ya que hace tan poco, nunca tienes que volver a tocarlo una vez creado (y si utilizas una distribución Symfony2, ¡ni siquiera tendrás que crearlo!):

.. code-block:: html+php

    <?php
    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $nucleo = new AppKernel('prod', false);
    $nucleo->handle(Request::createFromGlobals())->send();

El único trabajo del controlador frontal es iniciar el motor de Symfony2 (el ``núcleo``) y pasarle un objeto ``Petición`` para que lo manipule. El núcleo de Symfony2 entonces utiliza el mapa de enrutado para determinar qué controlador invocar. Al igual que antes, el método controlador es el responsable de devolver el objeto ``Respuesta`` final.
Realmente no hay mucho más sobre él.

Para conseguir una representación visual de cómo maneja Symfony2 cada petición, consulta el :ref:`diagrama de flujo de la petición <request-flow-figure>`.

Qué más ofrece Symfony2
~~~~~~~~~~~~~~~~~~~~~~~

En los siguientes capítulos, aprenderás más acerca de cómo funciona cada pieza de Symfony y la organización recomendada de un proyecto. Por ahora, vamos a ver cómo, migrar el blog de PHP simple a Symfony2 nos ha mejorado la vida:

* Tu aplicación cuenta con **código claro y organizado consistentemente** (aunque Symfony no te obliga a ello). Promueve la **reutilización** y permite a los nuevos desarrolladores ser productivos en el proyecto con mayor rapidez.

* 100% del código que escribes es para *tu* aplicación. **No necesitas desarrollar o mantener servicios públicos de bajo nivel** tales como la :ref:`carga automática <autoloading-introduction-sidebar>` de clases, el :doc:`enrutado </book/routing>` o la reproducción de :doc:`controladores </book/controller>`.

* Symfony2 te proporciona **acceso a herramientas de código abierto** tales como Doctrine, plantillas, seguridad, formularios, validación y traducción (por nombrar algunas).

* La aplicación ahora disfruta de **direcciones URL totalmente flexibles** gracias al componente ``Routing``.

* La arquitectura centrada en HTTP de Symfony2 te da acceso a poderosas herramientas, tal como la **memoria caché HTTP** impulsadas por la **caché HTTP interna de Symfony2** o herramientas más poderosas, tales como `Varnish`_. Esto se trata posteriormente en el capítulo todo sobre :doc:`caché </book/http_cache>`.

Y lo mejor de todo, utilizando Symfony2, ¡ahora tienes acceso a un conjunto de herramientas de **alta calidad de código abierto desarrolladas por la comunidad Symfony2**!
Para más información, visita `Symfony2Bundles.org`_

Mejores plantillas
------------------

Si decides utilizarlo, Symfony2 de serie viene con un motor de plantillas llamado `Twig`_ el cual hace que las plantillas se escriban más rápido y sean más fáciles de leer.
Esto significa que, incluso, ¡la aplicación de ejemplo podría contener mucho menos código! Tomemos, por ejemplo, la plantilla lista escrita en Twig:

.. code-block:: html+jinja

    {# src/Acme/BlogBundle/Resources/views/Blog/lista.html.twig #}

    {% extends "::base.html.twig" %}
    {% block titulo %}Lista de mensajes{% endblock %}

    {% block body %}
        <h1>Lista de mensajes</h1>
        <ul>
            {% for post in posts %}
            <li>
                <a href="{{ path('blog_show', { 'id': post.id }) }}">
                    {{ post.titulo }}
                </a>
            </li>
            {% endfor %}
        </ul>
    {% endblock %}

También es fácil escribir la plantilla ``base.html.twig`` correspondiente:

.. code-block:: html+jinja

    {# app/Resources/views/base.html.twig #}

    <html>
        <head>
            <title>{% block titulo %}Título predefinido{% endblock %}</title>
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

Twig es compatible con Symfony2. Y si bien las plantillas PHP siempre contarán con el apoyo de Symfony2, vamos a seguir explicando muchas de las ventajas de Twig. Para más información, consulta el capítulo :doc:`Plantillas </book/templating>`.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`descarga symfony`: http://symfony.com/download
.. _`Routing`: https://github.com/symfony/Routing
.. _`Templating`: https://github.com/symfony/Templating
.. _`Symfony2Bundles.org`: http://symfony2bundles.org
.. _`Twig`: http://www.twig-project.org
.. _`Varnish`: http://www.varnish-cache.org
.. _`PHPUnit`: http://www.phpunit.de
