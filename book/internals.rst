.. index::
   single: Desde dentro

Funcionamiento interno
======================

Parece que quieres entender cómo funciona y cómo extender Symfony2.
¡Eso me hace muy feliz! Esta sección es una explicación en profundidad de Symfony2 desde dentro.

.. note::

    Necesitas leer esta sección sólo si quieres entender cómo funciona Symfony2 detrás de la escena, o si deseas ampliar Symfony2.

Resumen
-------

El código Symfony2 está hecho de varias capas independientes. Cada capa está construida en lo alto de la anterior.

.. tip::

    La carga automática no es administrada directamente por la plataforma; sino que se hace independientemente con la ayuda de la clase :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` y el archivo ``src/autoload.php``. Lee el :doc:`capítulo dedicado </cookbook/tools/autoloader>` para más información.

Componente ``HttpFoundation``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Al nivel más profundo está el componente :namespace:`Symfony\\Component\\HttpFoundation`. HttpFoundation proporciona los principales objetos necesarios para hacer frente a HTTP.
Es una abstracción orientada a objetos de algunas funciones y variables nativas de PHP:

* La clase :class:`Symfony\\Component\\HttpFoundation\\Request` resume las principales variables globales de PHP como ``$_GET``, ``$_POST``, ``$_COOKIE``, ``$_FILES`` y ``$_SERVER``;

* La clase :class:`Symfony\\Component\\HttpFoundation\\Response` resume algunas funciones PHP como ``header()``, ``setcookie()`` y ``echo``;

* La clase :class:`Symfony\\Component\\HttpFoundation\\Session` y la interfaz :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\SessionStorageInterface`
resumen la gestión de sesión y las funciones ``session_*()``.

Componente ``HttpKernel``
~~~~~~~~~~~~~~~~~~~~~~~~~

En lo alto de HttpFoundation está el componente :namespace:`Symfony\\Component\\HttpKernel`. HttpKernel se encarga de la parte dinámica de HTTP, es una fina capa en la parte superior de las clases Petición y Respuesta para estandarizar la forma en que se manejan las peticiones. Esta también proporciona puntos de extensión y herramientas que lo convierten en el punto de partida ideal para crear una plataforma Web sin demasiado trabajo.

Además, opcionalmente añade configurabilidad y extensibilidad, gracias al componente de inyección de dependencias y un potente sistema de complementos (paquetes).

.. seealso::

    Lee más sobre el componente :​​doc:`HttpKernel <kernel>`. Lee más sobre la
    :doc:`Inyección de dependencias </book/service_container>` y los :doc:`Paquetes
    </cookbook/bundles/best_practices>`.

Paquete ``FrameworkBundle``
~~~~~~~~~~~~~~~~~~~~~~~~~~~

El paquete :namespace:`Symfony\\Bundle\\FrameworkBundle` es el paquete que une los principales componentes y bibliotecas para hacer una plataforma MVC ligera y rápida. Este viene con una configuración predeterminada sensible y convenios para facilitar la curva de aprendizaje.

.. index::
   single: Funcionamiento interno; Kernel

Kernel
------

La clase :class:`Symfony\\Component\\HttpKernel\\HttpKernel` es la clase central de Symfony2 y es responsable de tramitar las peticiones del cliente. Su objetivo principal es "convertir" un objeto :class:`Symfony\\Component\\HttpFoundation\\Request` a un objeto :class:`Symfony\\Component\\HttpFoundation\\Response`.

Cada núcleo de Symfony2 implementa :class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface`::

    function handle(Request $peticion, $type = self::MASTER_REQUEST, $catch = true)

.. index::
   single: Funcionamiento interno; Resolución de controlador

Controladores
~~~~~~~~~~~~~

Para convertir una Petición a una Respuesta, el Kernel cuenta con un "Controlador". Un controlador puede ser cualquier PHP ejecutable válido.

El Kernel delega la selección de cual controlador se debe ejecutar a una implementación de :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`::

    public function getController(Request $peticion);

    public function getArguments(Request $peticion, $controller);

El método :method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController` devuelve el controlador (un PHP ejecutable) asociado a la petición dada. La implementación predeterminada de (:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`) busca un atributo ``_controller`` en la petición que representa el nombre del controlador (una cadena "clase::método", cómo ``Bundle\BlogBundle\PostController:indexAction``).

.. tip::

    La implementación predeterminada utiliza la clase :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener` para definir el atributo ``_controller`` de la petición (consulta :ref:`kernel-core_request`).

El método :method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments` devuelve una matriz de argumentos para pasarla al Controlador ejecutable. La implementación predeterminada automáticamente resuelve los argumentos del método, basándose en los atributos de la Petición.

