.. index::
   single: Controlador

Controlador
===========

Un controlador es una función PHP que tú creas, misma que toma información de la petición HTTP y construye una respuesta HTTP y la devuelve (como un objeto ``Respuesta`` de Symfony2). La respuesta podría ser una página HTML, un documento XML, una matriz JSON serializada, una imagen, una redirección, un error 404 o cualquier otra cosa que se te ocurra. El controlador contiene toda la lógica arbitraria que *tu aplicación necesita* para reproducir el contenido de la página.

Para ver lo sencillo que es esto, echemos un vistazo a un controlador de Symfony2 en acción.
El siguiente controlador reproducirá una página que simplemente imprime ``¡Hola, mundo!``::

    use Symfony\Component\HttpFoundation\Response;

    public function holaAction()
    {
        return new Response('¡Hola mundo!');
    }

El objetivo de un controlador siempre es el mismo: crear y devolver un objeto ``Respuesta``. Por el camino, este puede leer la información de la petición, cargar un recurso de base de datos, enviar un correo electrónico, o fijar información en la sesión del usuario.
Pero en todos los casos, el controlador eventualmente devuelve el objeto ``Respuesta`` que será entregado al cliente.

¡No hay magia y ningún otro requisito del cual preocuparse! Aquí tienes unos cuantos ejemplos comunes:

* *Controlador A* prepara un objeto ``Respuesta`` que reproduce el contenido de la página principal del sitio.

* *Controlador B* lee el parámetro ``ficha`` de la petición para cargar una entrada del blog de la base de datos y crear un objeto ``Respuesta`` mostrando ese blog. Si la ``ficha`` no se puede encontrar en la base de datos, crea y devuelve un objeto ``Respuesta`` con un código de estado 404.

* *Controlador C* procesa la información presentada en un formulario de contacto. Este lee la información del formulario desde la petición, guarda la información del contacto en la base de datos y envía mensajes de correo electrónico con la información de contacto al administrador del sitio web. Por último, crea un objeto ``Respuesta`` que redirige el navegador del cliente desde el formulario de contacto a la página de *"agradecimiento"*.

.. index::
   single: Controlador; Ciclo de vida de petición-controlador-respuesta

Ciclo de vida de la petición, controlador, respuesta
----------------------------------------------------

Cada petición manejada por un proyecto Symfony2 pasa por el mismo ciclo de vida básico.
La plataforma se encarga de las tareas repetitivas y, finalmente, ejecuta el controlador, que contiene el código personalizado de tu aplicación:

#. Cada petición es manejada por un único archivo controlador frontal (por ejemplo, ``app.php`` o ``app_dev.php``) el caul es responsable de arrancar la aplicación;

#. El ``Enrutador`` lee la información de la petición (por ejemplo, la URI), encuentra una ruta que coincida con esa información, y lee el parámetro ``_controller`` de la ruta;

#. El controlador de la ruta encontrada es ejecutado y el código dentro del controlador crea y devuelve un objeto ``Respuesta``;

#. Las cabeceras HTTP y el contenido del objeto ``Respuesta`` se envían de vuelta al cliente.

