Cómo gestionar dependencias comunes con servicios padre
=======================================================

A medida que agregas más funcionalidad a tu aplicación, puedes comenzar a tener clases relacionadas que comparten algunas de las mismas dependencias. Por ejemplo, puedes tener un gestor de boletines que utiliza inyección para definir sus dependencias::

    namespace Acme\HolaBundle\Mail;

    use Acme\HolaBundle\Mailer;
    use Acme\HolaBundle\EmailFormatter;

    class NewsletterManager
    {
        protected $cartero;
        protected $emailFormatter;

        public function setMailer(Mailer $cartero)
        {
            $this->mailer = $cartero;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->mailer = $cartero;
        }
        // ...
    }

y también una clase Tarjeta de Saludo que comparte las mismas dependencias::

    namespace Acme\HolaBundle\Mail;

    use Acme\HolaBundle\Mailer;
    use Acme\HolaBundle\EmailFormatter;

    class GreetingCardManager
    {
        protected $cartero;
        protected $emailFormatter;

        public function setMailer(Mailer $cartero)
        {
            $this->mailer = $cartero;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->mailer = $cartero;
        }
        // ...
    }

La configuración del servicio de estas clases se vería algo como esto:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HolaBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HolaBundle\Mail\GreetingCardManager
        services:
            mi_cartero:
                # ...
            my_correo_formatter:
                # ...
            newsletter_manager:
                class:     %newsletter_manager.class%
                calls:
                    - [ setMailer, [ @mi_cartero ] ]
                    - [ setEmailFormatter, [ @my_correo_formatter] ]

            greeting_card_manager:
                class:     %greeting_card_manager.class%
                calls:
                    - [ setMailer, [ @mi_cartero ] ]
                    - [ setEmailFormatter, [ @my_email_formatter] ]

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HolaBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HolaBundle\Mail\GreetingCardManager</parameter>
        </parameters>

        <services>
            <service id="mi_cartero" ... >
              <!-- ... -->
            </service>
            <service id="my_correo_formatter" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="mi_cartero" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_correo_formatter" />
                </call>
            </service>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="mi_cartero" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_correo_formatter" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $contenedor->setParameter('newsletter_manager.class', 'Acme\HolaBundle\Mail\NewsletterManager');
        $contenedor->setParameter('greeting_card_manager.class', 'Acme\HolaBundle\Mail\GreetingCardManager');

        $contenedor->setDefinition('mi_cartero', ... );
        $contenedor->setDefinition('my_correo_formatter', ... );
        $contenedor->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('mi_cartero')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_correo_formatter')
        ));
        $contenedor->setDefinition('greeting_card_manager', new Definition(
            '%greeting_card_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('mi_cartero')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_correo_formatter')
        ));

Hay mucha repetición, tanto en las clases como en la configuración. Esto significa que si cambias, por ejemplo, las clases de correo de la aplicación ``Mailer`` de ``EmailFormatter`` para inyectarlas a través del constructor, tendrías que actualizar la configuración en dos lugares. Del mismo modo, si necesitas hacer cambios en los métodos definidores tendrías que hacerlo en ambas clases. La forma típica de hacer frente a los métodos comunes de estas clases relacionadas es extraerlas en una superclase::

    namespace Acme\HolaBundle\Mail;

    use Acme\HolaBundle\Mailer;
    use Acme\HolaBundle\EmailFormatter;

    abstract class MailManager
    {
        protected $cartero;
        protected $emailFormatter;

        public function setMailer(Mailer $cartero)
        {
            $this->mailer = $cartero;
        }

        public function setEmailFormatter(EmailFormatter $emailFormatter)
        {
            $this->mailer = $cartero;
        }
        // ...
    }

Entonces ``NewsletterManager`` y ``GreetingCardManager`` pueden extender esta
superclase::

    namespace Acme\HolaBundle\Mail;

    class NewsletterManager extends MailManager
    {
        // ...
    }

y::

    namespace Acme\HolaBundle\Mail;

    class GreetingCardManager extends MailManager
    {
        // ...
    }