.. sidebar:: Coincidiendo los argumentos del método Controlador desde los atributos de la Petición

    Por cada argumento del método, Symfony2 trata de obtener el valor de un atributo de la petición con el mismo nombre. Si no está definido, el valor predeterminado es el argumento utilizado de estar definido:

        // Symfony2 debe buscar un atributo
        // 'id' (obligatorio) y un
        // 'admin' (opcional)
        public function showAction($id, $admin = true)
        {
            // ...
        }

.. index::
  single: Funcionamiento interno; Manejando peticiones

Manejando peticiones
~~~~~~~~~~~~~~~~~~~~

El método ``handle()`` toma una ``Petición`` y *siempre* devuelve una ``Respuesta``.
Para convertir la ``Petición``, ``handle()`` confía en el mecanismo de resolución y una cadena ordenada de notificaciones de evento (consulta la siguiente sección para más información acerca de cada evento):

1. Antes de hacer alguna otra cosa, el evento ``kernel.request`` es notificado -- si uno de los escuchas devuelve una ``Respuesta``, este salta directamente al paso 8;

2. El mecanismo de resolución es llamado para determinar el controlador a ejecutar;

3. Los escuchas del evento ``kernel.controller`` ahora pueden manipular el controlador ejecutable como quieras (cambiarlo, envolverlo, ...);

4. El núcleo verifica que el controlador en realidad es un PHP ejecutable válido;

5. Se llama al mecanismo de resolución para determinar los argumentos a pasar al controlador;

6. El Kernel llama al controlador;

7. Si el controlador no devuelve una ``Respuesta``, los escuchas del evento ``kernel.view`` pueden convertir en ``Respuesta`` el valor devuelto por el Controlador;

8. Los escuchas del evento ``kernel.response`` pueden manipular la ``Respuesta`` (contenido y cabeceras);

9. Devuelve la respuesta.

Si se produce una Excepción durante el proceso, se notifica la ``kernel.exception`` y se dará una oportunidad a los escuchas de convertir la excepción en Respuesta. Si esto funciona, se notifica el evento ``kernel.response``, si no, se vuelve a lanzar la excepción.

Si no deseas que se capturen las Excepciones (para peticiones incrustadas, por ejemplo), desactiva el evento ``kernel.exception`` pasando ``false`` como tercer argumento del método ``handle()``.

.. index::
  single: Funcionamiento interno; Funcionamiento interno de las peticiones

Funcionamiento interno de las peticiones
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En cualquier momento durante el manejo de una petición (la 'maestra' uno), puede manejar una subpetición. Puedes pasar el tipo de petición al método ``handle()`` (su segundo argumento):

* ``HttpKernelInterface::MASTER_REQUEST``;
* ``HttpKernelInterface::SUB_REQUEST``.

El tipo se pasa a todos los eventos y los escuchas pueden actuar en consecuencia (algún procesamiento sólo debe ocurrir en la petición maestra).

.. index::
   pair: Kernel; Evento

Eventos
~~~~~~~

Cada evento lanzado por el Kernel es una subclase de :class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`. Esto significa que cada evento tiene acceso a la misma información básica:

* ``getRequestType()`` - devuelve el *tipo* de la petición   (``HttpKernelInterface::MASTER_REQUEST`` o ``HttpKernelInterface::SUB_REQUEST``);

* ``getKernel()`` - devuelve el Kernel que está manejando la petición;

* ``getRequest()`` - devuelve la ``Petición`` que se está manejando actualmente.

``getRequestType()``
....................

El método ``getRequestType()`` permite a los escuchas conocer el tipo de la petición. Por ejemplo, si un escucha sólo debe estar atento a las peticiones maestras, agrega el siguiente código al principio de tu método escucha::

    use Symfony\Component\HttpKernel\HttpKernelInterface;

    if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
        // regresa inmediatamente
        return;
    }

.. tip::

    Si todavía no estás familiarizado con el Despachador de Eventos de Symfony2, primero lee la sección
    :ref:`event_dispatcher`.

.. index::
   single: Evento; kernel.request

.. _kernel-core-request:

Evento ``kernel.request``
.........................

*Clase del evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`

El objetivo de este evento es devolver inmediatamente un objeto ``Respuesta`` o variables de configuración para poder invocar un controlador después del evento. Cualquier escucha puede devolver un objeto ``Respuesta`` a través del método ``setResponse()`` en el evento. En este caso, todos los otros escuchas no serán llamados.

