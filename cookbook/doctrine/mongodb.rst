.. index::
   pair: Doctrine; ODM MongoDB

Cómo utilizar MongoDB
=====================

El asignador de objeto a documento `MongoDB`_ (ODM por Object Document Mapper) es muy similar al ORM de Doctrine2 en su filosofía y funcionamiento. En otras palabras, similar al :doc:`ORM de Doctrine2 </book/doctrine>`, con el ODM de Doctrine, sólo tratas con objetos PHP simples, los cuales luego se persisten de forma transparente a y desde ``MongoDB``.

.. tip::

    Puedes leer más acerca del ODM de Doctrine MongoDB en la `documentación`_ del proyecto.

Está disponible el paquete, que integra al ODM Doctrine MongoDB en Symfony, por lo tanto es fácil configurarlo y usarlo.

.. note::

    Este capítulo lo debes de sentir muy parecido al capítulo :doc:`ORM de Doctrine2 </book/doctrine>`, que habla de cómo se puede utilizar el ORM de Doctrine para guardar los datos en bases de datos relacionales (por ejemplo, MySQL). Esto es a propósito - si persistes en una base de datos relacional por medio del ORM o a través del ODM MongoDB, las filosofías son muy parecidas.

Instalando
----------

Para utilizar el ODM MongoDB, necesitarás dos bibliotecas proporcionadas por Doctrine y un paquete que las integra en Symfony. Si estás usando la distribución estándar de Symfony, agrega lo siguiente al archivo ``deps`` en la raíz de tu proyecto:

.. code-block:: text

    [doctrine-mongodb]
        git=http://github.com/doctrine/mongodb.git

    [doctrine-mongodb-odm]
        git=http://github.com/doctrine/mongodb-odm.git

    [DoctrineMongoDBBundle]
        git=http://github.com/symfony/DoctrineMongoDBBundle.git
        target=/bundles/Symfony/Bundle/DoctrineMongoDBBundle

Ahora, actualiza las bibliotecas de proveedores ejecutando:

.. code-block:: bash

    $ php bin/vendors install

A continuación, agrega los espacios de nombres ``Doctrine\ODM\MongoDB`` y ``Doctrine\MongoDB`` al archivo ``app/autoload.php`` para que estas bibliotecas se puedan cargar automáticamente.
Asegúrate de añadirlas en cualquier lugar *por encima* del espacio de nombres ``Doctrine`` (cómo se muestra aquí)::

    // app/autoload.php
    $loader->registerNamespaces(array(
        // ...
        'Doctrine\\ODM\\MongoDB'    => __DIR__.'/../vendor/doctrine-mongodb-odm/lib',
        'Doctrine\\MongoDB'         => __DIR__.'/../vendor/doctrine-mongodb/lib',
        'Doctrine'                  => __DIR__.'/../vendor/doctrine/lib',
        // ...
    ));

A continuación, registra la biblioteca de anotaciones añadiendo las siguientes acciones al cargador (debajo de la línea ``AnnotationRegistry::registerFile`` existente)::

    // app/autoload.php
    AnnotationRegistry::registerFile(
        __DIR__.'/../vendor/doctrine-mongodb-odm/lib/Doctrine/ODM/MongoDB/Mapping/Annotations/DoctrineAnnotations.php'
    );

Por último, activa el nuevo paquete en el núcleo::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Symfony\Bundle\DoctrineMongoDBBundle\DoctrineMongoDBBundle(),
        );

        // ...
    }

¡Enhorabuena! Estás listo para empezar a trabajar.

Configurando
------------

Para empezar, necesitarás una estructura básica que configure el gestor de documentos. La forma más fácil es habilitar el ``auto_mapping``, el cual activará al ODM MongoDB a través de tu aplicación:

.. code-block:: yaml

    # app/config/config.yml
    doctrine_mongodb:
        connections:
            default:
                server: mongodb://localhost:27017
                options:
                    connect: true
        default_database: test_database
        document_managers:
            default:
                auto_mapping: true

.. note::

    Por supuesto, también tienes que asegurarte de que el servidor MongoDB se ejecute en segundo plano. Para más información, consulta la `Guía de inicio rápido`_ de MongoDB.

Un ejemplo sencillo: un producto
--------------------------------