La creación de una página es tan fácil como crear un controlador (#3) y hacer una ruta que vincula una URI con ese controlador (#2).

.. note::

    Aunque nombrados de manera similar, un "controlador frontal" es diferente de los "controladores" vamos a hablar acerca de eso en este capítulo. Un controlador frontal es un breve archivo PHP que vive en tu directorio web raíz y a través del cual se dirigen todas las peticiones. Una aplicación típica tendrá un controlador frontal de producción (por ejemplo, ``app.php``) y un controlador frontal de desarrollo (por ejemplo, ``app_dev.php``). Probablemente nunca necesites editar, ver o preocuparte por los controladores frontales en tu aplicación.

.. index::
   single: Controlador; Ejemplo sencillo

Un controlador sencillo
-----------------------

Mientras que un controlador puede ser cualquier cosa PHP ejecutable (una función, un método en un objeto o un ``Cierre``), en Symfony2, un controlador suele ser un único método dentro de un objeto controlador. Los controladores también se conocen como *acciones*.

.. code-block:: php
    :linenos:

    // src/Acme/HolaBundle/Controller/HolaController.php

    namespace Acme\HolaBundle\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HolaController
    {
        public function indexAction($nombre)
        {
          return new Response('<html><body>Hola '.$nombre.'!</body></html>');
        }
    }

.. tip::

    Ten en cuenta que el *controlador* es el método ``indexAction``, que vive dentro de una *clase controlador* (``HolaController``). No te dejes confundir por la nomenclatura: una *clase controlador* simplemente es una conveniente forma de agrupar varios controladores/acciones. Generalmente, la clase controlador albergará varios controladores (por ejemplo, ``updateAction``, ``deleteAction``, etc.).

Este controlador es bastante sencillo, pero vamos a revisarlo línea por línea:

* En la *línea 3*: Symfony2 toma ventaja de la funcionalidad del espacio de nombres de PHP 5.3 para el espacio de nombres de la clase del controlador completa. La palabra clave ``use`` importa la clase ``Respuesta``, misma que nuestro controlador debe devolver.

* En la *línea 6*: el nombre de clase es la concatenación del nombre de la clase controlador (es decir ``Hola``) y la palabra ``Controller``. Esta es una convención que proporciona consistencia a los controladores y permite hacer referencia sólo a la primera parte del nombre (es decir, ``Hola``) en la configuración del enrutador.

* En la *línea 8*: Cada acción en una clase controlador prefijada con ``Action`` y referida en la configuración de enrutado por el nombre de la acción (``index``).
  En la siguiente sección, crearás una ruta que asigna un URI a esta acción.
  Aprenderás cómo los marcadores de posición de la ruta (``{nombre}``) se convierten en argumentos para el método de acción (``$nombre``).

* En la *línea 10*: el controlador crea y devuelve un objeto ``Respuesta``.

.. index::
   single: Controlador; Rutas y controladores

Asignando una URI a un controlador
----------------------------------

El nuevo controlador devuelve una página HTML simple. Para realmente ver esta página en tu navegador, necesitas crear una ruta, la cual corresponda a un patrón de URL específico para el controlador:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hola:
            pattern:      /hola/{nombre}
            defaults:     { _controller: AcmeHolaBundle:Hola:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hola" pattern="/hola/{nombre}">
            <default key="_controller">AcmeHolaBundle:Hola:index</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $coleccion->add('hola', new Route('/hola/{nombre}', array(
            '_controller' => 'AcmeHolaBundle:Hola:index',
        )));

Yendo ahora a ``/hola/ryan`` se ejecuta el controlador ``HolaController``::``indexAction()`` y pasa ``ryan`` a la variable ``$nombre``. Crear una "página" significa simplemente que debes crear un método controlador y una ruta asociada.

Observa la sintaxis utilizada para referirse al controlador: ``AcmeHolaBundle:Hola:index``.
Symfony2 utiliza una flexible notación de cadena para referirse a diferentes controladores.
Esta es la sintaxis más común y le dice Symfony2 que busque una clase controlador llamada ``HolaController`` dentro de un paquete llamado ``AcmeHolaBundle``. Entonces ejecuta el método ``indexAction()``.

Para más detalles sobre el formato de cadena utilizado para referir a diferentes controladores, consulta el :ref:`controller-string-syntax`.

.. note::

    Este ejemplo coloca la configuración de enrutado directamente en el directorio ``app/config/``. Una mejor manera de organizar tus rutas es colocar cada ruta en el paquete al que pertenece. Para más información sobre este tema, consulta :ref:`routing-include-external-resources`.

.. tip::

    Puedes aprender mucho más sobre el sistema de enrutado en el :doc:`capítulo de enrutado </book/routing>`.

.. index::
   single: Controlador; Argumentos del Controlador

.. _route-parameters-controller-arguments:

Parámetros de ruta como argumentos para el controlador
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ya sabes que el parámetro ``_controller`` en ``AcmeHolaBundle:Hola:index`` se refiere al método ``HolaController::indexAction()`` que vive dentro del paquete ``AcmeHolaBundle``. Lo más interesante de esto son los argumentos que se pasan a este método:

.. code-block:: php

    <?php
    // src/Acme/HolaBundle/Controller/HolaController.php

    namespace Acme\HolaBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HolaController extends Controller
    {
        public function indexAction($nombre)
        {
          // ...
        }
    }

El controlador tiene un solo argumento, ``$nombre``, el cual corresponde al parámetro ``{nombre}`` de la ruta coincidente (``ryan`` en nuestro ejemplo). De hecho, cuando ejecutas tu controlador, Symfony2 empareja cada argumento del controlador con un parámetro de la ruta coincidente. Tomemos el siguiente ejemplo:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hola:
            pattern:      /hola/{nombre_de_pila}/{apellido}
            defaults:     { _controller: AcmeHolaBundle:Hola:index, color: verde }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hola" pattern="/hola/{nombre_de_pila}/{apellido}">
            <default key="_controller">AcmeHolaBundle:Hola:index</default>
            <default key="color">verde</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $coleccion->add('hola', new Route('/hola/{nombre_de_pila}/{apellido}', array(
            '_controller' => 'AcmeHolaBundle:Hola:index',
            'color'       => 'verde',
        )));

El controlador para esto puede tomar varios argumentos::

    public function indexAction($nombre_de_pila, $apellido, $color)
    {
        // ...
    }

Ten en cuenta que ambas variables marcadoras de posición (``{nombre_de_pila}``, ``{apellido}``) así como la variable predeterminada ``color`` están disponibles como argumentos en el controlador. Cuando una ruta corresponde, las variables marcadoras de posición se combinan con ``defaults`` para hacer que una matriz esté disponible para tu controlador.

Asignar parámetros de ruta a los argumentos del controlador es fácil y flexible. Ten muy en cuenta las siguientes pautas mientras desarrollas.

* **El orden de los argumentos del controlador no tiene importancia**

    Symfony2 es capaz de igualar los nombres de los parámetros de la ruta con los nombres de las variables en la firma del método controlador. En otras palabras, se da cuenta de que el parámetro ``{apellido}`` coincide con el argumento ``$apellido``.
    Los argumentos del controlador se pueden reorganizar completamente y aún así siguen funcionando perfectamente::

        public function indexAction($apellido, $color, $nombre_de_pila)
        {
            // ..
        }

* **Cada argumento requerido del controlador debe coincidir con un parámetro de enrutado**

    Lo siguiente lanzará una ``RuntimeException`` porque no hay ningún parámetro ``foo`` definido en la ruta::

        public function indexAction($nombre_de_pila, $apellido, $color, $foo)
        {
            // ..
        }

    Sin embargo, hacer que el argumento sea opcional, está perfectamente bien. El siguiente ejemplo no lanzará una excepción::

        public function indexAction($nombre_de_pila, $apellido, $color, $foo = 'bar')
        {
            // ..
        }

* **No todos los parámetros de enrutado deben ser argumentos en tu controlador**

    Si, por ejemplo, ``apellido`` no es tan importante para tu controlador, lo puedes omitir por completo::

        public function indexAction($nombre_de_pila, $color)
        {
            // ..
        }

.. tip::

    Además, todas las rutas tienen un parámetro especial ``_route``, el cual es igual al nombre de la ruta con la que fue emparejado (por ejemplo, ``hola``). Aunque no suele ser útil, igualmente está disponible como un argumento del controlador.

La ``Petición`` como argumento para el controlador
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Para mayor comodidad, también puedes hacer que Symfony pase el objeto ``Petición`` como un argumento a tu controlador. Esto especialmente es conveniente cuando trabajas con formularios, por ejemplo::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $peticion)
    {
        $formulario = $this->createForm(...);

        $formulario->bindRequest($peticion);
        // ...
    }

.. index::
   single: Controlador; Clase base controlador

La clase base del controlador
-----------------------------

Para mayor comodidad, Symfony2 viene con una clase base ``Controller`` que te ayuda en algunas de las tareas más comunes del controlador y proporciona acceso a cualquier recurso que tu clase controlador pueda necesitar. Al extender esta clase ``Controller``, puedes tomar ventaja de varios métodos ayudantes.

Agrega la instrucción ``use`` en lo alto de la clase ``Controller`` y luego modifica ``HolaController`` para extenderla:

.. code-block:: php

    // src/Acme/HolaBundle/Controller/HolaController.php

    namespace Acme\HolaBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HolaController extends Controller
    {
        public function indexAction($nombre)
        {
          return new Response('<html><body>Hola '.$nombre.'!</body></html>');
        }
    }

Esto, en realidad no cambia nada acerca de cómo funciona el controlador. En la siguiente sección, aprenderás acerca de los métodos ayudantes que la clase base del controlador pone a tu disposición. Estos métodos sólo son atajos para utilizar la funcionalidad del núcleo de Symfony2 que está a nuestra disposición, usando o no la clase base ``Controller``. Una buena manera de ver la funcionalidad del núcleo en acción es buscar en la misma clase :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`.

.. tip::

    Extender la clase base es *opcional* en Symfony; esta contiene útiles atajos, pero no es obligatorio. También puedes extender la clase ``Symfony\Component\DependencyInjection\ContainerAware```. El objeto contenedor del servicio será accesible a través de la propiedad ``container``.

.. note::

    Puedes definir tus :doc:`Controladores como Servicios </cookbook/controller/service>`.

.. index::
   single: Controlador; Tareas comunes

Tareas comunes del controlador
------------------------------

A pesar de que un controlador puede hacer prácticamente cualquier cosa, la mayoría de los controladores se encargarán de las mismas tareas básicas una y otra vez. Estas tareas, tal como redirigir, procesar plantillas y acceder a servicios básicos, son muy fáciles de manejar en Symfony2.

.. index::
   single: Controlador; Redirigiendo

Redirigiendo
~~~~~~~~~~~~

Si deseas redirigir al usuario a otra página, utiliza el método ``redirect()``::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('portada'));
    }

