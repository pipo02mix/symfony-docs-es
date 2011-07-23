Registrando funciones DQL personalizadas
========================================

Doctrine te permite especificar las funciones DQL personalizadas. Para más información sobre este tema, lee el artículo "`DQL Funciones definidas por el usuario`_" de Doctrine.

En Symfony, puedes registrar tus funciones DQL personalizadas de la siguiente manera:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            orm:
                # ...
                entity_managers:
                    default:
                        # ...
                        dql:
                            string_functions:
                                test_string: Acme\HolaBundle\DQL\StringFunction
                                second_string: Acme\HolaBundle\DQL\SecondStringFunction
                            numeric_functions:
                                test_numeric: Acme\HolaBundle\DQL\NumericFunction
                            datetime_functions:
                                test_datetime: Acme\HolaBundle\DQL\DatetimeFunction

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:orm>
                    <!-- ... -->
                    <doctrine:entity-manager name="default">
                        <!-- ... -->
                        <doctrine:dql>
                            <doctrine:string-function name="test_string>Acme\HolaBundle\DQL\StringFunction</doctrine:string-function>
                            <doctrine:string-function name="second_string>Acme\HolaBundle\DQL\SecondStringFunction</doctrine:string-function>
                            <doctrine:numeric-function name="test_numeric>Acme\HolaBundle\DQL\NumericFunction</doctrine:numeric-function>
                            <doctrine:datetime-function name="test_datetime>Acme\HolaBundle\DQL\DatetimeFunction</doctrine:datetime-function>
                        </doctrine:dql>
                    </doctrine:entity-manager>
                </doctrine:orm>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('doctrine', array(
            'orm' => array(
                // ...
                'entity_managers' => array(
                    'default' => array(
                        // ...
                        'dql' => array(
                            'string_functions' => array(
                                'test_string'   => 'Acme\HolaBundle\DQL\StringFunction',
                                'second_string' => 'Acme\HolaBundle\DQL\SecondStringFunction',
                            ),
                            'numeric_functions' => array(
                                'test_numeric' => 'Acme\HolaBundle\DQL\NumericFunction',
                            ),
                            'datetime_functions' => array(
                                'test_datetime' => 'Acme\HolaBundle\DQL\DatetimeFunction',
                            ),
                        ),
                    ),
                ),
            ),
        ));

.. _`DQL Funciones definidas por el usuario`: http://www.doctrine-project.org/docs/orm/2.0/en/cookbook/dql-user-defined-functions.html