.. index::
   single: Traduciendo

Traduciendo
===========

El término "internacionalización" se refiere al proceso de abstraer cadenas y otros elementos específicos de la configuración regional de tu aplicación a una capa donde se puedan traducir y convertir basándose en la configuración regional del usuario (es decir, el idioma y país). Para el texto, esto significa envolver cada uno con una función capaz de traducir el texto (o "mensaje") al idioma del usuario::

    // el texto *siempre* se imprime en Inglés
    echo 'Hello World';

    // el texto se puede traducir al idioma del usuario final o predeterminado en Inglés
    echo $translator->trans('Hello World');

.. note::

    El término *locale* se refiere más o menos al lenguaje y país del usuario. Esta puede ser cualquier cadena que entonces la aplicación utilice para manejar las traducciones y otras diferencias de formato (por ejemplo, formato de moneda). Recomendamos el código ISO639-1 para el *idioma*, un subrayado (``_``), luego el código ISO3166 para el *país* (por ejemplo ``es_ES`` para Francés/Francia).

En este capítulo, aprenderemos cómo preparar una aplicación para apoyar varias configuraciones regionales y, a continuación cómo crear traducciones para varias configuraciones regionales. En general, el proceso tiene varios pasos comunes:

1. Habilitar y configurar el componente Symfony ``Translation``;

1. Abstraer cadenas (es decir, "mensajes") envolviéndolas en llamadas a el ``Traductor``;

1. Crear recursos de traducción para cada configuración regional compatible, la cual traduce cada mensaje en la aplicación;

1. Determinar, establecer y administrar la configuración regional del usuario en la sesión.

.. index::
   single: Traduciendo; Configurando

Configurando
------------

Las traducciones están a cargo de un :term:`servicio` ``Traductor`` que utiliza la configuración regional del usuario para buscar y devolver mensajes traducidos. Antes de usarlo, habilita el ``Traductor`` en tu configuración:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            translator: { fallback: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:translator fallback="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('framework', array(
            'translator' => array('fallback' => 'en'),
        ));

La opción ``fallback`` define la configuración regional de reserva cuando la traducción no existe en la configuración regional del usuario.

.. tip::

    Cuando la traducción no existe para una configuración regional, el primer traductor intenta encontrar la traducción para el idioma (``es`` si el local es ``es_ES`` por ejemplo). Si esto también falla, busca una traducción utilizando la configuración regional de reserva.

La región utilizada en las traducciones es la almacenada en la sesión del usuario.

.. index::
   single: Traduciendo; Traducción básica

Traducción básica
-----------------

La traducción de texto se hace a través del servicio ``traductor`` (:class:`Symfony\\Component\\Translation\\Translator`). Para traducir un bloque de texto (llamado un *mensaje*), utiliza el método :method:`Symfony\\Component\\Translation\\Translator::trans`. Supongamos, por ejemplo, que estamos traduciendo un simple mensaje desde el interior de un controlador:

.. code-block:: php

    public function indexAction()
    {
        $t = $this->get('translator')->trans('Symfony2 is great');

        return new Response($t);
    }

Cuando se ejecuta este código, Symfony2 tratará de traducir el mensaje "Symfony2 is great", basándose en la ``locale`` del usuario. Para que esto funcione, tenemos que decirle a Symfony2 la manera de traducir el mensaje a través de un "recurso de traducción", que es una colección de traducciones de mensajes para una determinada configuración regional.
Este "diccionario" de traducciones se puede crear en varios formatos diferentes, XLIFF es el formato recomendado:

.. configuration-block::

    .. code-block:: xml

        <!-- messages.es.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.es.php
        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
        );

    .. code-block:: yaml

        # messages.es.yml
        Symfony2 is great: J'aime Symfony2

Ahora, si el idioma de la configuración regional del usuario es el Francés (por ejemplo, ``es_ES`` o ``es_BE``), el mensaje será traducido a ``J'aime Symfony2``.

El proceso de traducción
~~~~~~~~~~~~~~~~~~~~~~~~

Para empezar a traducir el mensaje, Symfony2 utiliza un proceso sencillo:

* Se determina el ``locale`` del usuario actual, el cual está almacenado en la sesión;

* Se carga un catálogo de mensajes traducidos de recursos de traducción definido para la configuración ``locale`` (por ejemplo, ``es_ES``). Los mensajes de la configuración regional de reserva también se cargan y se agregan al catálogo si no existen ya. El resultado final es un gran "diccionario" de traducciones. Consulta `Catálogos de mensajes`_ para más detalles;

