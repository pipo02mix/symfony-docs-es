.. index::
   single: Fundamentos de Symfony2

Symfony2 y fundamentos HTTP
===========================

¡Enhorabuena! Al aprender acerca de Symfony2, vas bien en tu camino para llegar a ser un más *productivo*, bien *enfocado* y *popular* desarrollador web (en realidad, en la última parte, estás por tu cuenta). Symfony2 está diseñado para volver a lo básico: las herramientas de desarrollo que te permiten desarrollar más rápido y construir aplicaciones más robustas, mientras que permanece fuera de tu camino. Symfony está basado en las mejores ideas de muchas tecnologías: las herramientas y conceptos que estás a punto de aprender representan el esfuerzo de miles de personas, durante muchos años. En otras palabras, no estás aprendiendo "Symfony", estás aprendiendo los fundamentos de la web, buenas prácticas de desarrollo, y cómo utilizar muchas nuevas y asombrosas bibliotecas PHP, dentro o independientemente de Symfony2. Por lo tanto, ¡prepárate!

Fiel a la filosofía Symfony2, este capítulo comienza explicando el concepto fundamental común para el desarrollo web: HTTP. Independientemente de tus antecedentes o lenguaje de programación preferido, este capítulo es una **lectura obligada** para todo mundo.

HTTP es Simple
--------------

HTTP ("HyperText Transfer Protocol" para los apasionados y, en Español *Protocolo de transferencia hipertexto*) es un lenguaje de texto que permite a dos máquinas comunicarse entre sí. ¡Eso es todo! Por ejemplo, al comprobar las últimas noticias acerca de cómica `xkcd`_, la siguiente conversación (aproximadamente) se lleva a cabo:

.. image:: /images/http-xkcd-es.png
   :align: center

Y aunque el lenguaje real utilizado es un poco más formal, sigue siendo bastante simple.
HTTP es el término utilizado para describir este lenguaje simple basado en texto. Y no importa cómo desarrolles en la web, el objetivo de tu servidor *siempre* es entender las peticiones de texto simple, y devolver respuestas en texto simple.

Symfony2 está construido basado en torno a esa realidad. Ya sea que te des cuenta o no, HTTP es algo que usas todos los días. Con Symfony2, aprenderás a dominarlo.

.. index::
   single: HTTP; el paradigma petición/respuesta

Paso 1: El cliente envía una petición
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Todas las conversaciones en la web comienzan con una *petición*. La petición es un mensaje de texto creado por un cliente (por ejemplo un navegador, una aplicación para el iPhone, etc.) en un formato especial conocido como HTTP. El cliente envía la petición a un servidor, y luego espera la respuesta.

Echa un vistazo a la primera parte de la interacción (la petición) entre un navegador y el servidor web `xkcd`:

.. image:: /images/http-xkcd-request-es.png
   :align: center

Hablando en HTTP, esta petición HTTP en realidad se vería algo parecida a esto:

.. code-block:: text

    GET / HTTP/1.1
    Host: xkcd.com
    Accept: text/html
    User-Agent: Mozilla/5.0 (Macintosh)

Este sencillo mensaje comunica *todo* lo necesario sobre qué recursos exactamente solicita el cliente. La primera línea de una petición HTTP es la más importante y contiene dos cosas: la URI y el método HTTP.

La URI (por ejemplo, ``/``, ``/contacto``, etc.) es la dirección o ubicación que identifica unívocamente al recurso que el cliente quiere. El método HTTP (por ejemplo, ``GET``) define lo que quieres *hacer* con el recurso. Los métodos HTTP son los *verbos* de la petición y definen las pocas formas más comunes en que puedes actuar sobre el recurso:

+----------+---------------------------------------+
| *GET*    | Recupera el recurso desde el servidor |
+----------+---------------------------------------+
| *POST*   | Crea un recurso en el servidor        |
+----------+---------------------------------------+
| *PUT*    | Actualiza el recurso en el servidor   |
+----------+---------------------------------------+
| *DELETE* | Elimina el recurso del servidor       |
+----------+---------------------------------------+

