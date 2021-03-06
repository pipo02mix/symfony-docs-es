.. index::
   single: Caché

Caché HTTP
==========

La naturaleza de las aplicaciones web ricas significa que son dinámicas. No importa qué tan eficiente sea tu aplicación, cada petición siempre contendrá más sobrecarga que servir un archivo estático.

Y para la mayoría de las aplicaciones Web, está bien. Symfony2 es tan rápido como el rayo, a menos que estés haciendo una muy complicada aplicación, cada petición se responderá rápidamente sin poner demasiada tensión a tu servidor.

Pero cuando tu sitio crezca, la sobrecarga general se puede convertir en un problema. El procesamiento que se realiza normalmente en cada petición se debe hacer sólo una vez. Este exactamente es el objetivo que tiene que consumar la memoria caché.

La memoria caché en hombros de gigantes
---------------------------------------

La manera más efectiva para mejorar el rendimiento de una aplicación es memorizar en caché la salida completa de una página y luego eludir por completo la aplicación en cada petición posterior. Por supuesto, esto no siempre es posible para los sitios web altamente dinámicos, ¿o no? En este capítulo, te mostraremos cómo funciona el sistema de caché Symfony2 y por qué pensamos que este es el mejor enfoque posible.

El sistema de cache Symfony2 es diferente porque se basa en la simplicidad y el poder de la caché HTTP tal como está definido en la :term:`especificación HTTP`.
En lugar de reinventar una metodología de memoria caché, Symfony2 adopta la norma que define la comunicación básica en la Web. Una vez que comprendas los principios fundamentales de los modelos de caducidad y validación de la memoria caché HTTP, estarás listo para dominar el sistema de caché Symfony2.

Para efectos de aprender cómo guardar en caché con Symfony2, vamos a cubrir el tema en cuatro pasos:

* **Paso 1**: Una :ref:`pasarela de caché <gateway-caches>`, o sustituto inverso, es una capa independiente situada frente a tu aplicación. La caché del sustituto inverso responde a medida que son devueltas desde tu aplicación y responde a peticiones con respuestas de la caché antes de que lleguen a tu aplicación. Symfony2 proporciona su propio sustituto inverso, pero puedes utilizar cualquier sustituto inverso.

* **Paso 2** :ref:`cache HTTP <http-cache-introduction>` las cabeceras se utilizan para comunicarse con la pasarela de caché y cualquier otro caché entre la aplicación y el cliente. Symfony2 proporciona parámetros predeterminados y una potente interfaz para interactuar con las cabeceras de caché.

* **Paso 3**: HTTP :ref:`caducidad y validación <http-expiration-validation>` son los dos modelos utilizados para determinar si el contenido memorizado en caché es *fresco* (se puede reutilizar de la memoria caché) u *obsoleto* (lo debe regenerar la aplicación).

* **Paso 4** : :ref:`Inclusión del borde lateral <edge-side-includes>` (Edge Side Includes -|ESI|) permite que la caché HTTP utilice fragmentos de la página en caché (incluso fragmentos anidados) independientemente.
  Con |ESI|, incluso puedes guardar en caché una página entera durante 60 minutos, pero una barra lateral integrada sólo por 5 minutos.

Dado que la memoria caché HTTP no es exclusiva de Symfony, ya existen muchos artículos sobre el tema. Si eres nuevo para la memoria caché HTTP, te *recomendamos* el artículo de Ryan Tomayko `Things Caches Do`_. Otro recurso en profundidad es la `Guía de caché`_ de Mark Nottingham.

.. index::
   single: Caché; Proxy
   single: Caché; Proxy inverso
   single: Caché; Pasarela

.. _gateway-caches:

Memoria caché con pasarela de caché
-----------------------------------

Cuándo memorizar caché con HTTP, la *caché* está separada de tu aplicación por completo y se sitúa entre tu aplicación y el cliente haciendo la petición.

El trabajo de la caché es aceptar las peticiones del cliente y pasarlas de nuevo a tu aplicación. La memoria caché también recibirá las respuestas devueltas por tu aplicación y las remitirá al cliente. La caché es el "geniecillo" de la comunicación petición-respuesta entre el cliente y tu aplicación.

En el camino, la memoria caché almacena cada respuesta que se considere "cacheable" (consulta :ref:`http-cache-introduction`). Si de nuevo se solicita el mismo recurso, la memoria caché envía la respuesta memorizada en caché al cliente, eludiendo tu aplicación por completo.

Este tipo de caché se conoce como caché de puerta de enlace HTTP y existen muchos como `Varnish`_, `Squid en modo sustituto inverso`_ y el sustituto inverso de Symfony2.

.. index::
   single: Caché; Tipos de

Tipos de Caché
~~~~~~~~~~~~~~

Sin embargo, una puerta de enlace caché no es el único tipo de caché. De hecho, las cabeceras de caché HTTP enviadas por tu aplicación son consumidas e interpretadas por un máximo de tres diferentes tipos de caché:

* *Caché de navegadores*: Cada navegador viene con su propia caché local que es realmente útil para cuando pulsas "atrás" o en imágenes y otros activos.
  La caché del navegador es una caché *privada*, los recursos memorizados en caché no se comparten con nadie más.

* *Cachés sustitutas*: Una sustituta es una caché *compartida* en que muchas personas pueden estar detrás de una sola. Por lo general instalada por las grandes corporaciones y proveedores de Internet para reducir latencia y tráfico de red.

