.. index::
   single: Doctrine

Bases de datos y Doctrine ("el modelo")
=======================================

Seamos realistas, una de las tareas más comunes y desafiantes para cualquier aplicación consiste en la persistencia y la lectura de la información hacia y desde una base de datos. Afortunadamente, Symfony viene integrado con `Doctrine`_, una biblioteca, cuyo único objetivo es dotarte de poderosas herramientas para facilitarte esto. En este capítulo, aprenderás la filosofía básica detrás de Doctrine y verás lo fácil que puede ser trabajar con una base de datos.

.. note::

    Doctrine está totalmente desconectado de Symfony y utilizarlo es opcional.
    Este capítulo trata acerca del ORM Doctrine, el cual te permite asignar objetos a una base de datos relacional (tal como *MySQL*, *PostgreSQL* o *Microsoft SQL*).
    Si prefieres utilizar las consultas de base de datos en bruto, es fácil, y se explica en el artículo ":doc:`/cookbook/doctrine/dbal`" del recetario.

    También puedes persistir tus datos en `MongoDB`_ utilizando la biblioteca ODM de Doctrine. Para más información, lee el artículo ":doc:`/cookbook/doctrine/mongodb`" en el recetario.

Un ejemplo sencillo: un producto
--------------------------------

La forma más fácil de entender cómo funciona Doctrine es verla en acción.
En esta sección, configuraremos tu base de datos, crearemos un objeto ``Producto``, lo persistiremos en la base de datos y lo recuperaremos de nuevo.

.. sidebar:: Código del ejemplo

    Si quieres seguir el ejemplo de este capítulo, crea el paquete ``AcmeTiendaBundle`` ejecutando la orden:

    .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/TiendaBundle

Configurando la base de datos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Antes de comenzar realmente, tendrás que configurar tu información de conexión a la base de datos. Por convención, esta información se suele configurar en un archivo
``app/config/parameters.ini``:

.. code-block:: ini

    ;app/config/parameters.ini
    [parameters]
        database_driver   = pdo_mysql
        database_host     = localhost
        database_name     = test_project
        database_user     = root
        database_password = password

.. note::

    Definir la configuración a través de ``parameters.ini`` sólo es una convención.
    Los parámetros definidos en este archivo son referidos en el archivo de configuración principal al configurar Doctrine:

    .. code-block:: yaml

        doctrine:
            dbal:
                driver:   %database_driver%
                host:     %database_host%
                dbname:   %database_name%
                user:     %database_user%
                password: %database_password%

    Al separar la información de la base de datos en un archivo independiente, puedes guardar fácilmente diferentes versiones del archivo en cada servidor. También puedes almacenar fácilmente la configuración de la base de datos (o cualquier otra información sensible) fuera de tu proyecto, posiblemente dentro de tu configuración de Apache, por ejemplo. Para más información, consulta :doc:`/cookbook/configuration/external_parameters`.

Ahora que Doctrine conoce acerca de tu base de datos, posiblemente tenga que crear la base de datos para ti:

.. code-block:: bash

    php app/console doctrine:database:create

Creando una clase Entidad
~~~~~~~~~~~~~~~~~~~~~~~~~

Supongamos que estás construyendo una aplicación donde necesitas mostrar tus productos.
Sin siquiera pensar en Doctrine o en una base de datos, ya sabes que necesitas un objeto ``Producto`` para representar los productos. Crea esta clase en el directorio ``Entity`` de tu paquete ``AcmeTiendaBundle``::

    // src/Acme/TiendaBundle/Entity/Producto.php    
    namespace Acme\TiendaBundle\Entity;

    class Producto
    {
        protected $nombre;

        protected $precio;

        protected $descripcion;
    }

La clase - a menudo llamada "entidad", es decir, *una clase básica que contiene datos* - es simple y ayuda a cumplir con el requisito del negocio de productos que necesita tu aplicación. Sin embargo, esta clase no se puede guardar en una base de datos - es sólo una clase PHP simple.

.. tip::

    Una vez que aprendas los conceptos de Doctrine, puedes dejar que Doctrine cree por ti la entidad para esta clase:

    .. code-block:: bash

        php app/console doctrine:generate:entity AcmeTiendaBundle:Producto "name:string(255) precio:float descripcion:text"

Agrega información de asignación
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine te permite trabajar con bases de datos de una manera mucho más interesante que solo recuperar filas de una tabla basada en columnas en una matriz. En cambio, Doctrine te permite persistir *objetos* completos a la base de datos y recuperar objetos completos desde la base de datos. Esto funciona asignando una clase PHP a una tabla de la base de datos, y las propiedades de esa clase PHP a las columnas de la tabla:

.. image:: /images/book/doctrine_imagen_1.png
   :align: center

Para que Doctrine sea capaz de hacer esto, sólo hay que crear "metadatos", o la configuración que le dice a Doctrine exactamente cómo debe *asignar* la clase ``Producto`` y sus propiedades a la base de datos. Estos metadatos se pueden especificar en una variedad de formatos diferentes, incluyendo YAML, XML o directamente dentro de la clase ``Producto`` a través de anotaciones:

