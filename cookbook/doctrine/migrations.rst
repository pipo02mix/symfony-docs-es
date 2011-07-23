.. index::
   pair: Doctrine; Migraciones

Cómo utilizar las migraciones Doctrine
======================================

La funcionalidad migración de base de datos, es una extensión de la capa de abstracción de bases de datos y te ofrece la posibilidad de desplegar programáticamente nuevas versiones del esquema de la base de datos de forma segura y estandarizada.

.. tip::

    Puedes leer más sobre las migraciones de base de datos de Doctrine en la `documentación`_ del proyecto.

Instalando
----------

Las migraciones de Doctrine para Symfony se mantienen en el `DoctrineMigrationsBundle`_.
Asegúrate de que tienes configuradas en tu proyecto ambas bibliotecas ``doctrine-migrations`` y ``DoctrineMigrationsBundle``. Sigue estos pasos para instalar las bibliotecas en la distribución estándar de Symfony.

Agrega lo siguiente a ``deps``. Esto registrará el paquete Migraciones y la biblioteca *doctrine-migrations* como dependencias en tu aplicación:

.. code-block:: text

    [doctrine-migrations]
        git=http://github.com/doctrine/migrations.git

    [DoctrineMigrationsBundle]
        git=http://github.com/symfony/DoctrineMigrationsBundle.git
        target=/bundles/Symfony/Bundle/DoctrineMigrationsBundle

Actualiza las bibliotecas de proveedores:

.. code-block:: bash

    $ php bin/vendors install

A continuación, asegúrate de que el nuevo espacio de nombres ``Doctrine\DBAL\Migrations`` se carga automáticamente a través de ``autoload.php``. El nuevo espacio de nombres Migraciones *debe* estar colocado encima de la entrada ``Doctrine\\DBAL`` de manera que el cargador automático vea dentro del directorio migraciones de esas clases:

.. code-block:: php

    // app/autoload.php
    $loader->registerNamespaces(array(
        //...
        'Doctrine\\DBAL\\Migrations' => __DIR__.'/../vendor/doctrine-migrations/lib',
        'Doctrine\\DBAL'             => __DIR__.'/../vendor/doctrine-dbal/lib',
    ));

Por último, asegúrate de activar el paquete en ``AppKernel.php`` incluyendo lo siguiente:

.. code-block:: php

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            //...
            new Symfony\Bundle\DoctrineMigrationsBundle\DoctrineMigrationsBundle(),
        );
    }

Utilizando
----------

Toda la funcionalidad de las migraciones se encuentra en unas cuantas ordenes de consola:

.. code-block:: bash

    doctrine:migrations
      :diff     Genera una migración comparando tu base de datos actual
                a tu información asignada.
      :execute  Ejecuta manualmente una sola versión de migración hacia
                arriba o abajo.
      :generate Genera una clase de migración en blanco.
      :migrate  Executa una migración a una versión especificada o la
                última versión disponible.
      :status   Ve el estado de un conjunto de migraciones.
      :version  Añade y elimina versiones de migración manualmente desde
                la tabla de versiones.

Empieza consiguiendo la situación de las migraciones en tu aplicación ejecutando la orden ``status``:

.. code-block:: bash

    php app/console doctrine:migrations:status

     == Configuración

        >> Nombre:                                 Migraciones de HolaBundle
        >> Fuente de configuración:                configurado manualmente
        >> Nombre de la tabla de versión:          hola_bundle_migration_versions
        >> Espacio de nombres de las migraciones:  Application\Migrations
        >> Directorio de las migraciones:          ruta/a/symfony-sandbox/app/DoctrineMigrations
        >> Versión actual:                         0
        >> Última versión:                         0
        >> Migraciones realizadas:                 0
        >> Migraciones disponibles:                0
        >> Nuevas Migraciones:                     0

Ahora, podemos empezar a trabajar con las migraciones generando una nueva clase de migración en blanco: Más adelante, aprenderás cómo Doctrine puede generar migraciones automáticamente.

.. code-block:: bash

    php app/console doctrine:migrations:generate
    Nueva clase migración generada para "/ruta/al/proyecto/app/DoctrineMigrations/Version20100621140655.php"

Echa un vistazo a la clase migración recién generada y verás algo como lo siguiente::

    namespace Application\Migrations;

    use Doctrine\DBAL\Migrations\AbstractMigration,
        Doctrine\DBAL\Schema\Schema;

    class Version20100621140655 extends AbstractMigration
    {
        public function up(Schema $schema)
        {

        }

        public function down(Schema $schema)
        {

        }
    }

Si ejecutas la orden ``status`` ahora te mostrará que tiene una nueva migración por ejecutar:

.. code-block:: bash

    php app/console doctrine:migrations:status

     == Configuración

       >> Nombre:                                 Aplicación de migraciones
       >> Fuente de configuración:                configurado manualmente
       >> Nombre de la tabla de versión:          migration_versions
       >> Espacio de nombres de las migraciones:  Application\Migrations
       >> Directorio de las migraciones:          /ruta/al/proyecto/app/DoctrineMigrations
       >> Versión actual:                         0
       >> Última versión:                         2011-06-21 14:06:55 (20100621140655)
       >> Migraciones realizadas:                 0
       >> Migraciones disponibles:                1
       >> Nuevas Migraciones:                     1

    == Versión de migración

       >> 2011-06-21 14:06:55 (20110621140655)           no migrada

Ahora puedes agregar algo de código de migración a los métodos ``up()`` y ``down()``, y finalmente cuando estés listo migrar:

.. code-block:: bash

    php app/console doctrine:migrations:migrate

Para más información sobre cómo escribir migraciones en sí mismas (es decir, la manera de rellenar los métodos ``up()`` y ``down()``), consulta la `documentación`_ oficial de las Migraciones de Doctrine.

Ejecutando migraciones durante la implantación
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Por supuesto, el objetivo final de la escritura de migraciones es poder utilizarlas para actualizar de forma fiable la estructura de tu base de datos cuando implantes tu aplicación.
Al ejecutar localmente las migraciones (o en un servidor beta), puedes asegurarte de que las migraciones trabajan según lo previsto.

Cuando finalmente emplees tu aplicación, sólo tienes que recordar ejecutar la orden ``doctrine:migrations:migrate``. Internamente, Doctrine crea una tabla ``migration_versions`` dentro de la base de datos y lleva a cabo el seguimiento de las migraciones que se han ejecutado allí. Por lo tanto, no importa cuantas migraciones hayas creado y ejecutado localmente, cuando se ejecuta la orden durante la implantación, Doctrine sabrá exactamente qué migraciones no se han ejecutado todavía mirando la tabla ``migration_versions`` de tu base de datos de producción. Independientemente de qué servidor esté activado, siempre puedes ejecutar esta orden de forma segura para realizar sólo las migraciones que todavía no se han llevado a cabo en *esa* base de datos particular.

Generando migraciones automáticamente
-------------------------------------

En realidad, no deberías tener que escribir migraciones manualmente, puesto que la biblioteca de migraciones puede generar las clases de la migración automáticamente comparando tu información asignada a Doctrine (es decir, cómo se *debe* ver tu base de datos) con la estructura de la base de datos actual.

Por ejemplo, supongamos que creas una nueva entidad ``User`` y agregas información asignándola al ORM Doctrine:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Entity/Usuario.php
        namespace Acme\HolaBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="hola_user")
         */
        class Usuario
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length="255")
             */
            protected $nombre;
        }

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/doctrine/User.orm.yml
        Acme\HolaBundle\Entity\User:
            type: entity
            table: hola_user
            id:
                id:
                    type: integer
                    generator:
                        strategy: AUTO
            fields:
                name:
                    type: string
                    length: 255

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/doctrine/User.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\HolaBundle\Entity\User" table="hola_user">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO"/>
                </id>
                <field name="name" column="name" type="string" length="255" />
            </entity>

        </doctrine-mapping>

Con esta información, Doctrine ya está listo para ayudarte a persistir tu nuevo objeto ``User`` a y desde la tabla ``hola_user``. Por supuesto, ¡esta tabla no existe aún! Genera una nueva migración para esta tabla automáticamente ejecutando la siguiente orden:

.. code-block:: bash

    php app/console doctrine:migrations:diff

Deberás ver un mensaje informado que se ha generado una nueva clase migración basada en las diferencias del esquema. Si abres ese archivo, encontrarás que tiene el código SQL necesario para crear la tabla ``hola_user``. A continuación, ejecuta la migración para agregar la tabla a la base de datos:

.. code-block:: bash

    php app/console doctrine:migrations:migrate

La moraleja de la historia es la siguiente: después de cada cambio que realices en tu información de asignación a Doctrine, ejecuta la orden ``doctrine:migrations:diff`` para generar automáticamente las clases de la migración.

Si lo haces desde el principio de tu proyecto (es decir, de modo que incluso las primeras tablas fueran cargadas a través de una clase migración), siempre podrás crear una base de datos actualizada y ejecutar las migraciones a fin de tener tu esquema de base de datos totalmente actualizado. De hecho, se trata de un flujo de trabajo fácil y confiable para tu proyecto.

.. _documentación: http://www.doctrine-project.org/projects/migrations/2.0/docs/reference/introduction/en
.. _DoctrineMigrationsBundle: https://github.com/symfony/DoctrineMigrationsBundle