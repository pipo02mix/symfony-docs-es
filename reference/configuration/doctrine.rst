.. index::
   single: Doctrine; Referencia de configuración del ORM
   single: Referencia de configuración; ORM de Doctrine

Referencia de configuración
===========================

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection:   default
                connections:
                    default:
                        dbname:               database
                        host:                 localhost
                        port:                 1234
                        user:                 user
                        password:             secret
                        driver:               pdo_mysql
                        driver_class:         MyNamespace\MyDriverImpl
                        options:
                            foo: bar
                        path:                 %kernel.data_dir%/data.sqlite
                        memory:               true
                        unix_socket:          /tmp/mysql.sock
                        wrapper_class:        MyDoctrineDbalConnectionWrapper
                        charset:              UTF8
                        logging:              %kernel.debug%
                        platform_service:     MyOwnDatabasePlatformService
                        mapping_types:
                            enum: string
                    conn1:
                        # ...
                types:
                    custom: Acme\HolaBundle\MyCustomType
            orm:
                auto_generate_proxy_classes:    true
                proxy_namespace:                Proxies
                proxy_dir:                      %kernel.cache_dir%/doctrine/orm/Proxies
                default_entity_manager:         default # The first defined is used if not set
                entity_managers:
                    default:
                        # The name of a DBAL connection (the one marked as default is used if not set)
                        connection:                     conn1
                        mappings: # Required
                            AcmeHelloBundle: ~
                        class_metadata_factory_name:    Doctrine\ORM\Mapping\ClassMetadataFactory
                        # All cache drivers have to be array, apc, xcache or memcache
                        metadata_cache_driver:          array
                        query_cache_driver:             array
                        result_cache_driver:
                            type:           memcache
                            host:           localhost
                            port:           11211
                            instance_class: Memcache
                            class:          Doctrine\Common\Cache\MemcacheCache
                        dql:
                            string_functions:
                                test_string: Acme\HolaBundle\DQL\StringFunction
                            numeric_functions:
                                test_numeric: Acme\HolaBundle\DQL\NumericFunction
                            datetime_functions:
                                test_datetime: Acme\HolaBundle\DQL\DatetimeFunction
                    em2:
                        # ...

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection
                        name="default"
                        dbname="database"
                        host="localhost"
                        port="1234"
                        user="user"
                        password="secret"
                        driver="pdo_mysql"
                        driver-class="MyNamespace\MyDriverImpl"
                        path="%kernel.data_dir%/data.sqlite"
                        memory="true"
                        unix-socket="/tmp/mysql.sock"
                        wrapper-class="MyDoctrineDbalConnectionWrapper"
                        charset="UTF8"
                        logging="%kernel.debug%"
                        platform-service="MyOwnDatabasePlatformService"
                    >
                        <doctrine:option key="foo">bar</doctrine:option>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                    <doctrine:connection name="conn1" />
                    <doctrine:type name="custom">Acme\HolaBundle\MyCustomType</doctrine:type>
                </doctrine:dbal>

                <doctrine:orm default-entity-manager="default" auto-generate-proxy-classes="true" proxy-namespace="Proxies" proxy-dir="%kernel.cache_dir%/doctrine/orm/Proxies">
                    <doctrine:entity-manager name="default" query-cache-driver="array" result-cache-driver="array" connection="conn1" class-metadata-factory-name="Doctrine\ORM\Mapping\ClassMetadataFactory">
                        <doctrine:metadata-cache-driver type="memcache" host="localhost" port="11211" instance-class="Memcache" class="Doctrine\Common\Cache\MemcacheCache" />
                        <doctrine:mapping name="AcmeHelloBundle" />
                        <doctrine:dql>
                            <doctrine:string-function name="test_string>Acme\HolaBundle\DQL\StringFunction</doctrine:string-function>
                            <doctrine:numeric-function name="test_numeric>Acme\HolaBundle\DQL\NumericFunction</doctrine:numeric-function>
                            <doctrine:datetime-function name="test_datetime>Acme\HolaBundle\DQL\DatetimeFunction</doctrine:datetime-function>
                        </doctrine:dql>
                    </doctrine:entity-manager>
                    <doctrine:entity-manager name="em2" connection="conn2" metadata-cache-driver="apc">
                        <doctrine:mapping
                            name="DoctrineExtensions"
                            type="xml"
                            dir="%kernel.root_dir%/../src/vendor/DoctrineExtensions/lib/DoctrineExtensions/Entity"
                            prefix="DoctrineExtensions\Entity"
                            alias="DExt"
                        />
                    </doctrine:entity-manager>
                </doctrine:orm>
            </doctrine:config>
        </container>

Describiendo la configuración
-----------------------------

El siguiente ejemplo de configuración muestra todos los valores de configuración predeterminados que resuelve ORM:

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            auto_generate_proxy_classes: true
            proxy_namespace: Proxies
            proxy_dir: %kernel.cache_dir%/doctrine/orm/Proxies
            default_entity_manager: default
            metadata_cache_driver: array
            query_cache_driver: array
            result_cache_driver: array

Hay un montón de opciones de configuración que puedes utilizar para redefinir ciertas clases, pero solamente son para casos de uso muy avanzado.