.. note::

    Un paquete sólo puede aceptar un formato para definir metadatos. Por ejemplo, no es posible mezclar metadatos para la clase Entidad definidos en YAML con definidos en anotaciones PHP.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/TiendaBundle/Entity/Producto.php
        namespace Acme\TiendaBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="producto")
         */
        class Producto
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length=100)
             */
            protected $nombre;

            /**
             * @ORM\Column(type="decimal", scale=2)
             */
            protected $precio;

            /**
             * @ORM\Column(type="text")
             */
            protected $descripcion;
        }

    .. code-block:: yaml

        # src/Acme/TiendaBundle/Resources/config/doctrine/Producto.orm.yml
        Acme\TiendaBundle\Entity\Producto:
            type: entity
            table: producto
            id:
                id:
                    type: integer
                    generator: { strategy: AUTO }
            fields:
                nombre:
                    type: string
                    length: 100
                precio:
                    type: decimal
                    scale: 2
                descripcion:
                    type: text

    .. code-block:: xml

        <!-- src/Acme/TiendaBundle/Resources/config/doctrine/Producto.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\TiendaBundle\Entity\Producto" table="producto">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO" />
                </id>
                <field name="nombre" column="nombre" type="string" length="100" />
                <field name="precio" column="precio" type="decimal" scale="2" />
                <field name="descripcion" column="descripcion" type="text" />
            </entity>
        </doctrine-mapping>

.. tip::

    El nombre de la tabla es opcional y si la omites, será determinada automáticamente basándose en el nombre de la clase entidad.

.. tip::

    Cuando utilizas otra biblioteca o programa (es decir, Doxygen) que utiliza anotaciones, debes utilizar la anotación ``@IgnoreAnnotation`` para indicar que se deben ignorar las anotaciones Symfony y Doctrine.  Esta anotación se debe colocar en el bloque de comentarios de la clase a que se aplica.  No hacerlo puede resultar en una excepción.
    
    Por ejemplo, para evitar que la anotación ``@fn`` lance una excepción, agrega lo siguiente::

        /**
         * @IgnoreAnnotation("fn")
         * 
         */
        class Producto

Doctrine te permite elegir entre una amplia variedad de tipos de campo diferentes, cada uno con sus propias opciones. Para obtener información sobre los tipos de campo disponibles, consulta la sección :ref:`book-doctrine-field-types`.

.. seealso::

    También puedes consultar la `Documentación de asignación básica`_ de Doctrine para todos los detalles sobre la información de asignación. Si utilizas anotaciones, tendrás que prefijar todas las anotaciones con ``ORM\`` (por ejemplo, ``ORM\Column(..)``), lo cual no se muestra en la documentación de Doctrine. También tendrás que incluir la declaración ``use Doctrine\ORM\Mapping as ORM;``, la cual *importa* las anotaciones prefijas ``ORM``.

Generando captadores y definidores
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A pesar de que Doctrine ahora sabe cómo persistir un objeto ``Producto`` a la base de datos, la clase en sí realmente no es útil todavía. Puesto que ``Producto`` es sólo una clase PHP regular, es necesario crear métodos captadores y definidores (por ejemplo, ``getNombre()``, ``setNombre()``) para poder acceder a sus propiedades (ya que las propiedades son ``protegidas``). Afortunadamente, Doctrine puede hacer esto por ti con la siguiente orden:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme/TiendaBundle/Entity/Producto

Esta orden se asegura de que se generen todos los captadores y definidores para la clase ``Producto``. Esta es una orden segura - la puedes ejecutar una y otra vez: sólo genera captadores y definidores que no existen (es decir, no sustituye métodos existentes).

.. note::

    A Doctrine no le importa si tus propiedades son ``protegidas`` o ``privadas``, o si una propiedad tiene o no una función captadora o definidora.
    Aquí, los captadores y definidores se generan sólo porque los necesitarás para interactuar con tu objeto PHP.

.. tip::

    También puedes generar todas las entidades conocidas (es decir, cualquier clase PHP con información de asignación de Doctrine) de un paquete o un espacio de nombres completo:

    .. code-block:: bash

        php app/console doctrine:generate:entities AcmeTiendaBundle
        php app/console doctrine:generate:entities Acme

Creando tablas/esquema de la base de datos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ahora tienes una clase ``Producto`` utilizable con información de asignación de modo que Doctrine conoce exactamente cómo persistirla. Por supuesto, en tu base de datos aún no tienes la tabla ``Producto`` correspondiente. Afortunadamente, Doctrine puede crear automáticamente todas las tablas de la base de datos necesarias para cada entidad conocida en tu aplicación. Para ello, ejecuta:

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. tip::

    En realidad, esta orden es increíblemente poderosa. Esta compara cómo se *debe* ver tu base de datos  (en base a la información de asignación de tus entidades) con la forma en que *realmente* se ve, y genera las declaraciones SQL necesarias para *actualizar* la base de datos a donde debe estar. En otras palabras, si agregas una nueva propiedad asignando metadatos a ``Producto`` y ejecutas esta tarea de nuevo, vas a generar la declaración "alter table" necesaria para añadir la nueva columna a la tabla ``Producto`` existente.

    Una forma aún mejor para tomar ventaja de esta funcionalidad es a través de las :doc:`migraciones </cookbook/doctrine/migrations>`, las cuales te permiten generar las instrucciones SQL y almacenarlas en las clases de la migración, mismas que se pueden ejecutar sistemáticamente en tu servidor en producción con el fin de seguir la pista y migrar el esquema de la base de datos segura y fiablemente.

Tu base de datos ahora cuenta con una tabla ``producto`` completamente funcional, con columnas que coinciden con los metadatos que haz especificado.

Persistiendo objetos a la base de datos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ahora que tienes asignada una entidad ``producto`` y la tabla ``Producto`` correspondiente, estás listo para persistir los datos a la base de datos. Desde el interior de un controlador, esto es bastante fácil. Agrega el siguiente método al ``DefaultController`` del paquete:

.. code-block:: php
    :linenos:

    // src/Acme/TiendaBundle/Controller/DefaultController.php
    use Acme\TiendaBundle\Entity\Producto;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    public function createAction()
    {
        $producto = new Producto();
        $producto->setNombre('A Foo Bar');
        $producto->setPrecio('19.99');
        $producto->setDescription('Lorem ipsum dolor');

        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($producto);
        $em->flush();

        return new Response('Id de producto '.$producto->getId().' creado.');
    }