* *Pasarelas de caché*: Al igual que un sustituto, también es una caché *compartida* pero de lado del servidor. Instalada por los administradores de red, esta tiene sitios web más escalables, confiables y prácticos.

.. tip::

    Las pasarelas de caché a veces se conocen como caché sustituta inversa, cachés alquiladas o incluso aceleradores HTTP.

.. note::

    La importancia de la caché *privada* frente a la *compartida* será más evidente a medida que hablemos de las respuestas en la memoria caché con contenido que es específico para un solo usuario (por ejemplo, información de cuenta).

Cada respuesta de tu aplicación probablemente vaya a través de uno o los dos primeros tipos de caché. Estas cachés están fuera de tu control, pero siguen las instrucciones de caché HTTP establecidas en la respuesta.

.. index::
   single: Caché; Sustituta inversa de Symfony2

.. _`symfony-gateway-cache`:

Sustituto inverso de Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 viene con una caché sustituta inversa (también llamada memoria caché de puerta de enlace) escrita en PHP. Que al activarlo, inmediatamente puede memorizar en caché respuestas de tu aplicación. La instalación es muy fácil. Cada nueva aplicación Symfony2 viene con un núcleo preconfigurado memorizado en caché (``AppCache``) que envuelve el (``AppKernel``) predeterminado. La memoria caché del núcleo *es* el sustituto inverso.

Para habilitar la memoria caché, modifica el código de un controlador frontal para utilizar la memoria caché del núcleo::

    // web/app.php

    require_once __DIR__.'/../app/bootstrap_cache.php.cache';
    require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\Request;

    // pone el AppKernel predeterminado con un AppCache
    $kernel = new AppCache(new AppKernel('prod', false));
    $kernel->handle(Request::createFromGlobals())->send();

La memoria caché del núcleo actúa de inmediato como un sustituto inverso - memorizando en caché las respuestas de tu aplicación y devolviéndolas al cliente.

.. tip::

    La caché del núcleo tiene un método especial `getLog()`, el cual devuelve una cadena que representa lo que sucedió en la capa de la caché. En el entorno de desarrollo, se usa para depurar y validar la estrategia de caché::

        error_log($kernel->getLog());

El objeto ``AppCache`` tiene una configuración predeterminada sensible, pero la puedes afinar por medio de un conjunto de opciones que puedes configurar sustituyendo el método ``getOptions()``::

    // app/AppCache.php
    class AppCache extends Cache
    {
        protected function getOptions()
        {
            return array(
                'debug'                  => false,
                'default_ttl'            => 0,
                'private_headers'        => array('Authorization', 'Cookie'),
                'allow_reload'           => false,
                'allow_revalidate'       => false,
                'stale_while_revalidate' => 2,
                'stale_if_error'         => 60,
            );
        }
    }

.. tip::

    A menos que la sustituyas en ``getOptions()``, la opción ``debug`` se establecerá automáticamente al valor de depuración del ``AppKernel`` envuelto.

Aquí está una lista de las principales opciones:

* ``default_ttl``: El número de segundos que una entrada de caché se debe considerar nueva cuando no hay información fresca proporcionada explícitamente en una respuesta. Las cabeceras explícitas ``Cache-Control`` o ``Expires`` sustituyen este valor (predeterminado: ``0``);

* ``private_headers``: Conjunto de cabeceras de la petición que desencadenan el comportamiento "privado" ``Cache-Control`` en las respuestas en que la respuesta explícitamente no es ``pública`` o ``privada`` vía una directiva ``Cache-Control``.
  (predeterminado: ``Authorization`` y ``Cookie``);

* ``allow_reload``: Especifica si el cliente puede forzar una recarga desde caché incluyendo una directiva ``Cache-Control`` "no-cache" en la petición. Selecciona ``true`` para cumplir con la RFC 2616 (por omisión: ``false``);

* ``allow_revalidate``: Especifica si el cliente puede forzar una revalidación de caché incluyendo una directiva ``Cache-Control`` ``max-age = 0`` en la petición. Selecciona ``true`` para cumplir con la RFC 2616 (por omisión: ``false``);

* ``stale_while_revalidate``: Especifica el número predeterminado de segundos (la granularidad es el segundo como la precisión de respuesta TTL es un segundo) en el que la memoria caché puede regresar inmediatamente una respuesta obsoleta mientras que revalida en el fondo (por omisión: ``2``), este valor lo reemplaza ``stale-while-revalidate`` de la extensión HTTP ``Cache-Control`` (consulta la RFC 5.861);

* ``stale_if_error``: Especifica el número de segundos predeterminado (la granularidad es el segundo) durante el cual la caché puede servir una respuesta obsoleta cuando se detecta un error (por omisión: ``60``). Este valor lo reemplaza ``stale-if-error`` de la extensión HTTP ``Cache-Control`` (consulta la RFC 5861).

Si ``debug`` es ``true``, Symfony2 automáticamente agrega una cabecera ``X-Symfony-Cache`` a la respuesta que contiene información útil acerca de aciertos y errores de caché.