De manera similar, el contenedor de servicios de Symfony2 también apoya la extensión de servicios en la configuración por lo que también puedes reducir la repetición especificando un padre para un servicio.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HolaBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HolaBundle\Mail\GreetingCardManager
            mail_manager.class: Acme\HolaBundle\Mail\MailManager
        services:
            mi_cartero:
                # ...
            my_correo_formatter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setMailer, [ @mi_cartero ] ]
                    - [ setEmailFormatter, [ @my_correo_formatter] ]
            
            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
            
            greeting_card_manager:
                class:     %greeting_card_manager.class%
                parent: mail_manager
            
    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HolaBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HolaBundle\Mail\GreetingCardManager</parameter>
            <parameter key="mail_manager.class">Acme\HolaBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="mi_cartero" ... >
              <!-- ... -->
            </service>
            <service id="my_correo_formatter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="mi_cartero" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_correo_formatter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager"/>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%" parent="mail_manager"/>
        </services>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $contenedor->setParameter('newsletter_manager.class', 'Acme\HolaBundle\Mail\NewsletterManager');
        $contenedor->setParameter('greeting_card_manager.class', 'Acme\HolaBundle\Mail\GreetingCardManager');
        $contenedor->setParameter('mail_manager.class', 'Acme\HolaBundle\Mail\MailManager');

        $contenedor->setDefinition('mi_cartero', ... );
        $contenedor->setDefinition('my_correo_formatter', ... );
        $contenedor->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('mi_cartero')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_correo_formatter')
        ));
        $contenedor->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        );
        $contenedor->setDefinition('greeting_card_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%greeting_card_manager.class%'
        );

En este contexto, tener un servicio ``padre`` implica que los argumentos y las llamadas a métodos del servicio padre se deben utilizar en los servicios descendientes.
En concreto, los métodos definidores especificados para el servicio padre serán llamados cuando se crean instancias del servicio descendiente.

.. note::

   Si quitas la clave de configuración del ``padre``, el servicio todavía seguirá siendo una instancia, por supuesto, extendiendo la clase ``MailManager``. La diferencia es que la omisión del ``padre`` en la clave de configuración significa que las ``llamadas`` definidas en el servicio ``mail_manager`` no se ejecutarán al crear instancias de los servicios descendientes.

La clase padre es abstracta, ya que no se deben crear instancias directamente. Al establecerla como abstracta en el archivo de configuración como se hizo anteriormente, significa que sólo se puede utilizar como un servicio primario y no se puede utilizar directamente como un servicio para inyectar y retirar en tiempo de compilación. En otras palabras, existe sólo como una "plantilla" que otros servicios pueden utilizar.

Sustituyendo dependencias padre
-------------------------------

Puede haber ocasiones en las que deses sustituir que clase se pasa a una dependencia en un servicio hijo único. Afortunadamente, añadiendo la llamada al método de configuración para el servicio hijo, las dependencias establecidas por la clase principal se sustituyen. Así que si necesitas pasar una dependencia diferente sólo para la clase ``NewsletterManager``, la configuración sería la siguiente:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HolaBundle\Mail\NewsletterManager
            greeting_card_manager.class: Acme\HolaBundle\Mail\GreetingCardManager
            mail_manager.class: Acme\HolaBundle\Mail\MailManager
        services:
            mi_cartero:
                # ...
            my_alternative_mailer:
                # ...
            my_correo_formatter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setMailer, [ @mi_cartero ] ]
                    - [ setEmailFormatter, [ @my_correo_formatter] ]

            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
                calls:
                    - [ setMailer, [ @my_alternative_mailer ] ]

            greeting_card_manager:
                class:     %greeting_card_manager.class%
                parent: mail_manager
            
    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HolaBundle\Mail\NewsletterManager</parameter>
            <parameter key="greeting_card_manager.class">Acme\HolaBundle\Mail\GreetingCardManager</parameter>
            <parameter key="mail_manager.class">Acme\HolaBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="mi_cartero" ... >
              <!-- ... -->
            </service>
            <service id="my_alternative_mailer" ... >
              <!-- ... -->
            </service>
            <service id="my_correo_formatter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setMailer">
                     <argument type="service" id="mi_cartero" />
                </call>
                <call method="setEmailFormatter">
                     <argument type="service" id="my_correo_formatter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager">
                 <call method="setMailer">
                     <argument type="service" id="my_alternative_mailer" />
                </call>
            </service>
            <service id="greeting_card_manager" class="%greeting_card_manager.class%" parent="mail_manager"/>
        </services>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $contenedor->setParameter('newsletter_manager.class', 'Acme\HolaBundle\Mail\NewsletterManager');
        $contenedor->setParameter('greeting_card_manager.class', 'Acme\HolaBundle\Mail\GreetingCardManager');
        $contenedor->setParameter('mail_manager.class', 'Acme\HolaBundle\Mail\MailManager');

        $contenedor->setDefinition('mi_cartero', ... );
        $contenedor->setDefinition('my_alternative_mailer', ... );
        $contenedor->setDefinition('my_correo_formatter', ... );
        $contenedor->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setMailer', array(
            new Reference('mi_cartero')
        ))->addMethodCall('setEmailFormatter', array(
            new Reference('my_correo_formatter')
        ));
        $contenedor->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        )->addMethodCall('setMailer', array(
            new Reference('my_alternative_mailer')
        ));
        $contenedor->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%greeting_card_manager.class%'
        );