.. note::

    Si estás siguiendo este ejemplo, tendrás que crear una ruta que apunte a esta acción para verla trabajar.

Vamos a recorrer este ejemplo:

* **líneas 8-11** En esta sección, creas una instancia y trabajas con el objeto ``$producto`` como con cualquier otro objeto PHP normal;

* **Línea 13** Esta línea recupera un objeto *gestor de entidad* de Doctrine, el cual es responsable de manejar el proceso de persistir y recuperar objetos hacia y desde la base de datos;

* **Línea 14** El método ``persist()`` dice a Doctrine que "maneje" el objeto ``$producto``. Esto en realidad no provoca una consulta que se deba introducir en la base de datos (todavía).

* **Línea 15** Cuando se llama al método ``flush()``, Doctrine examina todos los objetos que está gestionando para ver si necesitan persistirse en la base de datos. En este ejemplo, el objeto ``$producto`` aún no se ha persistido, por lo tanto el gestor de la entidad ejecuta una consulta ``INSERT`` y crea una fila en la tabla ``Producto``.

.. note::

  De hecho, ya que Doctrine es consciente de todas tus entidades gestionadas, cuando se llama al método ``flush()``, calcula el conjunto de cambios y ejecuta la(s) consulta(s) más eficiente(s) posible(s). Por ejemplo, si estás persistiendo 100 objetos ``producto`` y luego llamas a ``persist()``, Doctrine creará una *única* instrucción y la volverá a utilizar para cada inserción. Este patrón se conoce como *Unidad de trabajo*, y se usa porque es rápido y eficiente.

Al crear o actualizar objetos, el flujo de trabajo siempre es el mismo. En la siguiente sección, verás cómo Doctrine es lo suficientemente inteligente como para emitir automáticamente una consulta ``UPDATE`` si ya existe el registro en la base de datos.

.. tip::

    Doctrine proporciona una biblioteca que te permite cargar mediante programación los datos de prueba en tu proyecto (es decir, "accesorios"). Para más información, consulta :doc:`/cookbook/doctrine/doctrine_fixtures`.

Recuperando objetos desde la base de datos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Recuperar un objeto desde la base de datos es aún más fácil. Por ejemplo, supongamos que haz configurado una ruta para mostrar un ``Producto`` específico en función del valor de su ``identificador``::

    public function showAction($id)
    {
        $producto = $this->getDoctrine()
            ->getRepository('AcmeTiendaBundle:Producto')
            ->find($id);

        if (!$producto) {
            throw $this->createNotFoundException('Ningún producto encontrado con id '.$id);
        }

        // Hace algo, como pasar el objeto $producto a una plantilla
    }

Al consultar por un determinado tipo de objeto, siempre utilizas lo que se conoce como "repositorio". Puedes pensar en un repositorio como una clase PHP, cuyo único trabajo consiste en ayudarte a buscar las entidades de una determinada clase. Puedes acceder al objeto repositorio de una clase de entidad a través de::

    $repositorio = $this->getDoctrine()
        ->getRepository('AcmeTiendaBundle:Producto');

.. note::

    La cadena ``AcmeTiendaBundle:Producto`` es un método abreviado que puedes utilizar en cualquier lugar de Doctrine en lugar del nombre de clase completo de la entidad (es decir, ``Acme\TiendaBundle\Entity\Producto``).
    Mientras que tu entidad viva bajo el espacio de nombres ``Entity`` de tu paquete, esto debe funcionar.

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

    // pregunta por todos los productos en que coincide el nombre, ordenados por precio
    $producto = $repositorio->findBy(
        array('nombre' => 'foo'),
        array('precio', 'ASC')
    );

.. tip::

    Cuando reproduces una página, puedes ver, en la esquina inferior derecha de la barra de herramientas de depuración web, cuántas consultas se realizaron.

    .. image:: /images/book/doctrine_web_debug_toolbar.png
       :align: center
       :scale: 25
       :width: 350

    Si haces clic en el icono, se abrirá el generador de perfiles, mostrando las consultas exactas que se hicieron.

Actualizando un objeto
~~~~~~~~~~~~~~~~~~~~~~

Una vez que hayas extraído un objeto de Doctrine, actualizarlo es relativamente fácil. Supongamos que tienes una ruta que asigna un identificador de producto a una acción de actualización de un controlador::

    public function updateAction($id)
    {
        $em = $this->getDoctrine()->getEntityManager();
        $producto = $em->getRepository('AcmeTiendaBundle:Producto')->find($id);

        if (!$producto) {
            throw $this->createNotFoundException('Ningún producto encontrado con id '.$id);
        }

        $producto->setNombre('¡Nuevo nombre de producto!');
        $em->flush();

        return $this->redirect($this->generateUrl('portada'));
    }

La actualización de un objeto únicamente consiste en tres pasos:

1. Recuperar el objeto desde Doctrine;
2. Modificar el objeto;
3. Invocar a ``flush()`` en el gestor de la entidad

Ten en cuenta que no es necesario llamar a ``$em->persist($producto)``. Recuerda que este método simplemente dice a Doctrine que maneje o "vea" el objeto ``$producto``.
En este caso, ya que recuperaste desde Doctrine el objeto ``$producto``, este ya está gestionado.

Eliminando un objeto
~~~~~~~~~~~~~~~~~~~~

Eliminar un objeto es muy similar, pero requiere una llamada al método ``remove()`` del gestor de la entidad::

    $em->remove($producto);
    $em->flush();