Controladores de caché
~~~~~~~~~~~~~~~~~~~~~~

Para los controladores de memoria caché puedes especificar los valores "array", "apc", "memcache" o "xcache".

El siguiente ejemplo muestra una descripción de los ajustes de la memoria caché:

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            metadata_cache_driver: apc
            query_cache_driver: xcache
            result_cache_driver:
                type: memcache
                host: localhost
                port: 11211
                instance_class: Memcache

Configurando la asignación
~~~~~~~~~~~~~~~~~~~~~~~~~~

La definición explícita de todas las entidades asignadas es la única configuración necesaria para el ORM y hay varias opciones de configuración que puedes controlar. Existen las siguientes opciones de configuración para una asignación:

* ``type`` Uno de ``annotation``, ``xml``, ``yml``, ``php`` o ``staticphp``.
  Esto especifica cual tipo de metadatos usa el tipo de tu asignación.

* ``dir`` Ruta a la asignación o archivos de entidad (dependiendo del controlador). Si esta ruta es relativa, se supone que es relativa a la raíz del paquete. Esto sólo funciona si el nombre de tu asignación es un nombre de paquete. Si deseas utilizar esta opción para especificar rutas absolutas debes prefijar la ruta con los parámetros del núcleo existentes en el DIC (por ejemplo %kernel.root_dir%).

* ``prefix`` Un prefijo común del espacio de nombres que comparten todas las entidades de esta asignación. Este prefijo nunca debe entrar en conflicto con otros prefijos de asignación definidos, de otra manera Doctrine no podrá encontrar algunas de tus entidades. Esta opción tiene predeterminado el espacio de nombres paquete + ``Entidad``, por ejemplo, para un paquete llamado ``AcmeHelloBundle`` el prefijo sería ``Acme\HolaBundle\Entidad``.

* ``alias`` Doctrine ofrece una forma de simplificar el espacio de nombres de la entidad, para utilizar nombres más cortos en las consultas DQL o para acceder al Repositorio. Cuando utilices un paquete el alias predeterminado es el nombre del paquete.

* ``is_bundle`` Esta opción es un valor derivado de ``dir`` y por omisión se establece en ``true`` si dir es relativo provisto por un ``file_exists()`` comprueba que devuelve ``false``. Este es ``false`` si al comprobar la existencia devuelve ``true``. En este caso se ha especificado una ruta absoluta y es más probable que los archivos de metadatos estén en un directorio fuera del paquete.

.. index::
   single: Configurando; DBAL Doctrine
   single: Doctrine; DBAL configurando

.. _`reference-dbal-configuration`:

Configurando DBAL Doctrine
--------------------------

.. note::

    ``DoctrineBundle`` apoya todos los parámetros por omisión que los controladores de Doctrine aceptan, convertidos a la nomenclatura estándar de XML o YAML que Symfony hace cumplir. Consulta la `Documentación DBAL`_ de Doctrine para más información.

Además de las opciones por omisión de Doctrine, hay algunas relacionadas con Symfony que se pueden configurar. El siguiente bloque muestra todas las posibles claves de configuración:

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                dbname:               database
                host:                 localhost
                port:                 1234
                user:                 user
                password:             secret
                driver:               pdo_mysql
                driver_class:         MyNamespace\MyDriverImpl
                options:
                    foo: bar
                path:                 %kernel.data_dir%/data.sqlite
                memory:               true
                unix_socket:          /tmp/mysql.sock
                wrapper_class:        MyDoctrineDbalConnectionWrapper
                charset:              UTF8
                logging:              %kernel.debug%
                platform_service:     MyOwnDatabasePlatformService
                mapping_types:
                    enum: string
                types:
                    custom: Acme\HolaBundle\MyCustomType

    .. code-block:: xml

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="database"
                host="localhost"
                port="1234"
                user="user"
                password="secret"
                driver="pdo_mysql"
                driver-class="MyNamespace\MyDriverImpl"
                path="%kernel.data_dir%/data.sqlite"
                memory="true"
                unix-socket="/tmp/mysql.sock"
                wrapper-class="MyDoctrineDbalConnectionWrapper"
                charset="UTF8"
                logging="%kernel.debug%"
                platform-service="MyOwnDatabasePlatformService"
            >
                <doctrine:option key="foo">bar</doctrine:option>
                <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                <doctrine:type name="custom">Acme\HolaBundle\MyCustomType</doctrine:type>
            </doctrine:dbal>
        </doctrine:config>

Si deseas configurar varias conexiones en YAML, ponlas bajo la clave ``connections`` y dales un nombre único:

.. code-block:: yaml

    doctrine:
        dbal:
            default_connection:       default
            connections:
                default:
                    dbname:           Symfony2
                    user:             root
                    password:         null
                    host:             localhost
                customer:
                    dbname:           customer
                    user:             root
                    password:         null
                    host:             localhost

El servicio ``database_connection`` siempre se refiere a la conexión *default*, misma que es la primera definida o la configurada a través del parámetro ``default_connection``.

Cada conexión también es accesible a través del servicio ``doctrine.dbal.[name]_connection`` donde ``[name]`` es el nombre de la conexión.

.. _Documentación DBAL: http://www.doctrine-project.org/docs/dbal/2.0/en