Con esto en mente, te puedes imaginar que una petición HTTP podría ser similar a eliminar una entrada de blog específica, por ejemplo:

.. code-block:: text

    DELETE /blog/15 HTTP/1.1

.. note::

    En realidad, hay nueve métodos HTTP definidos por la especificación HTTP, pero muchos de ellos no se utilizan o apoyan ampliamente. En realidad, muchos navegadores modernos no apoyan los métodos ``PUT`` y ``DELETE``.

Además de la primera línea, una petición HTTP invariablemente contiene otras líneas de información conocidas como cabeceras de petición. Las cabeceras pueden suministrar una amplia gama de información como el ``Host`` solicitado, los formatos de respuesta que acepta el cliente (``Accept``) y la aplicación que utiliza el cliente para realizar la petición (``User-Agent``). Existen muchas otras cabeceras y se pueden encontrar en el artículo `Lista de campos de las cabeceras HTTP`_ en la Wikipedia.

Paso 2: El servidor devuelve una respuesta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Una vez que un servidor ha recibido la petición, sabe exactamente qué recursos necesita el cliente (a través de la URI) y lo que el cliente quiere hacer con ese recurso (a través del método). Por ejemplo, en el caso de una petición GET, el servidor prepara el recurso y lo devuelve en una respuesta HTTP. Considera la respuesta del servidor web, xkcd:

.. image:: /images/http-xkcd-es.png
   :align: center

Traducida a HTTP, la respuesta enviada de vuelta al navegador se verá algo similar a esto: 

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 02 Apr 2011 21:05:05 GMT
    Server: lighttpd/1.4.19
    Content-Type: text/html

    <html>
      <!-- HTML para la caricatura xkcd -->
    </html>

La respuesta HTTP contiene el recurso solicitado (contenido HTML en este caso), así como otra información acerca de la respuesta. La primera línea es especialmente importante y contiene el código de estado HTTP (200 en este caso) de la respuesta. El código de estado comunica el resultado global de la petición devuelto al cliente. ¿Tuvo éxito la petición? ¿Hubo algún error? Existen diferentes códigos de estado que indican éxito, un error o qué más se necesita hacer con el cliente (por ejemplo, redirigirlo a otra página). La lista completa se puede encontrar en el artículo `Lista de códigos de estado HTTP`_ en la Wikipedia.

Al igual que la petición, una respuesta HTTP contiene datos adicionales conocidos como cabeceras HTTP. Por ejemplo, una importante cabecera de la respuesta HTTP es ``Content-Type``. El cuerpo del mismo recurso se puede devolver en múltiples formatos, incluyendo HTML, XML o JSON por nombrar unos cuantos. La cabecera ``Content-Type`` indica al cliente en qué formato se está devolviendo.

Existen muchas otras cabeceras, algunas de las cuales son muy poderosas. Por ejemplo, ciertas cabeceras se pueden usar para crear un poderoso sistema de memoria caché.

Peticiones, respuestas y desarrollo Web
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Esta conversación petición-respuesta es el proceso fundamental que impulsa toda la comunicación en la web. Y tan importante y poderoso como es este proceso, inevitablemente es simple.

El hecho más importante es el siguiente: independientemente del lenguaje que utilices, el tipo de aplicación que construyas (web, móvil, API JSON), o la filosofía de desarrollo que sigas, el objetivo final de una aplicación siempre es **entender** cada petición y crear y devolver la respuesta adecuada.

Symfony está diseñado para adaptarse a esta realidad.

.. tip::

    Para más información acerca de la especificación HTTP, lee la referencia original `HTTP 1.1 RFC`_ o `HTTP Bis`_, el cual es un esfuerzo activo para aclarar la especificación original. Una gran herramienta para comprobar tanto la petición como las cabeceras de la respuesta mientras navegas es la extensión `Cabeceras HTTP en vivo`_ (Live HTTP Headers) para Firefox.

