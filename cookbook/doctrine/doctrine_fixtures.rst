.. index::
   single: Doctrine; Creando accesorios en Symfony2

Cómo crear accesorios en Symfony2
=================================

Los accesorios se utilizan para cargar un conjunto de datos controlado en una base de datos. Puedes utilizar esto datos para pruebas o podrían ser los datos iniciales necesarios para ejecutar la aplicación sin problemas. Symfony2 no tiene integrada forma alguna de administrar accesorios, pero Doctrine2 cuenta con una biblioteca para ayudarte a escribir accesorios para el :doc:`ORM </book/doctrine>` u :doc:`ODM </cookbook/doctrine/mongodb>` de Doctrine.

Instalando y configurando
-------------------------

Si todavía no tienes configurada en Symfony2 la biblioteca `Doctrine Data Fixtures`_, sigue estos pasos para hacerlo.

Si estás utilizando la distribución estándar, agrega lo siguiente a tu archivo ``deps``:

.. code-block:: text

    [doctrine-fixtures]
        git=http://github.com/doctrine/data-fixtures.git

    [DoctrineFixturesBundle]
        git=http://github.com/symfony/DoctrineFixturesBundle.git
        target=/bundles/Symfony/Bundle/DoctrineFixturesBundle

Actualiza las bibliotecas de proveedores:

.. code-block:: bash

    $ php bin/vendors install

Si todo funcionó, la biblioteca ``doctrine-fixtures`` ahora se puede encontrar en ``vendor/doctrine-fixtures``.

Registra el espacio de nombres ``Doctrine\Common\DataFixtures`` en ``app/autoload.php``.

.. code-block:: php

    // ...
    $loader->registerNamespaces(array(
        // ...
        'Doctrine\\Common\\DataFixtures' => __DIR__.'/../vendor/doctrine-fixtures/lib',
        'Doctrine\\Common' => __DIR__.'/../vendor/doctrine-common/lib',
        // ...
    ));

.. caution::

    Asegúrate de registrar el nuevo espacio de nombres *antes* de ``Doctrine\Common``. De lo contrario, Symfony buscará clases accesorio dentro del directorio ``Doctrine\Common``. El autocargador de Symfony, en primer lugar, siempre busca una clase dentro del directorio del espacio de nombres coincidente, los espacios de nombres más específicos *siempre* deben estar primero.

Por último, registra el paquete ``DoctrineFixturesBundle`` en ``app/AppKernel.php``.

.. code-block:: php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Symfony\Bundle\DoctrineFixturesBundle\DoctrineFixturesBundle(),
            // ...
        );
        // ...
    }

Escribiendo accesorios simples
------------------------------

Los accesorios Doctrine2 son clases PHP que pueden crear objetos y persisten a la base de datos. Al igual que todas las clases en Symfony2, los accesorios deben vivir dentro de uno de los paquetes de tu aplicación.

Para un paquete situado en ``src/Acme/HolaBundle``, las clases accesorio deben vivir dentro de ``src/Acme/HolaBundle/DataFixtures/ORM`` o ``src/Acme/HolaBundle/DataFixtures/ODM``, para ORM y ODM respectivamente, esta guía asume que estás utilizando ORM - pero, los accesorios se pueden agregar con la misma facilidad si estás utilizando ODM.

Imagina que tienes una clase ``Usuario``, y que te gustaría cargar una entrada ``Usuario``:

.. code-block:: php

    // src/Acme/HolaBundle/DataFixtures/ORM/LoadUserData.php
    namespace Acme\HolaBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Acme\HolaBundle\Entity\Usuario;

    class LoadUserData implements FixtureInterface
    {
        public function load($manager)
        {
            $usuarioAdmin = new User();
            $usuarioAdmin->setUsername('admin');
            $usuarioAdmin->setPassword('prueba');

            $manager->persist($usuarioAdmin);
            $manager->flush();
        }
    }

En Doctrine2, los accesorios son sólo objetos en los que cargas datos interactuando con tus entidades como lo haces normalmente. Esto te permite crear el accesorio exacto que necesitas para tu aplicación.

La limitación más importante es que no puedes compartir objetos entre accesorios.
Más tarde, veremos la manera de superar esta limitación.

Ejecutando accesorios
---------------------

Una vez que haz escrito tus accesorios, los puedes cargar a través de la línea de ordenes usando la orden ``doctrine:fixtures:load``

.. code-block:: bash

    $ php app/console doctrine:fixtures:load

Si estás utilizando ODM, en su lugar usa la orden ``doctrine:mongodb:fixtures:load``:

.. code-block:: bash

    php app/console doctrine:mongodb:fixtures:load

La tarea verá dentro del directorio ``DataFixtures/ORM`` (o ``DataFixtures/ODM`` para ODM) de cada paquete y ejecutará cada clase que implemente ``FixtureInterface``.

Ambas ordenes vienen con algunas opciones:

* ``--fixtures=/ruta/a/fixture`` - Usa esta opción para especificar manualmente el directorio de dónde se deben cargar las clases accesorio;

* ``--append`` - Utiliza esta opción para añadir datos en lugar de eliminarlos antes de cargarlos (borrar primero es el comportamiento predeterminado);

* ``--em=manager_name`` - Especifica manualmente el administrador de la entidad a utilizar para descargar los datos.

.. note::

   Si utilizas la tarea ``doctrine:mongodb:fixtures:load``, reemplaza la opción ``--em=`` con ``--dm=`` para especificar manualmente el gestor de documentos.

Un ejemplo de uso completo podría tener este aspecto:

.. code-block:: bash

   php app/console doctrine:fixtures:load --fixtures=/ruta/a/fixture1 --fixtures=/ruta/a/fixture2 --append --em=foo_manager