Este evento lo utiliza el ``FrameworkBundle`` para llenar el atributo ``_controller`` de la ``Petición``, a través de :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`. RequestListener usa un objeto :class:`Symfony\\Component\\Routing\\RouterInterface` para coincidir la ``Petición`` y determinar el nombre del controlador (guardado en el atributo ``_controller`` de la  ``Petición``).

.. index::
   single: Evento; kernel.controller

Evento ``kernel.controller``
............................

*Clase del evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

Este evento no lo utiliza ``FrameworkBundle``, pero puede ser un punto de entrada para modificar el controlador que se debe ejecutar:

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // el controlador se puede cambiar a cualquier PHP ejecutable
        $event->setController($controller);
    }

.. index::
   single: Evento; kernel.view

Evento ``kernel.view``
......................

*Clase del evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`

Este evento no lo utiliza el ``FrameworkBundle``, pero se puede usar para implementar un subsistema de vistas. Este evento se llama *sólo* si el controlador *no* devuelve un objeto ``Respuesta``. El propósito del evento es permitir que algún otro valor de retorno se convierta en una ``Respuesta``.

El valor devuelto por el controlador es accesible a través del método ``getControllerResult``::

    use Symfony\Component\HttpKernel\Event\GetResponseForControllerResultEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelView(GetResponseForControllerResultEvent $event)
    {
        $val = $event->getReturnValue();
        $respuesta = new Response();
        // modifica de alguna manera el valor de retorno de la respuesta

        $event->setResponse($respuesta);
    }

.. index::
   single: Evento; kernel.response

Evento ``kernel.response``
..........................

*Clase del evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

El propósito de este evento es permitir que otros sistemas modifiquen o sustituyan el objeto ``Respuesta`` después de su creación:

.. code-block:: php

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $respuesta = $event->getResponse();
        // .. modifica el objeto Respuesta
    }

El ``FrameworkBundle`` registra varios escuchas:

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`: recoge los datos de la petición actual;

* :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`: inyecta la barra de herramientas de depuración web;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`: fija el ``Content-Type`` de la respuesta basándose en el formato de la petición;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`:  agrega una cabecera HTTP ``Surrogate-Control`` cuando la respuesta necesita ser analizada por etiquetas |ESI|.

.. index::
   single: Evento; kernel.exception

.. _kernel-kernel.exception:

Evento ``kernel.exception``
...........................

*Clase del evento*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`

``FrameworkBundle`` registra un :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener` el cual remite la ``Petición`` a un determinado controlador (el valor del parámetro ``exception_listener.controller`` -- debe estar en notación ``clase::método``).

Un escucha en este evento puede crear y establecer un objeto ``Respuesta``, crear y establecer un nuevo objeto ``Excepción``, o simplemente no hacer nada:

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelException(GetResponseForExceptionEvent $event)
    {
        $exception = $event->getException();
        $respuesta = new Response();
        // configura el objeto Respuesta basándose en la excepción capturada
        $event->setResponse($respuesta);

        // alternativamente puedes establecer una nueva excepción
        // $exception = new \Exception('Alguna excepción especial');
        // $event->setException($exception);
    }

.. index::
   single: Despachador de eventos

El despachador de eventos
-------------------------

El paradigma orientado a objetos ha recorrido un largo camino para garantizar la extensibilidad del código. Al crear clases que tienen responsabilidades bien definidas, el código se vuelve más flexible y un desarrollador lo puede extender con subclases para modificar su comportamiento. Pero si quieres compartir tus cambios con otros desarrolladores que también han hecho sus propias subclases, la herencia de código es discutible.

Consideremos un ejemplo del mundo real en el que deseas proporcionar un sistema de complementos a tu proyecto. Un complemento debe ser capaz de agregar métodos, o hacer algo antes o después de ejecutar un método, sin interferir con otros complementos. Esto no es un problema fácil de resolver con la herencia simple y herencia múltiple (si fuera posible con PHP) tiene sus propios inconvenientes.

El despachador de eventos de Symfony2 implementa el patrón `observador`_ en una manera sencilla y efectiva para hacer todo esto posible y para hacer realmente extensibles tus proyectos.

Tomemos un ejemplo simple desde el `componente HttpKernel de Symfony2`_. Una vez creado un objeto ``Respuesta``, puede ser útil permitir que otros elementos en el sistema lo modifiquen (por ejemplo, añadan algunas cabeceras caché) antes de utilizarlo realmente. Para hacer esto posible, el núcleo de Symfony2 lanza un evento - ``kernel.response``. He aquí cómo funciona:

* Un *escucha* (objeto PHP) le dice a un objeto *despachador* central que quiere escuchar el evento ``kernel.response``;

* En algún momento, el núcleo de Symfony2 dice al objeto *despachador* que difunda el evento ``kernel.response``, pasando con este un objeto ``Evento`` que tiene acceso al objeto ``Respuesta``;

* El despachador notifica a (es decir, llama a un método en) todos los escuchas del evento ``kernel.response``, permitiendo que cada uno de ellos haga alguna modificación al objeto ``Respuesta``.

.. index::
   single: Despachador de eventos; Eventos

.. _event_dispatcher:

Eventos
~~~~~~~

Cuando se envía un evento, es identificado por un nombre único (por ejemplo, ``kernel.response``), al que cualquier cantidad de escuchas podría estar atento. También se crea una instancia de :class:`Symfony\\Component\\EventDispatcher\\Event` y se pasa a todos los escuchas. Como veremos más adelante, el objeto ``Evento`` mismo, a menudo contiene datos sobre cuando se despachó el evento.

.. index::
   pair: Despachador de eventos; Convenciones de nomenclatura

Convenciones de nomenclatura
............................

El nombre único del evento puede ser cualquier cadena, pero opcionalmente sigue una serie de convenciones de nomenclatura simples:

* Usa sólo letras minúsculas, números, puntos (``.``) y subrayados (``_``);

* Prefija los nombres con un espacio de nombres seguido de un punto (por ejemplo, ``kernel.``);

* Termina los nombres con un verbo que indica qué acción se está tomando (por ejemplo, ``petición``).

Estos son algunos ejemplos de nombres de evento aceptables:

* ``kernel.response``
* ``form.pre_set_data``

.. index::
   single: Despachador de eventos; Subclases de eventos

Nombres de evento y objetos evento
..................................

Cuando el despachador notifica a los escuchas, este pasa un objeto ``Evento`` real a los escuchas. La clase base ``Evento`` es muy simple: contiene un método para detener la :ref:`propagación de eventos <event_dispatcher-event-propagation>`, pero no mucho más.