El método ``generateUrl()`` es sólo una función auxiliar que genera la URL de una determinada ruta. Para más información, consulta el capítulo :doc:`Enrutando </book/routing>`.

Por omisión, el método ``redirect()`` produce una redirección 302 (temporal). Para realizar una redirección 301 (permanente), modifica el segundo argumento::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('portada'), 301);
    }

.. tip::

    El método ``redirect()`` simplemente es un atajo que crea un objeto ``Respuesta`` que se especializa en redirigir a los usuarios. Es equivalente a:

    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('portada'));

.. index::
   single: Controlador; Redirigiendo

Reenviando
~~~~~~~~~~

También, fácilmente puedes redirigir internamente hacia a otro controlador con el método ``forward()``. En lugar de redirigir el navegador del usuario, este hace una subpetición interna, y pide el controlador especificado. El método ``forward()`` devuelve el objeto ``Respuesta``, el cual es devuelto desde el controlador::

    public function indexAction($nombre)
    {
        $respuesta = $this->forward('AcmeHolaBundle:Hola:maravillosa', array(
            'nombre'  => $nombre,
            'color' => 'verde'
        ));

        // alguna modificación adicional a la respuesta o la devuelve directamente
        
        return $respuesta;
    }

Ten en cuenta que el método ``forward()`` utiliza la misma representación de cadena del controlador utilizada en la configuración de enrutado. En este caso, la clase controlador de destino será ``HolaController`` dentro de algún ``AcmeHolaBundle``.
La matriz pasada al método convierte los argumentos en el controlador resultante.
Esta misma interfaz se utiliza al incrustar controladores en las plantillas (consulta :ref:`templating-embedding-controller`). El método del controlador destino debe tener un aspecto como el siguiente::

    public function maravillosaAction($nombre, $color)
    {
        // ... crea y devuelve un objeto Response
    }

