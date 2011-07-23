Etiquetas de inyección de dependencias
======================================

Etiquetas;

* ``data_collector``
* ``form.type``
* ``form.type_extension``
* ``form.type_guesser``
* ``kernel.cache_warmer``
* ``kernel.event_listener``
* ``monolog.logger``
* ``monolog.processor``
* ``templating.helper``
* ``routing.loader``
* ``translation.loader``
* ``twig.extension``
* ``validator.initializer``

Habilitando ayudantes de plantilla PHP personalizados
-----------------------------------------------------

Para habilitar un ayudante de plantilla, añádelo como un servicio regular en una de tus configuraciones, etiquetalo con ``templating.helper`` y define un atributo ``alias`` (el ayudante será accesible a través de este alias en las plantillas):

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.nombre_de_tu_ayudante:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml

        <service id="templating.helper.nombre_de_tu_ayudante" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php

        $contenedor
            ->register('templating.helper.nombre_de_tu_ayudante', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;

Habilitando extensiones Twig personalizadas
-------------------------------------------

Para habilitar una extensión de Twig, añádela como un servicio regular en una de tus configuraciones, y etiquetalo con ``twig.extension``:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.nombre_de_tu_extensión:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.nombre_de_tu_extensión" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $contenedor
            ->register('twig.extension.nombre_de_tu_extensión', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

.. _dic-tags-kernel-event-listener:

Habilitando escuchas personalizados
-----------------------------------

Para habilitar un escucha personalizado, añádelo como un servicio regular en uno de tus archivos de configuración, y etiquétalo con ``kernel.event_listener``. Debes proporcionar el nombre del evento a tu servicio escucha, así como el método que a invocar:

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.listener.your_listener_name:
                class: Fully\Qualified\Listener\Class\Name
                tags:
                    - { name: kernel.event_listener, event: xxx, method: onXxx }

    .. code-block:: xml

        <service id="kernel.listener.your_listener_name" class="Fully\Qualified\Listener\Class\Name">
            <tag name="kernel.event_listener" event="xxx" method="onXxx" />
        </service>

    .. code-block:: php

        $contenedor
            ->register('kernel.listener.your_listener_name', 'Fully\Qualified\Listener\Class\Name')
            ->addTag('kernel.event_listener', array('event' => 'xxx', 'method' => 'onXxx'))
        ;

.. note::

    También puedes especificar la prioridad como un entero positivo o negativo, lo cual te permite asegurarte de que tu escucha siempre será invocado antes o después de otro.

Habilitando motores de plantilla personalizados
-----------------------------------------------

Para activar un motor de plantillas personalizado, añádelo como un servicio regular en una de tus configuraciones, etiquetalo con ``templating.engine``:

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.engine.nombre_de_tu_motor:
                class: Fully\Qualified\Engine\Class\Name
                tags:
                    - { name: templating.engine }

    .. code-block:: xml

        <service id="templating.engine.nombre_de_tu_motor" class="Fully\Qualified\Engine\Class\Name">
            <tag name="templating.engine" />
        </service>

    .. code-block:: php

        $contenedor
            ->register('templating.engine.nombre_de_tu_motor', 'Fully\Qualified\Engine\Class\Name')
            ->addTag('templating.engine')
        ;

Habilitando cargadores de enrutado personalizado
------------------------------------------------

Para habilitar un gestor de enrutado personalizado, añádelo como un servicio regular en una de tus configuraciones, y etiquetalo con ``routing.loader``:

.. configuration-block::

    .. code-block:: yaml

        services:
            routing.loader.nombre_de_tu_cargador:
                class: Fully\Qualified\Loader\Class\Name
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <service id="routing.loader.nombre_de_tu_cargador" class="Fully\Qualified\Loader\Class\Name">
            <tag name="routing.loader" />
        </service>

    .. code-block:: php

        $contenedor
            ->register('routing.loader.nombre_de_tu_cargador', 'Fully\Qualified\Loader\Class\Name')
            ->addTag('routing.loader')
        ;

.. _dic_tags-monolog:

Usando canal de registro personalizado con ``Monolog``
------------------------------------------------------

``Monolog`` te permite compartir sus controladores entre varios canales registradores.
El servicio registrador utiliza el canal ``app`` pero puedes cambiar de canal cuando inyectes el registrador en un servicio.

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Fully\Qualified\Loader\Class\Name
                arguments: [@logger]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <service id="my_service" class="Fully\Qualified\Loader\Class\Name">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php

        $definition = new Definition('Fully\Qualified\Loader\Class\Name', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $contenedor->register('my_service', $definition);;

.. note::

    Esto sólo funciona cuando el servicio registrador es un argumento del constructor, no cuando se inyecta a través de un definidor.

.. _dic_tags-monolog-processor:

Agregando un procesador para ``Monolog``
----------------------------------------

``Monolog`` te permite agregar procesadores en el registrador o en los controladores para añadir datos adicionales en los registros. Un procesador recibe el registro como argumento y lo tiene que devolver después de añadir alguna información adicional en el atributo ``extra`` del registro.

Vamos a ver cómo puedes utilizar el ``IntrospectionProcessor`` integrado para agregar el archivo, la línea, la clase y el método en que se activó el registrador.

Puedes agregar un procesador globalmente:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor');
        $contenedor->register('my_service', $definition);

.. tip::

    Si el servicio no es ejecutable (con ``__invoke``) puedes agregar el atributo ``method`` en la etiqueta para utilizar un método específico.

También puedes agregar un procesador para un controlador específico utilizando el atributo ``handler``:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, handler: firephp }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" handler="firephp" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('handler' => 'firephp');
        $contenedor->register('my_service', $definition);

También puedes agregar un procesador para un canal registrador específico usando el atributo ``channel``. Esto registrará el procesador únicamente para el canal registrador ``security`` utilizado en el componente de seguridad:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, channel: security }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" channel="security" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('channel' => 'security');
        $contenedor->register('my_service', $definition);

.. note::

    No puedes utilizar ambos atributos ``handler`` y ``channel`` para la misma etiqueta debido a que los controladores son compartidos entre todos los canales.