La mejor manera de entender el ODM de Doctrine MongoDB es verlo en acción.
En esta sección, recorreremos cada paso necesario para empezar a persistir documentos hacia y desde MongoDB.

.. sidebar:: Código del ejemplo

    Si quieres seguir el ejemplo de este capítulo, crea el paquete ``AcmeTiendaBundle`` ejecutando la orden:

    .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/TiendaBundle

Creando una clase Documento
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Supongamos que estás construyendo una aplicación donde necesitas mostrar tus productos.
Sin siquiera pensar en Doctrine o MongoDB, ya sabes que necesitas un objeto ``Producto`` para representar los productos. Crea esta clase en el directorio ``Documento`` de tu ``AcmeTiendaBundle``::

    // src/Acme/TiendaBundle/Document/Producto.php
    namespace Acme\TiendaBundle\Document;

    class Producto
    {
        protected $nombre;

        protected $precio;
    }

La clase - a menudo llamada "documento", es decir, *una clase básica que contiene los datos* - es simple y ayuda a cumplir con el requisito del negocio de que tu aplicación necesita productos. Esta clase, todavía no se puede persistir a Doctrine MongoDB - es sólo una clase PHP simple.

Agrega información de asignación
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine te permite trabajar con MongoDB de una manera mucho más interesante que solo recuperar datos de un lado a otro como una matriz. En cambio, Doctrine te permite persistir *objetos* completos a MongoDB y recuperar objetos enteros desde MongoDB. Esto funciona asignando una clase PHP y sus propiedades a las entradas de una colección MongoDB.

Para que Doctrine sea capaz de hacer esto, sólo tienes que crear "metadatos", o la configuración que le dice a Doctrine exactamente cómo se deben *asignar* a MongoDB la clase ``Producto`` y sus propiedades. Estos metadatos se pueden especificar en una variedad de formatos diferentes, incluyendo YAML, XML o directamente dentro de la clase ``Producto`` a través de anotaciones:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/TiendaBundle/Document/Producto.php
        namespace Acme\TiendaBundle\Document;

        use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;

        /**
         * @MongoDB\Document
         */
        class Producto
        {
            /**
             * @MongoDB\Id
             */
            protected $id;

            /**
             * @MongoDB\String
             */
            protected $nombre;

            /**
             * @MongoDB\Float
             */
            protected $precio;
        }

    .. code-block:: yaml

        # src/Acme/TiendaBundle/Resources/config/doctrine/Producto.mongodb.yml
        Acme\TiendaBundle\Document\Producto:
            fields:
                id:
                    id:  true
                name:
                    type: string
                precio:
                    type: float

    .. code-block:: xml

        <!-- src/Acme/TiendaBundle/Resources/config/doctrine/Producto.mongodb.xml -->
        <doctrine-mongo-mapping xmlns="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping
                            http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping.xsd">

            <document name="Acme\TiendaBundle\Document\Producto">
                <field fieldName="id" id="true" />
                <field fieldName="name" type="string" />
                <field fieldName="precio" type="float" />
            </document>
        </doctrine-mongo-mapping>

Doctrine te permite elegir entre una amplia variedad de tipos de campo diferentes, cada uno con sus propias opciones. Para más información sobre los tipos de campo disponibles, consulta la sección :ref:`cookbook-mongodb-field-types`.

.. seealso::

    También puedes consultar la `Documentación de asignación básica`_ de Doctrine para todos los detalles sobre la información de asignación. Si utilizas anotaciones, tendrás que prefijar todas tus anotaciones con ``MongoDB\`` (por ejemplo, ``MongoDB\Cadena``), lo cual no se muestra en la documentación de Doctrine. También tendrás que incluir la declaración ``use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;``, que *importa* el prefijo ``MongoDB`` de las anotaciones.

Generando captadores y definidores
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A pesar de que Doctrine ya sabe cómo persistir un objeto ``Producto`` a MongoDB, la clase en sí todavía no es realmente útil. Puesto que ``Producto`` es sólo una clase PHP regular, es necesario crear métodos captadores y definidores (por ejemplo, ``getNombre()``, ``setNombre()``) para poder acceder a sus propiedades (ya que las propiedades son ``protegidas``). Afortunadamente, Doctrine puede hacer esto por ti con la siguiente orden:

.. code-block:: bash

    php app/console doctrine:mongodb:generate:documents AcmeTiendaBundle

Esta orden se asegura de que se generen todos los captadores y definidores para la clase ``Producto``. Esta es una orden segura - la puedes ejecutar una y otra vez: sólo genera captadores y definidores que no existen (es decir, no sustituye métodos existentes).

.. note::

    A Doctrine no le importa si tus propiedades son ``protegidas`` o ``privadas``, o si una propiedad tiene o no una función captadora o definidora.
    Aquí, los captadores y definidores se generan sólo porque los necesitarás para interactuar con tu objeto PHP.

Persistiendo objetos a MongoDB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ahora que tienes asignado un documento ``Producto`` completo, con métodos captadores y definidores, estás listo para persistir los datos a MongoDB. Desde el interior de un controlador, esto es bastante fácil. Agrega el siguiente método al ``DefaultController`` del paquete:

.. code-block:: php
    :linenos:

    // src/Acme/TiendaBundle/Controller/DefaultController.php
    use Acme\TiendaBundle\Document\Producto;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    public function createAction()
    {
        $producto = new Producto();
        $producto->setNombre('A Foo Bar');
        $producto->setPrecio('19.99');

        $dm = $this->get('doctrine.odm.mongodb.document_manager');
        $dm->persist($producto);
        $dm->flush();

        return new Response('Id de producto '.$producto->getId().' creado.');
    }

.. note::

    Si estás siguiendo este ejemplo, tendrás que crear una ruta que apunte a esta acción para verla trabajar.

Vamos a recorrer este ejemplo:

* **Líneas 7-10** En esta sección, creas una instancia y trabajas con el objeto ``$producto`` como cualquier otro objeto PHP normal;

* **Línea 12** Esta línea recupera el objeto *administrador de documentos*, el cual es responsable de manejar el proceso de persistir y recuperar objetos a y desde MongoDB;

* **Línea 13** El método ``persist()`` dice a Doctrine que "maneje" el objeto ``$producto``. Esto en realidad no resulta en una consulta que se deba hacer a MongoDB (todavía).

* **Línea 14** Cuando se llama al método ``flush()``, Doctrine mira todos los objetos que está gestionando para ver si se necesita persistirlos a la base de datos. En este ejemplo, el objeto ``$producto`` aún no se ha persistido, por lo que el gestor de documentos hace una consulta a MongoDB, la cual añade una nueva entrada.

.. note::

    De hecho, ya que Doctrine es consciente de todos los objetos administrados, cuando llamas al método ``flush()``, se calcula un conjunto de cambios y ejecuta la operación más eficiente posible.

Al crear o actualizar objetos, el flujo de trabajo siempre es el mismo. En la siguiente sección, verás cómo Doctrine es lo suficientemente inteligente como para actualizar las entradas, si ya existen en MongoDB.

.. tip::

    Doctrine proporciona una biblioteca que te permite cargar mediante programación los datos de prueba en tu proyecto (es decir, "accesorios"). Para más información, consulta :doc:`/cookbook/doctrine/doctrine_fixtures`.

Recuperando objetos desde MongoDB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Recuperar un objeto de MongoDB incluso es más fácil. Por ejemplo, supongamos que haz configurado una ruta para mostrar un ``Producto`` específico en función del valor de su ``id``::

    public function showAction($id)
    {
        $producto = $this->get('doctrine.odm.mongodb.document_manager')
            ->getRepository('AcmeTiendaBundle:Producto')
            ->find($id);

        if (!$producto) {
            throw $this->createNotFoundException('Ningún producto encontrado con id '.$id);
        }

        // Hace algo, como pasar el objeto $producto a una plantilla
    }

Al consultar por un determinado tipo de objeto, siempre utilizas lo que se conoce como "repositorio". Puedes pensar en un repositorio como una clase PHP, cuyo único trabajo consiste en ayudarte a buscar los objetos de una determinada clase. Puedes acceder al repositorio de objetos de una clase de documento vía::

    $repositorio = $this->get('doctrine.odm.mongodb.document_manager')
        ->getRepository('AcmeTiendaBundle:Producto');

