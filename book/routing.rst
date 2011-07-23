.. index::
   single: Enrutando

Enrutando
~~~~~~~~~

Las URL bonitas absolutamente son una necesidad para cualquier aplicación web seria. Esto significa dejar atrás las URL feas como ``index.php?id_articulo=57`` en favor de algo así como ``/leer/intro-a-symfony``.

Tener tal flexibilidad es más importante aún. ¿Qué pasa si necesitas cambiar la URL de una página de ``/blog`` a ``/noticias``? ¿Cuántos enlaces necesitas cazar y actualizar para hacer el cambio? Si estás utilizando el enrutador de Symfony, el cambio es sencillo.

El enrutador de Symfony2 te permite definir direcciones URL creativas que se asignan a diferentes áreas de la aplicación. Al final de este capítulo, serás capaz de:

* Crear rutas complejas asignadas a controladores
* Generar URL dentro de plantillas y controladores
* Cargar recursos enrutando desde el paquete (o en cualquier otro lugar)
* Depurar tus rutas

.. index::
   single: Enrutando; Fundamentos

Enrutador en acción
-------------------

Una *ruta* es un mapa desde un patrón URL hasta un controlador. Por ejemplo, supongamos que deseas adaptar cualquier URL como ``/blog/mi-post`` o ``/blog/todo-sobre-symfony`` y enviarla a un controlador que puede buscar y reproducir esta entrada del blog.
La ruta es simple:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog_show:
            pattern:   /blog/{ficha}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{ficha}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('blog_show', new Route('/blog/{ficha}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $coleccion;

El patrón definido por la ruta ``blog_show`` actúa como ``/blog/*`` dónde al comodín se le da el nombre de ``ficha``. Para la URL ``/blog/mi-entrada-del-blog``, la variable ``ficha`` obtiene un valor de ``mi-entrada-del-blog``, que está disponible para usarla en el controlador (sigue leyendo).

El parámetro ``_controller`` es una clave especial que le dice a Symfony qué controlador se debe ejecutar cuando una URL coincide con esta ruta. La cadena ``_controller`` se conoce como el :ref:`nombre lógico <controller-string-syntax>`. Esta sigue un patrón que apunta hacia una clase PHP y un método:

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php

    namespace Acme/BlogBundle/Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function showAction($ficha)
        {
            $blog = // usa la variable $ficha para consultar la base de datos

            return $this->render('AcmeBlogBundle:Blog:show.html.twig', array(
                'blog' => $blog,
            ));
        }
    }

¡Enhorabuena! Acabas de crear tu primera ruta y la conectaste a un controlador. Ahora, cuando visites ``/blog/mi-comentario``, el controlador ``showAction`` será ejecutado y la variable ``$ficha`` será igual a ``mi-comentario``.

Este es el objetivo del enrutador de Symfony2: asignar la URL de una petición a un controlador. De paso, aprenderás todo tipo de trucos que incluso facilitan la asignación de direcciones URL complejas. 

.. index::
   single: Enrutado; Bajo el capó

Enrutador: bajo el capó
-----------------------

Cuando se hace una petición a tu aplicación, esta contiene una dirección al "recurso" exacto que solicitó el cliente. Esta dirección se conoce como URL (o URI), y podría ser ``/contacto``, ``/blog/leeme``, o cualquier otra cosa. Tomemos la siguiente petición HTTP, por ejemplo:

.. code-block:: text

    GET /blog/mi-entrada-del-blog

El objetivo del sistema de enrutado de Symfony2 es analizar esta URL y determinar qué controlador se debe ejecutar. Todo el proceso es el siguiente:

#. La petición es manejada por el controlador frontal de Symfony2 (por ejemplo, ``app.php``);

#. El núcleo de Symfony2 (es decir, el Kernel) pregunta al enrutador que examine la petición;

#. El enrutador busca la URL entrante para emparejarla con una ruta específica y devuelve información sobre la ruta, incluyendo el controlador que se debe ejecutar;

#. El núcleo de Symfony2 ejecuta el controlador, que en última instancia, devuelve un objeto ``Respuesta``.

.. figure:: /images/flujo-peticion.png
   :align: center
   :alt: flujo de la petición en Symfony2

   La capa del enrutador es una herramienta que traduce la URL entrante a un controlador específico a ejecutar.

.. index::
   single: Enrutando; Creando rutas

Creando rutas
-------------

Symfony carga todas las rutas de tu aplicación desde un archivo de configuración de enrutado. El archivo usualmente es ``app/config/routing.yml``, pero lo puedes configurar para que sea cualquier otro (incluyendo un archivo XML o PHP) vía el archivo de configuración de la aplicación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('framework', array(
            // ...
            'router'        => array('resource' => '%kernel.root_dir%/config/routing.php'),
        ));

.. tip::

    A pesar de que todas las rutas se cargan desde un solo archivo, es práctica común incluir recursos de enrutado adicionales desde el interior del archivo. Consulta la sección :ref:`routing-include-external-resources` para más información.

Configuración básica de rutas
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Definir una ruta es fácil, y una aplicación típica tendrá un montón de rutas.
Una ruta básica consta de dos partes: el ``patrón`` a coincidir y un arreglo ``defaults``:

.. configuration-block::

    .. code-block:: yaml

        _bienvenida:
            pattern:   /
            defaults:  { _controller: AcmeDemoBundle:Principal:portada }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_bienvenida" pattern="/">
                <default key="_controller">AcmeDemoBundle:Principal:portada</default>
            </route>

        </routes>

    ..  code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('_bienvenida', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Principal:portada',
        )));

        return $coleccion;

Esta ruta coincide con la página de inicio (``/``) y la asigna al controlador de la página principal ``AcmeDemoBundle:Principal:portada``. Symfony2 convierte la cadena ``_controller`` en una función PHP real y la ejecuta. Este proceso será explicado en breve en la sección :ref:`controller-string-syntax`.

.. index::
   single: Enrutado; Marcadores de posición

Enrutando con marcadores de posición
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Por supuesto, el sistema de enrutado es compatible con rutas mucho más interesantes. Muchas rutas contienen uno o más "comodines" llamados marcadores de posición:

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            pattern:   /blog/{ficha}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{ficha}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('blog_show', new Route('/blog/{ficha}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $coleccion;

El patrón coincidirá con cualquier cosa que se vea como ``/blog/*``. Aún mejor, el valor coincide con el marcador de posición ``{ficha}`` que estará disponible dentro de tu controlador. En otras palabras, si la URL es ``/blog/hola-mundo``, una variable ``$ficha``, con un valor de ``hola-mundo``, estará disponible en el controlador.
Esta se puede usar, por ejemplo, para cargar la entrada en el blog coincidente con esa cadena.

El patrón *no* es, sin embargo, simplemente una coincidencia con ``/blog``. Eso es porque, por omisión, todos los marcadores de posición son obligatorios. Esto se puede cambiar agregando un valor marcador de posición al arreglo ``defaults``.

Marcadores de posición obligatorios y opcionales
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Para hacer las cosas más emocionantes, añade una nueva ruta que muestre una lista de todas las entradas del 'blog' para la petición imaginaria 'blog':

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('blog', new Route('/blog', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $coleccion;

Hasta el momento, esta ruta es tan simple como es posible - no contiene marcadores de posición y sólo coincidirá con la URL exacta ``/blog``. ¿Pero si necesitamos que esta ruta sea compatible con paginación, donde ``/blog/2`` muestra la segunda página de las entradas del blog? Actualiza la ruta para que tenga un nuevo marcador de posición ``{pag}``:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{pag}
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{pag}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('blog', new Route('/blog/{pag}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $coleccion;

Al igual que el marcador de posición ``{ficha}`` anterior, el valor coincidente con ``{pag}`` estará disponible dentro de tu controlador. Puedes utilizar su valor para determinar cual conjunto de entradas del blog muestra determinada página.

¡Pero espera! Puesto que los marcadores de posición de forma predeterminada son obligatorios, esta ruta ya no coincidirá con ``/blog`` simplemente. En su lugar, para ver la página 1 del blog, ¡habrá la necesidad de utilizar la URL ``/blog/1``! Debido a que esa no es la manera en que se comporta una aplicación web rica, debes modificar la ruta para que el parámetro ``{pag}`` sea opcional.
Esto se consigue incluyendo la colección ``defaults``:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{pag}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, pag: 1 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{pag}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="pag">1</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('blog', new Route('/blog/{pag}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'pag' => 1,
        )));

        return $coleccion;

Agregando ``pag`` a la clave ``defaults``, el marcador de posición ``{pag}`` ya no es necesario. La URL ``/blog`` coincidirá con esta ruta y el valor del parámetro ``pag`` se fijará en un ``1``. La URL ``/blog/2`` también coincide, dando al parámetro ``pag`` un valor de ``2``. Perfecto.

+---------+------------+
| /blog   | {pag} = 1  |
+---------+------------+
| /blog/1 | {pag} = 1  |
+---------+------------+
| /blog/2 | {pag} = 2  |
+---------+------------+

.. index::
   single: Enrutando; Requisitos

Agregando requisitos
~~~~~~~~~~~~~~~~~~~~

Echa un vistazo a las rutas que hemos creado hasta ahora:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{pag}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, pag: 1 }

        blog_show:
            pattern:   /blog/{ficha}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{pag}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="pag">1</default>
            </route>

            <route id="blog_show" pattern="/blog/{ficha}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('blog', new Route('/blog/{pag}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'pag' => 1,
        )));

        $coleccion->add('blog_show', new Route('/blog/{show}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $coleccion;

¿Puedes ver el problema? Ten en cuenta que ambas rutas tienen patrones que coinciden con las URL que se parezcan a ``/blog/*``. El enrutador de Symfony siempre elegirá la **primera** ruta coincidente que encuentre. En otras palabras, la ruta ``blog_show`` *nunca* corresponderá. En cambio, una URL como ``/blog/mi-entrada-del-blog`` coincidirá con la primera ruta (``blog``) y devolverá un valor sin sentido de ``mi-entrada-del-blog`` para el parámetro ``{pag}``.

+---------------------------+-------+------------------------------+
| URL                       | ruta  | parámetros                   |
+===========================+=======+==============================+
| /blog/2                   | blog  | {pag} = 2                    |
+---------------------------+-------+------------------------------+
| /blog/mi-entrada-del-blog | blog  | {pag} = mi-entrada-del-blog  |
+---------------------------+-------+------------------------------+

La respuesta al problema es añadir *requisitos* a la ruta. Las rutas en este ejemplo deben funcionar a la perfección si el patrón ``/blog/{pag}`` *sólo* concuerda con una URL dónde la parte ``{pag}`` es un número entero. Afortunadamente, se puede agregar fácilmente una expresión regular de requisitos para cada parámetro. Por ejemplo:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{pag}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, pag: 1 }
            requirements:
                pag:  \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{pag}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="pag">1</default>
                <requirement key="pag">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('blog', new Route('/blog/{pag}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'pag' => 1,
        ), array(
            'pag' => '\d+',
        )));

        return $coleccion;

El requisito ``\d+`` es una expresión regular diciendo que el valor del parámetro ``{pag}`` debe ser un dígito (es decir, un número). La ruta ``blog`` todavía coincide con una URL como ``/blog/2`` (porque 2 es un número), pero ya no concuerda con una URL como ``/blog/my-blog-pos`` (porque ``mi-entrada-del-blog`` *no* es un número).

Como resultado, una URL como ``/blog/mi-entrada-del-blog`` ahora coincide correctamente con la ruta ``blog_show``.

+---------------------------+-----------+-------------------------------+
| URL                       | ruta      | parámetros                    |
+===========================+===========+===============================+
| /blog/2                   | blog      | {pag} = 2                     |
+---------------------------+-----------+-------------------------------+
| /blog/mi-entrada-del-blog | blog_show | {ficha} = mi-entrada-del-blog |
+---------------------------+-----------+-------------------------------+

.. sidebar:: Las primeras rutas siempre ganan

    ¿Qué significa todo eso de que el orden de las rutas es muy importante?
    Si la ruta ``blog_show`` se coloca por encima de la ruta ``blog``, la URL ``/blog/2`` coincidiría con ``blog_show`` en lugar de ``blog`` ya que el parámetro ``{ficha}`` de ``blog_show`` no tiene ningún requisito. Usando el orden adecuado y requisitos claros, puedes lograr casi cualquier cosa.

Puesto que el parámetro ``requirements`` son expresiones regulares, la complejidad y flexibilidad de cada requisito es totalmente tuya. Supongamos que la página principal de tu aplicación está disponible en dos diferentes idiomas, según la URL:

.. configuration-block::

    .. code-block:: yaml

        portada:
            pattern:   /{culture}
            defaults:  { _controller: AcmeDemoBundle:Principal:portada, culture: en }
            requirements:
                culture:  en|es

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="portada" pattern="/{culture}">
                <default key="_controller">AcmeDemoBundle:Principal:portada</default>
                <default key="culture">en</default>
                <requirement key="culture">en|es</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('portada', new Route('/{culture}', array(
            '_controller' => 'AcmeDemoBundle:Principal:portada',
            'culture' => 'en',
        ), array(
            'culture' => 'en|es',
        )));

        return $coleccion;

Para las peticiones entrantes, la porción ``{culture}`` de la dirección se compara con la expresión regular ``(en|es)``.

+-----+-------------------------------+
| /   | {culture} = en                |
+-----+-------------------------------+
| /en | {culture} = en                |
+-----+-------------------------------+
| /es | {culture} = es                |
+-----+-------------------------------+
| /fr | *no coincidirá con esta ruta* |
+-----+-------------------------------+

.. index::
   single: Enrutando; Requisito de método

Agregando requisitos de método HTTP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Además de la URL, también puedes coincidir con el *método* de la petición entrante (es decir, GET, HEAD, POST, PUT, DELETE). Supongamos que tienes un formulario de contacto con dos controladores - uno para mostrar el formulario (en una petición GET) y uno para procesar el formulario una vez presentada (en una petición POST). Esto se puede lograr con la siguiente configuración de ruta:

.. configuration-block::

    .. code-block:: yaml

        contacto:
            pattern:  /contacto
            defaults: { _controller: AcmeDemoBundle:Principal:contacto }
            requirements:
                _method:  GET

        contacto_process:
            pattern:  /contacto
            defaults: { _controller: AcmeDemoBundle:Principal:contactoProcess }
            requirements:
                _method:  POST

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contacto" pattern="/contacto">
                <default key="_controller">AcmeDemoBundle:Principal:contacto</default>
                <requirement key="_method">GET</requirement>
            </route>

            <route id="contact_process" pattern="/contacto">
                <default key="_controller">AcmeDemoBundle:Principal:contactoProcess</default>
                <requirement key="_method">POST</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('contacto', new Route('/contacto', array(
            '_controller' => 'AcmeDemoBundle:Principal:contacto',
        ), array(
            '_method' => 'GET',
        )));

        $coleccion->add('contacto_process', new Route('/contacto', array(
            '_controller' => 'AcmeDemoBundle:Principal:contactoProcess',
        ), array(
            '_method' => 'POST',
        )));

        return $coleccion;

A pesar de que estas dos rutas tienen patrones idénticos (``/contacto``), la primera ruta sólo coincidirá con las peticiones GET y la segunda sólo coincidirá con las peticiones POST. Esto significa que puedes mostrar y enviar el formulario a través de la misma URL, mientras usas controladores distintos para las dos acciones.

.. note::
    Si no especificas el requisito ``_method``, la ruta coincidirá con todos los *métodos*.

Al igual que los otros requisitos, el requisito ``_method`` se analiza como una expresión regular. Para hacer coincidir peticiones ``GET`` *o* ``POST``, puedes utilizar ``GET|POST``.

.. index::
   single: Enrutando; Ejemplo avanzado
   single: Enrutando; Parámetro _format

.. _advanced-routing-example:

Ejemplo de enrutado avanzado
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En este punto, tienes todo lo necesario para crear una poderosa estructura de enrutado Symfony. El siguiente es un ejemplo de cuán flexible puede ser el sistema de enrutado:

.. configuration-block::

    .. code-block:: yaml

        articulo_show:
          pattern:  /articulos/{culture}/{year}/{titulo}.{_format}
          defaults: { _controller: AcmeDemoBundle:Articulo:show, _format: html }
          requirements:
              culture:  en|es
              _format:  html|rss
              year:     \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="articulo_show" pattern="/articulos/{culture}/{year}/{titulo}.{_format}">
                <default key="_controller">AcmeDemoBundle:Articulo:show</default>
                <default key="_format">html</default>
                <requirement key="culture">en|es</requirement>
                <requirement key="_format">html|rss</requirement>
                <requirement key="year">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('portada', new Route('/articulos/{culture}/{year}/{titulo}.{_format}', array(
            '_controller' => 'AcmeDemoBundle:Articulo:show',
            '_format' => 'html',
        ), array(
            'culture' => 'en|es',
            '_format' => 'html|rss',
            'year' => '\d+',
        )));

        return $coleccion;

Como hemos visto, esta ruta sólo coincide si la porción ``{culture}`` de la URL es o bien ``en`` o ``es`` y si ``{year}`` es un número. Esta ruta también muestra cómo puedes utilizar un punto entre los marcadores de posición en lugar de una barra inclinada. Las URL que coinciden con esta ruta podrían ser:

 * ``/articulos/en/2010/mi-comentario``
 * ``/articulos/es/2010/mi-comentario.rss``

.. sidebar:: El parámetro especial de enrutado ``_format``

    Este ejemplo también resalta el parámetro especial de enrutado ``_format``.
    Cuando se utiliza este parámetro, el valor coincidente se convierte en el "formato de la petición" del objeto ``Petición``. En última instancia, el formato de la petición se usa para cosas tales como establecer el ``Content-Type`` de la respuesta (por ejemplo, un formato de petición ``json`` se traduce en un ``Content-Type`` de ``application/json``).
    Este también se puede usar en el controlador para reproducir una plantilla diferente por cada valor de ``_format``. El parámetro ``_format`` es una forma muy poderosa para reproducir el mismo contenido en distintos formatos.

.. index::
   single: Enrutando; Controladores
   single: Controlador; Formato de cadena en nomenclaturas

.. _controller-string-syntax:

Patrón de nomenclatura para controladores
-----------------------------------------

Cada ruta debe tener un parámetro ``_controller``, el cual determina qué controlador se debe ejecutar cuando dicha ruta se empareje. Este parámetro utiliza un patrón de cadena simple llamado el *nombre lógico del controlador*, que Symfony asigna a un método y clase PHP específico. El patrón consta de tres partes, cada una separada por dos puntos:

    **paquete**:**controlador**:**acción**

Por ejemplo, un valor ``_controller`` de ``AcmeBlogBundle:Blog:show`` significa:

+----------------+----------------------+---------------+
| Paquete        | Clase de controlador | Nombre método |
+================+======================+===============+
| AcmeBlogBundle | BlogController       | showAction    |
+----------------+----------------------+---------------+

El controlador podría tener este aspecto:

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php
    
    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class BlogController extends Controller
    {
        public function showAction($ficha)
        {
            // ...
        }
    }

Ten en cuenta que Symfony añade la cadena ``Controller`` al nombre de la clase (``Blog``
=> ``BlogController``) y ``Action`` al nombre del método (``show`` => ``showAction``).

También podrías referirte a este controlador utilizando su nombre de clase y método completamente cualificado: ``Acme\BlogBundle\Controller\BlogController::showAction``.
Pero si sigues algunas simples convenciones, el nombre lógico es más conciso y permite mayor flexibilidad.

.. note::

   Además de utilizar el nombre lógico o el nombre de clase completamente cualificado, Symfony es compatible con una tercera forma de referirse a un controlador. Este método utiliza un solo separador de dos puntos (por ejemplo, ``service_name:indexAction``) y hace referencia al controlador como un servicio (consulta :doc:`/cookbook/controller/service`).

Parámetros de ruta y argumentos del controlador
-----------------------------------------------

Los parámetros de ruta (por ejemplo, ``{ficha}``) son especialmente importantes porque cada uno se pone a disposición como argumento para el método controlador:

.. code-block:: php

    public function showAction($ficha)
    {
      // ...
    }

En realidad, toda la colección ``defaults`` se combina con los valores del parámetro para formar una sola matriz. Cada clave de esa matriz está disponible como un argumento en el controlador.

En otras palabras, por cada argumento de tu método controlador, Symfony busca un parámetro de ruta de ese nombre y asigna su valor a ese argumento.
En el ejemplo avanzado anterior, cualquier combinación (en cualquier orden) de las siguientes variables se podría utilizar como argumentos para el método ``showAction()``:

* ``$culture``
* ``$year``
* ``$titulo``
* ``$_format``
* ``$_controller``

Dado que los marcadores de posición y los valores de la colección ``defaults`` se combinan, incluso la variable ``$_controller`` está disponible. Para una explicación más detallada, consulta :ref:`route-parameters-controller-arguments`.

.. tip::

    También puedes utilizar una variable especial ``$_route``, que se fija al nombre de la ruta que se emparejó.

.. index::
   single: Enrutando; Importando recursos de enrutado

.. _routing-include-external-resources:

Incluyendo recursos de enrutado externos
----------------------------------------

Todas las rutas se cargan a través de un único archivo de configuración - usualmente ``app/config/routing.yml`` (consulta `Creando rutas`_ más arriba). Por lo general, sin embargo, deseas cargar rutas para otros lugares, como un archivo de enrutado que vive dentro de un paquete. Esto se puede hacer "importando" ese archivo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hola:
            resource: "@AcmeHolaBundle/Resources/config/routing.yml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHolaBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $coleccion = new RouteCollection();
 $coleccion->addCollection($loader->import("@AcmeHolaBundle/Resources/config/routing.php"));

        return $coleccion;

.. note::

   Cuando importas recursos desde YAML, la clave (por ejemplo, ``acme_hola``) no tiene sentido.
   Sólo asegúrate de que es única para que no haya otras líneas que reemplazar.

La clave ``resource`` carga el recurso dado enrutando. En este ejemplo, el recurso es la ruta completa a un archivo, donde la sintaxis contextual del atajo ``@AcmeHolaBundle`` se resuelve en la ruta a ese paquete. El archivo importado podría tener este aspecto:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/routing.yml
       acme_hola:
            pattern:  /hola/{nombre}
            defaults: { _controller: AcmeHolaBundle:Hola:index }

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_hola" pattern="/hola/{nombre}">
                <default key="_controller">AcmeHolaBundle:Hola:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('acme_hola', new Route('/Hola/{nombre}', array(
            '_controller' => 'AcmeHolaBundle:Hola:index',
        )));

        return $coleccion;

Las rutas de este archivo se analizan y cargan en la misma forma que el archivo de enrutado principal.

Prefijando rutas importadas
~~~~~~~~~~~~~~~~~~~~~~~~~~~

También puedes optar por proporcionar un "prefijo" para las rutas importadas. Por ejemplo, supongamos que deseas que la ruta ``acme_hola`` tenga un patrón final de ``/admin/hola/{nombre}`` en lugar de simplemente ``/hola/{nombre}``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hola:
            resource: "@AcmeHolaBundle/Resources/config/routing.yml"
            prefix:   /admin

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHolaBundle/Resources/config/routing.xml" prefix="/admin" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $coleccion = new RouteCollection();
 $coleccion->addCollection($loader->import("@AcmeHolaBundle/Resources/config/routing.php"), '/admin');

        return $coleccion;

La cadena ``/admin`` ahora se antepondrá al patrón de cada ruta cargada desde el nuevo recurso enrutado.

.. index::
   single: Enrutando; Depurando

Visualizando y depurando rutas
------------------------------

Si bien agregar y personalizar rutas, es útil para poder visualizar y obtener información detallada sobre tus rutas. Una buena manera de ver todas las rutas en tu aplicación es a través de la orden de consola ``router:debug``. Ejecuta la siguiente orden desde la raíz de tu proyecto.

.. code-block:: bash

    php app/console router:debug

Esta orden imprimirá una útil lista de *todas* las rutas configuradas en tu aplicación:

.. code-block:: text

    portada               ANY       /
    contact               GET       /contacto
    contact_process       POST      /contacto
    articulo_show         ANY       /articulos/{culture}/{year}/{titulo}.{_format}
    blog                  ANY       /blog/{pag}
    blog_show             ANY       /blog/{ficha}

También puedes obtener información muy específica de una sola ruta incluyendo el nombre de la ruta después de la orden:

.. code-block:: bash

    php app/console router:debug articulo_show

.. index::
   single: Enrutando; Generando URL

Generando URL
-------------

El sistema de enrutado también se debe utilizar para generar direcciones URL. En realidad, el enrutado es un sistema bidireccional: asignando la URL a un controlador+parámetros y la ruta+parámetros a una URL. Los métodos :method:`Symfony\\Component\\Routing\\Router::match` y :method:`Symfony\\Component\\Routing\\Router::generate` de este sistema bidireccional. Tomando la ruta ``blog_show`` del ejemplo anterior::

    $params = $enrutador->match('/blog/mi-entrada-del-blog');
    // array('ficha' => 'mi-entrada-del-blog', '_controller' => 'AcmeBlogBundle:Blog:show')

    $uri = $enrutador->generate('blog_show', array('ficha' => 'mi-entrada-del-blog'));
    // /blog/mi-entrada-del-blog

Para generar una URL, debes especificar el nombre de la ruta (por ejemplo, ``blog_show``) y ningún comodín (por ejemplo, ``ficha = mi-entrada-del-blog``) utilizados en el patrón para esa ruta. Con esta información, puedes generar fácilmente cualquier URL:

.. code-block:: php

    class PrincipalController extends Controller
    {
        public function showAction($ficha)
        {
          // ...

          $url = $this->get('router')->generate('blog_show', array('ficha' => 'mi-entrada-del-blog'));
        }
    }

En una sección posterior, aprenderás cómo generar direcciones URL desde dentro de plantillas.

.. index::
   single: Enrutando; URL absolutas

Generando URL absolutas
~~~~~~~~~~~~~~~~~~~~~~~

De forma predeterminada, el enrutador va a generar direcciones URL relativas (por ejemplo ``/blog``). Para generar una URL absoluta, sólo tienes que pasar ``true`` como tercer argumento del método ``generate()``:

.. code-block:: php

    $enrutador->generate('blog_show', array('ficha' => 'mi-entrada-del-blog'), true);
    // http://www.ejemplo.com/blog/mi-entrada-del-blog

.. note::

    El host que utiliza al generar una URL absoluta es el anfitrión del objeto ``Petición`` actual. Este, de forma automática, lo detecta basándose en la información del servidor proporcionada por PHP. Al generar direcciones URL absolutas para archivos desde la línea de ordenes, tendrás que configurar manualmente el anfitrión que desees en el objeto ``Petición``:

    .. code-block:: php
    
        $peticion->headers->set('HOST', 'www.ejemplo.com');

.. index::
   single: Enrutando; Generando URL en una plantilla

Generando URL con cadena de consulta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El método ``generate`` toma una matriz de valores comodín para generar la URI.
Pero si pasas adicionales, se añadirán a la URI como cadena de consulta::

    $enrutador->generate('blog', array('pag' => 2, 'categoria' => 'Symfony'));
    // /blog/2?categoria=Symfony

Generando URL desde una plantilla
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El lugar más común para generar una URL es dentro de una plantilla cuando creas enlaces entre las páginas de tu aplicación. Esto se hace igual que antes, pero utilizando una función ayudante de plantilla:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('blog_show', { 'ficha': 'mi-entrada-del-blog' }) }}">
          Leer esta entrada del blog.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('ficha' => 'mi-entrada-del-blog')) ?>">
            Leer esta entrada del blog.
        </a>

También se pueden generar URL absolutas.

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ url('blog_show', { 'ficha': 'mi-entrada-del-blog' }) }}">
          Leer esta entrada del blog.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('ficha' => 'mi-entrada-del-blog'), true) ?>">
            Leer esta entrada del blog.
        </a>

Resumen
-------

El enrutado es un sistema para asignar la dirección de las peticiones entrantes a la función controladora que se debe llamar para procesar la petición. Este permite especificar ambas direcciones URL bonitas y mantiene la funcionalidad de tu aplicación disociada de las direcciones URL. El enrutado es un mecanismo de dos vías, lo cual significa que también se debe usar para generar direcciones URL.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/routing/scheme`