Compartiendo objetos entre accesorios
-------------------------------------

Escribir un accesorio básico es simple. Pero, ¿si tienes varias clases de accesorios y quieres poder referirte a los datos cargados en otras clases accesorio?
Por ejemplo, ¿qué pasa si cargas un objeto ``Usuario`` en un accesorio, y luego quieres mencionar una referencia en un accesorio diferente con el fin de asignar dicho usuario a un grupo particular?

La biblioteca de accesorios de Doctrine maneja esto fácilmente permitiéndote especificar el orden en que se cargan los accesorios.

.. code-block:: php

    // src/Acme/HolaBundle/DataFixtures/ORM/LoadUserData.php
    namespace Acme\HolaBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Acme\HolaBundle\Entity\Usario;

    class LoadUserData extends AbstractFixture implements OrderedFixtureInterface
    {
        public function load($manager)
        {
            $usuarioAdmin = new User();
            $usuarioAdmin->setUsername('admin');
            $usuarioAdmin->setPassword('prueba');

            $manager->persist($userAdmin);
            $manager->flush();

            $this->addReference('admin-user', $userAdmin);
        }

        public function getOrder()
        {
            return 1; // el orden en el que se deben cargar los accesorios
        }
    }

La clase accesorio ahora implementa ``OrderedFixtureInterface``, la cual dice a Doctrine que deseas controlar el orden de tus accesorios. Crea otra clase accesorio y haz que se cargue después de ``LoadUserData`` devolviendo un orden de 2:

.. code-block:: php

    // src/Acme/HolaBundle/DataFixtures/ORM/LoadGroupData.php
    namespace Acme\HolaBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Acme\HolaBundle\Entity\Group;

    class LoadGroupData extends AbstractFixture implements OrderedFixtureInterface
    {
        public function load($manager)
        {
            $groupAdmin = new Group();
            $groupAdmin->setGroupName('admin');

            $manager->persist($groupAdmin);
            $manager->flush();

            $this->addReference('admin-group', $groupAdmin);
        }

        public function getOrder()
        {
            return 2; // el orden en el que se deben cargar los accesorios
        }
    }

Ambas clases accesorio extienden ``AbstractFixture``, lo cual te permite crear objetos y luego ponerlos como referencias para que se puedan utilizar posteriormente en otros accesorios. Por ejemplo, los objetos ``$UserAdmin`` y ``$groupAdmin`` se pueden referir posteriormente a través de las referencias ``admin-user`` y ``admin-group``:

.. code-block:: php

    // src/Acme/HolaBundle/DataFixtures/ORM/LoadUserGroupData.php
    namespace Acme\HolaBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Acme\HolaBundle\Entity\UserGroup;

    class LoadUserGroupData extends AbstractFixture implements OrderedFixtureInterface
    {
        public function load($manager)
        {
            $userGroupAdmin = new UserGroup();
            $userGroupAdmin->setUser($manager->merge($this->getReference('admin-user')));
            $userGroupAdmin->setGroup($manager->merge($this->getReference('admin-group')));

            $manager->persist($userGroupAdmin);
            $manager->flush();
        }

        public function getOrder()
        {
            return 3;
        }
    }

Los accesorios ahora se ejecutan en el orden ascendente del valor devuelto por ``getOrder()``. Cualquier objeto que se establece con el método ``setReference()`` se puede acceder a través de ``getReference()`` en las clases accesorio que tienen un orden superior.

Los accesorios te permiten crear cualquier tipo de dato que necesites a través de la interfaz normal de PHP para crear y persistir objetos. Al controlar el orden de los accesorios y establecer referencias, casi todo se puede manejar por medio de accesorios.

Usando el contenedor en los accesorios
--------------------------------------

En algunos casos será necesario que accedas a algunos servicios para cargar los accesorios.
Symfony2 hace esto realmente sencillo: el contenedor se inyectará en todas las clases accesorio que implementen :class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface`.

Vamos a rescribir el primer accesorio para codificar la contraseña antes de almacenarla en la base de datos (una muy buena práctica). Esto utilizará el generador de codificadores para codificar la contraseña, asegurando que está codificada en la misma forma que utiliza el componente de seguridad al efectuar la verificación:

.. code-block:: php

    // src/Acme/HolaBundle/DataFixtures/ORM/LoadUserData.php
    namespace Acme\HolaBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Symfony\Component\DependencyInjection\ContainerAwareInterface;
    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Acme\HolaBundle\Entity\User;

    class LoadUserData implements FixtureInterface, ContainerAwareInterface
    {
        private $contenedor;

        public function setContainer(ContainerInterface $contenedor = null)
        {
            $this->container = $contenedor;
        }

        public function load($manager)
        {
            $userAdmin = new User();
            $userAdmin->setUsername('admin');
            $userAdmin->setSalt(md5(time()));

            $encoder = $this->container->get('security.encoder_factory')->getEncoder($userAdmin);
            $userAdmin->setPassword($encoder->encodePassword('test', $userAdmin->getSalt()));

            $manager->persist($usuarioAdmin);
            $manager->flush();
        }
    }

Como puedes ver, todo lo que necesitas hacer es agregar ``ContainerAwareInterface`` a la clase y luego crear un nuevo método ``setContainer()`` que implementa esa interfaz. Antes de que se ejecute el accesorio, Symfony automáticamente llamará al método ``setContainer()``. Siempre y cuando guardes el contenedor como una propiedad en la clase (como se muestra arriba), puedes acceder a él en el método ``load()``.

.. _`Doctrine Data Fixtures`: https://github.com/doctrine/data-fixtures