.. index::
   single: Fundamentos de Symfony2; Peticiones y respuestas

Peticiones y respuestas en PHP
------------------------------

Entonces ¿cómo interactúas con la "petición" y creas una "respuesta" utilizando PHP? En realidad, PHP te abstrae un poco de todo el proceso:

.. code-block:: php

    <?php
    $uri = $_SERVER['REQUEST_URI'];
    $foo = $_GET['foo'];

    header('Content-type: text/html');
    echo 'La URI solicitada es: '.$uri;
    echo 'El valor del parámetro "foo" es: '.$foo;

Por extraño que parezca, esta pequeña aplicación, de hecho, está tomando información de la petición HTTP y la utiliza para crear una respuesta HTTP. En lugar de analizar el mensaje HTTP de la petición, PHP prepara variables superglobales tales como ``$_SERVER`` y ``$_GET`` que contienen toda la información de la petición. Del mismo modo, en lugar de devolver la respuesta HTTP con formato de texto, puedes usar la función ``header()`` para crear las cabeceras de la respuesta y simplemente imprimir el contenido real que será la porción que contiene el mensaje de la respuesta. PHP creará una verdadera respuesta HTTP y la devolverá al cliente:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 03 Apr 2011 02:14:33 GMT
    Server: Apache/2.2.17 (Unix)
    Content-Type: text/html

    La URI solicitada es: /probando?foo=symfony
    El valor del parámetro "foo" es: symfony

Peticiones y respuestas en Symfony
----------------------------------

Symfony ofrece una alternativa al enfoque de PHP a través de dos clases que te permiten interactuar con la petición HTTP y la respuesta de una manera más fácil.
La clase :class:`Symfony\\Component\\HttpFoundation\\Request` es una sencilla representación orientada a objeto del mensaje de la petición HTTP. Con ella, tienes toda la información a tu alcance::

    use Symfony\Component\HttpFoundation\Request;

    $peticion = Request::createFromGlobals();

    // la URI solicitada (por ejemplo, /sobre) menos los parámetros de la consulta
    $peticion->getPathInfo();

    // recupera las variables GET y POST respectivamente
    $peticion->query->get('foo');
    $peticion->request->get('bar');

    // recupera una instancia del archivo subido identificado por foo
    $peticion->files->get('foo');

    $peticion->getMethod();          // GET, POST, PUT, DELETE, HEAD
    $peticion->getLanguages();       // un arreglo de idiomas aceptados por el cliente

Como bono adicional, en el fondo la clase ``Petición`` hace un montón de trabajo del cual nunca tendrás que preocuparte. Por ejemplo, el método ``isSecure()`` comprueba *tres* diferentes valores en PHP que pueden indicar si el usuario está conectado a través de una conexión segura (es decir, ``https``).

Symfony también proporciona una clase ``Respuesta``: una simple representación PHP de un mensaje de respuesta HTTP. Esto permite que tu aplicación utilice una interfaz orientada a objetos para construir la respuesta que será devuelta al cliente::

    use Symfony\Component\HttpFoundation\Response;
    $respuesta = new Response();

    $respuesta->setContent('<html><body><h1>¡Hola mundo!</h1></body></html>');
    $respuesta->setStatusCode(200);
    $respuesta->headers->set('Content-Type', 'text/html');

    // imprime las cabeceras HTTP seguidas por el contenido
    $respuesta->send();

Si Symfony no ofreciera nada más, ya tendrías un conjunto de herramientas para acceder fácilmente a la información de la petición y una interfaz orientada a objetos para crear la respuesta. Incluso, a medida que aprendas muchas de las poderosas características de Symfony, nunca olvides que el objetivo de tu aplicación es *interpretar una petición y crear la respuesta apropiada basada en la lógica de tu aplicación*.