El ``GreetingCardManager`` recibirá las mismas dependencias que antes, pero la ``NewsletterManager`` será pasada a ``my_alternative_mailer`` en lugar del servicio ``mi_cartero``.

Colección de dependencias
-------------------------

Cabe señalar que el método definidor sustituido en el ejemplo anterior en realidad se llama dos veces - una vez en la definición del padre y otra más en la definición del hijo. En el ejemplo anterior, esto estaba muy bien, ya que la segunda llamada a ``setMailer`` sustituye al objeto mailer establecido por la primera llamada.

En algunos casos, sin embargo, esto puede ser un problema. Por ejemplo, si la sustitución a la llamada al método consiste en añadir algo a una colección, entonces se agregarán dos objetos a la colección. A continuación mostramos tal caso, si la clase padre se parece a esto::

    namespace Acme\HolaBundle\Mail;

    use Acme\HolaBundle\Mailer;
    use Acme\HolaBundle\EmailFormatter;

    abstract class MailManager
    {
        protected $filters;

        public function setFilter($filter)
        {
            $this->filters[] = $filter;
        }
        // ...
    }

Si tiene la siguiente configuración:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HolaBundle\Mail\NewsletterManager
            mail_manager.class: Acme\HolaBundle\Mail\MailManager
        services:
            my_filter:
                # ...
            another_filter:
                # ...
            mail_manager:
                class:     %mail_manager.class%
                abstract:  true
                calls:
                    - [ setFilter, [ @my_filter ] ]

            newsletter_manager:
                class:     %newsletter_manager.class%
                parent: mail_manager
                calls:
                    - [ setFilter, [ @another_filter ] ]

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HolaBundle\Mail\NewsletterManager</parameter>
            <parameter key="mail_manager.class">Acme\HolaBundle\Mail\MailManager</parameter>
        </parameters>

        <services>
            <service id="my_filter" ... >
              <!-- ... -->
            </service>
            <service id="another_filter" ... >
              <!-- ... -->
            </service>
            <service id="mail_manager" class="%mail_manager.class%" abstract="true">
                <call method="setFilter">
                     <argument type="service" id="my_filter" />
                </call>
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%" parent="mail_manager">
                 <call method="setFilter">
                     <argument type="service" id="another_filter" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $contenedor->setParameter('newsletter_manager.class', 'Acme\HolaBundle\Mail\NewsletterManager');
        $contenedor->setParameter('mail_manager.class', 'Acme\HolaBundle\Mail\MailManager');

        $contenedor->setDefinition('my_filter', ... );
        $contenedor->setDefinition('another_filter', ... );
        $contenedor->setDefinition('mail_manager', new Definition(
            '%mail_manager.class%'
        ))->SetAbstract(
            true
        )->addMethodCall('setFilter', array(
            new Reference('my_filter')
        ));
        $contenedor->setDefinition('newsletter_manager', new DefinitionDecorator(
            'mail_manager'
        ))->setClass(
            '%newsletter_manager.class%'
        )->addMethodCall('setFilter', array(
            new Reference('another_filter')
        ));

En este ejemplo, el ``setFilter`` del servicio ``newsletter_manager`` se llamará dos veces, dando lugar a que el array ``$filters`` contenga tanto a ``my_filter objetos`` como ``another_filter``. Esto es genial si sólo quieres agregar filtros adicionales para subclases. Si deseas reemplazar los filtros pasados a la subclase, elimina de la matriz el ajuste de la configuración, esto evitará que la clase ``setFilter`` base sea llamada.