Y al igual que al crear un controlador para una ruta, el orden de los argumentos para ``maravillosaAction`` no tiene la menor importancia. Symfony2 empareja las claves nombre con el índice (por ejemplo, ``nombre``) con el argumento del método (por ejemplo, ``$nombre``). Si cambias el orden de los argumentos, Symfony2 todavía pasará el valor correcto a cada variable.

.. tip::

    Al igual que otros métodos del ``Controller`` base, el método ``forward`` sólo es un atajo para la funcionalidad del núcleo de Symfony2. Puedes redirigir directamente por medio del servicio ``http_kernel``. Un método ``forward`` devuelve un objeto ``Respuesta``::

        $httpKernel = $this->container->get('http_kernel');
        $respuesta = $httpKernel->forward('AcmeHolaBundle:Hola:maravillosa', array(
            'nombre'  => $nombre,
            'color' => 'verde',
        ));

.. index::
   single: Controlador; Procesando plantillas

.. _controlador-procesando-plantillas:

Procesando plantillas
~~~~~~~~~~~~~~~~~~~~~

Aunque no es un requisito, la mayoría de los controladores en última instancia, reproducen una plantilla que es responsable de generar el código HTML (u otro formato) para el controlador.
El método ``renderView()`` procesa una plantilla y devuelve su contenido. Puedes usar el contenido de la plantilla para crear un objeto ``Respuesta``::

    $contenido = $this->renderView('AcmeHolaBundle:Hola:index.html.twig', array('nombre' => $nombre));

    return new Response($contenido);