Como es de esperar, el método ``remove()`` notifica a Doctrine que deseas eliminar la entidad de la base de datos. La consulta ``DELETE`` real, sin embargo, no se ejecuta realmente hasta que se invoca al método ``flush()``.

.. _`book-doctrine-queries`:

Consultando objetos
-------------------

Ya haz visto cómo el objeto repositorio te permite ejecutar consultas básicas sin ningún trabajo::

    $repositorio->find($id);

    $repositorio->findOneByName('Foo');

Por supuesto, Doctrine también te permite escribir consultas más complejas utilizando el lenguaje de consulta Doctrine (DQL por Doctrine Query Language). DQL es similar a SQL, excepto que debes imaginar que estás consultando por uno o más objetos de una clase entidad (por ejemplo, ``Producto``) en lugar de consultar por filas de una tabla (por ejemplo, ``productos``).

Al consultar en Doctrine, tienes dos opciones: escribir consultas Doctrine puras o utilizar el generador de consultas de Doctrine.

Consultando objetos con DQL
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Imagina que deseas consultar los productos, pero sólo quieres devolver los productos que cuestan más de ``19.99``, ordenados del más barato al más caro. Desde el interior de un controlador, haz lo siguiente::

    $em = $this->getDoctrine()->getEntityManager();
    $consulta = $em->createQuery(
        'SELECT p FROM AcmeTiendaBundle:Producto p WHERE p.precio > :precio ORDER BY p.precio ASC'
    )->setParameter('precio', '19.99');

    $productos = $consulta->getResult();

Si te sientes cómodo con SQL, entonces debes sentir a DQL muy natural. La mayor diferencia es que necesitas pensar en términos de "objetos" en lugar de filas de una base de datos. Por esta razón, seleccionas *from* ``AcmeTiendaBundle:Producto`` y luego lo apodas ``p``.

El método ``getResult()`` devuelve una matriz de resultados. Si estás preguntando por un solo objeto, en su lugar puedes utilizar el método ``getSingleResult()``::

    $producto = $consulta->getSingleResult();

.. caution::

    El método ``getSingleResult()`` lanza una excepción ``Doctrine\ORM\NoResultException`` si no se devuelven resultados y una ``Doctrine\ORM\NonUniqueResultException`` si se devuelve *más* de un resultado. Si utilizas este método, posiblemente tengas que envolverlo en un bloque try-catch y asegurarte de que sólo se devuelve uno de los resultados (si estás consultando sobre algo que sea viable podrías regresar más de un resultado)::

        $consulta = $em->createQuery('SELECT ....')
            ->setMaxResults(1);

        try {
            $producto = $consulta->getSingleResult();
        } catch (\Doctrine\Orm\NoResultException $e) {
            $producto = null;
        }
        // ...

La sintaxis DQL es increíblemente poderosa, permitiéndote fácilmente unir entidades (el tema de las :ref:`relaciones <book-doctrine-relations>` se describe más adelante), agrupar, etc. Para más información, consulta la documentación oficial de `Doctrine Query Language`_.

.. sidebar:: Configurando parámetros

    Toma nota del método ``setParameter()``. Cuando trabajes con Doctrine, siempre es buena idea establecer los valores externos como "marcadores de posición", cómo se hizo en la consulta anterior:

    .. code-block:: text

        ... WHERE p.precio > :precio ...

    Entonces, puedes establecer el valor del marcador de posición ``precio`` llamando al método ``setParameter()``::

        ->setParameter('precio', '19.99')

    Utilizar parámetros en lugar de colocar los valores directamente en la cadena de consulta, se hace para prevenir ataques de inyección SQL y *siempre* se debe hacer.
    Si estás utilizando varios parámetros, puedes establecer simultáneamente sus valores usando el método ``setParameters()``::

        ->setParameters(array(
            'precio' => '19.99',
            'nombre'  => 'Foo',
        ))

Usando el generador de consultas de Doctrine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En lugar de escribir las consultas directamente, también puedes usar el ``QueryBuilder`` de Doctrine para hacer el mismo trabajo con una agradable interfaz orientada a objetos.
Si usas un IDE, también puedes tomar ventaja del autocompletado a medida que escribes los nombres de método. Desde el interior de un controlador::

    $repositorio = $this->getDoctrine()
        ->getRepository('AcmeTiendaBundle:Producto');

    $consulta = $repositorio->createQueryBuilder('p')
        ->where('p.precio > :precio')
        ->setParameter('precio', '19.99')
        ->orderBy('p.precio', 'ASC')
        ->getQuery();
    
    $productos = $consulta->getResult();

El objeto ``QueryBuilder`` contiene todos los métodos necesarios para construir tu consulta. Al invocar al método ``getQuery()``, el generador de consultas devuelve un objeto ``Query`` normal, el cual es el mismo objeto que construiste directamente en la sección anterior.

Para más información sobre el generador de consultas de Doctrine, consulta la documentación del `Generador de consultas`_ de Doctrine.

Repositorio de clases personalizado
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En las secciones anteriores, comenzamos a construir y utilizar consultas más complejas desde el interior de un controlador. Con el fin de aislar, probar y volver a usar estas consultas, es buena idea crear una clase repositorio personalizada para tu entidad y agregar métodos con tu lógica de consulta allí.