.. tip::

    Las clases ``Respuesta`` y ``Petición`` forman parte de un componente independiente incluido en Symfony llamado ``HttpFoundation``. Este componente se puede utilizar completamente independiente de Symfony y también proporciona clases para manejar sesiones y subir archivos.

El viaje desde la petición hasta la respuesta
---------------------------------------------

Al igual que el mismo HTTP, los objetos ``Petición`` y ``Respuesta`` son bastante simples.
La parte difícil de la construcción de una petición es escribir lo que viene en el medio.
En otras palabras, el verdadero trabajo viene al escribir el código que interpreta la información de la petición y crea la respuesta.

Tu aplicación probablemente hace muchas cosas, como enviar correo electrónico, manejar los formularios presentados, guardar cosas en una base de datos, reproducir las páginas HTML y proteger el contenido con seguridad. ¿Cómo puedes manejar todo esto y todavía mantener tu código organizado y fácil de mantener?

Symfony fue creado para resolver estos problemas para que no tengas que hacerlo personalmente.

El controlador frontal
~~~~~~~~~~~~~~~~~~~~~~

Tradicionalmente, las aplicaciones eran construidas de modo que cada "página" de un sitio tenía su propio archivo físico:

.. code-block:: text

    index.php
    contacto.php
    blog.php

Hay varios problemas con este enfoque, incluyendo la falta de flexibilidad de las URL (¿qué pasa si quieres cambiar ``blog.php`` a ``noticias.php`` sin romper todos los vínculos?) y el hecho de que cada archivo *debe* incluir manualmente un conjunto de archivos básicos para la seguridad, conexiones a base de datos y que el "aspecto" del sitio pueda permanecer constante.

Una solución mucho mejor es usar un :term:`controlador frontal`: un solo archivo PHP que se encargue de todas las peticiones que llegan a tu aplicación. Por ejemplo:

+-------------------------+------------------------+
| ``/index.php``          | ejecuta ``index.php``  |
+-------------------------+------------------------+
| ``/index.php/contacto`` | ejecuta ``index.php``  |
+-------------------------+------------------------+
| ``/index.php/blog``     | ejecuta ``index.php``  |
+-------------------------+------------------------+

.. tip::

    Usando ``mod_rewrite`` de Apache (o equivalente con otros servidores web), las direcciones URL se pueden limpiar fácilmente hasta ser sólo ``/``, ``/contacto`` y ``/blog``.

Ahora, cada petición se maneja exactamente igual. En lugar de direcciones URL individuales ejecutando diferentes archivos PHP, el controlador frontal *siempre* se ejecuta, y el enrutado de diferentes direcciones URL a diferentes partes de tu aplicación se realiza internamente. Esto resuelve los problemas del enfoque original.
Casi todas las aplicaciones web modernas lo hacen - incluyendo aplicaciones como WordPress.

Mantente organizado
~~~~~~~~~~~~~~~~~~~

Pero dentro de tu controlador frontal, ¿cómo sabes qué página debe reproducir y cómo puedes reproducir cada una en forma sana? De una forma u otra, tendrás que comprobar la URI entrante y ejecutar diferentes partes de tu código en función de ese valor. Esto se puede poner feo rápidamente:

.. code-block:: php

    // index.php

    $peticion = Request::createFromGlobals();
    $ruta = $peticion->getPathInfo(); // la URL solicitada

    if (in_array($ruta, array('', '/')) {
        $respuesta = new Response('Bienvenido a la página inicial.');
    } elseif ($ruta == '/contacto') {
        $respuesta = new Response('Contáctanos');
    } else {
        $respuesta = new Response('Página no encontrada.', 404);
    }
    $respuesta->send();

La solución a este problema puede ser difícil. Afortunadamente esto es *exactamente* para lo que Symfony está diseñado.

El flujo de las aplicaciones Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cuando dejas que Symfony controle cada petición, la vida es mucho más fácil. Symfony sigue el mismo patrón simple en cada petición:

.. _request-flow-figure:

.. figure:: /images/flujo-peticion.png
   :align: center
   :alt: flujo de la petición en Symfony2

   Las peticiones entrantes son interpretadas por el enrutador y pasadas a las funciones controladoras que regresan objetos ``Respuesta``.

Cada "página" de tu sitio está definida en un archivo de configuración de enrutado que asigna las diferentes direcciones URL a diferentes funciones PHP. El trabajo de cada función PHP conocida como :term:`controlador`, es utilizar la información de la petición - junto con muchas otras herramientas que Symfony pone a tu disposición - para crear y devolver un objeto ``Respuesta``. En otras palabras, el controlador es donde *está tu* código: ahí es dónde se interpreta la petición y crea una respuesta.

¡Así de fácil! Repasemos:

* Cada petición ejecuta un archivo controlador frontal;

* El sistema de enrutado determina cual función PHP se debe ejecutar en base a la información de la petición y la configuración de enrutado que hemos creado;

* La función PHP correcta se ejecuta, donde tu código crea y devuelve el objeto ``Respuesta`` adecuado.

Una petición Symfony en acción
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sin bucear demasiado en los detalles, veamos este proceso en acción. Supongamos que deseas agregar una página ``/contacto`` a tu aplicación Symfony. En primer lugar, empezamos agregando una entrada ``/contacto`` a tu archivo de configuración de enrutado:

.. code-block:: yaml

    contacto:
        pattern:  /contacto
        defaults: { _controller: AcmeDemoBundle:Principal:contacto }

.. note::

   En este ejemplo utilizamos :doc:`YAML </reference/YAML>` para definir la configuración de enrutado.
   La configuración de enrutado también se puede escribir en otros formatos como XML o PHP.

Cuando alguien visita la página ``/contacto``, esta ruta coincide, y se ejecuta el controlador especificado. Como veremos en el capítulo :doc:`Enrutando </book/routing>`, La cadena ``AcmeDemoBundle:Principal:contacto`` es una sintaxis corta que apunta hacia el método PHP ``contactoAction`` dentro de una clase llamada ``PrincipalController``:

.. code-block:: php

    class PrincipalController
    {
        public function contactoAction()
        {
            return new Response('<h1>¡Contáctanos!</h1>');
        }
    }

En este ejemplo muy simple, el controlador simplemente crea un objeto ``Respuesta`` con el código HTML "<h1>¡Contáctanos!</h1>". En el capítulo :doc:`Controlador </book/controller>`, aprenderás cómo un controlador puede reproducir plantillas, permitiendo que tu código de "presentación" (es decir, algo que en realidad escribe HTML) viva en un archivo de plantilla separado. Esto libera al controlador de preocuparse sólo de las cosas difíciles: la interacción con la base de datos, la manipulación de los datos presentados o el envío de mensajes de correo electrónico. 

Symfony2: Construye tu aplicación, no tus herramientas.
-------------------------------------------------------

Ahora sabemos que el objetivo de cualquier aplicación es interpretar cada petición entrante y crear una respuesta adecuada. Cuando una aplicación crece, es más difícil mantener organizado tu código y que a la vez sea fácil darle mantenimiento. Invariablemente, las mismas tareas complejas siguen viniendo una y otra vez: la persistencia de cosas en la base de datos, procesamiento y reutilización de plantillas, manejo de formularios presentados, envío de mensajes de correo electrónico, validación de entradas del usuario y administración de la seguridad.

La buena nueva es que ninguno de estos problemas es único. Symfony proporciona una plataforma completa, con herramientas que te permiten construir tu aplicación, no tus herramientas. Con Symfony2, nada se te impone: eres libre de usar la plataforma Symfony completa, o simplemente una pieza de Symfony por sí misma.

.. index::
   single: Componentes de Symfony2

Herramientas independientes: *Componentes* de Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Entonces, ¿qué *es* Symfony2? En primer lugar, Symfony2 es una colección de más de veinte bibliotecas independientes que se pueden utilizar dentro de *cualquier* proyecto PHP. Estas bibliotecas, llamadas *componentes de Symfony2*, contienen algo útil para casi cualquier situación, independientemente de cómo desarrolles tu proyecto. Para nombrar algunos:

* `HttpFoundation`_ - Contiene las clases ``Petición`` y ``Respuesta``, así como otras clases para manejar sesiones y carga de archivos;

* `Routing`_ - Potente y rápido sistema de enrutado que te permite asignar una URI específica (por ejemplo ``/contacto``) a cierta información acerca de cómo dicha petición se debe manejar (por ejemplo, ejecutar el método ``contactoAction()``);

* `Form`_ - Una completa y flexible plataforma para crear formularios y manipular la presentación de los mismos;

* `Validator`_ Un sistema para crear reglas sobre datos y cuando el usuario envía los datos validar o no siguiendo esas reglas;

* `ClassLoader`_ Una biblioteca para carga automática que permite utilizar clases PHP sin necesidad de ``requerir`` manualmente los archivos que contienen esas clases;

* `Templating`_ Un juego de herramientas para reproducir plantillas, la cual gestiona la herencia de plantillas (es decir, una plantilla está decorada con un diseño) y realiza otras tareas de plantilla comunes;

* `Security`_ - Una poderosa biblioteca para manejar todo tipo de seguridad dentro de una aplicación;

* `Translation`_ Una plataforma para traducir cadenas en tu aplicación.

Todos y cada uno de estos componentes se desacoplan y se pueden utilizar en *cualquier* proyecto PHP, independientemente de si utilizas la plataforma Symfony2.
Cada parte está hecha para utilizarla si es conveniente y sustituirse cuando sea necesario.

La solución completa: La *plataforma* Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Entonces, ¿qué *es* la *plataforma* Symfony2? La *plataforma* Symfony2 es una biblioteca PHP que realiza dos distintas tareas:

#. Proporciona una selección de componentes (es decir, los componentes Symfony2) y bibliotecas de terceros (por ejemplo, ``SwiftMailer`` para enviar mensajes de correo electrónico);

#. Proporciona configuración sensible y un "pegamento" que une la biblioteca con todas estas piezas.

El objetivo de la plataforma es integrar muchas herramientas independientes con el fin de proporcionar una experiencia coherente al desarrollador. Incluso la propia plataforma es un paquete Symfony2 (es decir, un complemento) que se puede configurar o sustituir completamente.

Symfony2 proporciona un potente conjunto de herramientas para desarrollar aplicaciones web rápidamente sin imponerse en tu aplicación. Los usuarios normales rápidamente pueden comenzar el desarrollo usando una distribución Symfony2, que proporciona un esqueleto del proyecto con parámetros predeterminados. Para los usuarios más avanzados, el cielo es el límite.

.. _`xkcd`: http://xkcd.com/
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`HTTP Bis`: http://datatracker.ietf.org/wg/httpbis/
.. _`Cabeceras HTTP en vivo`: https://addons.mozilla.org/en-US/firefox/addon/3829/
.. _`Lista de códigos de estado HTTP`: http://es.wikipedia.org/wiki/Anexo:C%C3%B3digos_de_estado_HTTP
.. _`Lista de campos de las cabeceras HTTP`: http://en.wikipedia.org/wiki/List_of_HTTP_header_fields
.. _`HttpFoundation`: https://github.com/symfony/HttpFoundation
.. _`Routing`: https://github.com/symfony/Routing
.. _`Form`: https://github.com/symfony/Form
.. _`Validator`: https://github.com/symfony/Validator
.. _`ClassLoader`: https://github.com/symfony/ClassLoader
.. _`Templating`: https://github.com/symfony/Templating
.. _`Security`: https://github.com/symfony/Security
.. _`Translation`: https://github.com/symfony/Translation