Incluso puedes hacerlo en un solo paso con el método ``render()``, el cual devuelve un objeto ``Respuesta`` con el contenido de la plantilla::

    return $this->render('AcmeHolaBundle:Hola:index.html.twig', array('nombre' => $nombre));

En ambos casos, se reproducirá la plantilla ``Resources/views/Hola/index.html.twig`` dentro del ``AcmeHolaBundle``.

El motor de plantillas de Symfony se explica con gran detalle en el capítulo :doc:`Plantillas </book/templating>`.

.. tip::

    El método ``renderView`` es un atajo para usar el servicio de ``plantillas``. El servicio de ``plantillas`` también se puede utilizar directamente::

        $plantilla = $this->get('templating');
        $contenido = $plantilla->render('AcmeHolaBundle:Hola:index.html.twig', array('nombre' => $nombre));

.. index::
   single: Controlador; Accediendo a los servicios

Accediendo a otros servicios
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Al extender la clase base del controlador, puedes acceder a cualquier servicio de Symfony2 a través del método ``get()``. Aquí hay varios servicios comunes que puedes necesitar::

    $peticion = $this->getRequest();

    $respuesta = $this->get('response');

    $plantilla = $this->get('templating');

    $enrutador = $this->get('router');

    $cartero = $this->get('mailer');

Hay un sinnúmero de servicios disponibles y te animamos a definir tus propios servicios. Para listar todos los servicios disponibles, utiliza la orden ``container:debug`` de la consola:

.. code-block:: bash

    php app/console container:debug

Para más información, consulta el capítulo :doc:`/book/service_container`.

.. index::
   single: Controlador; Gestionando errores
   single: Controlador; páginas 404

Gestionando errores y páginas 404
---------------------------------

Cuando no se encuentra algo, debes jugar bien con el protocolo HTTP y devolver una respuesta 404. Para ello, debes lanzar un tipo de excepción especial.
Si estás extendiendo la clase base del controlador, haz lo siguiente::

    public function indexAction()
    {
        $producto = // recupera el objeto desde la base de datos
        if (!$producto) {
            throw $this->createNotFoundException('El producto no existe');
        }

        return $this->render(...);
    }

El método ``createNotFoundException()`` crea un objeto ``NotFoundHttpException`` especial, que en última instancia, desencadena una respuesta HTTP 404 en el interior de Symfony.

Por supuesto, estás en libertad de lanzar cualquier clase de ``Excepción`` en tu controlador - Symfony2 automáticamente devolverá una respuesta HTTP con código 500.

.. code-block:: php

    throw new \Exception('¡Algo salió mal!');

En todos los casos, se muestra al usuario final una página de error estilizada y a los desarrolladores se les muestra una página de error de depuración completa (cuando se visualiza la página en modo de depuración).
Puedes personalizar ambas páginas de error. Para más detalles, lee ":doc:`/cookbook/controller/error_pages`" en el recetario.

.. index::
   single: Controlador; La sesión
   single: Session

Gestionando la sesión
---------------------

Symfony2 proporciona un agradable objeto sesión que puedes utilizar para almacenar información sobre el usuario (ya sea una persona real usando un navegador, un robot o un servicio web) entre las peticiones. De manera predeterminada, Symfony2 almacena los atributos de una ``cookie`` usando las sesiones nativas de PHP.

Almacenar y recuperar información de la sesión se puede conseguir fácilmente desde cualquier controlador::

    $sesion = $this->getRequest()->getSession();

    // guarda un atributo para reutilizarlo durante una posterior petición del usuario
    $sesion->set('foo', 'bar');

    // en otro controlador por otra petición
    $foo = $sesion->get('foo');

    // fija la configuración regional del usuario
    $sesion->setLocale('es');

Estos atributos se mantendrán en el usuario por el resto de la sesión de ese usuario.

.. index::
   single Sesión; Mensajes flash

Mensajes flash
~~~~~~~~~~~~~~

