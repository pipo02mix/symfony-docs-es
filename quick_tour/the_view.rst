La vista
========

Después de leer la primera parte de esta guía, haz decidido que bien valen la pena otros 10 minutos en Symfony2. ¡Buena elección! En esta segunda parte, aprenderás más sobre el motor de plantillas de Symfony2, `Twig`_. Twig es un motor de plantillas flexible, rápido y seguro para PHP. Este hace tus plantillas más legibles y concisas, además de hacerlas más amigables para los diseñadores web.

.. note::

    En lugar de Twig, también puedes utilizar :doc:`PHP </cookbook/templating/PHP>` para tus plantillas. Ambos motores de plantillas son compatibles con Symfony2.

Familiarizándote con Twig
-------------------------

.. tip::

    Si quieres aprender Twig, te recomendamos que leas la `documentación`_ oficial. Esta sección es sólo una descripción rápida de los conceptos principales.

Una plantilla Twig es un archivo de texto que puede generar cualquier tipo de contenido (HTML, XML, CSV, LaTeX, ...). Twig define dos tipos de delimitadores:

* ``{{ ... }}``: Imprime una variable o el resultado de una expresión;

* ``{% ... %}``: Controla la lógica de la plantilla; se utiliza para ejecutar ``bucles for`` y declaraciones ``if``, por ejemplo.

A continuación mostramos una plantilla mínima que ilustra algunos conceptos básicos, usando dos variables ``titulo_pag`` y ``navegacion``, mismas que se pasaron a la plantilla:

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html lang="en">
        <head>
            <title>Mi página Web</title>
        </head>
        <body>
            <h1>{{ titulo_pag }}</h1>

            <ul id="navegacion">
                {% for elemento in navegacion %}
                    <li><a href="{{ elemento.href }}">{{ elemento.caption }}</a></li>
                {% endfor %}
            </ul>
        </body>
    </html>