.. sidebar:: Cambiando de un sustituto inverso a otro

    El sustituto inverso de Symfony2 es una gran herramienta a utilizar en el desarrollo de tu sitio web o al desplegar tu web en un servidor compartido donde no puedes instalar nada más allá que código PHP. Pero está escrito en PHP, no puede ser tan rápido como un sustituto escrito en C. Es por eso que recomendamos usar Vanish o Squid en tus servidores de producción de ser posible. La buena nueva es que el cambio de un servidor sustituto a otro es fácil y transparente, sin modificar el código necesario en tu aplicación. Comienza fácilmente con el sustituto inverso de Symfony2 y actualiza a Varnish cuando aumente el tráfico.

    Para más información sobre el uso de Varnish con Symfony2, consulta el capítulo :doc:`Cómo usar Varnish </cookbook/cache/varnish>` del recetario.

.. note::

    El rendimiento del sustituto inverso de Symfony2 es independiente de la complejidad de tu aplicación. Eso es porque el núcleo de tu aplicación sólo se inicia cuando la petición se debe remitir a ella.

.. index::
   single: Caché; HTTP

.. _http-cache-introduction:

Introducción a la memoria caché HTTP
------------------------------------

Para aprovechar las ventajas de las capas de memoria caché disponibles, tu aplicación se debe poder comunicar con las respuestas que son memorizables y las reglas que rigen cuándo y cómo la caché será obsoleta. Esto se hace ajustando las cabeceras de caché HTTP en la respuesta.

.. tip::

    Ten en cuenta que "HTTP" no es más que el lenguaje (un lenguaje de texto simple) que los clientes web (navegadores, por ejemplo) y los servidores web utilizan para comunicarse entre sí. Cuando hablamos de la memoria caché HTTP, estamos hablando de la parte de ese lenguaje que permite a los clientes y servidores intercambiar información relacionada con la memoria caché.

HTTP especifica cuatro cabeceras de memoria caché de la respuesta en que estamos interesados:

* ``Cache-Control``
* ``Expires``
* ``ETag``
* ``Last-Modified``

La cabecera más importante y versátil es la cabecera ``Cache-Control``, la cual en realidad es una colección de variada información de la caché.

.. note::

    Cada una de las cabeceras se explica en detalle en la sección :ref:`http-expiration-validation`.

.. index::
   single: Caché; Cabecera Cache-Control
   single: Cabeceras HTTP; Cache-Control

La cabecera ``Cache-Control``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La cabecera ``Cache-Control`` es la única que no contiene una, sino varias piezas de información sobre la memoria caché de una respuesta. Cada pieza de información está separada por una coma::

     Cache-Control: private, max-age=0, must-revalidate

     Cache-Control: max-age=3600, must-revalidate

Symfony proporciona una abstracción de la cabecera ``Cache-Control`` para hacer más manejable su creación:

.. code-block:: php

    $respuesta = new Response();

    // marca la respuesta como publica o privada
    $respuesta->setPublic();
    $respuesta->setPrivate();

    // fija la edad máxima de privado o compartido
    $respuesta->setMaxAge(600);
    $respuesta->setSharedMaxAge(600);

    // fija una directiva personalizada Cache-Control
    $respuesta->headers->addCacheControlDirective('must-revalidate', true);

Respuestas públicas frente a privadas
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ambas, la pasarela de caché y la caché sustituta, son consideradas como cachés "compartidas" debido a que el contenido memorizado en caché es compartido por más de un usuario. Si cada vez equivocadamente una memoria caché compartida almacena una respuesta específica al usuario, posteriormente la puede devolver a cualquier cantidad de usuarios diferentes. ¡Imagina si la información de tu cuenta se memoriza en caché y luego la regresa a todos los usuarios posteriores que soliciten la página de su cuenta!

Para manejar esta situación, cada respuesta se puede fijar para que sea pública o privada:

* *public*: Indica que la respuesta se puede memorizar en caché por ambas cachés privadas y compartidas;

* *private*: Indica que la totalidad o parte del mensaje de la respuesta es para un solo usuario y no se debe tener en caché en una memoria caché compartida.

Symfony por omisión conservadoramente fija cada respuesta para que sea privada. Para aprovechar las ventajas de cachés compartidas (como la sustituta inversa de Symfony2), la respuesta explícitamente se fijará como pública.

.. index::
   single: Caché; Métodos seguros

Métodos seguros
~~~~~~~~~~~~~~~

La memoria caché HTTP sólo funciona para métodos HTTP "seguros" (como GET y HEAD). Estar seguro significa que nunca cambia de estado la aplicación en el servidor al servir la petición (por supuesto puedes registrar información, datos de la caché, etc.)
Esto tiene dos consecuencias muy razonables:

* *Nunca* debes cambiar el estado de tu aplicación al responder a una petición GET o HEAD. Incluso si no utilizas una pasarela caché, la presencia de la caché sustituta significa que alguna petición ``GET`` o ``HEAD`` puede o no llegar a tu servidor.

* No esperas métodos PUT, POST o DELETE en caché. Estos métodos están diseñados para utilizarse al mutar el estado de tu aplicación (por ejemplo, borrar una entrada de blog). La memoria caché debe impedir que determinadas peticiones toquen y muten tu aplicación.

Reglas de caché y valores predeterminados
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

HTTP 1.1 por omisión, permite a cualquiera memorizar en caché a menos que haya una cabecera ``Cache-Control`` explícita. En la práctica, la mayoría de cachés no hacen nada cuando solicitan una galleta, una cabecera de autorización, utilizar un método no seguro (es decir, PUT, POST, DELETE), o cuando las respuestas tienen código de redirección de estado.