También puedes almacenar pequeños mensajes que se pueden guardar en la sesión del usuario para exactamente una petición adicional. Esto es útil cuando procesas un formulario: deseas redirigir y proporcionar un mensaje especial que aparezca en la *siguiente* petición.
Este tipo de mensajes se conocen como mensajes "flash".

Por ejemplo, imagina que estás procesando el envío de un formulario::

    public function updateAction()
    {
        $formulario = $this->createForm(...);

        $formulario->bindRequest($this->getRequest());
        if ($formulario->isValid()) {
            // hace algún tipo de procesamiento

            $this->get('session')->setFlash('aviso', '¡Tus cambios se han guardado!');

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

Después de procesar la petición, el controlador establece un mensaje flash ``aviso`` y luego redirige al usuario. El nombre (``aviso``) no es significativo - es lo que estás usando para identificar el tipo del mensaje.

En la siguiente acción de la plantilla, podrías utilizar el siguiente código para reproducir el mensaje de ``aviso``:

.. configuration-block::

    .. code-block:: html+jinja

        {% if app.session.hasFlash('aviso') %}
            <div class="flash-aviso">
                {{ app.session.flash('aviso') }}
            </div>
        {% endif %}

    .. code-block:: php

        <?php if ($view['session']->hasFlash('aviso')): ?>
            <div class="flash-aviso">
                <?php echo $view['session']->getFlash('aviso') ?>
            </div>
        <?php endif; ?>

Por diseño, los mensajes flash están destinados a vivir por exactamente una petición (estos "desaparecen en un flash"). Están diseñados para utilizarlos a través de redirecciones exactamente como lo hemos hecho en este ejemplo.

.. index::
   single: Controlador; Objeto Respuesta

El objeto ``Response``
----------------------

El único requisito para un controlador es que devuelva un objeto ``Respuesta``. La clase :class:`Symfony\\Component\\HttpFoundation\\Response` es una abstracción PHP en torno a la respuesta HTTP - el mensaje de texto, relleno con cabeceras HTTP y el contenido que se envía de vuelta al cliente::

    // crea una simple respuesta con un código de estado 200 (el predeterminado)
    $respuesta = new Response('Hola '.$nombre, 200);
    
    // crea una respuesta JSON con un código de estado 200
    $respuesta = new Response(json_encode(array('nombre' => $nombre)));
    $respuesta->headers->set('Content-Type', 'application/json');

.. tip::

    La propiedad ``headers`` es un objeto :class:`Symfony\\Component\\HttpFoundation\\HeaderBag` con varios métodos útiles para lectura y mutación de las cabeceras del objeto ``Respuesta``. Los nombres de las cabeceras están normalizados para que puedas usar ``Content-Type`` y este sea equivalente a ``content-type``, e incluso a ``content_type``.

.. index::
   single: Controlador; Objeto Petición

El objeto ``Request``
---------------------

Además de los valores de los marcadores de posición de enrutado, el controlador también tiene acceso al objeto ``Petición`` al extender la clase base ``Controlador``::

    $peticion = $this->getRequest();

    $peticion->isXmlHttpRequest(); // ¿es una petición Ajax?

    $peticion->getPreferredLanguage(array('en', 'es'));

    $peticion->query->get('pag'); // consigue un parámetro $_GET

    $peticion->request->get('pag'); // consigue un parámetro $_POST

Al igual que el objeto ``Respuesta``, las cabeceras de la petición se almacenan en un objeto ``HeaderBag`` y son fácilmente accesibles.

Consideraciones finales
-----------------------

Siempre que creas una página, en última instancia, tendrás que escribir algún código que contenga la lógica para esa página. En Symfony, a esto se le llama *controlador*, y es una función PHP que puede hacer lo que necesitas a fin de devolver el objeto ``Respuesta`` que se entrega al usuario final.

Para facilitarte la vida, puedes optar por extender la clase base ``Controller``, la cual contiene atajos a métodos para muchas tareas de control comunes. Por ejemplo, puesto que no deseas poner el código HTML en tu controlador, puedes usar el método ``render()`` para reproducir y devolver el contenido de una plantilla.

En otros capítulos, veremos cómo puedes usar el controlador para conservar y recuperar objetos desde una base de datos, procesar formularios presentados, manejar el almacenamiento en caché y mucho más.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`