.. note::

    La cadena ``AcmeTiendaBundle:Producto`` es un método abreviado que puedes utilizar en cualquier lugar de Doctrine en lugar del nombre completo de la clase del documento (es decir, ``Acme\TiendaBundle\Document\Producto``).
    Mientras tu documento viva en el espacio de nombres ``Document`` de tu paquete, esto va a funcionar.

Una vez que tengas tu repositorio, tienes acceso a todo tipo de útiles métodos::

    // consulta por la clave principal (por lo general "id")
    $producto = $repositorio->find($id);

    // nombres de método dinámicos para buscar basándose en un valor de columna
    $producto = $repositorio->findOneById($id);
    $producto = $repositorio->findOneByName('foo');

    // recupera *todos* los productos
    $productos = $repositorio->findAll();

    // busca un grupo de productos basándose en el valor de una columna arbitraria
    $productos = $repositorio->findByPrice(19.99);

.. note::

    Por supuesto, también puedes realizar consultas complejas, acerca de las cuales aprenderás más en la sección :ref:`book-doctrine-queries`.

También puedes tomar ventaja de los útiles métodos ``findBy`` y ``findOneBy`` para recuperar fácilmente los objetos basándote en varias condiciones::

    // consulta por un producto que coincide en nombre y precio
    $producto = $repositorio->findOneBy(array('nombre' => 'foo', 'precio' => 19.99));

    // consulta por todos los productos que coinciden con el nombre, y los ordena por precio
    $producto = $repositorio->findBy(
        array('nombre' => 'foo'),
        array('precio', 'ASC')
    );

Actualizando un objeto
~~~~~~~~~~~~~~~~~~~~~~

Una vez que hayas extraído un objeto de Doctrine, actualizarlo es relativamente fácil. Supongamos que tienes una ruta que asigna un identificador de producto a una acción de actualización de un controlador::

    public function updateAction($id)
    {
        $dm = $this->get('doctrine.odm.mongodb.document_manager');
        $producto = $dm->getRepository('AcmeTiendaBundle:Producto')->find($id);

        if (!$producto) {
            throw $this->createNotFoundException('Ningún producto encontrado con id '.$id);
        }

        $producto->setNombre('¡Nuevo nombre de producto!');
        $dm->flush();

        return $this->redirect($this->generateUrl('portada'));
    }

La actualización de un objeto únicamente consiste en tres pasos:

1. Recuperar el objeto desde Doctrine;
2. Modificar el objeto;
3. Llamar a ``flush()`` en el gestor del documento;

Ten en cuenta que ``$dm->persist($producto)`` no es necesario. Recuerda que este método simplemente dice a Doctrine que maneje o "vea" el objeto ``$producto``.
En este caso, ya que recuperaste desde Doctrine el objeto ``$producto``, este ya está gestionado.

Eliminando un objeto
~~~~~~~~~~~~~~~~~~~~

La eliminación de un objeto es muy similar, pero requiere una llamada al método ``remove()`` del gestor de documentos::

    $dm->remove($producto);
    $dm->flush();

Como es de esperar, el método ``remove()`` notifica a Doctrine que deseas eliminar el documento propuesto desde MongoDB. La operación real de eliminar sin embargo, no se ejecuta efectivamente hasta que invocas al método ``flush()``.

Consultando objetos
-------------------

Como vimos anteriormente, la clase repositorio incorporada te permite consultar por uno o varios objetos basándote en una serie de parámetros diferentes. Cuando esto es suficiente, esta es la forma más sencilla de consultar documentos. Por supuesto, también puedes crear consultas más complejas.

Usando el generador de consultas
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El ODM de Doctrine viene con un objeto "Constructor" de consultas, el cual te permite construir una consulta para exactamente los documentos que deseas devolver. Si usas un IDE, también puedes tomar ventaja del autocompletado a medida que escribes los nombres de métodos.
Desde el interior de un controlador::

    $productos = $this->get('doctrine.odm.mongodb.document_manager')
        ->createQueryBuilder('AcmeTiendaBundle:Producto')
        ->field('nombre')->equals('foo')
        ->limit(10)
        ->sort('precio', 'ASC')
        ->getQuery()
        ->execute()

En este caso, devuelve 10 productos con el nombre "foo", ordenados de menor a mayor precio.