Symfony2 automáticamente establece una sensible y conservadora cabecera ``Cache-Control`` cuando esta no está definida por el desarrollador, siguiendo estas reglas:

* Si no haz definido cabecera caché (``Cache-Control``, ``Expires``,  ``ETag`` o ``Last-Modified``), ``Cache-Control`` es establecida en ``no-cache``, lo cual significa que la respuesta no se memoriza en caché;

* Si ``Cache-Control`` está vacía (pero una de las otras cabeceras de caché está presente), su valor se establece en ``private, must-revalidate``;

* Pero si por lo menos una  directiva ``Cache-Control`` está establecida, y no se han añadido directivas ``public`` o ``private`` de forma explícita, Symfony2 agrega la directiva ``private`` automáticamente (excepto cuando ``s-maxage`` está establecida).

.. _http-expiration-validation:

Caducidad y validación HTTP
---------------------------

La especificación HTTP define dos modelos de memoria caché:

* Con el `modelo de caducidad`_, sólo tienes que especificar el tiempo en que la respuesta se debe considerar "fresca" incluyendo una cabecera ``Cache-Control`` y/o una ``Expires``. Las cachés que entienden de expiración no harán la misma petición hasta que la versión en caché llegue a su fecha de caducidad y se convierta en "obsoleta".

* Cuando las páginas realmente son muy dinámicas (es decir, su representación cambia frecuentemente), a menudo es necesario el `modelo de validación`_. Con este modelo, la memoria caché memoriza la respuesta, pero, pregunta al servidor en cada petición si la respuesta memorizada sigue siendo válida. La aplicación utiliza un identificador de respuesta único (la cabecera ``Etag``) y/o una marca de tiempo (la cabecera ``Last-Modified``) para comprobar si la página ha cambiado desde su caché.

El objetivo de ambos modelos es nunca generar la misma respuesta en dos ocasiones dependiendo de una caché para almacenar y devolver respuestas "fresco".

.. sidebar:: Leyendo la especificación HTTP

    La especificación HTTP define un lenguaje sencillo pero potente en el cual clientes y servidores se pueden comunicar. Como desarrollador web, el modelo petición-respuesta de la especificación domina nuestro trabajo. Lamentablemente, el documento de la especificación real - `RFC 2616`_ - puede ser difícil de leer.

    Hay un esfuerzo en curso (`HTTP Bis`_) para reescribir la RFC 2616. Este no describe una nueva versión de HTTP, sino sobre todo aclara la especificación HTTP original. También mejora la organización de la especificación dividiéndola en siete partes, todo lo relacionado con la memoria caché HTTP se puede encontrar en dos partes dedicadas (`P4 - Petición condicional`_ y `P6 - En caché: Exploración y caché intermediaria`_).

    Como desarrollador web, te invitamos a leer la especificación. Su claridad y poder - incluso más de diez años después de su creación - tiene un valor incalculable. No te desanimes por la apariencia de la especificación - su contenido es mucho más bello que la cubierta.

.. index::
   single: Caché; Caducidad HTTP

Caducidad
~~~~~~~~~

El modelo de caducidad es el más eficiente y simple de los dos modelos de memoria caché y se debe utilizar siempre que sea posible. Cuando una respuesta se memoriza en caché con una caducidad, la caché memorizará la respuesta y la enviará directamente sin tocar a la aplicación hasta que esta caduque.

El modelo de caducidad se puede lograr usando una de dos, casi idénticas, cabeceras HTTP: ``Expires`` o ``Cache-Control``.

.. index::
   single: Caché; Cabecera expires
   single: Cabeceras HTTP; Expires

Caducidad con la cabecera ``Expires``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

De acuerdo con la especificación HTTP "el campo de la cabecera ``Expires`` da la fecha/hora después de la cual se considera la respuesta es vieja". La cabecera ``Expires`` se puede establecer con el método ``setExpires()`` de ``Respuesta``. Esta necesita una instancia de ``DateTime`` como argumento::

    $date = new DateTime();
    $date->modify('+600 seconds');

    $respuesta->setExpires($date);

El resultado de la cabecera HTTP se verá así::

    Expires: Thu, 01 Mar 2011 16:00:00 GMT

.. note::

    El método ``setExpires()`` automáticamente convierte la fecha a la zona horaria GMT como lo requiere la especificación.

La cabecera ``Expires`` adolece de dos limitaciones. En primer lugar, los relojes en el servidor Web y la caché (por ejemplo el navegador) deben estar sincronizados. Luego, la especificación establece que "los servidores HTTP/1.1 no deben enviar a ``Expires`` fechas de más de un año en el futuro".

.. index::
   single: Caché; Cabecera Cache-Control
   single: Cabeceras HTTP; Cache-Control

Caducidad con la cabecera ``Cache-Control``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Debido a las limitaciones de la cabecera ``Expires``, la mayor parte del tiempo, debes usar la cabecera ``Cache-Control`` en su lugar. Recordemos que la cabecera ``Cache-Control`` se utiliza para especificar muchas directivas de caché diferentes. Para caducidad, hay dos directivas, ``max-age`` y ``s-maxage``. La primera la utilizan todas las cachés, mientras que la segunda sólo se tiene en cuenta por las cachés compartidas::

    // Establece el número de segundos después de que la
    // respuesta ya no se debe considerar fresca
    $respuesta->setMaxAge(600);

    // Lo mismo que la anterior pero sólo para cachés compartidas
    $respuesta->setSharedMaxAge(600);