.. tip::

   Puedes incluir comentarios dentro de las plantillas con el delimitador ``{# ... #}``.

Para reproducir una plantilla en Symfony, utiliza el método ``render`` dentro de un controlador, suministrando cualquier variable necesaria en la plantilla::

    $this->render('AcmeDemoBundle:Demo:hola.html.twig', array(
        'nombre' => $nombre,
    ));

Las variables pasadas a una plantilla pueden ser cadenas, matrices e incluso objetos. Twig abstrae la diferencia entre ellas y te permite acceder a los "atributos" de una variable con la notación de punto (``.``):

.. code-block:: jinja

    {# array('nombre' => 'Fabien') #}
    {{ nombre }}

    {# array('usuario' => array('nombre' => 'Fabien')) #}
    {{ usuario.nombre }}

    {# obliga a verlo como arreglo #}
    {{ usuario['nombre'] }}

    {# array('usuario' => new Usuario('Fabien')) #}
    {{ usuario.nombre }}
    {{ usuario.getNombre }}

    {# obliga a ver el nombre como método #}
    {{ usuario.nombre() }}
    {{ usuario.getNombre() }}

    {# pasa argumentos al método #}
    {{ usuario.fecha('Y-m-d') }}

.. note::

    Es importante saber que las llaves no son parte de la variable, sino de la declaración de impresión. Si accedes a variables dentro de las etiquetas no las envuelvas con llaves.

Decorando plantillas
--------------------

Muy a menudo, las plantillas en un proyecto comparten elementos comunes, como los bien conocidos encabezados y pies de página. En Symfony2, nos gusta pensar en este problema de forma diferente: una plantilla se puede decorar con otra. Esto funciona exactamente igual que las clases PHP: la herencia de plantillas permite crear un "diseño" de plantilla básico que contiene todos los elementos comunes de tu sitio y define "bloques" que las plantillas descendientes pueden reemplazar.

La plantilla ``hola.html.twig`` hereda de ``base.html.twig``, gracias a la etiqueta ``extends``:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hola.html.twig #}
    {% extends "AcmeDemoBundle::base.html.twig" %}

    {% block titulo "Hola " ~ nombre %}

    {% block contenido %}
        <h1>Hola {{ nombre }}!</h1>
    {% endblock %}

La notación ``AcmeDemoBundle::base.html.twig`` suena familiar, ¿no?
Es la misma notación utilizada para hacer referencia a una plantilla regular. La parte ``::`` simplemente significa que el elemento controlador está vacío, por lo tanto el archivo correspondiente se almacena directamente bajo el directorio ``Resources/views/``.

Ahora, echemos un vistazo a un ``base.html.twig`` simplificado:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/base.html.twig #}
    <div class="contenido-symfony">
        {% block contenido %}
        {% endblock %}
    </div>

La etiqueta ``{% block %}`` define bloques que las plantillas derivadas pueden
llenar. Todas las etiquetas de bloque le dicen al motor de plantillas que una
plantilla derivada puede reemplazar esas porciones de la plantilla.

En este ejemplo, la plantilla ``hola.html.twig`` sustituye el bloque ``contenido``, lo cual significa que el texto "Hola Fabien" se reproduce dentro del elemento ``div.contenido-symfony``.

Usando etiquetas, filtros y funciones
-------------------------------------

Una de las mejores características de Twig es su extensibilidad a través de etiquetas, filtros y funciones. Symfony2 viene empacado con muchas de estas integradas para facilitar el trabajo del diseñador de la plantilla.

Incluyendo otras plantillas
~~~~~~~~~~~~~~~~~~~~~~~~~~~

La mejor manera de compartir un fragmento de código entre varias plantillas diferentes es crear una nueva plantilla, que luego puedas incluir en otras plantillas.

Crea una plantilla ``integrada.html.twig``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/integrada.html.twig #}
    Hola {{ nombre }}

Y cambia la plantilla ``index.html.twig`` para incluirla:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hola.html.twig #}
    {% extends "AcmeDemoBundle::base.html.twig" %}

    {# sustituye el bloque 'contenido' con el de integrada.html.twig #}
    {% block contenido %}
        {% include "AcmeDemoBundle:Demo:integrada.html.twig" %}
    {% endblock %}

Integrando otros controladores
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

¿Y si deseas incrustar el resultado de otro controlador en una plantilla?
Eso es muy útil cuando se trabaja con Ajax, o cuando la plantilla incrustada necesita alguna variable que no está disponible en la plantilla principal.

Supongamos que haz creado una acción ``maravillosa``, y deseas incluirla dentro de la plantilla principal ``index``. Para ello, utiliza la etiqueta ``render``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/index.html.twig #}
    {% render "AcmeDemoBundle:Demo:maravillosa" with { 'nombre': nombre, 'color': 'verde' } %}

Aquí, la cadena ``AcmeDemoBundle:Demo:maravillosa`` se refiere a la acción ``maravillosa`` del controlador ``Demo``. Los argumentos (``nombre`` y ``color``) actúan como variables de la petición simulada (como si ``maravillosaAction`` estuviera manejando una petición completamente nueva) y se pone a disposición del controlador::

    // src/Acme/DemoBundle/Controller/DemoController.php

    class DemoController extends Controller
    {
        public function maravillosaAction($nombre, $color)
        {
            // crea algún objeto, basándose en la variable $color
            $object = ...;

            return $this->render('AcmeDemoBundle:Demo:maravillosa.html.twig', array('nombre' => $nombre, 'object' => $object));
        }

        // ...
    }

Creando enlaces entre páginas
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Hablando de aplicaciones web, crear enlaces entre páginas es una necesidad. En lugar de codificar las direcciones URL en las plantillas, la función ``path`` sabe cómo generar direcciones URL basándose en la configuración de enrutado. De esta manera, todas tus direcciones URL se pueden actualizar fácilmente con sólo cambiar la configuración:

.. code-block:: html+jinja

    <a href="{{ path('_demo_hola', { 'nombre': 'Tomás' }) }}">¡Hola Tomás!</a>

La función ``path`` toma el nombre de la ruta y una matriz de parámetros como argumentos. El nombre de la ruta es la clave principal en la cual se hace referencia a las rutas y los parámetros son los valores de los marcadores de posición definidos en el patrón de rutas::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hola/{nombre}", name="_demo_hola")
     * @Template()
     */
    public function holaAction($nombre)
    {
        return array('nombre' => $nombre);
    }

.. tip::

    La función ``url`` genera direcciones URL *absolutas*: ``{{ url('_demo_hola', {
    'nombre': 'Tomás' }) }}``.

Incluyendo Activos: imágenes, JavaScript y hojas de estilo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

¿Qué sería de Internet sin imágenes, JavaScript y hojas de estilo?
Symfony2 proporciona la función ``asset`` para hacerles frente fácilmente:

.. code-block:: jinja

    <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    <img src="{{ asset('images/logo.png') }}" />

El propósito principal de la función ``asset`` es hacer más portátil tu aplicación.
Gracias a esta función, puedes mover el directorio raíz de la aplicación a cualquier lugar bajo tu directorio web raíz sin cambiar nada en el código de tus plantillas.

Escapando variables
-------------------

Twig se configura de forma automática escapando toda salida de forma predeterminada. Lee la `documentación`_ de Twig para obtener más información sobre el mecanismo de escape y la extensión 'Escaper'.

Consideraciones finales
-----------------------

Twig es simple pero potente. Gracias a los diseños, bloques, plantillas e inclusión de acciones, es muy fácil organizar tus plantillas de manera lógica y extensible. Sin embargo, si no te sientes cómodo con Twig, siempre puedes utilizar las plantillas de PHP dentro de Symfony sin ningún problema.

Sólo haz estado trabajando con Symfony2 durante unos 20 minutos, pero ya puedes hacer cosas muy sorprendentes con él. Ese es el poder de Symfony2. Aprender los conceptos básicos es fácil, y pronto aprenderás que esta simplicidad está escondida bajo una arquitectura muy flexible.

Pero me estoy adelantando demasiado. En primer lugar, necesitas aprender más sobre el controlador y ese es exactamente el tema de la :doc:`siguiente parte de esta guía <the_controller>`.
¿Listo para otros 10 minutos con Symfony2?

.. _Twig:          http://www.twig-project.org/
.. _documentación: http://www.twig-project.org/documentation