Para ello, agrega el nombre de la clase del repositorio a la definición de asignación.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/TiendaBundle/Entity/Producto.php
        namespace Acme\TiendaBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity(repositoryClass="Acme\TiendaBundle\Repository\ProductoRepository")
         */
        class Producto
        {
            //...
        }

    .. code-block:: yaml

        # src/Acme/TiendaBundle/Resources/config/doctrine/Producto.orm.yml
        Acme\TiendaBundle\Entity\Producto:
            type: entity
            repositoryClass: Acme\TiendaBundle\Repository\ProductoRepository
            # ...

    .. code-block:: xml

        <!-- src/Acme/TiendaBundle/Resources/config/doctrine/Producto.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\TiendaBundle\Entity\Producto"
                    repository-class="Acme\TiendaBundle\Repository\ProductoRepository">
                    <!-- ... -->
            </entity>
        </doctrine-mapping>

Doctrine puede generar la clase repositorio por ti ejecutando la misma orden usada anteriormente para generar los métodos captadores y definidores omitidos:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

A continuación, agrega un nuevo método - ``findAllOrderedByName()`` - a la clase repositorio recién generada. Este método debe consultar por todas las entidades ``Producto``, ordenadas alfabéticamente.

.. code-block:: php

    // src/Acme/TiendaBundle/Repository/ProductoRepository.php
    namespace Acme\TiendaBundle\Repository;

    use Doctrine\ORM\EntityRepository;

    class ProductoRepository extends EntityRepository
    {
        public function findAllOrderedByName()
        {
            return $this->getEntityManager()
                ->createQuery('SELECT p FROM AcmeTiendaBundle:Producto p ORDER BY p.nombre ASC')
                ->getResult();
        }
    }

.. tip::

    Puedes acceder al gestor de la entidad a través de ``$this->getEntityManager()`` desde el interior del repositorio.

Puedes utilizar este nuevo método al igual que los métodos de búsqueda predeterminados del repositorio::

    $em = $this->getDoctrine()->getEntityManager();
    $productos = $em->getRepository('AcmeTiendaBundle:Producto')
                ->findAllOrderedByName();

.. note::

    Al utilizar una clase repositorio personalizada, todavía tienes acceso a los métodos de búsqueda predeterminados como ``find()`` y ``findAll()``.

.. _`book-doctrine-relations`:

Entidad relaciones/asociaciones
-------------------------------

Supongamos que los productos en tu aplicación pertenecen exactamente a una "categoría".
En este caso, necesitarás un objeto ``Categoría`` y una manera de relacionar un objeto ``Producto`` a un objeto ``Categoría``. Empieza por crear la entidad ``Categoría``.
Ya sabemos que tarde o temprano tendrás que persistir la clase a través de Doctrine, puedes dejar que Doctrine cree la clase para ti:

.. code-block:: bash

    php app/console doctrine:generate:entity AcmeTiendaBundle:Categoria "nombre:string(255)" --mapping-type=yml

Esta tarea genera la entidad ``Categoría`` para ti, con un campo ``id``, un campo ``Nombre`` y las funciones captadoras y definidoras asociadas.

Relación con la asignación de metadatos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Para relacionar las entidades ``Categoría`` y ``Producto``, empieza por crear una propiedad ``productos`` en la clase ``Categoría``::

    // src/Acme/TiendaBundle/Entity/Categoria.php
    // ...
    use Doctrine\Common\Collections\ArrayCollection;
    
    class Categoria
    {
        // ...

        /**
         * @ORM\OneToMany(targetEntity="Producto", mappedBy="categoria")
         */
        protected $productos;

        public function __construct()
        {
            $this->productos = new ArrayCollection();
        }
    }

En primer lugar, ya que un objeto ``Categoría`` debe relacionar muchos objetos ``Producto``, agregamos un arreglo como propiedad ``Productos`` para contener los objetos ``Producto``.
Una vez más, esto no se hace porque lo necesite Doctrine, sino porque tiene sentido en la aplicación para que cada ``Categoría`` mantenga una gran variedad de objetos ``Producto``.

.. note::

    El código de el método ``__construct()`` es importante porque Doctrine requiere que la propiedad ``$productos`` sea un objeto ``ArrayCollection``.
    Este objeto se ve y actúa casi *exactamente* como una matriz, pero tiene cierta flexibilidad. Si esto te hace sentir incómodo, no te preocupes. Sólo imagina que es una ``matriz`` y estarás bien.

A continuación, ya que cada clase ``Producto`` se puede referir exactamente a un objeto ``Categoría``, podrías desear agregar una propiedad ``$categoria`` a la clase ``Producto``::

    // src/Acme/TiendaBundle/Entity/Producto.php
    // ...

    class Producto
    {
        // ...

        /**
         * @ORM\ManyToOne(targetEntity="Categoria", inversedBy="productos")
         * @ORM\JoinColumn(name="categoria_id", referencedColumnName="id")
         */
        protected $categoria;
    }

Por último, ahora que hemos agregado una nueva propiedad a ambas clases ``Categoría`` y ``Producto``, le informamos a Doctrine que genere por ti los métodos captadores y definidores omitidos:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

No hagas caso de los metadatos de Doctrine por un momento. Ahora tienes dos clases - ``Categoría`` y ``Producto`` con una relación natural de uno a muchos. La clase ``Categoría`` tiene una matriz de objetos ``Producto`` y el objeto ``producto`` puede contener un objeto ``Categoría``. En otras palabras - hemos construido tus clases de una manera que tiene sentido para tus necesidades. El hecho de que los datos se tienen que persistir en una base de datos, siempre es secundario.

Ahora, veamos los metadatos sobre la propiedad ``$categoria`` en la clase ``Producto``. Esta información le dice a Doctrine que la clase está relacionada con ``Categoría`` y que debe guardar el ``id`` del registro de la categoría en un campo ``categoria_id`` que vive en la tabla ``producto``. En otras palabras, el objeto ``Categoría`` relacionado se almacenará en la propiedad ``$categoria``, pero tras bambalinas, Doctrine deberá persistir esta relación almacenando el valor del ``id`` de la categoría en una columna  ``categoria_id`` de la tabla ``producto``.