La cabecera ``Cache-Control`` debería tener el siguiente formato (el cual puede tener directivas adicionales)::

    Cache-Control: max-age=600, s-maxage=600

.. index::
   single: Caché; Validación

Validando
~~~~~~~~~

Cuando un recurso se tiene que actualizar tan pronto como se realiza un cambio en los datos subyacentes, el modelo de caducidad se queda corto. Con el modelo de caducidad, no se pedirá a la aplicación que devuelva la respuesta actualizada hasta que la caché finalmente se convierta en obsoleta.

El modelo de validación soluciona este problema. Bajo este modelo, la memoria caché sigue almacenando las respuestas. La diferencia es que, por cada petición, la caché pregunta a la aplicación cuando o no la respuesta memorizada sigue siendo válida. Si la caché todavía *es* válida, tu aplicación debe devolver un código de estado 304 y no el contenido. Esto le dice a la caché que está bien devolver la respuesta memorizada.

Bajo este modelo, sobre todo ahorras ancho de banda ya que la representación no se envía dos veces al mismo cliente (en su lugar se envía una respuesta 304). Pero si diseñas cuidadosamente tu aplicación, es posible que puedas obtener los datos mínimos necesarios para enviar una respuesta 304 y ahorrar CPU también (más abajo puedes ver una implementación de ejemplo).

.. tip::

    El código de estado 304 significa "No Modificado". Es importante porque este código de estado *no* tiene el contenido real solicitado.
    En cambio, la respuesta simplemente es un ligero conjunto de instrucciones que indican a la caché que se debe utilizar la versión almacenada.

Al igual que con la caducidad, hay dos cabeceras HTTP diferentes que se pueden utilizar para implementar el modelo de validación: ``Etag`` y ``Last-Modified``.

.. index::
   single: Caché; Cabecera Etag
   single: Cabeceras HTTP; Etag

Validación con la cabecera ``ETag``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La cabecera ``ETag`` es una cabecera de cadena (llamada "entidad-etiqueta") que identifica unívocamente una representación del recurso destino. Este es generado completamente y establecido por tu aplicación de modo que puedes decir, por ejemplo, si el recurso memorizado ``/sobre`` está al día con el que tu aplicación iba a devolver. Una ``ETag`` es como una huella digital y se utiliza para comparar rápidamente si dos versiones diferentes de un recurso son equivalentes. Como las huellas digitales, cada ``ETag`` debe ser única en todas las representaciones de un mismo recurso.

Vamos a caminar a través de una aplicación sencilla que genera el ETag como el md5 del contenido::

    public function indexAction()
    {
        $respuesta = $this->render('MiPaquete:Main:index.html.twig');
        $respuesta->setETag(md5($respuesta->getContent()));
        $respuesta->isNotModified($this->getRequest());

        return $respuesta;
    }

El método ``Response::isNotModified()`` compara la ``ETag`` enviada en la ``Petición`` con la enviada en la ``Respuesta``. Si ambas coinciden, el método establece automáticamente el código de estado de la ``Respuesta`` a 304.

Este algoritmo es bastante simple y muy genérico, pero es necesario crear la ``Respuesta`` completa antes de ser capaz de calcular el ETag, lo cual es subóptimo.
En otras palabras, esta ahorra ancho de banda, pero no ciclos de CPU.

En la sección :ref:`optimizing-cache-validation`, vamos a mostrar cómo puedes utilizar la validación de manera más inteligente para determinar la validez de una caché sin hacer tanto trabajo.

.. tip::

    Symfony2 también apoya ETags débiles pasando ``true`` como segundo argumento del método :method:`Symfony\\Component\\HttpFoundation\\Response::setETag`.

.. index::
   single: Caché; Cabecera Last-Modified
   single: Cabeceras HTTP; Last-Modified

Validación con la cabecera ``Last-Modified``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La cabecera ``Last-Modified`` es la segunda forma de validación. De acuerdo con la especificación HTTP, "El campo de la cabecera ``Last-Modified`` indica la fecha y hora en que el servidor origen considera que la representación fue modificada por última vez". En otras palabras, la aplicación decide si o no el contenido memorizado se ha actualizado en función de si es o no ha sido actualizado desde que la respuesta entró en caché.

Por ejemplo, puedes utilizar la última fecha de actualización de todos los objetos necesarios para calcular la representación del recurso como valor para el valor de la cabecera ``Last-Modified``::

    public function showAction($articleSlug)
    {
        // ...

        $articuloFecha = new \DateTime($articulo->getUpdatedAt());
        $autorFecha = new \DateTime($autor->getUpdatedAt());

        $date = $autorFecha > $articuloFecha ? $autorFecha : $articuloFecha;

        $respuesta->setLastModified($fecha);
        $respuesta->isNotModified($this->getRequest());

        return $respuesta;
    }

El método ``Response::isNotModified()`` compara la cabecera ``If-Modified-Since`` enviada por la petición con la cabecera ``Last-Modified`` situada en la respuesta. Si son equivalentes, la ``Respuesta`` establecerá un código de estado 304.