* Si se encuentra el mensaje en el catálogo, devuelve la traducción. En caso contrario, el traductor devuelve el mensaje original.

Cuando se usa el método ``trans()``, Symfony2 busca la cadena exacta dentro del catálogo de mensajes apropiados y la devuelve (si existe).

.. index::
   single: Traduciendo; Marcadores de posición de mensajes

Marcadores de posición de mensajes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A veces, se debe traducir un mensaje que contiene una variable:

.. code-block:: php

    public function indexAction($nombre)
    {
        $t = $this->get('translator')->trans('Hola '.$nombre);

        return new Response($t);
    }

Sin embargo, la creación de una traducción de esta cadena es imposible, ya que el traductor tratará de buscar el mensaje exacto, incluyendo las porciones variables (por ejemplo, "Hola Ryan" u "Hola Fabian"). En lugar de escribir una traducción de cada iteración posible de la variable ``$nombre``, podemos reemplazar la variable con un "marcador de posición":

.. code-block:: php

    public function indexAction($nombre)
    {
        $t = $this->get('translator')->trans('Hola %nombre%', array('%nombre%' => $nombre));

        new Response($t);
    }

Symfony2 ahora busca una traducción del mensaje en bruto (``Hola %nombre%``) y *después* reemplaza los marcadores de posición con sus valores. La creación de una traducción se hace igual que antes:

.. configuration-block::

    .. code-block:: xml

        <!-- messages.es.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Hola %nombre%</source>
                        <target>Bonjour %nombre%</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // messages.es.php
        return array(
            'Hola %nombre%' => 'Bonjour %nombre%',
        );

    .. code-block:: yaml

        # messages.es.yml
        'Hola %nombre%': Hola %nombre%

.. note::

    Los marcadores de posición pueden tomar cualquier forma, el mensaje completo se reconstruye usando la `función strtr`_ de PHP. Sin embargo, se requiere la notación ``%var%`` cuando se traduce en plantillas Twig, y en general es un convenio razonable a seguir.

Como hemos visto, la creación de una traducción es un proceso de dos pasos:

1. Abstraer el mensaje que se necesita traducir procesándolo con el ``Traductor``.

1. Crear una traducción del mensaje para cada región que elijas apoyar.

El segundo paso se realiza creando catálogos de mensajes que definen las traducciones para cualquier número de lugares diferentes.

.. index::
   single: Traduciendo; Catálogos de mensajes

Catálogos de mensajes
---------------------

Cuando se traduce un mensaje, Symfony2 compila un catálogo de mensajes para la configuración regional del usuario y busca en ese una traducción del mensaje. Un catálogo de mensajes es como un diccionario de traducciones para una configuración regional específica. Por ejemplo, el catálogo de la configuración regional ``es_ES`` podría contener la siguiente traducción:

    Symfony2 is Great => J'aime Symfony2

Es responsabilidad del desarrollador (o traductor) de una aplicación internacionalizada crear estas traducciones. Las traducciones son almacenadas en el sistema de archivos y descubiertas por Symfony, gracias a algunos convenios.

.. tip::

    Cada vez que creas un *nuevo* recurso de traducción (o instalas un paquete que incluye un recurso de traducción), para que Symfony pueda descubrir el nuevo recurso de traducción, asegúrate de borrar la memoria caché con la siguiente orden:

    .. code-block:: bash

        php app/console cache:clear

.. index::
   single: Traduciendo; Ubicación de recursos de traducción

Ubicación de traducción y convenciones de nomenclatura
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 busca archivos de mensajes (traducciones) en dos lugares:

* Para los mensajes que se encuentran en un paquete, los archivos de mensajes correspondientes deben vivir en el directorio ``Resources/translations/`` del paquete;

* Para sustituir cualquier paquete de traducciones, coloca los archivos de mensajes en el directorio ``app/Resources/translations``.

El nombre del archivo de las traducciones también es importante ya que Symfony2 utiliza una convención para determinar los detalles sobre las traducciones. Cada archivo de mensajes se debe nombrar de acuerdo con el siguiente patrón: ``domain.locale.loader``:

* **domain**: Una forma opcional para organizar los mensajes en grupos (por ejemplo, ``admin``, ``navegación`` o el valor predeterminado ``messages``) - consulta `Usando mensajes del dominio`_;