Muchas veces, los datos acerca de un evento específico se tienen que pasar junto con el objeto ``Evento`` para que los escuchas tengan la información necesaria. En el caso del evento ``kernel.response``, el objeto ``Evento`` creado y pasado a cada escucha realmente es de tipo :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`, una subclase del objeto ``Evento`` base. Esta clase contiene métodos como ``getResponse`` y ``setResponse``, que permiten a los escuchas recibir e incluso sustituir el objeto ``Respuesta``.

La moraleja de la historia es esta: cuando creas un escucha para un evento, el objeto ``Evento`` que se pasa al escucha puede ser una subclase especial que tiene métodos adicionales para recuperar información desde y para responder a evento.

El despachador
~~~~~~~~~~~~~~

El despachador es el objeto central del sistema despachador de eventos. En general, se crea un único despachador, el cual mantiene un registro de escuchas. Cuando se difunde un evento a través del despachador, este notifica a todos los escuchas registrados con ese evento.

.. code-block:: php

    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();

.. index::
   single: Despachador de eventos; Escuchas

Conectando escuchas
~~~~~~~~~~~~~~~~~~~

Para aprovechar las ventajas de un evento existente, es necesario conectar un escucha con el despachador para que pueda ser notificado cuando se despache el evento. Una llamada al método despachador ``addListener()`` asocia cualquier objeto PHP ejecutable a un evento:

.. code-block:: php

    $listener = new AcmeListener();
    $dispatcher->addListener('foo.action', array($listener, 'onFooAction'));

El método ``addListener()`` toma hasta tres argumentos:

* El nombre del evento (cadena) que este escucha quiere atender;

* Un objeto PHP ejecutable que será notificado cuando se produzca un evento al que está atento;

* Un entero de prioridad opcional (mayor es igual a más importante) que determina cuando un escucha se activa frente a otros escuchas (por omisión es ``0``). Si dos escuchas tienen la misma prioridad, se ejecutan en el orden en que se agregaron al despachador.

.. note::

    Un `PHP ejecutable`_ es una variable PHP que la función ``call_user_func()`` puede utilizar y devuelve ``true`` cuando pasa a la función ``is_callable()``. Esta puede ser una instancia de ``\Closure``, una cadena que representa una función, o una matriz que representa a un objeto método o un método de clase.

    Hasta ahora, hemos visto cómo los objetos PHP se pueden registrar como escuchas. También puedes registrar `Cierres`_ PHP como escuchas de eventos:

    .. code-block:: php

        use Symfony\Component\EventDispatcher\Event;

        $dispatcher->addListener('foo.action', function (Event $event) {
            // se debe ejecutar cuando se despache el evento foo.action
        });

Una vez que se registra el escucha en el despachador, este espera hasta que el evento sea notificado. En el ejemplo anterior, cuando se despacha el evento ``foo.action``, el despachador llama al método ``AcmeListener::onFooAction`` y pasa el objeto ``Evento`` como único argumento:

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class AcmeListener
    {
        // ...

        public function onFooAction(Event $event)
        {
            // hace alguna cosa
        }
    }

.. tip::

    Si utilizas la plataforma MVC de Symfony2, los escuchas se pueden registrar a través de tu :ref:`configuración <dic-tags-kernel-event-listener>`. Como bono adicional, los objetos escucha sólo se crean cuando son necesarios.

En muchos casos, una subclase especial ``Evento`` específica para el evento dado es pasada al escucha. Esto le da al escucha acceso a información especial sobre el evento. Consulta la documentación o la implementación de cada evento para determinar la instancia exacta de ``Symfony\Component\EventDispatcher\Event`` que se ha pasado. Por ejemplo, el evento ``kernel.event`` pasa una instancia de ``Symfony\Component\HttpKernel\Event\FilterResponseEvent``:

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterResponseEvent

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $respuesta = $event->getResponse();
        $peticion = $event->getRequest();

        // ...
    }

.. _event_dispatcher-closures-as-listeners:

.. index::
   single: Despachador de eventos; Creando y despachando un evento

Creando y despachando un evento
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Además de registrar escuchas con eventos existentes, puedes crear y lanzar tus propios eventos. Esto es útil cuando creas bibliotecas de terceros y también cuando deseas mantener flexibles y desconectados diferentes componentes de tu propio sistema.

La clase estática ``Events``
............................

Supongamos que deseas crear un nuevo evento - ``store.order`` - el cual se despacha cada vez que es creada una orden dentro de tu aplicación. Para mantener las cosas organizadas, empieza por crear una clase ``StoreEvents`` dentro de tu aplicación que sirva para definir y documentar tu evento:

.. code-block:: php

    namespace Acme\TiendaBundle;

    final class StoreEvents
    {
        /**
         * El evento store.order es lanzado cada vez que se crea una orden
         * en el sistema.
         * 
         * El escucha del evento recibe una instancia de
         * Acme\TiendaBundle\Event\FilterOrderEvent.
         *
         * @var string
         */
        const onStoreOrder = 'store.order';
    }

Ten en cuenta que esta clase en realidad no *hace* nada. El propósito de la clase ``StoreEvents`` sólo es ser un lugar donde se pueda centralizar la información sobre los eventos comunes. Observa también que se pasará una clase especial ``FilterOrderEvent`` a cada escucha de este evento.

Creando un objeto Evento
........................

Más tarde, cuando despaches este nuevo evento, debes crear una instancia del ``Evento`` y pasarla al despachador. Entonces el despachador pasa esta misma instancia a cada uno de los escuchas del evento. Si no necesitas pasar alguna información a tus escuchas, puedes utilizar la clase predeterminada ``Symfony\Component\EventDispatcher\Event``. La mayoría de las veces, sin embargo, *necesitarás* pasar información sobre el evento a cada escucha. Para lograr esto, vamos a crear una nueva clase que extiende a ``Symfony\Component\EventDispatcher\Event``.

En este ejemplo, cada escucha tendrá acceso a algún objeto ``Orden``. Crea una clase ``Evento`` que lo hace posible:

.. code-block:: php

    namespace Acme\TiendaBundle\Event;

    use Symfony\Component\EventDispatcher\Event;
    use Acme\TiendaBundle\Order;

    class FilterOrderEvent extends Event
    {
        protected $order;

        public function __construct(Order $order)
        {
            $this->order = $order;
        }

        public function getOrder()
        {
            return $this->order;
        }
    }

Ahora cada escucha tiene acceso al objeto ``Orden`` a través del método ``getOrder``.

Despachando el evento
.....................

El método :method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::dispatch` notifica a todos los escuchas del evento dado. Este toma dos argumentos: el nombre del evento a despachar, y la instancia del ``Evento`` a pasar a cada escucha de ese evento:

.. code-block:: php

    use Acme\TiendaBundle\StoreEvents;
    use Acme\TiendaBundle\Order;
    use Acme\TiendaBundle\Event\FilterOrderEvent;

    // la orden de alguna manera es creada o recuperada
    $order = new Order();
    // ...

    // crea el FilterOrderEvent y lo despacha
    $event = new FilterOrderEvent($order);
    $dispatcher->dispatch(StoreEvents::onStoreOrder, $event);

Ten en cuenta que el objeto especial ``FilterOrderEvent`` se crea y pasa al método ``dispatch``. Ahora, cualquier escucha del evento ``store.order`` recibirá el ``FilterOrderEvent`` y tendrá acceso al objeto ``Orden`` a través del método ``getOrder``:

.. code-block:: php

    // alguna clase escucha que se ha registrado para onStoreOrder
    use Acme\TiendaBundle\Event\FilterOrderEvent;

    public function onStoreOrder(FilterOrderEvent $event)
    {
        $order = $event->getOrder();
        // hace algo a/con la orden
    }

Pasando el objeto despachador de evento
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si echas un vistazo a la clase ``EventDispatcher``, te darás cuenta de que la clase no actúa como un Singleton (no hay un método estático ``getInstance()``).
Esto es intencional, ya que posiblemente desees tener varios despachadores de eventos simultáneos en una sola petición PHP. Pero también significa que necesitas una manera de pasar el despachador a los objetos que necesitan conectar o notificar eventos.

La mejor práctica consiste en inyectar el objeto despachador de eventos en tus objetos, también conocido como inyección de dependencias.

