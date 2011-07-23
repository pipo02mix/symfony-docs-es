.. index::
   single: Pruebas

Probando
========

Cada vez que escribes una nueva línea de código, potencialmente, también, añades nuevos errores.
Las pruebas automatizadas deben tener todo cubierto y esta guía muestra cómo escribir pruebas unitarias y funcionales para tu aplicación Symfony2.

Plataforma de pruebas
---------------------

En Symfony2 las pruebas dependen en gran medida de PHPUnit, tus buenas prácticas, y algunos convenios. Esta parte no documenta PHPUnit en sí mismo, pero si aún no lo conoces, puedes leer su excelente `documentación`_.

.. note::

    Symfony2 trabaja con PHPUnit 3.5.11 o posterior.

La configuración predeterminada de PHPUnit busca pruebas bajo el subdirectorio ``Tests/`` de tus paquetes:

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->

    <phpunit bootstrap="../src/autoload.php">
        <testsuites>
            <testsuite name="Proyecto banco de pruebas">
                <directory>../src/*/*Bundle/Tests</directory>
            </testsuite>
        </testsuites>

        ...
    </phpunit>

La ejecución del banco de pruebas para una determinada aplicación es muy sencilla:

.. code-block:: bash

    # especifica la configuración del directorio en la línea de ordenes
    $ phpunit -c app/

    # o ejecuta phpunit desde el directorio de la aplicación
    $ cd app/
    $ phpunit

.. tip::

    La cobertura de código se puede generar con la opción ``--coverage-html``.

.. index::
   single: Pruebas; Pruebas unitarias

Pruebas unitarias
-----------------

Escribir pruebas unitarias en Symfony2 no es diferente a escribir pruebas unitarias PHPUnit normales. Por convención, recomendamos replicar la estructura de directorios de tu paquete bajo el subdirectorio ``Tests/``. Por lo tanto, para escribir las pruebas para la clase ```Acme\HolaBundle\Model\Articulo`` en el archivo ``Acme/HolaBundle/Tests/Model/ArticuloTest.php``.

En una prueba unitaria, el autocargado se activa automáticamente a través del archivo ``src/autoload.php`` (tal como está configurado por omisión en el archivo ``phpunit.xml.dist``).

Correr las pruebas de un determinado archivo o directorio también es muy fácil:

.. code-block:: bash

    # corre todas las pruebas del controlador
    $ phpunit -c app src/Acme/HolaBundle/Tests/Controller/

    # ejecuta todas las pruebas del modelo
    $ phpunit -c app src/Acme/HolaBundle/Tests/Model/

    # ejecuta las pruebas para la clase Articulo
    $ phpunit -c app src/Acme/HolaBundle/Tests/Model/ArticuloTest.php

    # ejecuta todas las pruebas del paquete entero
    $ phpunit -c app src/Acme/HolaBundle/

.. index::
   single: Pruebas; Pruebas funcionales

Pruebas funcionales
-------------------

Las pruebas funcionales verifican la integración de las diferentes capas de una aplicación (desde el enrutado hasta la vista). Ellas no son diferentes de las pruebas unitarias en cuanto a PHPUnit se refiere, pero tienen un flujo de trabajo muy específico:

* Hacer una petición;
* Probar la respuesta;
* Hacer clic en un enlace o enviar un formulario;
* Probar la respuesta;
* Enjuagar y repetir.

Las peticiones, clics y los envíos los hace un cliente que sabe cómo hablar con la aplicación. Para acceder a este cliente, tus pruebas necesitan ampliar la clase ``WebTestCase`` de Symfony2. La Edición estándar de Symfony2 proporciona una sencilla prueba funcional para ``DemoController`` que dice lo siguiente::

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class DemoControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $cliente = static::createClient();

            $impulsor = $cliente->request('GET', '/demo/hola/Fabien');

            $this->assertTrue($impulsor->filter('html:contains("Hola Fabien")')->count() > 0);
        }
    }

El método ``createClient()`` devuelve un cliente vinculado a la aplicación actual::

    $impulsor = $cliente->request('GET', 'hola/Fabien');

El método ``request()`` devuelve un objeto ``Impulsor`` que puedes utilizar para seleccionar elementos en la respuesta, hacer clic en enlaces, y enviar formularios.

.. tip::

    El ``Impulsor`` sólo se puede utilizar si el contenido de la respuesta es un documento XML o HTML. Para otro tipo de contenido, consigue el contenido de la respuesta con ``$cliente->getResponse()->getContent()``.

Haz clic en un enlace seleccionándolo primero con el ``Impulsor`` utilizando una expresión XPath o un selector CSS, luego utiliza el cliente para hacer clic en él::

    $enlace = $impulsor->filter('a:contains("Bienvenido")')->eq(1)->link();

    $impulsor = $cliente->click($enlace);

El envío de un formulario es muy similar; selecciona un botón del formulario, opcionalmente sustituye algunos valores del formulario, y ​​envía el formulario correspondiente::

    $formulario = $impulsor->selectButton('submit')->form();

    // sustituye algunos valores
    $formulario['nombre'] = 'Lucas';

    // envía el formulario
    $impulsor = $cliente->submit($formulario);

Cada campo del ``formulario`` tiene métodos especializados en función de su tipo::

    // llena un campo de texto (input)
    $formulario['nombre'] = 'Lucas';

    // selecciona una opción o un botón de radio
    $formulario['country']->select('Francia');

    // marca una casilla de verificación (checkbox)
    $formulario['like_symfony']->tick();

    // carga un archivo
    $formulario['photo']->upload('/ruta/a/lucas.jpg');

En lugar de cambiar un campo a la vez, también puedes pasar una matriz de valores al método ``submit()``::

    $impulsor = $cliente->submit($formulario, array(
        'nombre'       => 'Lucas',
        'country'      => 'Francia',
        'like_symfony' => true,
        'photo'        => '/ruta/a/lucas.jpg',
    ));

Ahora que puedes navegar fácilmente a través de una aplicación, utiliza las aserciones para probar que en realidad hace lo que se espera. Utiliza el ``Impulsor`` para hacer aserciones sobre el DOM::

    // Afirma que la respuesta concuerda con un determinado selector CSS.
    $this->assertTrue($impulsor->filter('h1')->count() > 0);

O bien, prueba contra el contenido de la respuesta directamente si lo que deseas es acertar que el contenido contiene algún texto, o si la respuesta no es un documento XML/HTML::

    $this->assertRegExp('/Hola Fabien/', $cliente->getResponse()->getContent());

.. index::
   single: Pruebas; Aserciones

Útiles aserciones
~~~~~~~~~~~~~~~~~

Después de algún tiempo, te darás cuenta de que siempre escribes el mismo tipo de aserciones. Para empezar más rápido, aquí está una lista de las aserciones más comunes y útiles::

    // Afirma que la respuesta concuerda con un determinado selector CSS.
    $this->assertTrue($impulsor->filter($selector)->count() > 0);

    // Afirma que la respuesta concuerda n veces con un determinado selector CSS.
    $this->assertEquals($count, $impulsor->filter($selector)->count());

    // Afirma que la cabecera de la respuesta tiene un valor dado.
    $this->assertTrue($cliente->getResponse()->headers->contains($key, $value));

    // Afirma que el contenido de la respuesta concuerda con una expresión regular.
    $this->assertRegExp($regexp, $cliente->getResponse()->getContent());

    // Acierta el código de estado de la respuesta.
    $this->assertTrue($cliente->getResponse()->isSuccessful());
    $this->assertTrue($cliente->getResponse()->isNotFound());
    $this->assertEquals(200, $cliente->getResponse()->getStatusCode());

    // Afirma que el código de estado de la respuesta es una redirección.
    $this->assertTrue($cliente->getResponse()->isRedirected('google.com'));

.. _documentación: http://www.phpunit.de/manual/3.5/en/

.. index::
   single: Pruebas; Cliente

El Cliente de pruebas
---------------------

El Cliente de prueba simula un cliente HTTP tal como un navegador.

.. note::

    El Cliente de prueba está basado en los componentes ``BrowserKit`` e ``Impulsor``.

Haciendo peticiones
~~~~~~~~~~~~~~~~~~~

El cliente sabe cómo hacer peticiones a una aplicación Symfony2::

    $impulsor = $cliente->request('GET', '/hola/Fabien');

El método ``request()`` toma el método HTTP y una URL como argumentos y devuelve una instancia de ``Impulsor``.

Utiliza el rastreador Impulsor para encontrar los elementos del DOM en la respuesta. Puedes utilizar estos elementos para hacer clic en los enlaces y presentar formularios::

    $enlace = $impulsor->selectLink('Ve a algún lugar...')->link();
    $impulsor = $cliente->click($enlace);

    $formulario = $impulsor->selectButton('validate')->form();
    $impulsor = $cliente->submit($formulario, array('nombre' => 'Fabien'));

Ambos métodos ``click()`` y ``submit()`` devuelven un objeto ``Impulsor``.
Estos métodos son la mejor manera para navegar por una aplicación que esconde un montón de detalles. Por ejemplo, al enviar un formulario, este detecta automáticamente el método HTTP y la URL del formulario, te da una buena API para subir archivos, y combinar los valores presentados con el formulario predeterminado, y mucho más.

.. tip::

    Aprenderás más sobre los objetos ``Link`` y ``Form`` más adelante en la sección Impulsor.

Pero también puedes simular el envío de formularios y peticiones complejas con argumentos adicionales del método ``request()``::

    // envía el formulario
    $cliente->request('POST', '/submit', array('nombre' => 'Fabien'));

    // envía el formulario con una carga de archivo
    $cliente->request('POST', '/submit', array('nombre' => 'Fabien'), array('photo' => '/ruta/a/photo'));

    // especifica las cabeceras HTTP
    $cliente->request('DELETE', '/post/12', array(), array(), array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word'));

Cuando una petición devuelve una respuesta de redirección, el cliente la sigue automáticamente. Este comportamiento se puede cambiar con el método ``followRedirects()``::

    $cliente->followRedirects(false);

Cuando el cliente no sigue los cambios de dirección, puedes forzar el cambio de dirección con el método ``followRedirect()``::

    $impulsor = $cliente->followRedirect();

Por último pero no menos importante, puedes hacer que cada petición se ejecute en su propio proceso PHP para evitar efectos secundarios cuando se trabaja con varios clientes en el mismo archivo::

    $cliente->insulate();

Navegando
~~~~~~~~~

El cliente es compatible con muchas operaciones que se pueden hacer en un navegador real::

    $cliente->back();
    $cliente->forward();
    $cliente->reload();

    // Limpia todas las cookies y el historial
    $cliente->restart();

Accediendo a objetos internos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si utilizas el cliente para probar tu aplicación, posiblemente quieras acceder a los objetos internos del cliente::

    $history   = $cliente->getHistory();
    $cookieJar = $cliente->getCookieJar();

También puedes obtener los objetos relacionados con la última petición::

    $peticion  = $cliente->getRequest();
    $respuesta = $cliente->getResponse();
    $impulsor  = $cliente->getCrawler();

Si tus peticiones no son aisladas, también puedes acceder al ``Contenedor`` y al ``kernel``::

    $contenedor = $cliente->getContainer();
    $kernel    = $cliente->getKernel();

Accediendo al contenedor
~~~~~~~~~~~~~~~~~~~~~~~~

Es altamente recomendable que una prueba funcional sólo pruebe la respuesta. Sin embargo, bajo ciertas circunstancias muy raras, posiblemente desees acceder a algunos objetos internos para escribir aserciones. En tales casos, puedes acceder al contenedor de inyección de dependencias::

    $contenedor = $cliente->getContainer();

Ten en cuenta que esto no tiene efecto si aíslas el cliente o si utilizas una capa HTTP.

.. tip::

    Si la información que necesitas comprobar está disponible desde el generador de perfiles, úsalo en su lugar.

Accediendo a los datos del perfil
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Para afirmar los datos recogidos por el generador de perfiles, puedes conseguir el perfil de la petición actual de la siguiente manera::

    $perfil = $cliente->getProfile();

Redirigiendo
~~~~~~~~~~~~

De manera predeterminada, el cliente no sigue las redirecciones HTTP, por lo tanto puedes conseguir y analizar la respuesta antes de redirigir. Una vez que quieras redirigir al cliente, invoca al método ``followRedirect()``::

    // hace algo para emitir la redirección (por ejemplo, llenar un formulario)

    // sigue la redirección
    $impulsor = $cliente->followRedirect();

Si deseas que el cliente siempre sea redirigido automáticamente, puedes llamar al método ``followRedirects()``::

    $cliente->followRedirects();

    $impulsor = $cliente->request('GET', '/');

    // sigue todas las redirecciones

    // configura de nuevo al Cliente para redirigirlo manualmente
    $cliente->followRedirects(false);

.. index::
   single: Pruebas; El rastreador

El ``Impulsor``
---------------

Cada vez que hagas una petición con el cliente devolverá una instancia del ``Impulsor``.
Este nos permite recorrer documentos HTML, seleccionar nodos, encontrar enlaces y formularios.

Creando una instancia de ``Impulsor``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cuando haces una petición con un Cliente, automáticamente se crea una instancia del ``Impulsor``. Pero puedes crear el tuyo fácilmente::

    use Symfony\Component\DomCrawler\Crawler;

    $impulsor = new Crawler($html, $url);

El constructor toma dos argumentos: el segundo es la URL que se utiliza para generar las direcciones absolutas para los enlaces y formularios, el primero puede ser cualquiera de los siguientes:

* Un documento HTML;
* Un documento XML;
* Una instancia de ``DOMDocument``;
* Una instancia de ``DOMNodeList``;
* Una instancia de ``DOMNode``;
* Un arreglo de todos los elementos anteriores.

Después de crearla, puedes agregar más nodos:

+-----------------------+----------------------------------+
| Método                | Descripción                      |
+=======================+==================================+
| ``addHTMLDocument()`` | Un documento HTML                |
+-----------------------+----------------------------------+
| ``addXMLDocument()``  | Un documento XML                 |
+-----------------------+----------------------------------+
| ``addDOMDocument()``  | Una instancia de ``DOMDocument`` |
+-----------------------+----------------------------------+
| ``addDOMNodeList()``  | Una instancia de ``DOMNodeList`` |
+-----------------------+----------------------------------+
| ``addDOMNode()``      | Una instancia de ``DOMNode``     |
+-----------------------+----------------------------------+
| ``addNodes()``        | Un arreglo de los                |
|                       | elementos anteriores             |
+-----------------------+----------------------------------+
| ``add()``             | Acepta cualquiera de los         |
|                       | elementos anteriores             |
+-----------------------+----------------------------------+

Recorriendo
~~~~~~~~~~~

Como jQuery, el ``Impulsor`` tiene métodos para recorrer el DOM de un documento HTML/XML:

+-----------------------+----------------------------------------------------+
| Método                | Descripción                                        |
+=======================+====================================================+
| ``filter('h1')``      | Nodos que concuerdan con el selector CSS           |
+-----------------------+----------------------------------------------------+
| ``filterXpath('h1')`` | Nodos que concuerdan con la expresión XPath        |
+-----------------------+----------------------------------------------------+
| ``eq(1)``             | Nodo para el índice especificado                   |
+-----------------------+----------------------------------------------------+
| ``first()``           | Primer nodo                                        |
+-----------------------+----------------------------------------------------+
| ``last()``            | Último nodo                                        |
+-----------------------+----------------------------------------------------+
| ``siblings()``        | Hermanos                                           |
+-----------------------+----------------------------------------------------+
| ``nextAll()``         | Los siguientes hermanos                            |
+-----------------------+----------------------------------------------------+
| ``previousAll()``     | Los hermanos precedentes                           |
+-----------------------+----------------------------------------------------+
| ``parents()``         | Nodos padre                                        |
+-----------------------+----------------------------------------------------+
| ``children()``        | Hijos                                              |
+-----------------------+----------------------------------------------------+
| ``reduce($lambda)``   | Nodos para los que el ejecutable no devuelve falso |
+-----------------------+----------------------------------------------------+

Puedes limitar iterativamente tu selección de nodo encadenando llamadas a métodos, ya que cada método devuelve una nueva instancia del ``Impulsor`` para los nodos coincidentes::

    $impulsor
        ->filter('h1')
        ->reduce(function ($node, $i)
        {
            if (!$node->getAttribute('class')) {
                return false;
            }
        })
        ->first();

.. tip::

    Utiliza la función ``count()`` para obtener el número de nodos almacenados en un ``Impulsor``: ``count($impulsor)``

Extrayendo información
~~~~~~~~~~~~~~~~~~~~~~

El ``Impulsor`` puede extraer información de los nodos::

    // Devuelve el valor atributo del primer nodo
    $impulsor->attr('class');

    // Devuelve el valor del nodo para el primer nodo
    $impulsor->text();

    // Extrae una arreglo de atributos de todos los nodos (_text devuelve el valor del nodo)
    $impulsor->extract(array('_text', 'href'));

    // Ejecuta una lambda por cada nodo y devuelve una matriz de resultados
    $data = $impulsor->each(function ($node, $i)
    {
        return $node->getAttribute('href');
    });

Enlaces
~~~~~~~

Puedes seleccionar enlaces con los métodos de recorrido, pero el acceso directo ``selectLink()`` a menudo es más conveniente::

    $impulsor->selectLink('Haz clic aquí');

Este selecciona los enlaces que contienen el texto dado, o hace clic en imágenes en las que el atributo ``alt`` contiene el texto dado.

El método ``click()`` del cliente toma una instancia de ``Link`` como si la hubiera devuelto el método ``link()``::

    $enlace = $impulsor->link();

    $cliente->click($enlace);

.. tip::

    El método ``links()`` devuelve un arreglo de objetos ``Link`` para todos los nodos.

Formularios
~~~~~~~~~~~

En cuanto a los enlaces, tú seleccionas formularios con el método ``selectButton()``::

    $impulsor->selectButton('submit');

Ten en cuenta que seleccionas botones del formulario y no el formulario porque un formulario puede tener varios botones, si utilizas la API de recorrido, ten en cuenta que debes buscar un botón.

El método ``selectButton()`` puede seleccionar etiquetas ``button`` y enviar etiquetas ``input``, este tiene varias heurísticas para encontrarlas:

* El valor ``value`` del atributo;

* El valor del atributo ``id`` o ``alt`` de imágenes;

* El valor del atributo ``id`` o ``name`` de las etiquetas ``button``.

Cuando tienes un nodo que representa un botón, llama al método ``form()`` para conseguir una instancia de ``Form`` que envuelve al nodo botón::

    $formulario = $impulsor->form();

Cuando llamas al método ``form()``, también puedes pasar una matriz de valores de campo que sustituyen los valores predeterminados::

    $formulario = $impulsor->form(array(
        'nombre'       => 'Fabien',
        'like_symfony' => true,
    ));

Y si quieres simular un método HTTP específico del formulario, pásalo como segundo argumento::

    $formulario = $impulsor->form(array(), 'DELETE');

El cliente puede enviar instancias de ``Form``::

    $cliente->submit($formulario);

Los valores del campo también se pueden pasar como segundo argumento del método ``submit()``::

    $cliente->submit($formulario, array(
        'nombre'       => 'Fabien',
        'like_symfony' => true,
    ));

Para situaciones más complejas, utiliza la instancia de ``Form`` como una matriz para establecer el valor de cada campo individualmente::

    // Cambia el valor de un campo
    $formulario['nombre'] = 'Fabien';

También hay una buena API para manipular los valores de los campos de acuerdo a su tipo::

    // selecciona una opción o un botón de radio
    $formulario['country']->select('Francia');

    // marca una casilla de verificación (checkbox)
    $formulario['like_symfony']->tick();

    // carga un archivo
    $formulario['photo']->upload('/ruta/a/lucas.jpg');

.. tip::

    Puedes conseguir los valores que se enviarán llamando al método ``getValues()``. Los archivos subidos están disponibles en un arreglo separado devuelto por ``getFiles()``. ``getPhpValues​​()`` y ``getPhpFiles()`` también devuelven los valores presentados, pero en formato PHP (que convierte las claves con notación de corchetes a matrices PHP).

.. index::
   pair: Pruebas; Configuración

Probando la configuración
-------------------------

.. index::
   pair: PHPUnit; Configuración

Configuración de PHPUnit
~~~~~~~~~~~~~~~~~~~~~~~~

Cada aplicación tiene su propia configuración de PHPUnit, almacenada en el archivo ``phpunit.xml.dist``. Puedes editar este archivo para cambiar los valores predeterminados o crear un archivo ``phpunit.xml`` para modificar la configuración de tu máquina local.

.. tip::

    Guarda el archivo ``phpunit.xml.dist`` en tu repositorio de código, e ignora el archivo ``phpunit.xml``.

De forma predeterminada, sólo las pruebas almacenadas en los paquetes "estándar" las ejecuta la orden ``PHPUnit`` (las pruebas estándar están bajo el espacio de nombres Vendor\\*Bundle\\Tests). Pero, fácilmente puedes agregar más espacios de nombres. Por ejemplo, la siguiente configuración agrega las pruebas de los paquetes de terceros instalados:

.. code-block:: xml

    <!-- hola/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Proyecto banco de pruebas">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

Para incluir otros espacios de nombres en la cobertura de código, también edita la sección ``<filter>``:

.. code-block:: xml

    <filter>
        <whitelist>
            <directory>../src</directory>
            <exclude>
                <directory>../src/*/*Bundle/Resources</directory>
                <directory>../src/*/*Bundle/Tests</directory>
                <directory>../src/Acme/Bundle/*Bundle/Resources</directory>
                <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
            </exclude>
        </whitelist>
    </filter>

Configurando el cliente
~~~~~~~~~~~~~~~~~~~~~~~

El cliente utilizado por las pruebas funcionales crea un núcleo que se ejecuta en un entorno de ``prueba`` especial, por lo tanto puedes ajustar tanto como desees:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        imports:
            - { resource: config_dev.yml }

        framework:
            error_handler: false
            test: ~

        web_profiler:
            toolbar: false
            intercept_redirects: false

        monolog:
            handlers:
                main:
                    type:  stream
                    path:  %kernel.logs_dir%/%kernel.environment%.log
                    level: debug

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <container>
            <imports>
                <import resource="config_dev.xml" />
            </imports>

            <webprofiler:config
                toolbar="false"
                intercept-redirects="false"
            />

            <framework:config error_handler="false">
                <framework:test />
            </framework:config>

            <monolog:config>
                <monolog:main
                    type="stream"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                 />               
            </monolog:config>
        </container>

    .. code-block:: php

        // app/config/config_test.php
        $loader->import('config_dev.php');

        $contenedor->loadFromExtension('framework', array(
            'error_handler' => false,
            'test'          => true,
        ));

        $contenedor->loadFromExtension('web_profiler', array(
            'toolbar' => false,
            'intercept-redirects' => false,
        ));

        $contenedor->loadFromExtension('monolog', array(
            'handlers' => array(
                'main' => array('type' => 'stream',
                                'path' => '%kernel.logs_dir%/%kernel.environment%.log'
                                'level' => 'debug')

        )));

Además, puedes cambiar el entorno predeterminado (``test``) y sustituir el modo de depuración por omisión (``true``) pasándolos como opciones al método ``createClient()``::

    $cliente = static::createClient(array(
        'environment' => 'mi_entorno_de_prueba',
        'debug'       => false,
    ));

Si tu aplicación se comporta de acuerdo a algunas cabeceras HTTP, pásalas como segundo argumento de ``createClient()``::

    $cliente = static::createClient(array(), array(
        'HTTP_HOST'       => 'en.ejemplo.com',
        'HTTP_USER_AGENT' => 'MiSuperNavegador/1.0',
    ));

También puedes reemplazar cabeceras HTTP en base a la petición::

    $cliente->request('GET', '/', array(), array(
        'HTTP_HOST'       => 'en.ejemplo.com',
        'HTTP_USER_AGENT' => 'MiSuperNavegador/1.0',
    ));

.. tip::

    Para proveer tu propio cliente, reemplaza el parámetro ``test.client.class``, o define un servicio ``test.client``.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`