.. note::

    La cabecera ``If-Modified-Since`` de la petición es igual a la cabecera ``Last-Modified`` de la última respuesta enviada al cliente por ese recurso en particular.
    Así es como se comunican el cliente y el servidor entre ellos y deciden si el recurso se ha actualizado desde que se memorizó.

.. index::
   single: Caché; Get Condicional
   single: HTTP; 304

.. _optimizing-cache-validation:

Optimizando tu código con validación
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El objetivo principal de cualquier estrategia de memoria caché es aligerar la carga de la aplicación.
Dicho de otra manera, cuanto menos hagas en tu aplicación para devolver una respuesta 304, mejor. El método ``Response::isNotModified()`` hace exactamente eso al exponer un patrón simple y eficiente::

    public function showAction($articleSlug)
    {
        // Obtiene la mínima información para calcular la ETag
        // o el valor de Last-Modified (basado en la petición,
        // los datos se recuperan de una base de datos o un par
        // clave-valor guardado, por ejemplo)
        $articulo = // ...

        // crea una respuesta con una ETag y/o una cabecera Last-Modified
        $respuesta = new Response();
        $respuesta->setETag($articulo->computeETag());
        $respuesta->setLastModified($articulo->getPublishedAt());

        // verifica que la respuesta no sea modificada por la petición dada
        if ($respuesta->isNotModified($this->getRequest())) {
            // devuelve inmediatamente la Respuesta 304
            return $respuesta;
        } else {
            // aquí hace más trabajo - como recuperar más datos
            $comentarios = // ...
            
            // o reproduce una plantilla con la $respuesta que ya hemos iniciado
            return $this->render(
                'MiPaquete:MiController:articulo.html.twig',
                array('articulo' => $articulo, 'comentarios' => $comentarios),
                $respuesta
            );
        }
    }

Cuando la ``Respuesta`` no es modificada, el ``isNotModified()`` automáticamente fija el código de estado de la respuesta a ``304``, remueve el contenido, y remueve algunas cabeceras que no deben estar presentes en respuestas ``304`` (consulta
:method:`Symfony\\Component\\HttpFoundation\\Response::setNotModified`).

.. index::
   single: Caché; Varíe
   single: Cabeceras HTTP; Varíe

Variando la respuesta
~~~~~~~~~~~~~~~~~~~~~

Hasta ahora, hemos supuesto que cada URI tiene exactamente una representación del recurso destino. De forma predeterminada, la memoria caché HTTP se realiza mediante la URI del recurso como la clave de caché. Si dos personas solicitan la misma URI de un recurso memorizable, la segunda persona recibirá la versión en caché.

A veces esto no es suficiente y diferentes versiones de la misma URI necesitan memorizarse en caché basándose en uno o más valores de las cabeceras de la petición. Por ejemplo, si comprimes las páginas cuando el cliente lo permite, cualquier URI tiene dos representaciones: una cuando el cliente es compatible con la compresión, y otra cuando no. Esta determinación se hace por el valor de la cabecera ``Accept-Encoding`` de la petición.

En este caso, necesitamos que la memoria almacene una versión comprimida y otra sin comprimir de la respuesta para la URI particular y devolverlas basándose en el valor de la cabecera ``Accept-Encoding``. Esto se hace usando la cabecera ``Vary`` de la respuesta, la cual es una lista separada por comas de diferentes cabeceras cuyos valores lanzan una representación diferente de los recursos solicitados::

    Vary: Accept-Encoding, User-Agent

.. tip::

    Esta cabecera ``Vary`` particular debería memorizar diferentes versiones de cada recurso en base a la URI y el valor de las cabeceras ``Accept-Encoding`` y ``User-Agent`` de la petición.

El objeto ``Respuesta`` ofrece una interfaz limpia para gestionar la cabecera ``Vary``::

    // establece una cabecera vary
    $respuesta->setVary('Accept-Encoding');

    // establece múltiples cabeceras vary
    $respuesta->setVary(array('Accept-Encoding', 'User-Agent'));

El método ``setVary()`` toma un nombre de cabecera o un arreglo de nombres de cabecera de cual respuesta varía.

Caducidad y validación
~~~~~~~~~~~~~~~~~~~~~~

Por supuesto, puedes utilizar tanto la caducidad como la validación de la misma ``Respuesta``.
La caducidad gana a la validación, te puedes beneficiar de lo mejor de ambos mundos. En otras palabras, utilizando tanto la caducidad como la validación, puedes instruir a la caché para que sirva el contenido memorizado, mientras que revisas de nuevo algún intervalo (de  caducidad) para verificar que el contenido sigue siendo válido.

.. index::
    pair: Caché; Configuración

Más métodos de respuesta
~~~~~~~~~~~~~~~~~~~~~~~~

La clase Respuesta proporciona muchos métodos más relacionados con la caché. Estos son los más útiles::

    // Marca la respuesta como obsoleta
    $respuesta->expire();

    // Fuerza a la respuesta a devolver una adecuada respuesta 304 sin contenido
    $respuesta->setNotModified();

Además, puedes confugurar muchas de las cabeceras HTTP relacionadas con la caché a través del método ``setCache()``::

    // Establece la configuración de caché en una llamada
    $respuesta->setCache(array(
        'etag'          => $etag,
        'last_modified' => $date,
        'max_age'       => 10,
        's_maxage'      => 10,
        'public'        => true,
        // 'private'    => true,
    ));

