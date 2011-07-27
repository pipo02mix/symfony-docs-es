Cómo utilizar el patrón fábrica para crear servicios
====================================================

Los contenedores de servicios de Symfony2 proporcionan una forma eficaz de controlar la creación de objetos, lo cual te permite especificar los argumentos pasados ​​al constructor, así como llamar a los métodos y establecer parámetros. A veces, sin embargo, esto no te proporcionará todo lo necesario para construir tus objetos.
Por esta situación, puedes utilizar una fábrica para crear el objeto y decirle al contenedor de servicios que llame a un método en la fábrica y no directamente una instancia del objeto.

Supongamos que tienes una fábrica que configura y devuelve un nuevo objeto BoletinGestor::

    namespace Acme\HolaBundle\Newletter;

    class NewsletterFactory
    {
        public function get()
        {
            $newsletterManager = new BoletinGestor();

            // ...

            return $newsletterManager;
        }
    }

Para que el objeto ``BoletinGestor`` esté disponible como servicio, puedes configurar el contenedor de servicios para usar la clase de fábrica ``NewsletterFactory``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/services.yml
        parameters:
            # ...
            boletin_gestor.class: Acme\HolaBundle\Boletin\BoletinGestor
            boletin_factory.class: Acme\HolaBundle\Boletin\NewsletterFactory
        services:
            boletin_gestor:
                class:          %boletin_gestor.class%
                factory_class:  %newsletter_factory.class%
                factory_method: get 

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="boletin_gestor.class">Acme\HolaBundle\Boletin\BoletinGestor</parameter>
            <parameter key="boletin_factory.class">Acme\HolaBundle\Boletin\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="boletin_gestor" 
                     class="%boletin_gestor.class%"
                     factory-class="%newsletter_factory.class%"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $contenedor->setParameter('boletin_gestor.class', 'Acme\HolaBundle\Boletin\BoletinGestor');
        $contenedor->setParameter('boletin_factory.class', 'Acme\HolaBundle\Boletin\NewsletterFactory');

        $contenedor->setDefinition('boletin_gestor', new Definition(
            '%boletin_gestor.class%'
        ))->setFactoryClass(
            '%newsletter_factory.class%'
        )->setFactoryMethod(
            'get'
        );

Cuando especificas la clase que utiliza la fábrica (a través de ``factory_class``), el método será llamado estáticamente. Si la fábrica debe crear una instancia y se llama al método del objeto resultante (como en este ejemplo), configura la fábrica misma como un servicio:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/services.yml
        parameters:
            # ...
            boletin_gestor.class: Acme\HolaBundle\Boletin\BoletinGestor
            boletin_factory.class: Acme\HolaBundle\Boletin\NewsletterFactory
        services:
            boletin_factory:
                class:            %newsletter_factory.class%
            boletin_gestor:
                class:            %boletin_gestor.class%
                factory_service:  boletin_factory
                factory_method:   get 

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="boletin_gestor.class">Acme\HolaBundle\Boletin\BoletinGestor</parameter>
            <parameter key="boletin_factory.class">Acme\HolaBundle\Boletin\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="boletin_factory" class="%newsletter_factory.class%"/>
            <service id="boletin_gestor" 
                     class="%boletin_gestor.class%"
                     factory-service="boletin_factory"
                     factory-method="get"
            />
        </services>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $contenedor->setParameter('boletin_gestor.class', 'Acme\HolaBundle\Boletin\BoletinGestor');
        $contenedor->setParameter('boletin_factory.class', 'Acme\HolaBundle\Boletin\NewsletterFactory');

        $contenedor->setDefinition('boletin_factory', new Definition(
            '%newsletter_factory.class%'
        ))
        $contenedor->setDefinition('boletin_gestor', new Definition(
            '%boletin_gestor.class%'
        ))->setFactoryService(
            'boletin_factory'
        )->setFactoryMethod(
            'get'
        );

.. note::

   El servicio fábrica se indica por su nombre de id y no una referencia al propio servicio. Por lo tanto, no es necesario utilizar la sintaxis @.

Pasando argumentos al método fábrica
------------------------------------

Si tienes que pasar argumentos al método fábrica, puedes utilizar la opción ``arguments`` dentro del contenedor de servicios. Por ejemplo, supongamos que el método ``get`` en el ejemplo anterior tiene el servicio de ``templating`` como argumento:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/services.yml
        parameters:
            # ...
            boletin_gestor.class: Acme\HolaBundle\Boletin\BoletinGestor
            boletin_factory.class: Acme\HolaBundle\Boletin\NewsletterFactory
        services:
            boletin_factory:
                class:            %newsletter_factory.class%
            boletin_gestor:
                class:            %boletin_gestor.class%
                factory_service:  boletin_factory
                factory_method:   get
                arguments:
                    -             @templating

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="boletin_gestor.class">Acme\HolaBundle\Boletin\BoletinGestor</parameter>
            <parameter key="boletin_factory.class">Acme\HolaBundle\Boletin\NewsletterFactory</parameter>
        </parameters>

        <services>
            <service id="boletin_factory" class="%newsletter_factory.class%"/>
            <service id="boletin_gestor" 
                     class="%boletin_gestor.class%"
                     factory-service="boletin_factory"
                     factory-method="get"
            >
                <argument type="service" id="templating" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HolaBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        // ...
        $contenedor->setParameter('boletin_gestor.class', 'Acme\HolaBundle\Boletin\BoletinGestor');
        $contenedor->setParameter('boletin_factory.class', 'Acme\HolaBundle\Boletin\NewsletterFactory');

        $contenedor->setDefinition('boletin_factory', new Definition(
            '%newsletter_factory.class%'
        ))
        $contenedor->setDefinition('boletin_gestor', new Definition(
            '%boletin_gestor.class%',
            array(new Reference('templating'))
        ))->setFactoryService(
            'boletin_factory'
        )->setFactoryMethod(
            'get'
        );