.. image:: /images/book/doctrine_image_2.png
   :align: center

Los metadatos sobre la propiedad ``$productos`` del objeto ``Categoría`` son menos importantes, y simplemente dicen a Doctrine que vea la propiedad ``Producto.categoria`` para averiguar cómo se asigna la relación.

Antes de continuar, asegúrate de decirle a Doctrine que agregue la nueva tabla ``Categoría``, la columna ``producto.categoria_id`` y la nueva clave externa:

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. note::

    Esta tarea sólo la deberías utilizar durante el desarrollo. Para un método más robusto de actualización sistemática para tu base de datos de producción, lee sobre las :doc:`migraciones Doctrine </cookbook/doctrine/migrations>`.

Guardando entidades relacionadas
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ahora, vamos a ver el código en acción. Imagina que estás dentro de un controlador::

    // ...
    use Acme\TiendaBundle\Entity\Categoria;
    use Acme\TiendaBundle\Entity\Producto;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    class DefaultController extends Controller
    {
        public function creaProductoAction()
        {
            $categoria = new Categoria();
            $categoria->setNombre('Productos principales');

            $producto = new Producto();
            $producto->setNombre('Foo');
            $producto->setPrecio(19.99);
            // relaciona este producto con la categoría
            $producto->setCategoria($categoria);

            $em = $this->getDoctrine()->getEntityManager();
            $em->persist($categoria);
            $em->persist($producto);
            $em->flush();

            return new Response(
                'Producto con id: '.$producto->getId().' e id de categoría: '.$categoria->getId().' creado.'
            );
        }
    }

Ahora, se agrega una sola fila a las tablas ``categoría`` y ``producto``.
La columna ``producto.categoria_id`` para el nuevo producto se ajusta a algún ``id`` de la nueva categoría. Doctrine gestiona la persistencia de esta relación para ti.

Recuperando objetos relacionados
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cuando necesites recuperar objetos asociados, tu flujo de trabajo se ve justo como lo hacías antes. En primer lugar, buscas un objeto ``$producto`` y luego accedes a su ``Categoría`` asociada::

    public function showAction($id)
    {
        $producto = $this->getDoctrine()
            ->getRepository('AcmeTiendaBundle:Producto')
            ->find($id);

        $nombreCategoria = $producto->getCategoria()->getNombre();

        // ...
    }

En este ejemplo, primero consultas por un objeto ``Producto`` basándote en el ``id`` del producto. Este emite una consulta *solo* para los datos del producto e hidrata al objeto ``$producto`` con esos datos. Más tarde, cuando llames a ``$producto->getCategoria()->getNombre()``, Doctrine silenciosamente hace una segunda consulta para encontrar la ``Categoría`` que está relacionada con este ``Producto``. Entonces, prepara el objeto ``$categoria`` y te lo devuelve.

.. image:: /images/book/doctrine_image_3.png
   :align: center

Lo importante es el hecho de que tienes fácil acceso a la categoría relacionada con el producto, pero, los datos de la categoría realmente no se recuperan hasta que pides la categoría (es decir, trata de "cargarlos de manera diferida").

También puedes consultar en la dirección contraria::

    public function showProductoAction($id)
    {
        $categoria = $this->getDoctrine()
            ->getRepository('AcmeTiendaBundle:Categoria')
            ->find($id);

        $productos = $categoria->getProductos();

        // ...
    }

En este caso, ocurre lo mismo: primero consultas por un único objeto ``Categoría``, y luego Doctrine hace una segunda consulta para recuperar los objetos ``Producto`` relacionados, pero sólo una vez/si le preguntas por ellos (es decir, cuando invoques a ``->getProductos()``).
La variable ``$productos`` es una matriz de todos los objetos ``Producto`` relacionados con el objeto ``Categoría`` propuesto a través de sus valores ``categoria_id``.

.. sidebar:: Relaciones y clases sustitutas

    Esta "carga diferida" es posible porque, cuando sea necesario, Doctrine devuelve un objeto "sustituto" en lugar del verdadero objeto. Veamos de nuevo el ejemplo anterior::

        $producto = $this->getDoctrine()
            ->getRepository('AcmeTiendaBundle:Producto')
            ->find($id);

        $categoria = $producto->getCategoria();

        // prints "Proxies\AcmeTiendaBundleEntityCategoriaProxy"
        echo get_class($categoria);

    Este objeto sustituto extiende al verdadero objeto ``Categoría``, y se ve y actúa exactamente igual que él. La diferencia es que, al usar un objeto sustituto, Doctrine puede retrasar la consulta de los datos reales de la ``Categoría`` hasta que realmente se necesitan esos datos (por ejemplo, hasta que se invoque a ``$categoria->getNombre()``).

    Las clases sustitutas las genera Doctrine y se almacenan en el directorio cache.
    Y aunque probablemente nunca te des cuenta de que tu objeto ``$categoria`` en realidad es un objeto sustituto, es importante tenerlo en cuenta.

    En la siguiente sección, al recuperar simultáneamente los datos del producto y la categoría (a través de una *unión*), Doctrine devolverá el *verdadero* objeto ``Categoría``, puesto que nada se tiene que cargar de forma diferida.

Uniendo registros relacionados
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En los ejemplos anteriores, se realizaron dos consultas - una para el objeto original (por ejemplo, una ``Categoría``) y otra para el/los objetos relacionados (por ejemplo, los objetos ``Producto``).

.. tip::

    Recuerda que puedes ver todas las consultas realizadas durante una petición a través de la barra de herramientas de depuración web.