Puedes usar el constructor de inyección::

    class Foo
    {
        protected $dispatcher = null;

        public function __construct(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

O definir la inyección::

    class Foo
    {
        protected $dispatcher = null;

        public function setEventDispatcher(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

La elección entre los dos realmente es cuestión de gusto. Muchos tienden a preferir el constructor de inyección porque los objetos son totalmente iniciados en tiempo de construcción. Pero cuando tienes una larga lista de dependencias, la inyección de definidores puede ser el camino a seguir, especialmente para dependencias opcionales.

.. tip::

    Si utilizas la inyección de dependencias como lo hicimos en los dos ejemplos anteriores, puedes utilizar el `componente Inyección de dependencias de Symfony2`_ para manejar elegantemente estos objetos.

.. index::
   single: Despachador de eventos; Suscriptores de evento

Usando suscriptores de evento
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La forma más común para escuchar a un evento es registrar un *escucha de evento* con el despachador. Este escucha puede estar atento a uno o más eventos y ser notificado cada vez que se envían los eventos.

Otra forma de escuchar eventos es a través de un *suscriptor de eventos*. Un suscriptor de eventos es una clase PHP que es capaz de decir al despachador exactamente a cuales eventos debe estar suscrito. Este implementa la interfaz :class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`, que requiere un solo método estático llamado ``getSubscribedEvents``. Considera el siguiente ejemplo de un suscriptor que está inscrito a los eventos ``kernel.response`` y ``store.order``:

.. code-block:: php

    namespace Acme\TiendaBundle\Event;

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\HttpKernel\Event\FilterResponseEvent;

    class StoreSubscriber implements EventSubscriberInterface
    {
        static public function getSubscribedEvents()
        {
            return array(
                'kernel.response' => 'onKernelResponse',
                'store.order'     => 'onStoreOrder',
            );
        }

        public function onKernelResponse(FilterResponseEvent $event)
        {
            // ...
        }

        public function onStoreOrder(FilterOrderEvent $event)
        {
            // ...
        }
    }

Esto es muy similar a una clase escucha, salvo que la propia clase puede decir al despachador cuales eventos debe escuchar. Para registrar un suscriptor al despachador, utiliza el método
 :method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::addSubscriber`:

.. code-block:: php

    use Acme\TiendaBundle\Event\StoreSubscriber;

    $subscriber = new StoreSubscriber();
    $dispatcher->addSubscriber($subscriber);

El despachador registrará automáticamente al suscriptor para cada evento devuelto por el método ``getSubscribedEvents``. Al igual que con los escuchas, el método ``addSubscriber`` tiene un segundo argumento opcional, que es la prioridad que se debe dar a cada evento.

.. index::
   single: Despachador de eventos; Deteniendo el flujo del evento

.. _event_dispatcher-event-propagation:

Deteniendo el flujo/propagación del evento
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En algunos casos, puede tener sentido que un escucha evite que se llame a otros escuchas. En otras palabras, el escucha tiene que poder decirle al despachador detenga la propagación de todos los eventos a los escuchas en el futuro (es decir, no notificar a más escuchas). Esto se puede lograr desde el interior de un escucha a través del método :method:`Symfony\\Component\\EventDispatcher\\Event::stopPropagation`:

.. code-block:: php

   use Acme\TiendaBundle\Event\FilterOrderEvent;

   public function onStoreOrder(FilterOrderEvent $event)
   {
       // ...

       $event->stopPropagation();
   }

Ahora, cualquier escucha de ``store.order`` que aún no ha sido llamado *no* será invocado.

.. index::
   single: Generador de perfiles

Generador de perfiles
---------------------

Cuando se activa, el generador de perfiles de Symfony2 recoge información útil sobre cada petición presentada a tu aplicación y la almacena para su posterior análisis. Utiliza el generador de perfiles en el entorno de desarrollo para ayudar a depurar el código y mejorar el rendimiento, úsalo en el entorno de producción para explorar problemas después del hecho.

Rara vez tienes que lidiar con el generador de perfiles directamente puesto que Symfony2 proporciona herramientas de visualización como la barra de herramientas de depuración web y el generador de perfiles web. Si utilizas la Edición estándar de Symfony2, el generador de perfiles, la barra de herramientas de depuración web, y el generador de perfiles web, ya están configurados con ajustes razonables.

.. note::

    El generador de perfiles recopila información para todas las peticiones (peticiones simples, redirecciones, excepciones, peticiones Ajax, peticiones |ESI|, y para todos los métodos HTTP y todos los formatos). Esto significa que para una única URL, puedes tener varios perfiles de datos asociados (un par petición/respuesta externa).

.. index::
   single: Generador de perfiles; Visualizando

Visualizando perfiles de datos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Usando la barra de depuración web
.................................

En el entorno de desarrollo, la barra de depuración web está disponible en la parte inferior de todas las páginas. Esta muestra un buen resumen de los datos de perfiles que te da acceso instantáneo a una gran cantidad de información útil cuando algo no funciona como se esperaba.

Si el resumen presentado por las herramientas de la barra de depuración web no es suficiente, haz clic en el enlace simbólico (una cadena compuesta de 13 caracteres aleatorios) para acceder al generador de perfiles web.

.. note::

    Si no se puede hacer clic en el enlace, significa que las rutas del generador de perfiles no están registradas (más abajo hay información de configuración).

Analizando datos del perfil con el generador de perfiles web
............................................................

El generador de perfiles web es una herramienta de visualización para el perfilado de datos que puedes utilizar en desarrollo para depurar tu código y mejorar el rendimiento, pero también lo puedes utilizar para explorar problemas que se producen en producción. Este expone toda la información recogida por el generador de perfiles en un interfaz web.

.. index::
   single: Generador de perfiles; Usando el servicio generador de perfiles

Accediendo a información del generador de perfiles
..................................................

No es necesario utilizar el visualizador predeterminado para acceder a la información de perfiles. Pero ¿cómo se puede recuperar información de perfiles de una petición específica después del hecho? Cuando el generador de perfiles almacena datos sobre una petición, también asocia un símbolo con ella, esta muestra está disponible en la cabecera HTTP ``X-Debug-Token`` de la Respuesta::

    $perfil = $contenedor->get('profiler')->loadProfileFromResponse($respuesta);

    $perfil = $contenedor->get('profiler')->loadProfile($token);

.. tip::

    Cuando el generador de perfiles está habilitado pero no la barra de herramientas de depuración web, o cuando desees obtener el símbolo de una petición Ajax, utiliza una herramienta como Firebug para obtener el valor de la cabecera HTTP ``X-Debug-Token``.

Usa el método ``find()`` para acceder a elementos basándose en algún criterio::

    // consigue los 10 últimas fragmentos
    $tokens = $contenedor->get('profiler')->find('', '', 10);

    // consigue los 10 últimos fragmentos de todas las URL que contienen /admin/
    $tokens = $contenedor->get('profiler')->find('', '/admin/', 10);

    // consigue los 10 últimos fragmentos de peticiones locales
    $tokens = $contenedor->get('profiler')->find('127.0.0.1', '', 10);

Si deseas manipular los datos del perfil en una máquina diferente a la que generó la información, utiliza los métodos ``export()`` e ``import()``::

    // en la máquina en producción
    $perfil = $contenedor->get('profiler')->loadProfile($token);
    $data = $profiler->export($perfil);

    // en la máquina de desarrollo
    $profiler->import($data);

.. index::
   single: Generador de perfiles; Visualizando

Configurando
............

La configuración predeterminada de Symfony2 viene con sensibles ajustes para el generador de perfiles, la barra de herramientas de depuración web, y el generador de perfiles web. Aquí está por ejemplo la configuración para el entorno de desarrollo:

.. configuration-block::

    .. code-block:: yaml

        # carga el generador de perfiles
        framework:
            profiler: { only_exceptions: false }

        # activa el generador de perfiles web
        web_profiler:
            toolbar: true
            intercept_redirects: true
            verbose: true

    .. code-block:: xml

        <!-- xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"> -->

        <!-- carga el generador de perfiles -->
        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- activa el generador de perfiles web -->
        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
            verbose="true"
        />

    .. code-block:: php

        // carga el generador de perfiles
        $contenedor->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

        // activa el generador de perfiles web
        $contenedor->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            'intercept-redirects' => true,
            'verbose' => true,
        ));

Cuando ``only-exceptions`` se establece a ``true``, el generador de perfiles sólo recoge dados cuando tu aplicación lanza una excepción.

Cuando ``intercept-redirects`` está establecido en ``true``, el generador de perfiles web intercepta las redirecciones y te da la oportunidad de analizar los datos recogidos antes de seguir la redirección.

Cuando ``verbose`` está establecido en ``true``, la barra de herramientas de depuración web muestra una gran cantidad de información. Configurar ``verbose`` a ``false`` oculta algo de información secundaria para hacer más corta la barra de herramientas.

Si activas el generador de perfiles web, también es necesario montar las rutas de los perfiles:

.. configuration-block::

    .. code-block:: yaml

        _profiler:
            resource: @WebProfilerBundle/Resources/config/routing/profiler.xml
            prefix:   /_profiler

    .. code-block:: xml

        <import resource="@WebProfilerBundle/Resources/config/routing/profiler.xml" prefix="/_profiler" />

    .. code-block:: php

        $coleccion->addCollection($loader->import("@WebProfilerBundle/Resources/config/routing/profiler.xml"), '/_profiler');

Dado que el generador de perfiles añade algo de sobrecarga, posiblemente desees activarlo sólo bajo ciertas circunstancias en el entorno de producción. La configuración ``only-exceptions`` limita al generador de perfiles a 500 páginas, ¿pero si quieres obtener información cuando el cliente IP proviene de una dirección específica, o para una parte limitada de la página web? Puedes utilizar una petición emparejadora:

.. configuration-block::

    .. code-block:: yaml

        # activa el generador de perfiles sólo para peticiones entrantes de la red 192.168.0.0
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24 }

        # activa el generado de perfiles sólo para las URL /admin
        framework:
            profiler:
                matcher: { path: "^/admin/" }

        # combina reglas
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24, path: "^/admin/" }

        # usa una instancia emparejadora personalizada definida en el servicio "custom_matcher"
        framework:
            profiler:
                matcher: { service: custom_matcher }

    .. code-block:: xml

        <!-- activa el generador de perfiles sólo para peticiones entrantes de la red 192.168.0.0 -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" />
            </framework:profiler>
        </framework:config>

        <!-- activa el generador de perfiles sólo para las URL /admin -->
        <framework:config>
            <framework:profiler>
                <framework:matcher path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- combina reglas -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- usa una instancia emparejadora personalizada definida en el servicio "custom_matcher" -->
        <framework:config>
            <framework:profiler>
                <framework:matcher service="custom_matcher" />
            </framework:profiler>
        </framework:config>

    .. code-block:: php

        // activa el generador de perfiles sólo para peticiones entrantes de la red 192.168.0.0
        $contenedor->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24'),
            ),
        ));

        // activa el generador de perfiles sólo para las URL /admin
        $contenedor->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('path' => '^/admin/'),
            ),
        ));

        // combina reglas
        $contenedor->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24', 'path' => '^/admin/'),
            ),
        ));

        # usa una instancia emparejadora personalizada definida en el servicio "custom_matcher"
        $contenedor->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('service' => 'custom_matcher'),
            ),
        ));

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/profiler/data_collector`
* :doc:`/cookbook/event_dispatcher/class_extension`
* :doc:`/cookbook/event_dispatcher/method_behavior`

.. _observador: http://es.wikipedia.org/wiki/Observer_(patr%C3%B3n_de_dise%C3%B1o)
.. _`componente HttpKernel de Symfony2`: https://github.com/symfony/HttpKernel
.. _Cierres: http://php.net/manual/en/functions.anonymous.php
.. _`componente Inyección de dependencias de Symfony2`: https://github.com/symfony/DependencyInjection _PHP ejecutable: http://www.php.net/manual/en/language.pseudo-types.php#language.types.callback