.. index::
  single: Caché; ESI
  single: ESI

.. _edge-side-includes:

Usando inclusión del borde lateral
----------------------------------

Las pasarelas de caché son una excelente forma de hacer que tu sitio web tenga un mejor desempeño. Pero tienen una limitación: sólo podrán memorizar páginas enteras. Si no puedes memorizar todas las páginas o si partes de una página tienen "más" elementos dinámicos, se te acabó la suerte. Afortunadamente, Symfony2 ofrece una solución para estos casos, basada ​​en una tecnología llamada `ESI`_, o Inclusión de bordes laterales (Edge Side Includes). Akamaï escribió esta especificación hace casi 10 años, y esta permite que partes específicas de una página tengan una estrategia de memorización diferente a la de la página principal.

La especificación |ESI| describe las etiquetas que puedes incrustar en tus páginas para comunicarte con la pasarela de caché. Symfony2 sólo implementa una etiqueta, ``include``, ya que es la única útil fuera del contexto de Akamaï:

.. code-block:: html

    <html>
        <body>
            Algún contenido

            <!-- aquí integra el contenido de otra página -->
            <esi:include src="http://..." />

            Más contenido
        </body>
    </html>

.. note::

    Observa que en el ejemplo cada etiqueta |ESI| tiene una URL completamente cualificada.
    Una etiqueta |ESI| representa un fragmento de página que se puede recuperar a través de la URL.

Cuando se maneja una petición, la pasarela de caché obtiene toda la página de su caché o la pide a partir de la interfaz de administración de tu aplicación. Si la respuesta contiene una o más etiquetas |ESI|, estas se procesan de la misma manera. En otras palabras, la pasarela caché o bien, recupera el fragmento de página incluida en su caché o de nuevo pide el fragmento de página desde la interfaz de administración de tu aplicación. Cuando se han resuelto todas las etiquetas |ESI|, la pasarela caché une cada una en la página principal y envía el contenido final al cliente.

Todo esto sucede de forma transparente a nivel de la pasarela caché (es decir, fuera de tu aplicación). Como verás, si decides tomar ventaja de las etiquetas |ESI|, Symfony2 hace que el proceso de incluirlas sea casi sin esfuerzo.

Usando |ESI| en Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~

Primero, para usar |ESI|, asegúrate de activarlo en la configuración de tu aplicación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            esi: { enabled: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:esi enabled="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('framework', array(
            // ...
            'esi'    => array('enabled' => true),
        ));

Ahora, supongamos que tenemos una página que es relativamente estable, a excepción de un teletipo de noticias en la parte inferior del contenido. Con |ESI|, podemos memorizar el teletipo de noticias independiente del resto de la página.

.. code-block:: php

    public function indexAction()
    {
        $respuesta = $this->render('MiPaquete:MiController:index.html.twig');
        $respuesta->setSharedMaxAge(600);

        return $respuesta;
    }

En este ejemplo, hemos dado al caché de la página completa un tiempo de vida de diez minutos.
En seguida, vamos a incluir el teletipo de noticias en la plantilla incorporando una acción.
Esto se hace a través del ayudante ``render`` (consulta la sección :ref:`templating-embedding-controller` para más detalles).

Como el contenido integrado viene de otra página (o controlador en este caso), Symfony2 utiliza el ayudante ``render`` estándar para configurar las etiquetas |ESI|:

.. configuration-block::

    .. code-block:: jinja

        {% render '...:noticias' with {}, {'standalone': true} %}

    .. code-block:: php

        <?php echo $view['actions']->render('...:noticias', array(), array('standalone' => true)) ?>

Al establecer ``standalone`` a ``true``, le dices a Symfony2 que la acción se debe reproducir como una etiqueta |ESI|. Tal vez te preguntes por qué querría usar un ayudante en vez de escribir la etiqueta |ESI| en si misma. Eso es porque usar un ayudante hace que tu aplicación trabaje, incluso si no hay pasarela caché instalada. Vamos a ver cómo funciona.

Cuando ``standalone`` es ``false`` (predeterminado), Symfony2 combina el contenido de la página incluida en la principal antes de enviar la respuesta al cliente. Pero cuando ``standalone`` es ``true``, *y* si Symfony2 detecta que está hablando con una pasarela caché compatible con |ESI|, genera una etiqueta ``include`` |ESI|. Pero si no hay una pasarela caché o si no es compatible con |ESI|, Symfony2 termina fusionando el contenido de las páginas incluidas en la principal como lo habría hecho si ``standalone`` se hubiera establecido en ``false``.

.. note::

    Symfony2 detecta si una pasarela caché admite |ESI| a través de otra especificación Akamaï que fuera de la caja es compatible con el sustituto inverso de Symfony2.

La acción integrada ahora puede especificar sus propias reglas de caché, totalmente independientes de la página principal.

.. code-block:: php

    public function noticiasAction()
    {
      // ...

      $respuesta->setSharedMaxAge(60);
    }

Con |ESI|, la caché de la página completa será válida durante 600 segundos, pero la caché del componente de noticias sólo dura 60 segundos.