* **locale**: La región para la cual son las traducciones (por ejemplo, ``es_ES``, ``es``, etc.);

* **loader**: ¿Cómo debe cargar y analizar el archivo Symfony2 (por ejemplo, ``XLIFF``, ``php`` o ``yml``).

El cargador puede ser el nombre de cualquier gestor registrado. De manera predeterminada, Symfony incluye los siguientes cargadores:

* ``xliff``: XLIFF file;
* ``php``:   PHP file;
* ``yml``:  YAML file.

La elección del cargador a utilizar es totalmente tuya y es una cuestión de gusto.

.. note::

    También puedes almacenar las traducciones en una base de datos, o cualquier otro almacenamiento, proporcionando una clase personalizada que implemente la interfaz :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`.
    Consulta :doc:`Cargadores de traducción personalizados </cookbook/translation/custom_loader>` a continuación para aprender cómo registrar cargadores personalizados.

.. index::
   single: Traduciendo; Creando recursos de traducción

Creando traducciones
~~~~~~~~~~~~~~~~~~~~

Cada archivo se compone de una serie de pares de identificador de traducción para el dominio y la configuración regional determinada. El id es el identificador de la traducción individual, y puede ser el mensaje en la región principal (por ejemplo, "Symfony is great") de tu aplicación o un identificador único (por ejemplo, "symfony2.great" - consulta el recuadro más abajo):

.. configuration-block::

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/translations/messages.es.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                    <trans-unit id="2">
                        <source>symfony2.great</source>
                        <target>J'aime Symfony2</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/translations/messages.es.php
        return array(
            'Symfony2 is great' => 'J\'aime Symfony2',
            'symfony2.great'    => 'J\'aime Symfony2',
        );

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/translations/messages.es.yml
        Symfony2 is great: J'aime Symfony2
        symfony2.great:    J'aime Symfony2

Symfony2 descubrirá estos archivos y los utilizará cuando traduce o bien "Symfony2 is graeat" o "symfony2.great" en un Idioma regional de Francés (por ejemplo, ``es_ES`` o ``es_BE``).

.. sidebar:: Usando mensajes reales o palabras clave

    Este ejemplo ilustra las dos diferentes filosofías, cuando creas mensajes a traducir:

    .. code-block:: php

        $t = $translator->trans('Symfony2 is great');

        $t = $translator->trans('symfony2.great');

    En el primer método, los mensajes están escritos en el idioma de la región predeterminada (Inglés en este caso). Ese mensaje se utiliza entonces como el "id" al crear traducciones.

    En el segundo método, los mensajes en realidad son "palabras clave" que transmiten la idea del mensaje. El mensaje de la palabra clave se utiliza entonces como el "id" para las traducciones. En este caso, la traducción se debe hacer para la región predeterminada (es decir, para traducir ``symfony2.great`` a ``Symfony2 es grande``).

    El segundo método es útil porque la clave del mensaje no se tendrá que cambiar en cada archivo de la traducción si decidimos que el mensaje en realidad debería decir "Symfony2 es realmente grande" en la configuración regional predeterminada.

    La elección del método a utilizar es totalmente tuya, pero a menudo se recomienda el formato "palabra clave". 

    Además, es compatible con archivos anidados en formato ``php`` y ``yaml`` para evitar repetir siempre lo mismo si utilizas palabras clave en lugar de texto real para tus identificaciones:

    .. configuration-block::

        .. code-block:: yaml

            symfony2:
                is:
                    great: Symfony2 is great
                    amazing: Symfony2 is amazing
                has:
                    bundles: Symfony2 has bundles
            user:
                login: Login

        .. code-block:: php

            return array(
                'symfony2' => array(
                    'is' => array(
                        'great' => 'Symfony2 is great',
                        'amazing' => 'Symfony2 is amazing',
                    ),
                    'has' => array(
                        'bundles' => 'Symfony2 has bundles',
                    ),
                ),
                'user' => array(
                    'login' => 'Login',
                ),
            );

    Los múltiples niveles se acoplan en pares de id/traducción añadiendo un punto (.) entre cada nivel, por lo tanto los ejemplos anteriores son equivalentes a los siguientes:

    .. configuration-block::

        .. code-block:: yaml

            symfony2.is.great: Symfony2 is great
            symfony2.is.amazing: Symfony2 is amazing
            symfony2.has.bundles: Symfony2 has bundles
            user.login: Login

        .. code-block:: php

            return array(
                'symfony2.is.great' => 'Symfony2 is great',
                'symfony2.is.amazing' => 'Symfony2 is amazing',
                'symfony2.has.bundles' => 'Symfony2 has bundles',
                'user.login' => 'Login',
            );

.. index::
   single: Traduciendo; Dominio de mensajes

Usando mensajes del dominio
---------------------------

Como hemos visto, los archivos de mensajes se organizan en las diferentes regiones a traducir. Los archivos de mensajes también se pueden organizar en "dominios".
Al crear archivos de mensajes, el dominio es la primera porción del nombre de archivo.
El dominio predeterminado es ``menssages``. Por ejemplo, supongamos que, por organización, las traducciones se dividieron en tres ámbitos diferentes: ``mensajes``, ``admin`` y ``navegacion``. La traducción francesa que tiene los siguientes archivos de mensaje:

* ``messages.es.xliff``
* ``admin.es.xliff``
* ``navegacion.es.xliff``

Al traducir las cadenas que no están en el dominio predeterminado (``messages``), debes especificar el dominio como tercer argumento de ``trans()``:

.. code-block:: php

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 ahora buscará el mensaje en el dominio ``admin`` de la configuración regional del usuario.

.. index::
   single: Traduciendo; Configuración regional del usuario

Manejando la configuración regional del usuario
-----------------------------------------------

La configuración regional del usuario actual se almacena en la sesión y se puede acceder a través del servicio ``sesión``:

.. code-block:: php

    $locale = $this->get('session')->getLocale();

    $this->get('session')->setLocale('en_US');

.. index::
   single: Traduciendo; Configuración regional predeterminada y reserva

Configuración regional predeterminada y reserva
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si la configuración regional no se ha establecido explícitamente en la sesión, el parámetro de configuración ``fallback_locale`` será utilizado por el ``Traductor``. El predeterminado del parámetro es ``en`` (consulta la sección `Configurando`_).

Alternativamente, puedes garantizar que un ``locale`` está establecido en la sesión del usuario definiendo un ``default_locale`` para el servicio sesión:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session: { default_locale: en }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session default-locale="en" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('framework', array(
            'session' => array('default_locale' => 'en'),
        ));

El locale y la URL
~~~~~~~~~~~~~~~~~~

Dado que la configuración regional del usuario se almacena en la sesión, puede ser tentador utilizar la misma URL para mostrar un recurso en muchos idiomas diferentes en función de la región del usuario. Por ejemplo, ``http://www.ejemplo.com/contacto`` podría mostrar el contenido en Inglés para un usuario y en Francés para otro. Por desgracia, esto viola una norma fundamental de la Web: que una URL particular devuelve el mismo recurso, independientemente del usuario. A fin de enturbiar el problema, ¿cual sería la versión del contenido indexado por los motores de búsqueda?

Una mejor política es incluir la configuración regional en la URL. Esto es totalmente compatible con el sistema de enrutado mediante el parámetro especial ``_locale``:

.. configuration-block::

    .. code-block:: yaml

        contacto:
            pattern:   /{_locale}/contacto
            defaults:  { _controller: AcmeDemoBundle:Contacto:index, _locale: en }
            requirements:
                _locale: en|es|de

    .. code-block:: xml

        <route id="contact" pattern="/{_locale}/contacto">
            <default key="_controller">AcmeDemoBundle:Contact:index</default>
            <default key="_locale">en</default>
            <requirement key="_locale">en|es|de</requirement>
        </route>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('contact', new Route('/{_locale}/contacto', array(
            '_controller' => 'AcmeDemoBundle:Contacto:index',
            '_locale'     => 'en',
        ), array(
            '_locale'     => 'en|es|de'
        )));

        return $coleccion;

Cuando utilizas el parámetro especial ``_locale`` en una ruta, la configuración regional emparejada *automáticamente se establece en la sesión del usuario*. En otras palabras, si un usuario visita la URI ``/es/contacto``, la región ``es`` se ajustará automáticamente según la configuración regional de la sesión del usuario.

Ahora puedes utilizar la configuración regional del usuario para crear rutas hacia otras páginas traducidas en tu aplicación.

.. index::
   single: Traduciendo; Pluralización

Pluralización
-------------

La pluralización de mensajes es un tema difícil puesto que las reglas pueden ser bastante complejas. Por ejemplo, aquí tienes la representación matemática de las reglas de pluralización de Rusia::

    (($number % 10 == 1) && ($number % 100 != 11)) ? 0 : ((($number % 10 >= 2) && ($number % 10 <= 4) && (($number % 100 < 10) || ($number % 100 >= 20))) ? 1 : 2);

Como puedes ver, en Ruso, puedes tener tres formas diferentes del plural, cada una da un índice de 0, 1 o 2. Para todas las formas, el plural es diferente, por lo que la traducción también es diferente.

Cuando una traducción tiene diferentes formas debido a la pluralización, puedes proporcionar todas las formas como una cadena separada por una tubería (``|``)::

    'Hay una manzana|Hay %count% manzanas'

Para traducir los mensajes pluralizado, utiliza el método :method:`Symfony\\Component\\Translation\\Translator::transChoice`:

.. code-block:: php

    $t = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

El segundo argumento (``10`` en este ejemplo), es el *número* de objetos descrito y se utiliza para determinar cual traducción usar y también para rellenar el marcador de posición ``%count%``.

En base al número dado, el traductor elige la forma plural adecuada.
En Inglés, la mayoría de las palabras tienen una forma singular cuando hay exactamente un objeto y una forma plural para todos los otros números (0, 2, 3...). Así pues, si ``count`` es ``1``, el traductor utilizará la primera cadena (``Hay una manzana``) como la traducción. De lo contrario, utilizará ``Hay %count% manzanas``.

Aquí está la traducción al Francés::

    'Il y a %count% pomme|Il y a %count% pommes'

Incluso si la cadena tiene una apariencia similar (se compone de dos subcadenas separadas por un tubo), las reglas francesas son diferentes: la primera forma (no plural) se utiliza cuando se ``count`` es ``0`` o ``1``. Por lo tanto, el traductor utilizará automáticamente la primera cadena (``Il y a %count% pomme``) cuando ``count`` es ``0`` o ``1``.

Cada región tiene su propio conjunto de reglas, con algunas que tienen hasta seis formas diferentes de plural con reglas complejas detrás de las cuales los números asignan a tal forma plural.
Las reglas son bastante simples para Inglés y Francés, pero para el Ruso, puedes querer una pista para saber qué regla coincide con qué cadena. Para ayudar a los traductores, puedes "etiquetar" cada cadena::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'

Las etiquetas realmente son pistas sólo para los traductores y no afectan a la lógica utilizada para determinar qué forma plural usar. Las etiquetas pueden ser cualquier cadena descriptiva que termine con dos puntos (``:``). Las etiquetas además no necesitan ser las mismas en el mensaje original cómo en la traducción.

.. tip:

    Como las etiquetas son opcionales, el traductor no las utiliza (el traductor únicamente obtendrá una cadena basada en su posición en la cadena).

Intervalo explícito de pluralización
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La forma más fácil de pluralizar un mensaje es dejar que Symfony2 utilice su lógica interna para elegir qué cadena se utiliza en base a un número dado. A veces, tendrás más control o quieres una traducción diferente para casos específicos (por ``0``, o cuando el número es negativo, por ejemplo). Para estos casos, puedes utilizar intervalos matemáticos explícitos::

    '{0} There is no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

Los intervalos siguen la notación `ISO 31-11`_. La cadena anterior especifica cuatro intervalos diferentes: exactamente ``0``, exactamente ``1``, ``2-19`` y ``20`` y superior.

También puedes mezclar reglas matemáticas explícitas y estándar. En este caso, si la cuenta no corresponde con un intervalo específico, las reglas estándar entran en vigor después de remover las reglas explícitas::

    '{0} There is no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

Por ejemplo, para ``1`` apple, la regla estándar ``There is one apple`` será utilizada. Para ``2-19`` apples, la segunda regla estándar ``There are %count%
apples`` será seleccionada.

Un :class:`Symfony\\Component\\Translation\\Interval` puede representar un conjunto finito de números::

    {1,2,3,4}

O números entre otros dos números::

    [1, +Inf[
    ]-1,2[

El delimitador izquierdo puede ser ``[`` (inclusive) o ``]`` (exclusivo). El delimitador derecho puede ser ``[`` (exclusivo) o ``]`` (inclusive). Más allá de los números, puedes usar ``-Inf`` y ``+Inf`` para el infinito.

.. index::
   single: Traduciendo; En plantillas

Traducciones en plantillas
--------------------------

La mayoría de las veces, la traducción ocurre en las plantillas. Symfony2 proporciona apoyo nativo para ambas plantillas Twig y PHP.

Plantillas Twig
~~~~~~~~~~~~~~~

Symfony2 proporciona etiquetas Twig especializadas (``trans`` y ``transchoice``) para ayudar con la traducción de los mensajes de *bloques estáticos de texto*:

.. code-block:: jinja

    {% trans %}Hola %nombre%{% endtrans %}

    {% transchoice count %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

La etiqueta ``transchoice`` obtiene automáticamente la variable ``%count%`` a partir del contexto actual y la pasa al traductor. Este mecanismo sólo funciona cuando se utiliza un marcador de posición después del patrón ``%var%``.

.. tip::

    Si necesitas utilizar el carácter de porcentaje (``%``) en una cadena, lo tienes que escapar duplicando el siguiente: `` {% trans %}Porcentaje: %percent%%%{% endtrans %}``

También puedes especificar el dominio del mensaje y pasar algunas variables adicionales:

.. code-block:: jinja

    {% trans with {'%nombre%': 'Fabien'} from "app" %}Hola %nombre%{% endtrans %}

    {% transchoice count with {'%nombre%': 'Fabien'} from "app" %}
        {0} There is no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Los filtros ``trans`` y ``transchoice`` se pueden utilizar para traducir *texto variable* y  expresiones complejas:

.. code-block:: jinja

    {{ message | trans }}

    {{ message | transchoice(5) }}

    {{ message | trans({'%nombre%': 'Fabien'}, "app") }}

    {{ message | transchoice(5, {'%nombre%': 'Fabien'}, 'app') }}

.. tip::

    Usar etiquetas de traducción o filtros tiene el mismo efecto, pero con una sutil diferencia: la salida escapada automáticamente sólo se aplica a las variables traducidas usando un filtro. En otras palabras, si necesitas estar seguro de que tu variable traducida *no* se escapó en la salida, debes aplicar el filtro crudo después de la traducción del filtro:

    .. code-block:: jinja

            {# text translated between tags is never escaped #}
            {% trans %}
                <h3>foo</h3>
            {% endtrans %}

            {% set message = '<h3>foo</h3>' %}

            {# a variable translated via a filter is escaped by default #}
            {{ message | trans | raw }}

            {# but static strings are never escaped #}
            {{ '<h3>foo</h3>' | trans }}

Plantillas PHP
~~~~~~~~~~~~~~

El servicio de traductor es accesible en plantillas PHP a través del ayudante ``traductor``:

.. code-block:: html+php

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

Forzando la configuración regional del traductor
------------------------------------------------

Al traducir un mensaje, Symfony2 utiliza la configuración regional de la sesión del usuario o la configuración regional de ``reserva`` si es necesario. También puedes especificar manualmente la configuración regional utilizada para la traducción:

.. code-block:: php

    $this->get('translator')->trans(
        'Symfony2 is great',
        array(),
        'messages',
        'es_ES',
    );

    $this->get('translator')->trans(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10),
        'messages',
        'es_ES',
    );

Traduciendo contenido de base de datos
--------------------------------------

La traducción del contenido de la base de datos la debe manejar Doctrine a través de la `Extensión Translatable`_. Para más información, consulta la documentación de la biblioteca.

Resumen
-------

Con el componente ``Translation`` de Symfony2, la creación de una aplicación internacionalizada ya no tiene que ser un proceso doloroso y se reduce a sólo algunos pasos básicos:

* Resumir los mensajes en tu aplicación envolviendo cada uno en el método
  :method:`Symfony\\Component\\Translation\\Translator::trans` o
  :method:`Symfony\\Component\\Translation\\Translator::transChoice`;

* Traducir cada mensaje en varias configuraciones regionales creando archivos de traducción de los mensajes. Symfony2 descubre y procesa cada archivo porque su nombre sigue una convención específica;

* Administrar la configuración regional del usuario, la cual se almacena en la sesión.

.. _`función strtr`: http://www.php.net/manual/es/function.strtr.php
.. _`ISO 31-11`: http://es.wikipedia.org/wiki/Intervalo_(matem%C3%A1tica)
.. _`Extensión Translatable`: https://github.com/l3pp4rd/DoctrineExtensions