El objeto ``QueryBuilder`` contiene todos los métodos necesarios para construir tu consulta. Para más información sobre el generador de consultas de Doctrine, consulta la documentación del `Generador de consultas`_ de Doctrine. Para una lista de las condiciones disponibles que puedes colocar en la consulta, ve la documentación específica a los `Operadores condicionales`_.

Repositorio de clases personalizado
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En la sección anterior, comenzaste a construir y utilizar consultas más complejas desde el interior de un controlador. A fin de aislar, probar y reutilizar esas consultas, es buena idea crear una clase repositorio personalizada para tu documento y agregar métodos con la lógica de la consulta allí.

Para ello, agrega el nombre de la clase del repositorio a la definición de asignación.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/TiendaBundle/Document/Producto.php
        namespace Acme\TiendaBundle\Document;

        use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;

        /**
         * @MongoDB\Document(repositoryClass="Acme\TiendaBundle\Repository\ProductoRepository")
         */
        class Producto
        {
            //...
        }

    .. code-block:: yaml

        # src/Acme/TiendaBundle/Resources/config/doctrine/Producto.mongodb.yml
        Acme\TiendaBundle\Document\Producto:
            repositoryClass: Acme\TiendaBundle\Repository\ProductoRepository
            # ...

    .. code-block:: xml

        <!-- src/Acme/TiendaBundle/Resources/config/doctrine/Producto.mongodb.xml -->
        <!-- ... -->
        <doctrine-mongo-mapping xmlns="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping
                            http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping.xsd">

            <document name="Acme\TiendaBundle\Document\Producto"
                    repository-class="Acme\TiendaBundle\Repository\ProductoRepository">
                <!-- ... -->
            </document>

        </doctrine-mong-mapping>

Doctrine puede generar la clase repositorio para ti ejecutando:

.. code-block:: bash

    php app/console doctrine:mongodb:generate:repositories AcmeTiendaBundle

A continuación, agrega un nuevo método - ``findAllOrderedByName()`` - a la clase repositorio recién generada. Este método de consulta será para todos los documentos ``Producto``, ordenados alfabéticamente.

.. code-block:: php

    // src/Acme/TiendaBundle/Repository/ProductoRepository.php
    namespace Acme\TiendaBundle\Repository;

    use Doctrine\ODM\MongoDB\DocumentRepository;

    class ProductoRepository extends DocumentRepository
    {
        public function findAllOrderedByName()
        {
            return $this->createQueryBuilder()
                ->sort('nombre', 'ASC')
                ->getQuery()
                ->execute();
        }
    }

Puedes utilizar este nuevo método al igual que los métodos de búsqueda predeterminados del repositorio::

    $producto = $this->get('doctrine.odm.mongodb.document_manager')
        ->getRepository('AcmeTiendaBundle:Producto')
        ->findAllOrderedByName();


.. note::

    Al utilizar una clase repositorio personalizada, todavía tienes acceso a los métodos de búsqueda predeterminados como ``find()`` y ``findAll()``.

Extensiones Doctrine: ``Timestampable``, ``Sluggable``, etc.
------------------------------------------------------------

Doctrine es bastante flexible, y dispone de una serie de extensiones de terceros que te permiten realizar fácilmente tareas repetitivas y comunes en tus entidades.
Estas incluyen cosas tales como ``Sluggable``, ``Timestampable``, ``registrable``, ``traducible`` y ``Tree``.

Para más información sobre cómo encontrar y utilizar estas extensiones, ve el artículo sobre el uso de :doc:`extensiones comunes Doctrine </cookbook/doctrine/common_extensions>`.

.. _cookbook-mongodb-field-types:

Referencia de tipos de campo Doctrine
-------------------------------------

Doctrine dispone de una gran cantidad de tipos de campo. Cada uno de estos asigna un tipo de dato PHP a un determinado `tipo de MongoDB`_. Los siguientes son sólo *algunos* de los tipos admitidos por Doctrine:

* ``string``
* ``int``
* ``float``
* ``date``
* ``timestamp``
* ``boolean``
* ``file``

Para más información, consulta la sección `Asignando tipos`_ en la documentación de Doctrine.

.. index::
   single: Doctrine; Ordenes de consola ODM
   single: CLI; ODM de Doctrine

Ordenes de consola
------------------

La integración ODM de Doctrine2 ofrece varias ordenes de consola en el espacio de nombres ``doctrine:mongodb``. Para ver la lista de ordenes puedes ejecutar la consola sin ningún tipo de argumento:

.. code-block:: bash

    php app/console

Mostrará una lista con las ordenes disponibles, muchas de las cuales comienzan con el prefijo ``doctrine:mongodb``. Puedes encontrar más información sobre cualquiera de estas ordenes (o cualquier orden de Symfony) ejecutando la orden ``help``.
Por ejemplo, para obtener detalles acerca de la tarea ``doctrine:mongodb:query``, ejecuta:

.. code-block:: bash

    php app/console help doctrine:mongodb:query

.. note::

   Para poder cargar accesorios en MongoDB, necesitas tener instalado el paquete ``DoctrineFixturesBundle``. Para saber cómo hacerlo, lee el artículo ":doc:`/cookbook/doctrine/doctrine_fixtures`" del recetario.

.. index::
   single: Configurando; Doctrine ODM MongoDB
   single: Doctrine; configurando ODM MongoDB

Configurando
------------

Para información más detallada sobre las opciones de configuración disponibles cuando utilizas el ODM de Doctrine, consulta la sección :doc:`Referencia MongoDB </reference/configuration/mongodb>`.

Registrando escuchas y suscriptores de eventos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine te permite registrar los escuchas y suscriptores que recibirán una notificación cuando se produzcan diferentes eventos al interior del ODM Doctrine. Para más información, consulta la sección `Documentación de eventos`_ de Doctrine.

En Symfony, puedes registrar un escucha o suscriptor creando un :term:`servicio` y, a continuación :ref:`marcarlo <book-service-container-tags>` con una etiqueta específica.

* **escucha de eventos**: Usa la etiqueta ``doctrine.odm.mongodb.<conexión>_event_listener``, donde el nombre ``<conexión>`` es sustituido por el nombre de la conexión (por lo general ``default``). Además, asegúrate de agregar un ``evento`` clave para la etiqueta que especifica el evento que escucha. Suponiendo que la conexión se llama ``default``, entonces:

    .. configuration-block::

        .. code-block:: yaml

            services:
                my_doctrine_listener:
                    class:   Acme\HolaBundle\Listener\MyDoctrineListener
                    # ...
                    tags:
                        -  { name: doctrine.odm.mongodb.default_event_listener, event: postPersist }

        .. code-block:: xml

            <service id="my_doctrine_listener" class="Acme\HolaBundle\Listener\MyDoctrineListener">
                <!-- ... -->
                <tag name="doctrine.odm.mongodb.default_event_listener" event="postPersist" />
            </service>.

        .. code-block:: php

            $definition = new Definition('Acme\HolaBundle\Listener\MyDoctrineListener');
            // ...
            $definition->addTag('doctrine.odm.mongodb.default_event_listener');
            $contenedor->setDefinition('my_doctrine_listener', $definition);

* **Suscriptor de evento**: Utiliza la etiqueta ``doctrine.odm.mongodb.<conexión>_event_subscriber``. Ninguna otra clave es necesaria en la etiqueta.

Resumen
-------

Con Doctrine, te puedes enfocar en los objetos y la forma en que son útiles en tu aplicación y en segundo lugar preocuparte por la persistencia a través de MongoDB. Esto se debe a que Doctrine te permite utilizar cualquier objeto PHP para almacenar los datos y confía en la información de asignación de metadatos para asignar los datos de un objeto a una tabla particular de la base de datos.

Y aunque Doctrine gira en torno a un concepto simple, es increíblemente poderosa, lo cual te permite crear consultas complejas y suscribirte a los eventos que te permiten realizar diferentes acciones conforme los objetos recorren su ciclo de vida en la persistencia.

.. _`MongoDB`:          http://www.mongodb.org/
.. _`documentación`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en _`Guía de inicio rápido`:      http://www.mongodb.org/display/DOCS/Quickstart
.. _`Documentación de asignación básica`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/basic-mapping.html
.. _`tipo de MongoDB`: http://us.php.net/manual/en/mongo.types.php
.. _`Asignando tipos`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/basic-mapping.html#doctrine-mapping-types
.. _`Generador de consultas`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/query-builder-api.html
.. _`Operadores condicionales`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/query-builder-api.html#conditional-operators
.. _`Documentación de eventos`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/events.html