Un requisito de |ESI|, sin embargo, es que la acción incrustada sea accesible a través de una URL para que la pasarela caché se pueda buscar independientemente del resto de la página. Por supuesto, una acción no se puede acceder a través de una URL a menos que haya una ruta que apunte a la misma. Symfony2 se encarga de esto a través de una ruta genérica y un controlador. Para que la etiqueta |ESI| ``include`` funcione correctamente, debes definir la ruta ``_internal``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        _internal:
            resource: "@FrameworkBundle/Resources/config/routing/internal.xml"
            prefix:   /_internal

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@FrameworkBundle/Resources/config/routing/internal.xml" prefix="/_internal" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion->addCollection($loader->import('@FrameworkBundle/Resources/config/routing/internal.xml', '/_internal'));

        return $coleccion;

.. tip::

    Puesto que esta ruta permite que todas las acciones se accedan a través de una URL, posiblemente desees protegerla usando el cortafuegos de Symfony2 (permitiendo acceder al rango IP del sustituto inverso).

Una gran ventaja de esta estrategia de memoria caché es que puedes hacer tu aplicación tan dinámica como sea necesario y al mismo tiempo, tocar la aplicación lo menos posible.

.. note::

    Una vez que comiences a usar |ESI|, recuerda usar siempre la directiva ``s-maxage`` en lugar de ``max-age``. Como el navegador nunca recibe recursos agregados, no es consciente del subcomponente, y por lo tanto obedecerá la directiva ``max-age`` y memorizará la página completa. Y no quieres eso.

El ayudante ``render`` es compatible con otras dos útiles opciones:

* ``alt``: utilizada como el atributo ``alt`` en la etiqueta |ESI|, el cual te permite especificar una URL alternativa para utilizarla si no se puede encontrar ``src``;

* ``ignore_errors``: si se establece en ``true``, se agrega un atributo ``onerror`` a la |ESI| con un valor de ``continue`` indicando que, en caso de una falla, la pasarela caché simplemente debe eliminar la etiqueta |ESI| silenciosamente.

.. index::
    single: Caché; Invalidación

.. _http-cache-invalidation:

Invalidando la caché
--------------------

    "Sólo hay dos cosas difíciles en Ciencias de la Computación. Invalidación de caché y nombrar cosas" --Phil Karlton

Nunca debería ser necesario invalidar los datos memorizados en caché porque la invalidación ya se tiene en cuenta de forma nativa en los modelos de caché HTTP. Si utilizas la validación, por definición, no será necesario invalidar ninguna cosa, y si se utiliza la caducidad y necesitas invalidar un recurso, significa que estableciste la fecha de caducidad muy adelante en el futuro.

.. note::

    También es porque no existe un mecanismo de invalidación que cualquier sustituto inverso pueda utilizar, sin cambiar nada en el código de tu aplicación.

En realidad, todos los sustitutos inversos proporcionan una manera de purgar datos almacenados en caché, pero la debes evitar tanto como sea posible. La forma más habitual es purgar la caché de una URL dada solicitándola con el método especial ``PURGE`` de HTTP.

Aquí está cómo puedes configurar la caché sustituta inversa de Symfony2 para apoyar al método ``PURGE`` de HTTP::

    // app/AppCache.php
    class AppCache extends Cache
    {
        protected function invalidate(Request $peticion)
        {
            if ('PURGE' !== $peticion->getMethod()) {
                return parent::invalidate($peticion);
            }

            $respuesta = new Response();
            if (!$this->store->purge($peticion->getUri())) {
                $respuesta->setStatusCode(404, 'Not purged');
            } else {
                $respuesta->setStatusCode(200, 'Purged');
            }

            return $respuesta;
        }
    }

.. caution::

    De alguna manera, debes proteger el método ``PURGE`` HTTP para evitar que alguien aleatoriamente purgue los datos memorizados.

Resumen
-------

Symfony2 fue diseñado para seguir las reglas probadas de la carretera: HTTP. El almacenamiento en caché no es una excepción. Dominar el sistema caché de Symfony2 significa familiarizarse con los modelos de caché HTTP y usarlos eficientemente. Esto significa que, en lugar de confiar sólo en la documentación de Symfony2 y ejemplos de código, tienes acceso a un mundo de conocimientos relacionados con la memorización en caché HTTP y la pasarela caché, tal como Varnish.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/cache/varnish`

.. _`Things Caches Do`: http://tomayko.com/writings/things-caches-do
.. _`Guía de caché`: http://www.mnot.net/cache_docs/
.. _`Varnish`: http://www.varnish-cache.org/
.. _`Squid en modo sustituto inverso`: http://wiki.squid-cache.org/SquidFaq/ReverseProxy
.. _`modelo de caducidad`: http://tools.ietf.org/html/rfc2616#section-13.2
.. _`modelo de validación`: http://tools.ietf.org/html/rfc2616#section-13.3
.. _`RFC 2616`: http://tools.ietf.org/html/rfc2616
.. _`HTTP Bis`: http://tools.ietf.org/wg/httpbis/
.. _`P4 - Petición condicional`: http://tools.ietf.org/html/draft-ietf-httpbis-p4-conditional-12
.. _`P6 - En caché: Exploración y caché intermediaria`: http://tools.ietf.org/html/draft-ietf-httpbis-p6-cache-12
.. _`ESI`: http://www.w3.org/TR/esi-lang
..  |ESI| replace:: :abbr:`ESI (Edge Side Includes o Inclusión del borde lateral)`
