.. index::
   single: Entornos; Parámetros externos

Cómo configurar parámetros externos en el contenedor de servicios
=================================================================

En el capítulo :doc:`/cookbook/configuration/environments`, aprendiste cómo gestionar la configuración de tu aplicación. A veces, puedes beneficiar a tu aplicación almacenando ciertas credenciales fuera del código de tu proyecto. La configuración de la base de datos es tal ejemplo. La flexibilidad del contenedor de servicios Symfony te permite hacer esto fácilmente.

Variables de entorno
--------------------

Symfony grabará cualquier variable de entorno con el prefijo ``SYMFONY__`` y lo configurará como parámetro en el contenedor de servicios.  Los subrayados dobles son reemplazados por un punto, ya que un punto no es un carácter válido en un nombre de variable de entorno.

Por ejemplo, si estás usando Apache, las variables de entorno se pueden fijar usando la siguiente configuración de ``VirtualHost``:

.. code-block:: apache

    <VirtualHost *:80>
        ServerName      Symfony2
        DocumentRoot    "/ruta/a/symfony_2_app/web"
        DirectoryIndex  index.php index.html
        SetEnv          SYMFONY__DATABASE__USER user
        SetEnv          SYMFONY__DATABASE__PASSWORD secret

        <Directory "/ruta/a/my_symfony_2_app/web">
            AllowOverride All
            Allow from All
        </Directory>
    </VirtualHost>

.. note::

    El ejemplo anterior es una configuración para Apache, con la directiva `SetEnv`_.  Sin embargo, esta funcionará para cualquier servidor web compatible con la configuración de variables de entorno.

Ahora que haz declarado una variable de entorno, estará presente en la variable global PHP ``$_SERVER``. Entonces Symfony automáticamente fija todas las variables ``$_SERVER`` con el prefijo ``SYMFONY__`` como parámetros en el contenedor de servicios.

Ahora puedes referirte a estos parámetros donde los necesites.

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                driver    pdo_mysql
                dbname:   symfony2_project
                user:     %database.user%
                password: %database.password%

    .. code-block:: xml

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                driver="pdo_mysql"
                dbname="symfony2_projet"
                user="%database.user%"
                password="%database.password%"
            />
        </doctrine:config>

    .. code-block:: php

        $contenedor->loadFromExtension('doctrine', array('dbal' => array(
            'driver'   => 'pdo_mysql',
            'dbname'   => 'symfony2_project',
            'user'     => '%database.user%',
            'password' => '%database.password%',
        ));

Constantes
----------

El contenedor también cuenta con apoyo para fijar constantes PHP como parámetros. Para aprovechar esta característica, asigna el nombre de tu constante a un parámetro clave, y define el tipo como ``constant``.

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8"?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        >

            <parameters>
                <parameter key="global.constant.value" type="constant">GLOBAL_CONSTANT</parameter>
                <parameter key="my_class.constant.value" type="constant">My_Class::CONSTANT_NAME</parameter>
            </parameters>
        </container>

.. note::

    Esto sólo funciona para la configuración XML. Si *no* estás usando XML, sólo tienes que importar un archivo XML para tomar ventaja de esta funcionalidad:

    .. code-block:: yaml

        // app/config/config.yml
        imports:
            - { resource: parameters.xml }

Otra configuración
------------------

La directiva ``imports`` se puede utilizar para extraer parámetros almacenados en otro lugar. 
Importar un archivo PHP te da la flexibilidad de añadir al contenedor lo que sea necesario. La siguiente directiva importa un archivo llamado ``parameters.php``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.php }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.php" />
        </imports>

    .. code-block:: php

        // app/config/config.php
        $loader->import('parameters.php');

.. note::

    Un archivo de recursos puede tener uno de muchos tipos. Los recursos PHP, XML, YAML, INI y cierre son compatibles con la directiva ``imports``.

En ``parameters.php``, dile al contenedor de servicios los parámetros que deseas configurar. Esto es útil cuando la configuración importante está en un formato no estándar. El siguiente ejemplo incluye la configuración de una base de datos Drupal en el contenedor de servicios de Symfony.

.. code-block:: php

    // app/config/parameters.php

    include_once('/ruta/a/drupal/sites/all/default/settings.php');
    $contenedor->setParameter('drupal.database.url', $db_url);

.. _`SetEnv`: http://httpd.apache.org/docs/current/env.html