Por supuesto, si sabes por adelantado que necesitas tener acceso a los objetos, puedes evitar la segunda consulta emitiendo una unión en la consulta original. Agrega el siguiente método a la clase ``ProductoRepository``::

    // src/Acme/TiendaBundle/Repository/ProductoRepository.php
    
    public function findOneByIdJoinedToCategory($id)
    {
        $consulta = $this->getEntityManager()
            ->createQuery('
                SELECT p, c FROM AcmeTiendaBundle:Producto p
                JOIN p.categoria c
                WHERE p.id = :id'
            )->setParameter('id', $id);

        try {
            return $consulta->getSingleResult();
        } catch (\Doctrine\ORM\NoResultException $e) {
            return null;
        }
    }

Ahora, puedes utilizar este método en el controlador para consultar un objeto ``Producto`` y su correspondiente ``Categoría`` con una sola consulta::

    public function showAction($id)
    {
        $producto = $this->getDoctrine()
            ->getRepository('AcmeTiendaBundle:Producto')
            ->findOneByIdJoinedToCategory($id);

        $categoria = $producto->getCategoria();

        // ...
    }    

Más información sobre asociaciones
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Esta sección ha sido una introducción a un tipo común de relación entre entidades, la relación uno a muchos. Para obtener detalles más avanzados y ejemplos de cómo utilizar otros tipos de relaciones (por ejemplo, ``uno a uno``, ``muchos a muchos``), consulta la sección `Asignando asociaciones`_ de la documentación de Doctrine.

.. note::

    Si estás utilizando anotaciones, tendrás que prefijar todas las anotaciones con ``ORM\`` (por ejemplo, ``ORM\UnoAMuchos``), lo cual no se refleja en la documentación de Doctrine. También tendrás que incluir la declaración ``use Doctrine\ORM\Mapping as ORM;``, la cual *importa* las anotaciones prefijas ``ORM``.

Configurando
------------

Doctrine es altamente configurable, aunque probablemente nunca tendrás que preocuparte de la mayor parte de sus opciones. Para más información sobre la configuración de Doctrine, consulta la sección Doctrine del :doc:`Manual de referencia </reference/configuration/doctrine>`.

Ciclo de vida de las retrollamadas
----------------------------------

A veces, es necesario realizar una acción justo antes o después de insertar, actualizar o eliminar una entidad. Este tipo de acciones se conoce como "ciclo de vida" de las retrollamadas, ya que son métodos retrollamados que necesitas ejecutar durante las diferentes etapas del ciclo de vida de una entidad (por ejemplo, cuando la entidad es insertada, actualizada, eliminada, etc.)

Si estás utilizando anotaciones para los metadatos, empieza por permitir el ciclo de vida de las retrollamadas. Esto no es necesario si estás usando YAML o XML para tu asignación:

.. code-block:: php-annotations

    /**
     * @ORM\Entity()
     * @ORM\HasLifecycleCallbacks()
     */
    class Producto
    {
        // ...
    }

Ahora, puedes decir a Doctrine que ejecute un método en cualquiera de los eventos del ciclo de vida disponibles. Por ejemplo, supongamos que deseas establecer una columna de fecha ``creado`` a la fecha actual, sólo cuando se persiste por primera vez la entidad (es decir, se inserta):

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * @ORM\prePersist
         */
        public function setValorCreado()
        {
            $this->creado = new \DateTime();
        }

    .. code-block:: yaml

        # src/Acme/TiendaBundle/Resources/config/doctrine/Producto.orm.yml
        Acme\TiendaBundle\Entity\Producto:
            type: entity
            # ...
            lifecycleCallbacks:
                prePersist: [ setValorCreado ]

    .. code-block:: xml

        <!-- src/Acme/TiendaBundle/Resources/config/doctrine/Producto.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\TiendaBundle\Entity\Producto">
                    <!-- ... -->
                    <lifecycle-callbacks>
                        <lifecycle-callback type="prePersist" method="setValorCreado" />
                    </lifecycle-callbacks>
            </entity>
        </doctrine-mapping>

.. note::

    En el ejemplo anterior se supone que haz creado y asignado una propiedad ``creado`` (no mostrada aquí).

Ahora, justo antes de persistir la primer entidad, Doctrine automáticamente llamará a este método y establecerá el campo ``creado`` a la fecha actual.

Esto se puede repetir en cualquiera de los otros eventos del ciclo de vida, los cuales incluyen a:

* ``preRemove``
* ``postRemove``
* ``prePersist``
* ``postPersist``
* ``preUpdate``
* ``postUpdate``
* ``postLoad``
* ``loadClassMetadata``

Para más información sobre que significan estos eventos del ciclo de vida de las retrollamadas en general, consulta la sección `Eventos del ciclo de vida`_ en la documentación de Doctrine.

.. sidebar:: Ciclo de vida de retrollamada y escuchas de eventos

    Observa que el método ``setValorCreado()`` no recibe argumentos. Este siempre es el caso del ciclo de vida de las retrollamadas y es intencional: el ciclo de vida de las retrollamadas debe ser un método sencillo que se ocupe de transformar los datos internos de la entidad (por ejemplo, estableciendo un campo a creado/actualizado, generar un valor ficticio).
    
    Si necesitas hacer alguna tarea más pesada - como llevar el registro de eventos o enviar un correo electrónico - debes registrar una clase externa como un escucha o suscriptor de eventos y darle acceso a todos los recursos que necesites. Para más información, consulta :doc:`/cookbook/doctrine/event_listeners_subscribers`.

Extensiones Doctrine: ``Timestampable``, ``Sluggable``, etc.
------------------------------------------------------------

Doctrine es bastante flexible, y dispone de una serie de extensiones de terceros que te permiten realizar fácilmente tareas repetitivas y comunes en tus entidades.
Estas incluyen cosas tales como ``Sluggable``, ``Timestampable``, ``registrable``, ``traducible`` y ``Tree``.

Para más información sobre cómo encontrar y utilizar estas extensiones, ve el artículo sobre el uso de :doc:`extensiones comunes Doctrine </cookbook/doctrine/common_extensions>`.

.. _book-doctrine-field-types:

Referencia de tipos de campo Doctrine
-------------------------------------

Doctrine dispone de una gran cantidad de tipos de campo. Cada uno de estos asigna un tipo de dato PHP a un tipo de columna específica en cualquier base de datos que estés utilizando. Los siguientes tipos son compatibles con Doctrine:

* **Cadenas**

  * ``string`` (usado para cadenas cortas)
  * ``text`` (usado para cadenas grandes)

* **Números**

  * ``integer``
  * ``smallint``
  * ``bigint``
  * ``decimal``
  * ``float``

* **Fechas y horas** (usa un objeto `DateTime`_ para estos campos en PHP)

  * ``date``
  * ``time``
  * ``datetime``

* **Otros tipos**

  * ``booleanos``
  * ``object`` (serializado y almacenado en un campo ``CLOB``)
  * ``array`` (serializado y almacenado en un campo ``CLOB``)

Para más información, consulta la sección `Asignando tipos`_ en la documentación de Doctrine.

Opciones de campo
~~~~~~~~~~~~~~~~~

Cada campo puede tener un conjunto de opciones aplicables. Las opciones disponibles incluyen ``type`` (el predeterminado es ``string``), ``name``, ``lenght``, ``unique`` y ``nullable``. Aquí tienes unos cuantos ejemplos de anotaciones:

.. code-block:: php-annotations

    /**
     * Un campo cadena con longitud de 255 que no puede ser nulo
     * (reflejando los valores predeterminados para las opciones "tipo", "longitud" y "nulo")
     * 
     * @ORM\Column()
     */
    protected $nombre;

    /**
     * Un campo cadena de longitud 150 que persiste a una columna
     * "direccion_correo" y tiene un índice único.
     *
     * @ORM\Column(name="direccion_correo", unique="true", length="150")
     */
    protected $correo;

.. note::

    Hay algunas opciones más que no figuran en esta lista. Para más detalles, consulta la sección `Asignando propiedades`_ de la documentación de Doctrine.

.. index::
   single: Doctrine; Ordenes de consola ORM
   single: CLI; ORM de Doctrine

Ordenes de consola
------------------

La integración del ORM de Doctrine2 ofrece varias ordenes de consola bajo el espacio de nombres ``doctrine``. Para ver la lista de ordenes puedes ejecutar la consola sin ningún tipo de argumento:

.. code-block:: bash

    php app/console

Mostrará una lista de ordenes disponibles, muchas de las cuales comienzan con el prefijo ``doctrine:``. Puedes encontrar más información sobre cualquiera de estas ordenes (o cualquier orden de Symfony) ejecutando la orden ``help``. Por ejemplo, para obtener detalles acerca de la tarea ``doctrine:database:create``, ejecuta:

.. code-block:: bash

    php app/console help doctrine:database:create

Algunas tareas notables o interesantes son:

* ``doctrine:ensure-production-settings`` - comprueba si el entorno actual está configurado de manera eficiente para producción. Esta siempre se debe ejecutar en el entorno ``prod``:

  .. code-block:: bash

    php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - permite a Doctrine llevar a cabo una introspección a una base de datos existente y crear información de asignación. Para más información, consulta :doc:`/cookbook/doctrine/reverse_engineering`.

* ``doctrine:mapping:info`` - te dice todas las entidades de las que Doctrine es consciente y si hay algún error básico con la asignación.

* ``doctrine:query:dql`` y ``doctrine:query:sql`` - te permiten ejecutar DQL o consultas SQL directamente desde la línea de ordenes.

.. note::

   Para poder cargar accesorios a tu base de datos, necesitas tener instalado el paquete ``DoctrineFixturesBundle``. Para saber cómo hacerlo, lee el artículo ":doc:`/cookbook/doctrine/doctrine_fixtures`" del recetario.

Resumen
-------

Con Doctrine, puedes centrarte en tus objetos y la forma en que son útiles en tu aplicación y luego preocuparte por su persistencia en la base de datos. Esto se debe a que Doctrine te permite utilizar cualquier objeto PHP para almacenar los datos y se basa en la información de asignación de metadatos para asignar los datos de un objeto a una tabla particular de la base de datos.

Y aunque Doctrine gira en torno a un concepto simple, es increíblemente poderosa, lo cual te permite crear consultas complejas y suscribirte a los eventos que te permiten realizar diferentes acciones conforme los objetos recorren su ciclo de vida en la persistencia.

Para más información acerca de Doctrine, ve la sección *Doctrine* del :doc:`recetario </cookbook/index>`, que incluye los siguientes artículos:

* :doc:`/cookbook/doctrine/doctrine_fixtures`
* :doc:`/cookbook/doctrine/migrations`
* :doc:`/cookbook/doctrine/mongodb`
* :doc:`/cookbook/doctrine/common_extensions`

.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Documentación de asignación básica`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html
.. _`Generador de consultas`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/query-builder.html
.. _`Doctrine Query Language`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/dql-doctrine-query-language.html
.. _`Asignando asociaciones`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/es/class.datetime.php
.. _`Asignando tipos`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#doctrine-mapping-types
.. _`Asignando propiedades`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#property-mapping
.. _`Eventos del ciclo de vida`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/events.html#lifecycle-events
